package pro.atabakov.gradle.plugin.pro.atabakov

import org.gradle.api.Plugin
import org.gradle.api.Project
import org.gradle.api.tasks.JavaExec
import pro.atabakov.CmakeExtension


class CmakeSetJNIPlugin : Plugin<Project> {
    override fun apply(project: Project) {
        val extension = project.extensions.create("jniLibProjectName", CmakeExtension::class.java)
        // Set default value
        extension.jniLibProjectName.convention("jniLibProjectName")

        project.tasks.withType(JavaExec::class.java) {
            val includeCppProjects =
                listOf(extension.jniLibProjectName.get())
                    .map { projectName -> project.rootProject.project(projectName).layout.buildDirectory.get() }
                    .map { buildDirectory -> buildDirectory.dir("cmake-build").asFile }

            systemProperty("java.library.path", System.getProperty("java.library.path") + ":" + includeCppProjects.joinToString(":"))
        }
    }
}