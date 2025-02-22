package pro.tabakov.kllama

import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext

object InferenceFactory {
    init {
        System.loadLibrary("llama")
    }
    suspend fun loadModel(modelPath: String, temperature: Float, contextSize: Long): KLLaMa {
        val pointer = withContext(Dispatchers.IO) {
                inferenceAdapter.loadModel(modelPath, temperature, contextSize,false)
            }
        return KLLaMa(pointer)
    }
}