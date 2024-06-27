
add_executable(stack_test
                unit/stack.cpp 
)

target_link_libraries(stack_test PRIVATE GTest::GTest)

target_compile_options(stack_test PRIVATE -g -fsanitize=leak -fsanitize=address )
target_link_options(stack_test PRIVATE -g -fsanitize=leak -fsanitize=address )
# target_compile_options(stack_test PRIVATE -g -fsanitize=thread )
# target_link_options(stack_test PRIVATE -g -fsanitize=thread )

add_test(
	NAME stack_test
	COMMAND stack_test
)