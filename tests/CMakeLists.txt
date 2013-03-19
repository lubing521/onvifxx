project(${CMAKE_PROJECT_NAME}-tests)
cmake_minimum_required(VERSION 2.6)

include_directories(
    ${onvif_SOURCE_DIR}/include
)

aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} onvifxx gsoap gsoap++)
