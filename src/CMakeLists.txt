cmake_minimum_required(VERSION 3.8)

project(${lib_name} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

set(HEADERS_FILES_LIB
        ${CMAKE_CURRENT_SOURCE_DIR}/include/list.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/forward_list.h)

set(SOURCE_FILES_DUMMY dummy.cpp)

add_library(${lib_name} INTERFACE)
target_sources(${lib_name} INTERFACE ${HEADERS_FILES_LIB})

target_include_directories(${lib_name} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/)

#set_target_properties(lib_lists PROPERTIES LINKER_LANGUAGE CXX)

find_program(CPPCHECK NAMES cppcheck)
if (CPPCHECK)
    message(STATUS "Found cppcheck: ${CPPCHECK}")
    set(CMAKE_CXX_CPPCHECK ${CPPCHECK} --enable=warning,performance,portability,style --language=c++ --force --verbose ${PROJECT_SOURCE_DIR}/src/include/*.h)
endif ()

add_executable(${dummy_exec_name} ${SOURCE_FILES_DUMMY})
target_link_libraries(${dummy_exec_name} ${lib_name})

target_compile_options(${dummy_exec_name} PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic -Werror>
        $<$<CXX_COMPILER_ID:GNU>:$<$<CONFIG:Debug>:${COMPILE_OPTIONS_GNU_DEBUG}>>
        $<$<CXX_COMPILER_ID:GNU>:$<$<CONFIG:Release>:-O3>>
        )


target_compile_options(${dummy_exec_name} PRIVATE
        $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic -Werror>
        $<$<CXX_COMPILER_ID:Clang>:$<$<CONFIG:Debug>:${COMPILE_OPTIONS_GNU_DEBUG}>>
        $<$<CXX_COMPILER_ID:Clang>:$<$<CONFIG:Release>:-O3>>
        )

target_compile_options(${dummy_exec_name} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<CXX_COMPILER_ID:MSVC>:$<$<CONFIG:Debug>:/RTC1 /Od /Zi>>
        $<$<CXX_COMPILER_ID:MSVC>:$<$<CONFIG:Release>:/O2>>
        )

if (UNIX AND ENABLE_SANITIZERS AND ((${CMAKE_CXX_COMPILER_ID} MATCHES "GNU") OR (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")))
    target_link_libraries(${dummy_exec_name} $<$<CONFIG:Debug>:${LIBS_GNU_DEBUG}>)
endif ()

