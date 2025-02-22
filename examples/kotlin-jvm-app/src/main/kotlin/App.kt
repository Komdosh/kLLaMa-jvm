import kotlinx.coroutines.runBlocking
import pro.tabakov.inference.InferenceFactory

fun main() {
    System.loadLibrary("llama")

    runBlocking {
        val kLLaMa = InferenceFactory.loadModel(
            "/Users/komdosh/Projects/kLLaMa-jvm/mythologic-mini-7b.Q2_K.gguf",
            0.0f,
            0L
        )
        println(kLLaMa.getContextSizeUsed())

        kLLaMa.ask("HI!!").collect { message ->
            print(message)
        }
    }
}