package pro.tabakov.kllama

import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.flow

data class KLLaMa(val pointer: Long) : InferenceAdapterMethods {
    override fun getContextSizeUsed(): Int {
        return inferenceAdapter.contextSizeUsed(pointer)
    }

    override fun close() {
        inferenceAdapter.close(pointer)
    }

    override fun addChatMessage(message: String, role: String) {
        inferenceAdapter.addChatMessage(pointer, message, role)
    }

    override fun generationTime() {
        inferenceAdapter.generationTime(pointer)
    }

    override fun startCompletion(promptString: String) {
        inferenceAdapter.startCompletion(pointer, promptString)
    }

    override fun completionLoop(): String {
        return inferenceAdapter.completionLoop(pointer)
    }

    override fun stopCompletion() {
        inferenceAdapter.stopCompletion(pointer)
    }

    fun ask(request: String): Flow<String> =
        flow {
            startCompletion(request)
            var piece = completionLoop()
            while (piece != "[EOG]") {
                emit(piece)
                piece = completionLoop()
            }
            stopCompletion()
        }
}