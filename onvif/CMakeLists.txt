project(${CMAKE_PROJECT_NAME}_onvif)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

set(ONVIF_SOURCE_DIR ${PROJECT_BINARY_DIR}/src)
set(ONVIF_INCLUDE_DIR ${ONVIF_SOURCE_DIR} PARENT_SCOPE)

aux_source_directory(${ONVIF_SOURCE_DIR} ONVIF_SOURCES)

if(NOT IS_DIRECTORY ${ONVIF_SOURCE_DIR})
	file(MAKE_DIRECTORY ${ONVIF_SOURCE_DIR})

	execute_process(
		COMMAND ${GSOAP_WSDL2H} -gxP -owsa5.h -t${PROJECT_SOURCE_DIR}/typemap.dat ${XML_WSDL_DIR}/ws-addr.xsd
		WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
	)

	execute_process(
		COMMAND ${GSOAP_WSDL2H} -gxP -oh -t${PROJECT_SOURCE_DIR}/typemap.dat ${XML_WSDL_DIR}/devicemgmt.wsdl
		WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
	)

	execute_process(
		COMMAND ln -s ${GSOAP_IMPORT_DIR}/stlvector.h stlvector.h
		WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
	)

		execute_process(
				COMMAND ln -s ${GSOAP_IMPORT_DIR}/soap12.h soap12.h
				WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
		)

	execute_process(
		COMMAND ${GSOAP_SOAPCPP2} -abinxL -f100 -dsrc -pOnvif h
		WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
	)

	aux_source_directory(${ONVIF_SOURCE_DIR} ONVIF_SOURCES)
endif()

include_directories(
	${CMAKE_SOURCE_DIR}/include
	${GSOAP_INCLUDE_DIR}
)

file(GLOB ONVIF_HEADERS ${ONVIF_SOURCE_DIR}/*.h)

file(GLOB WSDD_HEADERS ${WSDD_SOURCE_DIR}/*.h)
if(ONLY_SERVER)
	file(GLOB ONVIF_SOURCES ${PROJECT_SOURCE_DIR}/*_service.cpp)
elseif(ONLY_CLIENT)
	file(GLOB ONVIF_SOURCES ${PROJECT_SOURCE_DIR}/*_proxy.cpp)
else(ONLY_SERVER)
	aux_source_directory(${PROJECT_SOURCE_DIR} ONVIF_SOURCES)
endif(ONLY_SERVER)

add_definitions(-DWITH_PURE_VIRTUAL)

add_library(${PROJECT_NAME} STATIC
	typemap.dat
	${ONVIF_HEADERS} ${ONVIF_SOURCES}
)
