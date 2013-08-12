cmake_minimum_required (VERSION 2.6)
project (mysync)
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/build/modules)

set(CMAKE_CXX_FLAGS "-Wall -O2 -funroll-loops -fstrict-aliasing -ffast-math -fno-strict-aliasing -march=native -mtune=native")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -O2 -funroll-loops -fstrict-aliasing -ffast-math -fno-strict-aliasing -march=native -mtune=native")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

set(BUILD_SHARED_LIBRARIES, false)
set(BUILD_STATIC_EXECUTABLES false)

file(GLOB_RECURSE mysync_SRC src/*)
# set(mysync_SRC src/main.cpp src/dbtable.cpp src/dbtable.h)
add_executable(mysync ${mysync_SRC})

#find_package (Threads) // ${CMAKE_THREAD_LIBS_INIT}
FIND_PACKAGE(MySQLClient)
FIND_PACKAGE(MySQLConnectorCPP)
FIND_PACKAGE(LibConfig)

target_link_libraries (mysync ${LIBCONFIGPP_LIBRARY} ${MYSQLCONNECTORCPP_LIBRARIES} ${MYSQL_LIBRARIES})
include_directories(
${LIBCONFIGPP_INCLUDE_DIR}
${MYSQLCONNECTORCPP_INCLUDE_DIR}
${MYSQL_INCLUDE_DIR}
/usr/local/include
/usr/include
)

# distclean
ADD_CUSTOM_TARGET (distclean @echo cleaning for source distribution)
SET(DISTCLEANED
  cmake.depends
  cmake.check_depends
  CMakeCache.txt
  cmake.check_cache
  *.cmake
  Makefile
  core core.*
  gmon.out
  *~
  CMakeFiles
  yaml-cpp.pc
  Debug
  DerivedData
  CMakeScripts
  mysync.build
  mysync.xcodeproj
  nbproject
)

ADD_CUSTOM_COMMAND(
  DEPENDS clean
  COMMENT "distribution clean"
  COMMAND rm
  ARGS    -Rf CMakeTmp ${DISTCLEANED}
  TARGET  distclean
)
