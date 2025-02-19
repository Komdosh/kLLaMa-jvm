package pro.tabakov.inference

class InferenceAdapter {
    external fun loadModel(modelPath: String, temperature: Float, contextSize: Long, saveChatMessages: Boolean): Long
    external fun contextSizeUsed(ptr: Long): Int
    external fun close(ptr: Long)

    external fun addChatMessage(ptr: Long, message: String, role: String)
    external fun generationTime(ptr: Long)
    external fun startCompletion(ptr: Long, promptString: String)
    external fun completionLoop(ptr: Long): String
    external fun stopCompletion(ptr: Long)

}

val inferenceAdapter = InferenceAdapter()