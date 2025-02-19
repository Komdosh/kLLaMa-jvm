fun main() {
    System.loadLibrary("native-lib")
    App().printMessage()
}

class App {
    external fun printMessage()
}