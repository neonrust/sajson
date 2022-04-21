add_executable(benchmark benchmark.cpp)

target_include_directories(benchmark PRIVATE ../include/)

set_target_properties(benchmark PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../build/debug
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../build/release
)
