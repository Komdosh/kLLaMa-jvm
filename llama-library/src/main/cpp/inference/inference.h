#include "llama.h"
#include <string>
#include <vector>
#include <jni.h>

class Inference {
    llama_context *_ctx;
    llama_model *_model;
    llama_sampler *_sampler;

    std::vector<llama_chat_message> _messages;
    std::vector<char> _formattedMessages;
    int _ctxSizeUsed = 0;

public:
    void loadModel(const char *path_to_model, float temperature, long contextSize);

    int getContextSizeUsed() const;

    ~Inference();
};