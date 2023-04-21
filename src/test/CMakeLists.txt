cmake_minimum_required(VERSION 3.17)
project(transmute C)
enable_testing()

set(CMAKE_C_STANDARD 99)

set(deps ../../deps/)
set(local_deps ./deps/)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(isDebug TRUE)
else()
    set(isDebug FALSE)
endif()

if (isDebug)
    message("Debug build detected")
    add_compile_definitions(CONFIGURATION_DEBUG TYRAN_CONFIGURATION_DEBUG)
    set(CONFIGURATION_DEBUG 1)
endif()

file(GLOB_RECURSE local_deps_src FOLLOW_SYMLINKS
        "${local_deps}*/src/lib/*.c"
        "${deps}*/*/src/platform/posix/*.c"
        "${deps}*/*/src/lib/*.c"
        )

add_executable(transmute_test
    main.c
    test.c
    ${local_deps_src}
)

add_test(NAME transmute_test
         COMMAND transmute_test)


target_include_directories(transmute_test PUBLIC ${deps}piot/imprint/src/include)
target_include_directories(transmute_test PUBLIC ${deps}piot/clog/src/include)
target_include_directories(transmute_test PUBLIC ${deps}piot/tiny-libc/src/include)

if (WIN32)
target_link_libraries(transmute_test transmute)
else()
target_link_libraries(transmute_test transmute m)
endif(WIN32)

