plugins {
    id("java")
    application
}

group = "org.example"
version = "1.0-SNAPSHOT"

java {
    toolchain {
        languageVersion = JavaLanguageVersion.of(25)
    }
}

repositories {
    mavenCentral()
}

dependencies {
    implementation("io.github.bonede:tree-sitter:0.24.4")
    implementation("io.github.bonede:tree-sitter-bash:0.23.3")
    implementation("com.google.code.gson:gson:2.9.0")
    implementation("com.fasterxml.jackson.module:jackson-module-jsonSchema:2.18.3")
    implementation("org.apache.commons:commons-lang3:3.12.0")
    implementation("org.apache.pdfbox:pdfbox:3.0.1")
    implementation("org.apache.poi:poi:5.2.2")
    implementation("org.apache.poi:poi-ooxml:5.2.2")
    implementation("org.apache.httpcomponents:httpclient:4.5.13")
    implementation("com.googlecode.json-simple:json-simple:1.1.1")
    implementation("com.sun.mail:jakarta.mail:2.0.1")



    testImplementation("org.junit.jupiter:junit-jupiter-api:5.9.0")
    testImplementation("org.junit.platform:junit-platform-runner:1.2.0")

    testImplementation(platform("org.junit:junit-bom:6.0.0"))
    testImplementation("org.junit.jupiter:junit-jupiter")
    testRuntimeOnly("org.junit.platform:junit-platform-launcher")

}

application {
    mainClass = "io.Clash.Main"
    applicationDefaultJvmArgs = listOf("--enable-native-access=ALL-UNNAMED")
}

tasks.test {
    useJUnitPlatform()
}