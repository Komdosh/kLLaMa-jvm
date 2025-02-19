import pro.atabakov.CmakeBuildNativePlugin
import pro.atabakov.CmakeExtension

apply<CmakeBuildNativePlugin>()

project.extensions.configure<CmakeExtension>("cmake") {
    cmakePath.set("/opt/homebrew/bin/cmake")
}