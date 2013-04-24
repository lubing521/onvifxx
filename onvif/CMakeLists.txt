project(${CMAKE_PROJECT_NAME}_onvif)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

set(ONVIF_SOURCE_DIR ${PROJECT_BINARY_DIR}/src)
set(ONVIF_INCLUDE_DIR ${ONVIF_SOURCE_DIR} PARENT_SCOPE)

aux_source_directory(${ONVIF_SOURCE_DIR} ONVIF_SOURCES)

if(NOT IS_DIRECTORY ${ONVIF_SOURCE_DIR})
	file(MAKE_DIRECTORY ${ONVIF_SOURCE_DIR})

	execute_process(
		COMMAND ${GSOAP_WSDL2H} -xP -owsa5.h -t${PROJECT_SOURCE_DIR}/typemap.dat ${XML_WSDL_DIR}/ws-addr.xsd
		WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
	)

	execute_process(
		COMMAND ${GSOAP_WSDL2H} -xP -oh -t${PROJECT_SOURCE_DIR}/typemap.dat ${XML_WSDL_LIST}
		WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
	)


	execute_process(
		COMMAND ln -s ${GSOAP_IMPORT_DIR}/stlvector.h stlvector.h
		WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
	)

	execute_process(
		COMMAND ${GSOAP_SOAPCPP2} -binxL -f100 -dsrc -pOnvif h
		WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
	)

	aux_source_directory(${ONVIF_SOURCE_DIR} ONVIF_SOURCES)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB ONVIF_HEADERS ${ONVIF_SOURCE_DIR}/*.h)
aux_source_directory(${PROJECT_SOURCE_DIR} ONVIF_SOURCES)

add_library(${PROJECT_NAME} STATIC
	${ONVIF_HEADERS} ${ONVIF_SOURCES}
	${ONVIF_SOURCE_DIR}/Onvif.nsmap
	typemap.dat
)
