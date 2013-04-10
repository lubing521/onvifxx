project(onvifxx)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_BUILD_TYPE Debug)

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
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

if(ONLY_SERVER)
	file(GLOB SOURCES src/*_s.cpp)
elseif(ONLY_CLIENT)
	file(GLOB SOURCES src/*_c.cpp)
else(ONLY_SERVER)
	aux_source_directory(${PROJECT_SOURCE_DIR}/src SOURCES)
endif(ONLY_SERVER)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.49.0 COMPONENTS system)

set(GSOAP_LOCATION "/usr")
find_package(GSOAP REQUIRED)

add_subdirectory(sock)
add_subdirectory(gsoap)
add_subdirectory(wsdd)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${CMAKE_BINARY_DIR}/gsoap/src)

file(GLOB XML_FILES xml/*.*)
file(GLOB_RECURSE HEADERS  ${PROJECT_SOURCE_DIR}/include/*.hpp)

add_library(${PROJECT_NAME} SHARED
	${SOURCES} ${HEADERS} ${XML_FILES}
	generator.sh typemap.dat
)
target_link_libraries(${PROJECT_NAME}
	${PROJECT_NAME}_sock ${PROJECT_NAME}_gsoap ${PROJECT_NAME}_wsdd
	${Boost_LIBRARIES} gsoap++)

if(WITH_TESTS)
	 add_subdirectory(tests)
endif(WITH_TESTS)
