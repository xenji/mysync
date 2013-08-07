cmake_minimum_required (VERSION 2.6)
project (mysync)

set(CMAKE_CXX_FLAGS "-Wall -O2 -funroll-loops -fstrict-aliasing -ffast-math -fno-strict-aliasing -march=native -m64 -mtune=native -m64")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -O2 -funroll-loops -fstrict-aliasing -ffast-math -fno-strict-aliasing -march=native -m64 -mtune=native -m64")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

set(BUILD_SHARED_LIBRARIES, false)

set(mysync_SRC src/main.cpp)
add_executable(mysync ${mysync_SRC})

find_package (Threads)

#yaml-cpp-0.5.1
add_subdirectory(lib/yaml-cpp-0.5.1)
add_library(YAML_CPP)
target_link_libraries (mysync ${CMAKE_THREAD_LIBS_INIT})

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
)

ADD_CUSTOM_COMMAND(
  DEPENDS clean
  COMMENT "distribution clean"
  COMMAND rm
  ARGS    -Rf CMakeTmp ${DISTCLEANED}
  TARGET  distclean
)

# - Find mysqlclient
# Find the native MySQL includes and library
#
#  MYSQL_INCLUDE_DIR - where to find mysql.h, etc.
#  MYSQL_LIBRARIES   - List of libraries when using MySQL.
#  MYSQL_FOUND       - True if MySQL found.

IF (MYSQL_INCLUDE_DIR)
  # Already in cache, be silent
  SET(MYSQL_FIND_QUIETLY TRUE)
ENDIF (MYSQL_INCLUDE_DIR)

FIND_PATH(MYSQL_INCLUDE_DIR mysql.h
  /usr/local/include/mysql
  /usr/include/mysql
)

SET(MYSQL_NAMES mysqlclient mysqlclient_r)
FIND_LIBRARY(MYSQL_LIBRARY
  NAMES ${MYSQL_NAMES}
  PATHS /usr/lib /usr/local/lib
  PATH_SUFFIXES mysql
)

IF (MYSQL_INCLUDE_DIR AND MYSQL_LIBRARY)
  SET(MYSQL_FOUND TRUE)
  SET( MYSQL_LIBRARIES ${MYSQL_LIBRARY} )
ELSE (MYSQL_INCLUDE_DIR AND MYSQL_LIBRARY)
  SET(MYSQL_FOUND FALSE)
  SET( MYSQL_LIBRARIES )
ENDIF (MYSQL_INCLUDE_DIR AND MYSQL_LIBRARY)

IF (MYSQL_FOUND)
  IF (NOT MYSQL_FIND_QUIETLY)
    MESSAGE(STATUS "Found MySQL: ${MYSQL_LIBRARY}")
  ENDIF (NOT MYSQL_FIND_QUIETLY)
ELSE (MYSQL_FOUND)
  IF (MYSQL_FIND_REQUIRED)
    MESSAGE(STATUS "Looked for MySQL libraries named ${MYSQL_NAMES}.")
    MESSAGE(FATAL_ERROR "Could NOT find MySQL library")
  ENDIF (MYSQL_FIND_REQUIRED)
ENDIF (MYSQL_FOUND)

MARK_AS_ADVANCED(
  MYSQL_LIBRARY
  MYSQL_INCLUDE_DIR
)

