set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main main.cpp)
add_executable(zeroalloc zero-allocation.cpp)

set(cxxflags
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

target_include_directories(main PRIVATE ../include/)
target_compile_options(main PRIVATE ${cxxflags})
set_target_properties(main PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../build/debug
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../build/release
)

target_include_directories(zeroalloc PRIVATE ../include/)
target_compile_options(zeroalloc PRIVATE ${cxxflags})
set_target_properties(zeroalloc PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../build/debug
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../build/release
)
