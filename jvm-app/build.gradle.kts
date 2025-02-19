plugins {
    kotlin("jvm")
}

dependencies {
    implementation(kotlin("stdlib"))
    implementation(libs.kotlinxCoroutines)
}

val includeCppProjects =
    listOf(":llama-library")
        .map { projectName -> project(projectName).layout.buildDirectory.get() }
        .map { buildDirectory -> buildDirectory.dir("cmake-build").asFile }

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

    dependsOn(":llama-library:compileNative")
}