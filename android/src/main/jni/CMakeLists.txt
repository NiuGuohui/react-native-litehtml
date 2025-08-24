cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -frtti")

project("react-native-litehtml")

add_library(${CMAKE_PROJECT_NAME} SHARED
        jni-native-call.cpp
        android-container.cpp
        native-lib.cpp)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ./)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ../../../../litehtml/include)

# Add litehtml dependency
set(LITEHTML_LIB_DIR ./lib/${ANDROID_ABI})
file(GLOB LITEHTML_LIBS "${LITEHTML_LIB_DIR}/liblitehtml.a")


target_link_libraries(${CMAKE_PROJECT_NAME}
        android
        log
        ${LITEHTML_LIBS})
