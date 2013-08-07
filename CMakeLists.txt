cmake_minimum_required (VERSION 2.6)
project (mysync)

set(CMAKE_CXX_FLAGS "-Wall -O2 -funroll-loops -fstrict-aliasing -ffast-math -fno-strict-aliasing -march=native -m64 -mtune=native -m64")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -O2 -funroll-loops -fstrict-aliasing -ffast-math -fno-strict-aliasing -march=native -m64 -mtune=native -m64")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

set(BUILD_SHARED_LIBRARIES, false)
set(BUILD_STATIC_EXECUTABLES false)

set(mysync_SRC src/main.cpp)
add_executable(mysync ${mysync_SRC})

find_package (Threads)

# MySQL
add_subdirectory(lib/mysql-connector-c++-1.1.3)

#yaml-cpp-0.5.1
add_subdirectory(lib/yaml-cpp-0.5.1)
#add_library(yaml-cpp)
target_link_libraries (mysync yaml-cpp ${CMAKE_THREAD_LIBS_INIT} mysql-connector-c++)

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

