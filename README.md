# kLLaMa-jvm

[![Kotlin Version](https://img.shields.io/badge/Kotlin-2.1.0-blue.svg)](https://kotlinlang.org/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![llama.cpp](https://img.shields.io/badge/llama.cpp-%F0%9F%94%97-brightgreen)](https://github.com/ggerganov/llama.cpp)

Simple example of using [llama.cpp](https://github.com/ggerganov/llama.cpp) with kotlin (JVM)

## Prerequisites 📋

- JDK 21
- Kotlin 2.1.0
- CMake 3.31
- C++ compiler (Clang++/GCC/MSVC)

## Installation 🛠️

For now llama is built only for Mac M processors.
You can tweak build options in 

`llama-library/CMakeLists.txt`

Compile llama:

`gradle :llama-library:compileNative`

## Example 🚀

```kotlin
import kotlinx.coroutines.runBlocking
import pro.tabakov.kllama.InferenceFactory

fun main() {
    System.loadLibrary("llama")

    runBlocking {
        val kLLaMa = InferenceFactory.loadModel(
            "/path/to/model.gguf", // Path to model
            0.0f, // Temperature
            0L // Context Size
        )
        println(kLLaMa.getContextSizeUsed())

        kLLaMa.ask("HI!").collect { message ->
            print(message)
        }
    }
}
```

Just put a path to model in `kotlin/pro/atabakov/App.kt` and simply run this example with gradle

`gradle examples:kotlin-jvm-app:run`
