cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -frtti")

project("react-native-litehtml")


add_subdirectory(../../../../cpp/litehtml ${CMAKE_BINARY_DIR}/litehtml)

add_library(${CMAKE_PROJECT_NAME} SHARED
        jni-native-call.cpp
        android-container.cpp
        native-lib.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME}
        android
        log
        litehtml)
