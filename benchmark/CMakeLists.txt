set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(benchmark benchmark.cpp)

target_include_directories(benchmark PRIVATE ../include/)
target_compile_options(benchmark PRIVATE
    -Wextra
    -Wall
    -Wpedantic
    -Werror
    --pedantic-errors
    -Wconversion
    -Wmissing-declarations
    -Wold-style-cast
    -Wno-padded
)

set_target_properties(benchmark PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../build/debug
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../build/release
)
