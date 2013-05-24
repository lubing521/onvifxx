project(onvifxx)
cmake_minimum_required(VERSION 2.6)

# INITIALIZE
#-DENABLE_DEBUG=TRUE -DGSOAP_ROOT=/usr/local -DBOOST_ROOT=/usr/local

if(ENABLE_DEBUG)
	set(CMAKE_BUILD_TYPE Debug)
endif()

if(NOT CMAKE_COMPILER_IS_GNUCXX)
	message(FATAL_ERROR "Use gcc compiler!")
endif(NOT CMAKE_COMPILER_IS_GNUCXX)

OPTION(WITH_TESTS "Build tests." ON)
OPTION(ONLY_SERVER "Build server." OFF)
OPTION(ONLY_CLIENT "Build client." OFF)

# PREPARE

if(ONLY_SERVER AND ONLY_CLIENT)
	message(FATAL_ERROR "Set just one option ONLY_SERVER or ONLY_CLIENT!")
endif(ONLY_SERVER AND ONLY_CLIENT)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++0x")
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.49.0 REQUIRED COMPONENTS system thread)
message(STATUS "The Boost version: ${Boost_VERSION}")
message(STATUS "The Boost include directory: ${Boost_INCLUDE_DIR}")
message(STATUS "The Boost libraries: ${Boost_LIBRARIES})")


find_package(GSOAP 2.8.15 REQUIRED)
message(STATUS "The GSOAP version: ${GSOAP_VERSION}")
message(STATUS "The GSOAP include directory: ${GSOAP_INCLUDE_DIR}")
message(STATUS "The GSOAP libraries: ${GSOAP_LIBRARIES}")

# BUILD

if(ONLY_SERVER)
	file(GLOB SOURCES src/*_service.cpp)
elseif(ONLY_CLIENT)
	file(GLOB SOURCES src/*_proxy.cpp)
else(ONLY_SERVER)
	aux_source_directory(${PROJECT_SOURCE_DIR}/src SOURCES)
endif(ONLY_SERVER)


add_subdirectory(xml)
#add_subdirectory(sock)
add_subdirectory(onvif)
add_subdirectory(wsdd)

include_directories(
	${PROJECT_SOURCE_DIR}/include
	${ONVIF_INCLUDE_DIR}
	${WSDD_INCLUDE_DIR}
	${GSOAP_INCLUDE_DIR}
)

#message(FATAL_ERROR "** ${Boost_INCLUDE_DIR} **")

file(GLOB_RECURSE HEADERS  ${PROJECT_SOURCE_DIR}/include/*.hpp)
aux_source_directory(${PROJECT_SOURCE_DIR} SOURCES)

add_library(${PROJECT_NAME} SHARED
	${HEADERS} ${SOURCES}
)

target_link_libraries(${PROJECT_NAME}
#	${PROJECT_NAME}_sock
	${PROJECT_NAME}_onvif
#	${PROJECT_NAME}_wsdd
	${Boost_LIBRARIES} ${GSOAP_LIBRARIES}
)

if(WITH_TESTS)
	 add_subdirectory(tests)
endif(WITH_TESTS)


# INSTALL
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/onvifxx
	DESTINATION include
)
install(TARGETS ${PROJECT_NAME}
		LIBRARY DESTINATION lib
)

