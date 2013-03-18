project(onvif)
cmake_minimum_required(VERSION 2.8)

#file(GLOB_RECURSE FILE_LIST ${PROJECT_BINARY_DIR}/*h.txt)
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

#foreach(FILE ${FILE_LIST})
#    get_filename_component(FILE_DIR ${FILE} PATH)
#    add_subdirectory(${FILE_DIR} ${FILE_DIR}_build)
#    include_directories(${FILE_DIR})
#endforeach(FILE ${FILE_LIST})

aux_source_directory(${PROJECT_SOURCE_DIR}/include/onvif INC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)
file(GLOB XML_LIST xml/*.*)

include_directories(include)
include_directories(${GSOAP_DIR}/src)

#add_library(${PROJECT_NAME} STATIC ${GSOAP_LIST} ${XML_LIST} generator.sh)
#set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS -fPIC)
#add_library(${PROJECT_NAME} SHARED ${INC_LIST} ${SRC_LIST} ${GSOAP_LIST} ${XML_LIST} generator.sh)
#target_link_libraries(${PROJECT_NAME} gsoap++)
add_library(${PROJECT_NAME} STATIC ${INC_LIST} ${SRC_LIST} ${GSOAP_LIST} ${XML_LIST} generator.sh)

#add_library(${PROJECT_NAME}c SHARED ${INC_LIST} ${SRC_LIST})
#target_link_libraries(${PROJECT_NAME}_core ${PROJECT_NAME} gsoap++)
