#include "inference.h"
#include "gguf.h"
#include "common.h"
#include "string-utils.h"
#include <cstring>
#include <iostream>

void Inference::loadModel(
        const char *path_to_model,
        float temperature,
        long contextSize,
        bool saveChatMessages
) {
    std::cout << "Loading model: " << path_to_model << std::endl;
    std::cout << "Context size: " << contextSize << std::endl;
    std::cout << "Temperature: " << temperature << std::endl;

    this->_saveChatMessages = saveChatMessages;

    // create an instance of llama_model
    llama_model_params model_params = llama_model_default_params();
    _model = llama_model_load_from_file(path_to_model, model_params);

    if (!_model) {
        throw std::runtime_error("loadModel() failed");
    }

    // create an instance of llama_context
    llama_context_params ctx_params = llama_context_default_params();
    ctx_params.no_perf = true;
    ctx_params.n_ctx = contextSize;
    _ctx = llama_init_from_model(_model, ctx_params);

    if (!_ctx) {
        throw std::runtime_error("llama_new_context_with_model() returned null context");
    }

    llama_sampler_chain_params sampler_params = llama_sampler_chain_default_params();
    sampler_params.no_perf = true;
    _sampler = llama_sampler_chain_init(sampler_params);
    llama_sampler_chain_add(_sampler, llama_sampler_init_temp(temperature));
    llama_sampler_chain_add(_sampler, llama_sampler_init_dist(LLAMA_DEFAULT_SEED));

    _messages.clear();

    _formattedMessages = std::vector<char>(llama_n_ctx(_ctx));
}

int Inference::usedContextSize() const {
    return _ctxSizeUsed;
}

Inference::~Inference() {
    // free message memory
    for (llama_chat_message &message: _messages) {
        free(const_cast<char *>(message.role));
        free(const_cast<char *>(message.content));
    }
    llama_model_free(_model);
    llama_free(_ctx);
}

void Inference::addChatMessage(const char *message, const char *role) {
    _messages.push_back({strdup(role), strdup(message)});
}

float Inference::generationTime() const {
    return (float) _responseTokensCount / (_responseTime / 1e6);
}

void Inference::startCompletion(const char *request) {
    if (!_saveChatMessages) {
        _prevLen = 0;
        _formattedMessages.clear();
    }
    _responseTime = 0;
    _responseTokensCount = 0;
    addChatMessage(request, "user");
    // apply the chat-template
    const char *tmpl = llama_model_chat_template(_model, nullptr);
    int newLen = llama_chat_apply_template(
            tmpl,
            _messages.data(),
            _messages.size(),
            true,
            _formattedMessages.data(),
            _formattedMessages.size()
    );
    if (newLen > (int) _formattedMessages.size()) {
        // resize the output buffer `_formattedMessages`
        // and re-apply the chat template
        _formattedMessages.resize(newLen);
        newLen = llama_chat_apply_template(tmpl, _messages.data(), _messages.size(), true,
                                           _formattedMessages.data(), _formattedMessages.size());
    }
    if (newLen < 0) {
        throw std::runtime_error("llama_chat_apply_template() in Inference::startCompletion() failed");
    }
    std::string prompt(_formattedMessages.begin() + _prevLen, _formattedMessages.begin() + newLen);
    _promptTokens = common_tokenize(llama_model_get_vocab(_model), prompt, true, true);

    _batch.token = _promptTokens.data();
    _batch.n_tokens = _promptTokens.size();
}

std::string Inference::completionLoop() {
    // check if the length of the inputs to the model
    // have exceeded the context size of the model
    uint32_t contextSize = llama_n_ctx(_ctx);
    _ctxSizeUsed = llama_get_kv_cache_used_cells(_ctx);
    if (_ctxSizeUsed + _batch.n_tokens > contextSize) {
        throw std::runtime_error("context size reached");
    }

    auto start = ggml_time_us();
    // run the model
    if (llama_decode(_ctx, _batch) < 0) {
        throw std::runtime_error("llama_decode() failed");
    }

    // sample a token and check if it is an EOG (end of generation token)
    // convert the integer token to its corresponding word-piece
    _currToken = llama_sampler_sample(_sampler, _ctx, -1);
    if (llama_vocab_is_eog(llama_model_get_vocab(_model), _currToken)) {
        addChatMessage(strdup(_response.data()), "assistant");
        _response.clear();
        return "[EOG]";
    }
    std::string piece = common_token_to_piece(_ctx, _currToken, true);
    auto end = ggml_time_us();
    _responseTime += (end - start);
    _responseTokensCount += 1;
    _cacheResponseTokens += piece;

    // re-init the batch with the newly predicted token
    // key, value pairs of all previous tokens have been cached
    // in the KV cache
    _batch.token = &_currToken;
    _batch.n_tokens = 1;

    if (_isValidUtf8(_cacheResponseTokens.c_str())) {
        _response += _cacheResponseTokens;
        std::string valid_utf8_piece = _cacheResponseTokens;
        _cacheResponseTokens.clear();
        return valid_utf8_piece;
    }

    return "";
}


void Inference::stopCompletion() {
    if (_saveChatMessages) {
        addChatMessage(_response.c_str(), "assistant");
    }
    _response.clear();
    const char *tmpl = llama_model_chat_template(_model, nullptr);
    _prevLen = llama_chat_apply_template(
            tmpl,
            _messages.data(),
            _messages.size(),
            false,
            nullptr,
            0
    );
    if (_prevLen < 0) {
        throw std::runtime_error("llama_chat_apply_template() in Inference::stopCompletion() failed");
    }
}