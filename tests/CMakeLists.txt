project(${CMAKE_PROJECT_NAME}-tests)
cmake_minimum_required(VERSION 2.6)

include_directories(
	${CMAKE_SOURCE_DIR}/include
)

aux_source_directory(. TESTS_SOURCES)
file(GLOB TEST_HEADERS "*.h")

add_executable(${PROJECT_NAME} ${TESTS_SOURCES})
target_link_libraries(${PROJECT_NAME} ${CMAKE_PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} ${CMAKE_PROJECT_NAME}-wsdd)
