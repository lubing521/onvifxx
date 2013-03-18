project(onvif)
cmake_minimum_required(VERSION 2.8)

OPTION(BUILD_TESTS "Build tests." ON)

set(GSOAP_DIR ${PROJECT_BINARY_DIR}/gsoap)
file(MAKE_DIRECTORY ${GSOAP_DIR})

aux_source_directory(${GSOAP_DIR}/src GSOAP_LIST)
if(NOT "${GSOAP_LIST}")
    execute_process(
        COMMAND ${PROJECT_SOURCE_DIR}/generator.sh
        WORKING_DIRECTORY ${GSOAP_DIR}
    )
    aux_source_directory(${GSOAP_DIR}/src GSOAP_LIST)
endif(NOT "${GSOAP_LIST}")

aux_source_directory(${PROJECT_SOURCE_DIR}/include/onvif INC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)
file(GLOB XML_LIST xml/*.*)

include_directories(include)
include_directories(${GSOAP_DIR}/src)

add_library(${PROJECT_NAME} STATIC ${INC_LIST} ${SRC_LIST} ${GSOAP_LIST} ${XML_LIST} generator.sh)


if(BUILD_TESTS)
    add_subdirectory(tests)
endif(BUILD_TESTS)
