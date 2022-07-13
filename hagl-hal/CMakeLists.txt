cmake_minimum_required(VERSION 3.0.0)   


add_library(hagl_hal ${CMAKE_CURRENT_LIST_DIR}/src/hagl_hal.c)

target_link_libraries(hagl_hal PRIVATE hagl)
target_include_directories(hagl_hal PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
