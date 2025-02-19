package pro.tabakov.inference

data class KLLaMa(val pointer: Long) : InferenceAdapterMethods {
    override fun getContextSizeUsed(): Int {
        return inferenceAdapter.getContextSizeUsed(pointer)
    }

    override fun close() {
        inferenceAdapter.close(pointer)
    }
}