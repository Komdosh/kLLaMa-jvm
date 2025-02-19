package pro.tabakov.inference

object InferenceFactory {
    fun loadModel(modelPath: String, temperature: Float, contextSize: Long): KLLaMa {
        val pointer = inferenceAdapter.loadModel(modelPath, temperature, contextSize)
        return KLLaMa(pointer)
    }
}