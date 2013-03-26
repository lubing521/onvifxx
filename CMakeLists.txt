project(onvifxx)
cmake_minimum_required(VERSION 2.8)

if(NOT CMAKE_COMPILER_IS_GNUCXX)
	message(FATAL_ERROR "Use gcc compiler!")
endif(NOT CMAKE_COMPILER_IS_GNUCXX)

OPTION(WITH_TESTS "Build tests." ON)
OPTION(ONLY_SERVER "Build server." OFF)
OPTION(ONLY_CLIENT "Build client." OFF)

if(ONLY_SERVER AND ONLY_CLIENT)
	message(FATAL_ERROR "Set just one option ONLY_SERVER or ONLY_CLIENT!")
endif(ONLY_SERVER AND ONLY_CLIENT)

set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")
set(GSOAP_DIR ${PROJECT_BINARY_DIR}/gsoap)
file(MAKE_DIRECTORY ${GSOAP_DIR})

aux_source_directory(${GSOAP_DIR}/src GSOAP_SOURCES)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SOURCES)

if(NOT "${GSOAP_SOURCES}")
	if(ONLY_SERVER)
		set(GENERATOR_OPTION "S")
		file(GLOB CLIENT_SOURCES src/*_c.cpp)
		LIST(REMOVE_ITEM SOURCES ${CLIENT_SOURCES})
	elseif(ONLY_CLIENT)
		set(GENERATOR_OPTION "C")
		file(GLOB SERVER_SOURCES src/*_c.cpp)
		LIST(REMOVE_ITEM SOURCES ${SERVER_SOURCES})
	# else no option and do not filter SOURCES
	endif(ONLY_SERVER)

	message(STATUS "Running generator with option ${GENERATOR_OPTION}")
	execute_process(
		COMMAND ${PROJECT_SOURCE_DIR}/generator.sh ${GENERATOR_OPTION}
		WORKING_DIRECTORY ${GSOAP_DIR}
	)

	aux_source_directory(${GSOAP_DIR}/src GSOAP_SOURCES)
endif(NOT "${GSOAP_SOURCES}")

file(GLOB XML_LIST xml/*.*)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${GSOAP_DIR}/src)

add_definitions(-DWITH_PURE_VIRTUAL)
add_library(${PROJECT_NAME} STATIC
	${SOURCES} ${GSOAP_SOURCES} ${XML_LIST}
	generator.sh typemap.dat
)
target_link_libraries(${PROJECT_NAME} gsoap++)

if(WITH_TESTS)
	 add_subdirectory(tests)
endif(WITH_TESTS)
