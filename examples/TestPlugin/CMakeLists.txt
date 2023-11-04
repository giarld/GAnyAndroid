cmake_minimum_required(VERSION 3.20)

project(TestPlugin)

set(TARGET_NAME test-plugin)

file(GLOB_RECURSE SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_library(${TARGET_NAME} SHARED
        ${SRCS}
)

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${TARGET_NAME} PUBLIC gany-interface)

target_compile_definitions(${TARGET_NAME} PUBLIC BUILD_SHARED_LIBS=1)
