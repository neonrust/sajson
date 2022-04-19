set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(
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

set(sources
	test.cpp

	../third-party/UnitTest++/src/AssertException.cpp
	../third-party/UnitTest++/src/Checks.cpp
	../third-party/UnitTest++/src/CurrentTest.cpp
	../third-party/UnitTest++/src/DeferredTestReporter.cpp
	../third-party/UnitTest++/src/DeferredTestResult.cpp
	../third-party/UnitTest++/src/MemoryOutStream.cpp
	../third-party/UnitTest++/src/ReportAssert.cpp
	../third-party/UnitTest++/src/Test.cpp
	../third-party/UnitTest++/src/TestDetails.cpp
	../third-party/UnitTest++/src/TestList.cpp
	../third-party/UnitTest++/src/TestReporter.cpp
	../third-party/UnitTest++/src/TestReporterStdout.cpp
	../third-party/UnitTest++/src/TestResults.cpp
	../third-party/UnitTest++/src/TestRunner.cpp
	../third-party/UnitTest++/src/TimeConstraint.cpp
	../third-party/UnitTest++/src/XmlTestReporter.cpp
)
# add platform-specific files
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux|Darwin")
	list(APPEND sources
		../third-party/UnitTest++/src/Posix/SignalTranslator.cpp
		../third-party/UnitTest++/src/Posix/TimeHelpers.cpp
	)
    set_source_files_properties(../third-party/UnitTest++/src/Posix/TimeHelpers.cpp PROPERTIES COMPILE_FLAGS -Wno-conversion)
elseif(${CMAKE_SYSTEM_NAME} EQUAL "Windows")
	list(APPEND sources
		../third-party/UnitTest++/src/Win32/TimeHelpers.cpp
	)
    set_source_files_properties(../third-party/UnitTest++/src/Win32/TimeHelpers.cpp PROPERTIES COMPILE_FLAGS -Wno-conversion)
else()
	message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif()

set_source_files_properties(../third-party/UnitTest++/src/TestRunner.cpp PROPERTIES COMPILE_FLAGS -Wno-conversion)
set_source_files_properties(../third-party/UnitTest++/src/ReportAssert.cpp PROPERTIES COMPILE_FLAGS -Wno-missing-declarations)


add_executable(tests ${sources})

target_include_directories(tests
	PRIVATE
	../include/
	../third-party/UnitTest++/src/
)

set_target_properties(tests PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../build/debug
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../build/release
)
