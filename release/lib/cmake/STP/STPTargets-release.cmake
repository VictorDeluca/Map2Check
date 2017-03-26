#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libstp" for configuration "Release"
set_property(TARGET libstp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libstp PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C;CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "/usr/lib/x86_64-linux-gnu/libboost_program_options.a;/home/map2check/devel_tool/map_src_on_docker/release/lib/libminisat.a"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libstp.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS libstp )
list(APPEND _IMPORT_CHECK_FILES_FOR_libstp "${_IMPORT_PREFIX}/lib/libstp.a" )

# Import target "stp" for configuration "Release"
set_property(TARGET stp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(stp PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/stp-2.1.2"
  )

list(APPEND _IMPORT_CHECK_TARGETS stp )
list(APPEND _IMPORT_CHECK_FILES_FOR_stp "${_IMPORT_PREFIX}/bin/stp-2.1.2" )

# Import target "stp_simple" for configuration "Release"
set_property(TARGET stp_simple APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(stp_simple PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/stp_simple"
  )

list(APPEND _IMPORT_CHECK_TARGETS stp_simple )
list(APPEND _IMPORT_CHECK_FILES_FOR_stp_simple "${_IMPORT_PREFIX}/bin/stp_simple" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)