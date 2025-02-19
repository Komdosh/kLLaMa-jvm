package pro.tabakov.inference

class InferenceAdapter {
    external fun loadModel(modelPath: String, temperature: Float, contextSize: Long): Long
    external fun getContextSizeUsed(ptr: Long): Int
    external fun close(ptr: Long)
}

val inferenceAdapter = InferenceAdapter()