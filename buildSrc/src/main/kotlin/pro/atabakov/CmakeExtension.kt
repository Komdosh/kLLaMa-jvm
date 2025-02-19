package pro.atabakov

import org.gradle.api.provider.Property

abstract class CmakeExtension {
    abstract val cmakePath: Property<String>
}


