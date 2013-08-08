cmake_minimum_required (VERSION 2.6)
project (mysync)
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/build/modules)

set(CMAKE_CXX_FLAGS "-Wall -O2 -funroll-loops -fstrict-aliasing -ffast-math -fno-strict-aliasing -march=native -m64 -mtune=native -m64")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -O2 -funroll-loops -fstrict-aliasing -ffast-math -fno-strict-aliasing -march=native -m64 -mtune=native -m64")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

set(BUILD_SHARED_LIBRARIES, false)
set(BUILD_STATIC_EXECUTABLES false)

set(mysync_SRC src/main.cpp)
add_executable(mysync ${mysync_SRC})

find_package (Threads)
FIND_PACKAGE(MySQLConnectorCPP)

add_subdirectory(lib/yaml-cpp-0.5.1)
target_link_libraries (mysync yaml-cpp ${CMAKE_THREAD_LIBS_INIT} ${MYSQLCONNECTORCPP_LIBRARIES})

include_directories(
${CMAKE_SOURCE_DIR}/lib/yaml-cpp-0.5.1/include
${MYSQLCONNECTORCPP_INCLUDE_DIR}
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
)

ADD_CUSTOM_COMMAND(
  DEPENDS clean
  COMMENT "distribution clean"
  COMMAND rm
  ARGS    -Rf CMakeTmp ${DISTCLEANED}
  TARGET  distclean
)