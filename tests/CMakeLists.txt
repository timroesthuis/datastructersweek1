project(tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

message("loading ${PROJECT_NAME}")


add_subdirectory(lib/gtest/googletest)
add_subdirectory(list)


