cmake_minimum_required(VERSION 3.15)

#
# Project setup and configuration
#
set(PROJECT_NAME "CloudCrusher")
set(PROJECT_VERSION 0.2.0)

project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

include(CTest)
option(BUILD_TESTING "Build the testing tree." ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_GTEST_DISCOVER_TESTS_DISCOVERY_MODE PRE_TEST)

set(${PROJECT_NAME}_Sources
    src/processors/BoxMullerNoise.cpp
    src/processorsDelay.cpp
    src/components/Dial.cpp
    src/PluginProcessor.cpp
    src/PluginEditor.cpp
)

#
# Add googletest as a dependency
#
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/32f9f4c82afa4249af66b55278df15c16b3031ea.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

#
# Setup project as JUCE VST3 plugin
#
add_subdirectory(JUCE)
juce_add_plugin(${PROJECT_NAME}
    COMPANY_NAME "GlossolaliaAudio"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE Glos
    PLUGIN_CODE ClCr
    FORMATS VST3
    PRODUCT_NAME ${PROJECT_NAME}
)

# Make template headers available to all targets
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/components
    ${CMAKE_CURRENT_SOURCE_DIR}/include/processors
)

# Create the main plugin target
target_sources(${PROJECT_NAME} PRIVATE
    ${PROJECT_NAME}_Sources
)
target_compile_definitions(${PROJECT_NAME} PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)
target_link_libraries(${PROJECT_NAME}
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
if(BUILD_TESTING)
    enable_testing()
    add_test(
        NAME ${PROJECT_NAME}Tests
        COMMAND ${PROJECT_NAME}Test
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )
endif()

# Replace existing gtest_discover_tests with:
add_executable(${PROJECT_NAME}Test
    ${PROJECT_NAME}_Sources
    test/utils/ConstNoise.cpp
    test/TestBoxMullerNoise.cpp
    test/TestBitCrusher.cpp
    test/TestPluginProcessor.cpp
    test/TestDelay.cpp
    test/main.cpp
)
target_include_directories(${PROJECT_NAME}Test PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/components
    ${CMAKE_CURRENT_SOURCE_DIR}/include/processors
)

target_link_libraries(${PROJECT_NAME}Test
    PRIVATE
        GTest::gtest_main
        juce::juce_audio_processors
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
include(GoogleTest)
