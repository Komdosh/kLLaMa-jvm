import pro.atabakov.CmakeExtension
import pro.atabakov.gradle.plugin.pro.atabakov.CmakeSetJNIPlugin

plugins {
    kotlin("jvm")
}

apply<CmakeSetJNIPlugin>()

project.extensions.configure<CmakeExtension>("jniLibProjectName") {
    jniLibProjectName = ":llama-library"
}

dependencies {
    implementation(kotlin("stdlib"))
    implementation(project(":kLLaMa"))
    implementation(libs.kotlinxCoroutines)
}

tasks.register<JavaExec>("run") {
    group = "application"
    description = "Runs the JVM application"
    mainClass.set("pro.atabakov.AppKt") // Fully qualified name of the main class
    classpath = sourceSets.main.get().runtimeClasspath
}