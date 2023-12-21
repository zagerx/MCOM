# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\MCOM_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\MCOM_autogen.dir\\ParseCache.txt"
  "MCOM_autogen"
  )
endif()
