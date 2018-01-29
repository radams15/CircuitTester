set(TEST_FILES
        test_main.cpp)

add_executable(Test ${TEST_FILES})
target_link_libraries(Test doctest)
