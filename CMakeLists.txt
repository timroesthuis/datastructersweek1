cmake_minimum_required(VERSION 3.8)
project(lists LANGUAGES CXX)

set(main_exec_name main)
set(dummy_exec_name dummy)
set(lib_name lib_list)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

message("Detected compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
if ((MINGW OR MSYS) AND WIN32)
    set(WIN_GNU TRUE)
    message("Gnu Windows : ${WIN_GNU}")
endif()

find_program(CPPCHECK NAMES cppcheck)
if (CPPCHECK)
    message(STATUS "Found cppcheck: ${CPPCHECK}")
    set(CMAKE_CXX_CPPCHECK ${CPPCHECK} --enable=warning,performance,portability,style --language=c++ --force --verbose ${PROJECT_SOURCE_DIR}/src/include/*.h)
else()
    message(STATUS "cppcheck not found - consider adding it to the path.")
endif ()

set(SOURCE_FILES_MAIN main.cpp)
add_executable(${main_exec_name} ${SOURCE_FILES_MAIN})
target_link_libraries(${main_exec_name} ${lib_name})


set(LIBS_GNU_DEBUG "-fsanitize=address -fsanitize=undefined")
set(COMPILE_OPTIONS_GNU_DEBUG -O0 -g3 -ggdb)

option(ENABLE_SANITIZERS "Enable memory and undefined behavior sanitizers." ON)
option(LINK_GOLD "Use gold linker on *nix." OFF)


if (UNIX AND ENABLE_SANITIZERS)
    set(COMPILE_OPTIONS_GNU_DEBUG ${COMPILE_OPTIONS_GNU_DEBUG} -fsanitize=address -fsanitize=pointer-compare -fsanitize=leak -fsanitize=undefined -fno-omit-frame-pointer)
    message(STATUS "Sanitizers enabled for *nix.")
elseif (UNIX)
    message(STATUS "Sanitizers disabled by user for *nix.")
endif()

if (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") AND UNIX AND (LINK_GOLD))
    message(STATUS "Linking with ld-gold")
    set(CMAKE_EXE_LINKER_FLAGS, ${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold)
endif ()

target_compile_options(${main_exec_name} PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic -Werror>
        $<$<CXX_COMPILER_ID:GNU>:$<$<CONFIG:Debug>:${COMPILE_OPTIONS_GNU_DEBUG}>>
        $<$<CXX_COMPILER_ID:GNU>:$<$<CONFIG:Release>:-O3>>
        )


target_compile_options(${main_exec_name} PRIVATE
        $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic -Werror>
        $<$<CXX_COMPILER_ID:Clang>:$<$<CONFIG:Debug>:${COMPILE_OPTIONS_GNU_DEBUG}>>
        $<$<CXX_COMPILER_ID:Clang>:$<$<CONFIG:Release>:-O3>>
        )


target_compile_options(${main_exec_name} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<CXX_COMPILER_ID:MSVC>:$<$<CONFIG:Debug>:/RTC1 /Od /Zi>>
        $<$<CXX_COMPILER_ID:MSVC>:$<$<CONFIG:Release>:/O2>>
        )


if (UNIX AND ENABLE_SANITIZERS AND ((${CMAKE_CXX_COMPILER_ID} MATCHES "GNU") OR (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")))
    target_link_libraries(${main_exec_name} $<$<CONFIG:Debug>:${LIBS_GNU_DEBUG}>)
endif ()


enable_testing()

add_subdirectory(tests)
add_subdirectory(src)
