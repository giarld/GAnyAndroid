cmake_minimum_required(VERSION 3.20)
project(GAnyAndroid)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_SIZEOF_VOID_P)
    if (CMAKE_CL_64)
        set(CMAKE_SIZEOF_VOID_P 8)
    else ()
        set(CMAKE_SIZEOF_VOID_P 4)
    endif ()
endif ()

if (NOT CMAKE_BUILD_TYPE)
    message(WARNING "CMAKE_BUILD_TYPE must be a valid value")
    set(CMAKE_BUILD_TYPE Release)
endif ()

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    SET(CMAKE_CXX_FLAGS "-fPIC ${CMAKE_CXX_FLAGS}")
    SET(CMAKE_C_FLAGS "-fPIC ${CMAKE_C_FLAGS}")
endif ()

if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif ()

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")

    add_compile_options(
            $<$<CONFIG:>:/MT>
            $<$<CONFIG:Debug>:/MTd>
            $<$<CONFIG:Release>:/MT>
    )
endif ()


if (NOT T_EXECUTABLE_OUTPUT_PATH)
    set(T_EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
endif ()

if (NOT T_LIBRARY_OUTPUT_PATH)
    set(T_LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${T_EXECUTABLE_OUTPUT_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${T_EXECUTABLE_OUTPUT_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${T_EXECUTABLE_OUTPUT_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${T_EXECUTABLE_OUTPUT_PATH})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${T_LIBRARY_OUTPUT_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${T_LIBRARY_OUTPUT_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${T_LIBRARY_OUTPUT_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ${T_LIBRARY_OUTPUT_PATH})

if (NOT WIN32)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${T_LIBRARY_OUTPUT_PATH})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${T_LIBRARY_OUTPUT_PATH})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${T_LIBRARY_OUTPUT_PATH})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL ${T_LIBRARY_OUTPUT_PATH})
endif ()

option(BUILD_SHARED_LIBS "build shared libs" ON)

option(DEPLOY_EXAMPLE "Enable deployment example" ON)


set(OUTPUT_LIBS_DIR ${T_LIBRARY_OUTPUT_PATH}/libs)

include(cmake/GetGitDep.cmake)

GetGitDependency(git@github.com:giarld/GxAny.git GxAny main)
add_subdirectory(deps/GxAny/gany-interface)
add_subdirectory(deps/GxAny/gany-core)

add_library(gany-droid SHARED
        src/gany-droid/src/gany_jni.cpp
        src/gany-droid/src/gany_c_api.cpp
        src/gany-droid/src/import_modules.cpp
)
target_include_directories(gany-droid PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/gany-droid/include)
target_link_libraries(gany-droid PUBLIC gany-core log)
target_compile_definitions(gany-droid PUBLIC BUILD_SHARED_LIBS=1)

add_custom_command(TARGET gany-droid POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_LIBS_DIR}/${ANDROID_ABI}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${T_LIBRARY_OUTPUT_PATH}/libgany-droid.so ${OUTPUT_LIBS_DIR}/${ANDROID_ABI}
)

if (DEPLOY_EXAMPLE)
    target_compile_definitions(gany-droid PRIVATE ENABLE_EXAMPLE=1)

    add_subdirectory(examples/TestPlugin)
    target_link_libraries(gany-droid PRIVATE test-plugin)

    set(EXAMPLE_LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples/AndroidExample/app/libs)
    add_custom_command(TARGET gany-droid POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${T_LIBRARY_OUTPUT_PATH}/libtest-plugin.so ${OUTPUT_LIBS_DIR}/${ANDROID_ABI}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${EXAMPLE_LIBS_DIR}
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${OUTPUT_LIBS_DIR} ${EXAMPLE_LIBS_DIR}
    )
endif ()
