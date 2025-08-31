cmake_minimum_required(VERSION 3.15)

project(CloudCrusher VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_GTEST_DISCOVER_TESTS_DISCOVERY_MODE PRE_TEST)
set(CloudCrusher_Sources
    src/BoxMullerNoise.cpp
    src/PluginProcessor.cpp
    src/PluginEditor.cpp
    src/components/Dial.cpp
)

# Add googletest as a dependency
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/32f9f4c82afa4249af66b55278df15c16b3031ea.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Setup JUCE
add_subdirectory(JUCE)
juce_add_plugin(CloudCrusher
    COMPANY_NAME "GlossolaliaAudio"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE Glos
    PLUGIN_CODE ClCr
    FORMATS VST3
    PRODUCT_NAME "CloudCrusher"
)

# Make template headers available to all targets
target_include_directories(CloudCrusher PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/components
)

# Create the main plugin target
target_sources(CloudCrusher PRIVATE
    ${CloudCrusher_Sources}
)
target_compile_definitions(CloudCrusher PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)
target_link_libraries(CloudCrusher
    PRIVATE
        juce::juce_core
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_gui_basics
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# Create plugin test target
enable_testing()
add_executable(CloudCrusherTest
    ${CloudCrusher_Sources}
    test/utils/ConstNoise.cpp
    test/TestBoxMullerNoise.cpp
    test/TestBitCrusher.cpp
    test/TestPluginProcessor.cpp
    test/main.cpp
)
target_include_directories(CloudCrusherTest PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/components
)

target_link_libraries(CloudCrusherTest
    PRIVATE
        GTest::gtest_main
        juce::juce_audio_processors
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
include(GoogleTest)
gtest_discover_tests(CloudCrusherTest)
