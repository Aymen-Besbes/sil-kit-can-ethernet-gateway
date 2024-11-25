#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SilKit::SilKit" for configuration "Debug"
set_property(TARGET SilKit::SilKit APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(SilKit::SilKit PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libSilKitd.so"
  IMPORTED_SONAME_DEBUG "libSilKitd.so"
  )

list(APPEND _cmake_import_check_targets SilKit::SilKit )
list(APPEND _cmake_import_check_files_for_SilKit::SilKit "${_IMPORT_PREFIX}/lib/libSilKitd.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
