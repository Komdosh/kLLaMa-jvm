#include <jni.h>
#include "inference/inference.h"

extern "C"
JNIEXPORT jlong JNICALL
Java_pro_tabakov_inference_InferenceAdapter_loadModel(
        JNIEnv *env,
        jobject that,
        jstring modelPath,
        jfloat temperature,
        jlong contextSize
) {
    jboolean isCopy = true;
    const char *modelPathStr = env->GetStringUTFChars(modelPath, &isCopy);
    Inference *inference = new Inference();

    try {
        inference->loadModel(modelPathStr, temperature, contextSize);
    }
    catch (std::runtime_error &error) {
        env->ThrowNew(env->FindClass("java/lang/IllegalStateException"), error.what());
    }

    env->ReleaseStringUTFChars(modelPath, modelPathStr);
    return reinterpret_cast<jlong>(inference);
}

extern "C"
JNIEXPORT jint JNICALL
Java_pro_tabakov_inference_InferenceAdapter_getContextSizeUsed(JNIEnv *env, jobject that, jlong modelPtr) {
    Inference *inference = reinterpret_cast<Inference *>(modelPtr);
    return inference->getContextSizeUsed();
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