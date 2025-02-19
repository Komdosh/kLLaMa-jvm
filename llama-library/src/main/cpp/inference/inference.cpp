#include "inference.h"
#include "gguf.h"
#include "common.h"
#include <cstring>
#include <iostream>

void Inference::loadModel(
        const char *path_to_model,
        float temperature,
        long contextSize
) {
    std::cout << "Loading model: " << path_to_model << std::endl;
    std::cout << "Context size: " << contextSize << std::endl;
    std::cout << "Temperature: " << temperature << std::endl;

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

    // initialize sampler
    llama_sampler_chain_params sampler_params = llama_sampler_chain_default_params();
    sampler_params.no_perf = true;
    _sampler = llama_sampler_chain_init(sampler_params);
    llama_sampler_chain_add(_sampler, llama_sampler_init_temp(temperature));
    llama_sampler_chain_add(_sampler, llama_sampler_init_dist(LLAMA_DEFAULT_SEED));

    _formattedMessages = std::vector<char>(llama_n_ctx(_ctx));
}

int Inference::getContextSizeUsed() const {
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