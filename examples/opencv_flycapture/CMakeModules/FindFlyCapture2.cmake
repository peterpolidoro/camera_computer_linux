# - Try to find FlyCapture2
#
# Once done this will define
#
#  FlyCapture2_FOUND
#  FlyCapture2_INCLUDE_DIRS
#  FlyCapture2_LIBRARIES
#
# ------------------------------------------------------------------------------

if(WIN32)
  # -------------------------------------------------------------------------
  # TO DO
  #
  # I think this pretty fragile ... what if user puts flycapure in
  # different location
  # -------------------------------------------------------------------------
  set(typical_fc2_dir "C:/Program Files/Point Grey Research/FlyCapture2")
  set(typical_fc2_inc_dir "${typical_fc2_dir}/include")

else(WIN32)
  set(typical_fc2_inc_dir "/usr/include/flycapture")
endif(WIN32)
find_path(
  FlyCapture2_INCLUDE_DIR
  "FlyCapture2.h"
  HINTS ${typical_fc2_inc_dir}
  )
message(STATUS "FlyCapture2_INCLUDE_DIR: " ${FlyCapture2_INCLUDE_DIR})

if(WIN32)
  # -------------------------------------------------------------------------
  # TO DO
  #
  # I think this pretty fragile ... what if user puts flycapure in
  # different location
  # -------------------------------------------------------------------------
  set(typical_fc2_lib_dir "${typical_fc2_dir}/lib64")
  set(fc2_lib_name "FlyCapture2.lib")

else(WIN32)
  set(typical_fc2_lib_dir "/usr/lib")
  set(fc2_lib_name "flycapture")
endif(WIN32)
find_library(
  FlyCapture2_LIBRARY
  NAMES ${fc2_lib_name}
  HINTS ${typical_fc2_lib_dir}
  )
#message(STATUS "FlyCapture2_LIBRARY: " ${FlyCapture2_LIBRARY})


set(FlyCapture2_LIBRARIES ${FlyCapture2_LIBRARY} )
set(FlyCapture2_INCLUDE_DIRS ${FlyCapture2_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set FlyCapture2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(
  FlyCapture2  DEFAULT_MSG
  FlyCapture2_LIBRARY
  FlyCapture2_INCLUDE_DIR
  )

mark_as_advanced(FlyCapture2_INCLUDE_DIR FlyCapture2_LIBRARY )
