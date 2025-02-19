package pro.atabakov

import org.gradle.api.Plugin
import org.gradle.api.Project
import org.gradle.api.logging.LogLevel
import org.gradle.api.tasks.Exec

class CmakeBuildNativePlugin : Plugin<Project> {
    override fun apply(project: Project) {
        val extension = project.extensions.create("cmake", CmakeExtension::class.java)
        // Set default value
        extension.cmakePath.convention("cmake")

        // Create a task to configure and build with CMake
        project.tasks.register("buildNative", Exec::class.java) {
            notCompatibleWithConfigurationCache("Process execution cannot be cached.")
            group = "build"
            description = "Builds the native C++ library using CMake"

            // Ensure the build directory exists
            val buildDir = project.layout.buildDirectory.dir("cmake-build").get().asFile
            if (!buildDir.exists()) {
                buildDir.mkdirs()
            }

            val cmakePath = extension.cmakePath.get()


            workingDir = project.projectDir

            inputs.files(workingDir.resolve("CMakeLists.txt"))

            commandLine(
                cmakePath,
                "-S", project.projectDir.absolutePath,
                "-B", buildDir.absolutePath,
                "-DCMAKE_BUILD_TYPE=Release"
            )

            // Add error handling
            isIgnoreExitValue = false

            // Optional: Add logging
            logging.captureStandardOutput(LogLevel.INFO)
            logging.captureStandardError(LogLevel.ERROR)
        }


        // Create a task to compile the native library
        project.tasks.register("compileNative", Exec::class.java) {
            notCompatibleWithConfigurationCache("Process execution cannot be cached.")
            group = "build"
            description = "Compiles the native C++ library using CMake"

            val cmakePath = extension.cmakePath.get()

            val buildDir = project.layout.buildDirectory.dir("cmake-build").get().asFile

            inputs.files(project.layout.buildDirectory.dir("cmake-build/CMakeCache.txt").get().asFile)
            commandLine(
                cmakePath,
                "--build", buildDir.absolutePath,
                "-j", "8",
                "--config", "Release"
            )

            dependsOn("buildNative")
        }
    }
}