plugins {
    kotlin("jvm")
}

dependencies {
    implementation(kotlin("stdlib"))
//    implementation(project(":cpp-library"))
}

val includeCppProjects =
    listOf(":cpp-library", ":llama-library")
        .map { project(it).layout.buildDirectory.get() }
        .map { it.dir("cmake-build").asFile }

tasks.register<JavaExec>("run") {
    notCompatibleWithConfigurationCache("Task uses dynamic configuration")

    group = "application"
    description = "Runs the JVM application"
    mainClass.set("AppKt") // Fully qualified name of the main class
    classpath = sourceSets.main.get().runtimeClasspath
    doFirst {
        systemProperty(
            "java.library.path",
            includeCppProjects.joinToString(":")
        )
    }
}