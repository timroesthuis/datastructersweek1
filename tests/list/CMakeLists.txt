find_program(CPPCHECK NAMES cppcheck)
if (CPPCHECK)
    message(STATUS "Found cppcheck: ${CPPCHECK}")
    set(CMAKE_CXX_CPPCHECK ${CPPCHECK} --enable=warning,performance,portability,style --language=c++ --force --verbose ${PROJECT_SOURCE_DIR}/src/include/*.h)
endif ()

list(APPEND targets tests_custom tests_singly tests_doubly)
list(APPEND sources custom_tests.cpp forward_list_tests.cpp list_tests.cpp)

list(LENGTH targets n_targets)
math(EXPR n_loop "${n_targets}-1")

foreach(ind RANGE ${n_loop})
    list(GET targets ${ind} test_exec_name)
    list(GET sources ${ind} tests_source)

    message(STATUS "Creating test target: ${test_exec_name}")


    add_executable(${test_exec_name} test_runner.cpp ${tests_source})

    target_include_directories(${test_exec_name} SYSTEM
        INTERFACE ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


    target_link_libraries(${test_exec_name} gtest gtest_main)
    target_link_libraries(${test_exec_name} ${lib_name})

    target_compile_options(${test_exec_name} PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic -Werror>
        $<$<CXX_COMPILER_ID:GNU>:$<$<CONFIG:Debug>:${COMPILE_OPTIONS_GNU_DEBUG}>>
        $<$<CXX_COMPILER_ID:GNU>:$<$<CONFIG:Release>:-O3>>
        )


    target_compile_options(${test_exec_name} PRIVATE
        $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic -Werror>
        $<$<CXX_COMPILER_ID:Clang>:$<$<CONFIG:Debug>:${COMPILE_OPTIONS_GNU_DEBUG}>>
        $<$<CXX_COMPILER_ID:Clang>:$<$<CONFIG:Release>:-O3>>
        )

    target_compile_options(${test_exec_name} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<CXX_COMPILER_ID:MSVC>:$<$<CONFIG:Debug>:/RTC1 /Od /Zi>>
        $<$<CXX_COMPILER_ID:MSVC>:$<$<CONFIG:Release>:/O2>>
        )

    if (UNIX AND ENABLE_SANITIZERS AND ((${CMAKE_CXX_COMPILER_ID} MATCHES "GNU") OR (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")))
        target_link_libraries(${test_exec_name} $<$<CONFIG:Debug>:${LIBS_GNU_DEBUG}>)
    endif ()

    add_test(unit_${test_exec_name} ${test_exec_name})

endforeach()