project(${CMAKE_PROJECT_NAME}-ws-discovery)
cmake_minimum_required(VERSION 2.6)

include_directories(${CMAKE_SOURCE_DIR}/include)

aux_source_directory(. WSDISCOVERY_SOURCES)

add_executable(${PROJECT_NAME} ${WSDISCOVERY_SOURCES})

target_link_libraries(${PROJECT_NAME}
	${Boost_LIBRARIES}
	${CMAKE_PROJECT_NAME}
	${CMAKE_PROJECT_NAME}-wsdd
)
