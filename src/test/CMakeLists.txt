set(TEST_FILES
        test_main.cpp)

add_executable(test ${TEST_FILES})
target_link_libraries(test doctest)
