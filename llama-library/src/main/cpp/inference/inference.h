#include "llama.h"
#include <string>
#include <vector>
#include <jni.h>

class Inference {
    llama_context *_ctx;
    llama_model *_model;
    llama_sampler *_sampler;

    llama_token _currToken;
    llama_batch _batch;
    std::vector<llama_token> _promptTokens;

    std::vector<llama_chat_message> _messages;
    std::vector<char> _formattedMessages;
    int _ctxSizeUsed = 0;

    int _prevLen = 0;

    std::string _response;
    std::string _cacheResponseTokens;
    bool _saveChatMessages;

    int64_t _responseTime = 0;
    long _responseTokensCount = 0;
public:
    void loadModel(const char *path_to_model, float temperature, long contextSize, bool saveChatMessages);

    void addChatMessage(const char *message, const char *role);

    void startCompletion(const char *request);

    std::string completionLoop();

    void stopCompletion();

    int usedContextSize() const;

    float generationTime() const;

    ~Inference();
};