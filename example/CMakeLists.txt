add_executable(main main.cpp)

target_include_directories(main PRIVATE ../include/)

set_target_properties(main PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../build/debug
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../build/release
)


add_executable(zeroalloc zero-allocation.cpp)

target_include_directories(zeroalloc PRIVATE ../include/)

set_target_properties(zeroalloc PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../build/debug
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../build/release
)


add_executable(dump dump.cpp ../include/sajson_dump.h)

target_include_directories(dump PRIVATE ../include/)

set_target_properties(dump PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../build/debug
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../build/release
)
