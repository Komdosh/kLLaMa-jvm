import kotlinx.coroutines.runBlocking
import pro.tabakov.inference.InferenceFactory

fun main() {
    System.loadLibrary("llama")

    runBlocking {
        val kLLaMa = InferenceFactory.loadModel(
            "/Users/komdosh/Projects/kLLaMa-jvm/dolphin-2.2.1-mistral-7b.Q2_K.gguf",
            0.0f,
            0L
        )
        println(kLLaMa.getContextSizeUsed())

        kLLaMa.ask("Hello!").collect { message ->
            print(message)
        }
    }
}