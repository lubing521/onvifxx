
FIND_PROGRAM(
  GSOAP_SOAPCPP2
  NAMES soapcpp2 soapcpp2.exe
  PATHS /usr/local /usr
)

FIND_PATH(
  GSOAP_IMPORT_DIR
  wsa.h
  PATHS /usr/local/share/gsoap/import /usr/share/gsoap/import
)

FIND_PATH(
  GSOAP_INCLUDE_DIR
  stdsoap2.h
  ${GSOAP_INCLUDE_PATH}
)

FIND_LIBRARY(
  GSOAP_LIBRARY
  NAMES gsoapssl++
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GSOAP DEFAULT_MSG GSOAP_LIBRARY GSOAP_INCLUDE_DIR)

if(GSOAP_FOUND)
    set(GSOAP_LIBRARIES ${GSOAP_LIBRARY})
    set(GSOAP_INCLUDE_DIRS ${GSOAP_INCLUDE_DIR})
endif()

