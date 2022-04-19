cmake_minimum_required(VERSION 3.11)

project(sajson
	DESCRIPTION "sajson"
	LANGUAGES CXX
)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(SAJSON_BUILD_BENCHMARKS "Build benchmarks. Default=ON" ON)
option(SAJSON_BUILD_EXAMPLES   "Build examples.   Default=ON" ON)
option(SAJSON_BUILD_TESTS      "Build tests.      Default=ON" ON)


if(SAJSON_BUILD_EXAMPLES)
	# demo application(s)
	add_subdirectory(example)
endif()

if(SAJSON_BUILD_BENCHMARKS)
	# demo application(s)
	add_subdirectory(benchmark)
endif()

if(SAJSON_BUILD_TESTS)
	if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
		include(CTest)
	endif()
	if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
		add_subdirectory(tests)
	endif()
endif()
