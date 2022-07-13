FILE(GLOB Sources ${CMAKE_CURRENT_LIST_DIR}/src/*.c ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
add_library(remote-access   ${CMAKE_CURRENT_LIST_DIR}/src/wrapper.c
                            ${CMAKE_CURRENT_LIST_DIR}/src/packet_parser.cpp
                            ${CMAKE_CURRENT_LIST_DIR}/src/cdc_dev.c
                            ${CMAKE_CURRENT_LIST_DIR}/src/PacketHolder.cpp)
target_include_directories(remote-access PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(remote-access PRIVATE hagl)