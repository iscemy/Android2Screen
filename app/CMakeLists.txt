
# add the executable
add_executable(app src/main.cpp)
target_link_libraries(app PRIVATE remote-access)
target_link_libraries(app PRIVATE hagl)
