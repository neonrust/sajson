include(FetchContent)

# UnitTest++ code is a bit lax on conversions
add_compile_options(-Wno-conversion)

FetchContent_Declare(utpp
	GIT_REPOSITORY git@github.com:unittest-cpp/unittest-cpp.git
	GIT_TAG        v2.0.0
)
set(UTPP_INCLUDE_TESTS_IN_BUILD OFF CACHE INTERNAL "Turn off UTPP tests")
FetchContent_MakeAvailable(utpp)

set(sources
	test.cpp
)


add_executable(tests ${sources})

target_include_directories(tests
	PRIVATE
	../include/
	${utpp_SOURCE_DIR}/UnitTest++/
)

target_link_libraries(tests
	PRIVATE
	UnitTest++
)

set_target_properties(tests PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../build/debug
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../build/release
)
