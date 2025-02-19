plugins {
    kotlin("jvm")
}

dependencies {
    implementation(kotlin("stdlib"))
    implementation(libs.kotlinxCoroutines)
}

tasks.getByName("compileKotlin") {
    dependsOn(":llama-library:compileNative")
}