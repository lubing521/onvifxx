project(${CMAKE_PROJECT_NAME}_wsdd)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")

aux_source_directory(. WSDD_SOURCES)
file(GLOB WSDD_HEADERS "*.h")

include_directories(${CMAKE_SOURCE_DIR}/include)
add_definitions(-DWITH_NOIO -DWITH_SOAPDEFS_H)

add_library(${PROJECT_NAME} STATIC ${WSDD_SOURCES} ${WSDD_HEADERS})
