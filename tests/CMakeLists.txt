# --- Unit Tests

file(GLOB TEST_SOURCES "Test-*.cpp")

foreach (TEST_SRC ${TEST_SOURCES})
    # Test Source File and Name
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
    string(TOLOWER ${TEST_NAME} EXE_NAME)
    
    # Test Target
    add_executable(${EXE_NAME} ${TEST_SRC})
    
    # Test Dependencies
    target_include_directories(${EXE_NAME} PRIVATE "lest/include")

    target_link_libraries(${EXE_NAME} ECS)
    add_dependencies(${EXE_NAME} ECS)
	
	# Disable Specific Warnings
	if (MSVC)
		target_compile_options(${EXE_NAME} PRIVATE "/IGNORE:4389")
	else()
		target_compile_options(${EXE_NAME} PRIVATE "-Wno-missing-braces" "-Wno-sign-compare")
	endif()
    
    # Test Command
    add_test(NAME ${TEST_NAME} COMMAND ${EXE_NAME} "-p")
endforeach()
