project(${CMAKE_PROJECT_NAME}-wsdd)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

set(WSDD_SOURCE_DIR ${PROJECT_BINARY_DIR}/src)
set(WSDD_INCLUDE_DIR ${WSDD_SOURCE_DIR} PARENT_SCOPE)

aux_source_directory(${WSDD_SOURCE_DIR} WSDD_SOURCES)

if(NOT IS_DIRECTORY ${WSDD_SOURCE_DIR})
	file(MAKE_DIRECTORY ${WSDD_SOURCE_DIR})

	execute_process(
		COMMAND ${GSOAP_WSDL2H} -gxP -oh -t${PROJECT_SOURCE_DIR}/typemap.dat ${XML_WSDL_DIR}/remotediscovery.wsdl
		WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
	)

	execute_process(
		COMMAND ln -s ${GSOAP_IMPORT_DIR}/stlvector.h stlvector.h
		WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
	)

	file(APPEND ${PROJECT_BINARY_DIR}/h
		"struct SOAP_ENV__Header {\n"
			"std::string wsa__MessageID;\n"
			"wsa__Relationship *wsa__RelatesTo;\n"
			"wsa__EndpointReferenceType *wsa__From;\n"
			"wsa__EndpointReferenceType *wsa__ReplyTo;\n"
			"wsa__EndpointReferenceType *wsa__FaultTo;\n"
			"std::string  wsa__To;\n"
			"std::string  wsa__Action;\n"
			"wsdd__AppSequenceType *wsdd__AppSequence;\n"
		"};"
	)

	execute_process(
		COMMAND ${GSOAP_SOAPCPP2} -2abinxL -dsrc -pWsdd h
		WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
	)

	aux_source_directory(${WSDD_SOURCE_DIR} WSDD_SOURCES)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${GSOAP_IMPORT_DIR}/../plugin)
include_directories(${WSDD_SOURCE_DIR})

file(GLOB WSDD_HEADERS ${WSDD_SOURCE_DIR}/*.h)
if(ONLY_SERVER)
	file(GLOB WSDD_SOURCES ${PROJECT_SOURCE_DIR}/*_service.cpp)
elseif(ONLY_CLIENT)
	file(GLOB WSDD_SOURCES ${PROJECT_SOURCE_DIR}/*_proxy.cpp)
else(ONLY_SERVER)
	aux_source_directory(${PROJECT_SOURCE_DIR} WSDD_SOURCES)
endif(ONLY_SERVER)

add_definitions(-DWITH_PURE_VIRTUAL)

add_library(${PROJECT_NAME} SHARED
	${WSDD_HEADERS} ${WSDD_SOURCES}
	${WSDD_SOURCE_DIR}/Wsdd.nsmap
	typemap.dat
	${Boost_LIBRARIES} ${GSOAP_LIBRARIES}
)
