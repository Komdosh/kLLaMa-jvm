package pro.atabakov

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