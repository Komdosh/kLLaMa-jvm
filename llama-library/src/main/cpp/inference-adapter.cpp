#include <jni.h>
#include "inference/inference.h"

extern "C"
JNIEXPORT jlong JNICALL
Java_pro_tabakov_inference_InferenceAdapter_loadModel(
        JNIEnv *env,
        jobject that,
        jstring modelPath,
        jfloat temperature,
        jlong contextSize,
        jboolean saveChatMessages
) {
    jboolean isCopy = true;
    const char *modelPathStr = env->GetStringUTFChars(modelPath, &isCopy);
    Inference *inference = new Inference();

    try {
        inference->loadModel(modelPathStr, temperature, contextSize, saveChatMessages);
    }
    catch (std::runtime_error &error) {
        env->ThrowNew(env->FindClass("java/lang/IllegalStateException"), error.what());
    }

    env->ReleaseStringUTFChars(modelPath, modelPathStr);
    return reinterpret_cast<jlong>(inference);
}

extern "C"
JNIEXPORT jint JNICALL
Java_pro_tabakov_inference_InferenceAdapter_contextSizeUsed(JNIEnv *env, jobject that, jlong modelPtr) {
    Inference *inference = reinterpret_cast<Inference *>(modelPtr);
    return inference->usedContextSize();
}

extern "C"
JNIEXPORT void JNICALL
Java_pro_tabakov_inference_InferenceAdapter_close(
        JNIEnv *env,
        jobject that,
        jlong modelPtr
) {
    Inference *inference = reinterpret_cast<Inference *>(modelPtr);
    delete inference;
}

extern "C"
JNIEXPORT void JNICALL
Java_pro_tabakov_inference_InferenceAdapter_addChatMessage(JNIEnv *env, jobject that, jlong modelPtr, jstring message,
                                                 jstring role) {
    jboolean isCopy = true;
    const char *messageCstr = env->GetStringUTFChars(message, &isCopy);
    const char *roleCstr = env->GetStringUTFChars(role, &isCopy);
    Inference *inference = reinterpret_cast<Inference *>(modelPtr);
    inference->addChatMessage(messageCstr, roleCstr);
    env->ReleaseStringUTFChars(message, messageCstr);
    env->ReleaseStringUTFChars(role, roleCstr);
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_pro_tabakov_inference_InferenceAdapter_generationTime(JNIEnv *env, jobject that, jlong modelPtr) {
    Inference *inference = reinterpret_cast<Inference *>(modelPtr);
    return inference->generationTime();
}


extern "C"
JNIEXPORT void JNICALL
Java_pro_tabakov_inference_InferenceAdapter_startCompletion(
        JNIEnv *env,
        jobject that,
        jlong modelPtr,
        jstring promptString
) {
    jboolean isCopy = true;
    const char *request = env->GetStringUTFChars(promptString, &isCopy);
    Inference *inference = reinterpret_cast<Inference *>(modelPtr);
    inference->startCompletion(request);
    env->ReleaseStringUTFChars(promptString, request);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_pro_tabakov_inference_InferenceAdapter_completionLoop(
        JNIEnv *env,
        jobject that,
        jlong modelPtr
) {
    Inference *inference = reinterpret_cast<Inference *>(modelPtr);
    try {
        std::string response = inference->completionLoop();
        return env->NewStringUTF(response.c_str());
    }
    catch (std::runtime_error &error) {
        env->ThrowNew(env->FindClass("java/lang/IllegalStateException"), error.what());
        return nullptr;
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_pro_tabakov_inference_InferenceAdapter_stopCompletion(
        JNIEnv *env,
        jobject that,
        jlong modelPtr
) {
    Inference *inference = reinterpret_cast<Inference *>(modelPtr);
    inference->stopCompletion();
}