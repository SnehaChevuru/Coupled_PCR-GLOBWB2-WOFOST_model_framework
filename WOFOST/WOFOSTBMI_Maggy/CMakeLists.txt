cmake_minimum_required(VERSION 3.0)

project (WOFOSTBMI_Maggy C)

set(_cflags "-ansi -Wall -Wundef -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -O2")
#if(NOT WIN32)
#  if(CMAKE_C_FLAGS)
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${_cflags}")
#  else()
#    set(CMAKE_C_FLAGS ${_cflags})
#  endif()
#endif()
#set(CMAKE_C_FLAGS ${_cflags})

find_package(PkgConfig REQUIRED)
pkg_check_modules(BMIC REQUIRED IMPORTED_TARGET bmic)
message("--   bmic include - ${BMIC_INCLUDE_DIRS}")
include_directories(${BMIC_INCLUDE_DIRS})
#target_link_libraries(bmic -lm -lnetcdf)
add_subdirectory (WOFOST_Maggy)


