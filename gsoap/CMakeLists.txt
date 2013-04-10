project(${CMAKE_PROJECT_NAME}_gsoap)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

aux_source_directory(${PROJECT_BINARY_DIR}/src GSOAP_SOURCES)
if(NOT "${GSOAP_SOURCES}")
	if(ONLY_SERVER)
		set(GENERATOR_OPTION "S")
	elseif(ONLY_CLIENT)
		set(GENERATOR_OPTION "C")
	#else(ONLY_SERVER)
	#	set(GENERATOR_OPTION "SC")
	endif(ONLY_SERVER)

	message(STATUS "Running generator with option ${GENERATOR_OPTION}")
	execute_process(
		COMMAND ${CMAKE_SOURCE_DIR}/generator.sh ${GENERATOR_OPTION}
		WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
	)

	aux_source_directory(${PROJECT_BINARY_DIR}/src GSOAP_SOURCES)
endif(NOT "${GSOAP_SOURCES}")

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB GSOAP_HEADERS ${PROJECT_BINARY_DIR}/src/*.h)

add_library(${PROJECT_NAME} STATIC
	${GSOAP_SOURCES} ${GSOAP_HEADERS}
)
