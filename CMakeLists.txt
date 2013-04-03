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

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++0x")

add_subdirectory(gsoap)
add_subdirectory(wsdd)

if(ONLY_SERVER)
	file(GLOB SOURCES src/*_s.cpp)
elseif(ONLY_CLIENT)
	file(GLOB SOURCES src/*_c.cpp)
else(ONLY_SERVER)
	aux_source_directory(${PROJECT_SOURCE_DIR}/src SOURCES)
endif(ONLY_SERVER)

file(GLOB XML_FILES xml/*.*)
file(GLOB_RECURSE HEADERS  ${PROJECT_SOURCE_DIR}/include/*.hpp)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${CMAKE_BINARY_DIR}/gsoap/src)

add_library(${PROJECT_NAME} SHARED
	${SOURCES} ${HEADERS} ${XML_FILES}
	generator.sh typemap.dat
)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
find_package(gsoap)
if(gsoap_found)
	set(gsoap_path ${gsoap_path_found} cache path "path where gsoap is installed" force)
else(gsoap_found)
	message(fatal_error
	"could not find gsoap! please specify path to gsoap either with the "
	"gsoap_path cmake variable or gsoap_path environment variable. see "
	"for more information.")
endif(gsoap_found)

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_gsoap ${PROJECT_NAME}_wsdd gsoap)

if(WITH_TESTS)
	 add_subdirectory(tests)
endif(WITH_TESTS)
