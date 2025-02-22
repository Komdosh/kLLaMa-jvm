package pro.tabakov.kllama

interface InferenceAdapterMethods {
    fun getContextSizeUsed(): Int
    fun close()

    fun addChatMessage(message: String, role: String)
    fun generationTime()
    fun startCompletion(promptString: String)
    fun completionLoop(): String
    fun stopCompletion()
}