project(${CMAKE_PROJECT_NAME}_sock)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

include_directories(${CMAKE_SOURCE_DIR}/include)

aux_source_directory(. SOCK_SOURCES)

add_library(${PROJECT_NAME} STATIC ${SOCK_SOURCES} ${SOCK_HEADERS})
