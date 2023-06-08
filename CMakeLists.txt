cmake_minimum_required(VERSION 3.2)

project(base64client)

set(CMAKE_CXX_STANDARD 14)

add_executable(base64client main.cpp getopt_windows.h)

target_include_directories(base64client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(UNIX)
    target_compile_definitions(base64client PRIVATE UNIX)
    target_link_libraries(base64client PRIVATE pthread)  # Link the pthread library
elseif(WIN32)
    target_compile_definitions(base64client PRIVATE WIN32)
    target_link_libraries(base64client PRIVATE ws2_32)  # Link the Winsock library
endif()
