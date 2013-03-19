project(onvifxx)
cmake_minimum_required(VERSION 2.8)

OPTION(WITH_TESTS "Build tests." ON)

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

aux_source_directory(${PROJECT_SOURCE_DIR}/include/onvifxx INC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)
file(GLOB XML_LIST xml/*.*)

include_directories(include)
include_directories(${GSOAP_DIR}/src)

add_definitions(-DWITH_PURE_VIRTUAL)
add_library(${PROJECT_NAME} STATIC ${INC_LIST} ${SRC_LIST} ${GSOAP_LIST} ${XML_LIST} generator.sh)


if(WITH_TESTS)
    add_subdirectory(tests)
endif(WITH_TESTS)
