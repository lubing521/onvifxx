project(${CMAKE_PROJECT_NAME}-wsdd)
cmake_minimum_required(VERSION 2.6)

include_directories(
	${CMAKE_SOURCE_DIR}/include
)

aux_source_directory(. WSDD_SOURCES)
file(GLOB WSDD_HEADERS "*.h")

add_library(${PROJECT_NAME} SHARED ${WSDD_SOURCES} ${WSDD_HEADERS})
target_link_libraries(${PROJECT_NAME} gsoap)
