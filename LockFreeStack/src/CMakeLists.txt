
# find_package(Boost COMPONENTS thread program_options REQUIRED)
# include_directories(${Boost_INCLUDE_DIR} ${BOOST_LOCKFREE_DIR})


add_executable(stack
				main.cpp
                atomic_stamped_ptr.hpp 
                lock_free_stack.hpp
)

# target_compile_options(stack PRIVATE -g -fsanitize=leak -fsanitize=address) 
# target_link_options(stack PRIVATE -g -fsanitize=leak  -fsanitize=address) 


# target_link_libraries(stack ${Boost_LIBRARIES})