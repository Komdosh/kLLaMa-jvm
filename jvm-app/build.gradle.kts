plugins {
    kotlin("jvm")
}

dependencies {
    implementation(kotlin("stdlib"))
//    implementation(project(":cpp-library"))
}

tasks.register<JavaExec>("run") {
    notCompatibleWithConfigurationCache("Task uses dynamic configuration")

    group = "application"
    description = "Runs the JVM application"
    mainClass.set("AppKt") // Fully qualified name of the main class
    classpath = sourceSets.main.get().runtimeClasspath
    doFirst {
        systemProperty("java.library.path", "${project(":cpp-library").layout.buildDirectory.get()}/cmake-build")
    }
}