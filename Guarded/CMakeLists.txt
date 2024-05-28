
add_executable(guarded
				main.cpp
				Guarded.h			
)

add_executable(guarded_tests TestGuarded/test.cpp)
target_link_libraries(guarded_tests GTest::GTest)

add_test(
	NAME guarded_tests
	COMMAND guarded_tests
)