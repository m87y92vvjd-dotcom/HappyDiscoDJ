cmake_minimum_required(VERSION 3.22)

project(HappyDiscoDJ VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(HAPPYDISCO_FETCH_JUCE "Download JUCE automatically when not provided locally" ON)
option(HAPPYDISCO_ENABLE_LTO "Enable link-time optimization in release builds" ON)
option(HAPPYDISCO_ENABLE_WARNINGS "Enable extra compiler warnings" ON)

if(HAPPYDISCO_FETCH_JUCE)
    include(FetchContent)
    FetchContent_Declare(JUCE
        GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
        GIT_TAG 8.0.4
        GIT_SHALLOW TRUE)
    FetchContent_MakeAvailable(JUCE)
else()
    find_package(JUCE CONFIG REQUIRED)
endif()

set(APP_NAME "Happy Disco DJ")
set(APP_COMPANY "Happy Mixer")
set(APP_BUNDLE_ID "com.happymixer.disco.dj")

juce_add_gui_app(HappyDiscoDJ
    PRODUCT_NAME "${APP_NAME}"
    COMPANY_NAME "${APP_COMPANY}"
    BUNDLE_ID "${APP_BUNDLE_ID}"
    VERSION "${PROJECT_VERSION}"
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT TRUE)

juce_generate_juce_header(HappyDiscoDJ)

target_sources(HappyDiscoDJ PRIVATE
    Source/Main.cpp
    Source/MainComponent.cpp Source/MainComponent.h
    Source/AudioEngine.cpp Source/AudioEngine.h
    Source/DeckComponent.cpp Source/DeckComponent.h
    Source/MixerComponent.cpp Source/MixerComponent.h
    Source/SplashComponent.cpp Source/SplashComponent.h)

target_include_directories(HappyDiscoDJ PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source")

target_compile_definitions(HappyDiscoDJ PRIVATE
    JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_REPORT_APP_USAGE=0 JUCE_APPLICATION_NAME_STRING="${APP_NAME}"
    JUCE_APPLICATION_VERSION_STRING="${PROJECT_VERSION}" NOMINMAX WIN32_LEAN_AND_MEAN)

target_link_libraries(HappyDiscoDJ PRIVATE
    juce::juce_audio_utils juce::juce_audio_devices juce::juce_audio_formats
    juce::juce_audio_processors juce::juce_dsp juce::juce_gui_extra
    juce::juce_graphics juce::juce_events juce::juce_data_structures juce::juce_core
    PUBLIC juce::juce_recommended_config_flags juce::juce_recommended_warning_flags)

if(HAPPYDISCO_ENABLE_LTO)
    target_link_libraries(HappyDiscoDJ PUBLIC juce::juce_recommended_lto_flags)
endif()

if(HAPPYDISCO_ENABLE_WARNINGS)
    if(MSVC)
        target_compile_options(HappyDiscoDJ PRIVATE /W4 /permissive- /EHsc /utf-8)
    else()
        target_compile_options(HappyDiscoDJ PRIVATE -Wall -Wextra -Wpedantic -Wshadow)
    endif()
endif()

if(APPLE)
    set_target_properties(HappyDiscoDJ PROPERTIES
        MACOSX_BUNDLE TRUE MACOSX_BUNDLE_GUI_IDENTIFIER "${APP_BUNDLE_ID}"
        MACOSX_BUNDLE_BUNDLE_NAME "${APP_NAME}"
        MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}")
endif()

include(GNUInstallDirs)
install(TARGETS HappyDiscoDJ BUNDLE DESTINATION . RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

set(CPACK_PACKAGE_NAME "HappyDiscoDJ")
set(CPACK_PACKAGE_VENDOR "${APP_COMPANY}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Happy Disco DJ desktop application")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "HappyDiscoDJ")
if(WIN32)
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_NSIS_DISPLAY_NAME "${APP_NAME}")
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
    set(CPACK_DMG_VOLUME_NAME "${APP_NAME}")
endif()
include(CPack)
