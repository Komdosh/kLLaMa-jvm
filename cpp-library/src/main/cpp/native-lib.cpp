#include <jni.h>
#include <iostream>

extern "C"
JNIEXPORT void JNICALL Java_App_printMessage(JNIEnv* env, jobject obj) {
    std::cout << "Hello from C++!" << std::endl;
}