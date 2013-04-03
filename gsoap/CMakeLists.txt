project(${CMAKE_PROJECT_NAME}_gsoap)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-fPIC")

find_package(GSOAP)
if(GSOAP_FOUND)
	set(GSOAP_PATH ${gsoap_path_found} CACHE PATH "path where gsoap is installed" FORCE)
else(GSOAP_FOUND)
	message(FATAL_ERROR
	"could not find gsoap! please specify path to gsoap either with the "
	"gsoap_path cmake variable or gsoap_path environment variable. see "
	"for more information.")
endif(GSOAP_FOUND)

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

file(GLOB GSOAP_SOURCES stdsoap2.c)
file(GLOB GSOAP_HEADERS ${PROJECT_BINARY_DIR}/src/*.h)
add_library(${PROJECT_NAME} STATIC
	${GSOAP_SOURCES} ${GSOAP_HEADERS}
)
