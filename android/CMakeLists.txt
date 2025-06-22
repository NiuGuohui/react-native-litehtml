cmake_minimum_required(VERSION 3.22.1)
project(ReactNativeLitehtml)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)

# Compile sources
add_library(
        react-native-litehtml
        SHARED
        ../cpp/NativeLitehtmlModule.cpp
        ../cpp/JSLitehtml.cpp
        ../cpp/JSDocumentContainer.cpp
)

# Add headers search paths
target_include_directories(react-native-litehtml PUBLIC ../litehtml)
target_include_directories(react-native-litehtml PUBLIC ../cpp)

# Add litehtml core dependency
add_subdirectory(../litehtml litehtml)

# Add android/log dependency
find_library(log-lib log)

target_link_libraries(
        react-native-litehtml
        litehtml                        # <-- litehtml
        ${log-lib}                  # <-- Logcat logger
        android                     # <-- Android JNI core
        react_codegen_LitehtmlSpec    # <-- Generated Specs from CodeGen
)
