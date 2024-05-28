
find_package(Boost COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(echo
				main.cpp
				server.h
				server.cpp
				client.h
				client.cpp				
)

target_link_libraries(echo ${Boost_LIBRARIES})


add_library(echo_lib
				server.h
				server.cpp
				client.h
				client.cpp				
)

target_include_directories(echo_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(echo_lib ${Boost_LIBRARIES})

add_executable(echo_tests Tests/test.cpp)

target_link_libraries(echo_tests 
				echo_lib 
				${Boost_LIBRARIES}
				GTest::GTest						
)
add_test(echo_tests echo_tests)

