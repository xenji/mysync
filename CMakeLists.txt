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

add_subdirectory(lib/yaml-cpp-0.5.1)
target_link_libraries (mysync yaml-cpp ${CMAKE_THREAD_LIBS_INIT} ${MYSQL_LIBRARIES})

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

set(MYSQLCONNECTORCPP_ROOT_DIR "/usr/local")

set(MYSQLCONNECTORCPP_ROOT_DIR 
        "${MYSQLCONNECTORCPP_ROOT_DIR}" 
        CACHE 
        PATH "/usr/local"
        "Where to start looking for this component.") 

find_path(MYSQLCONNECTORCPP_INCLUDE_DIR 
        mysql_connection.h 
        HINTS 
        ${MYSQLCONNECTORCPP_ROOT_DIR} 
        PATH_SUFFIXES 
        include)

find_library(MYSQLCONNECTORCPP_LIBRARY 
        NAMES 
        mysqlcppconn 
        mysqlcppconn-static 
        HINTS 
        ${MYSQLCONNECTORCPP_ROOT_DIR} 
        PATH_SUFFIXES 
        lib64 
        lib) 

mark_as_advanced(MYSQLCONNECTORCPP_INCLUDE_DIR MYSQLCONNECTORCPP_LIBRARY) 

include(FindPackageHandleStandardArgs) 
find_package_handle_standard_args(MysqlConnectorCpp 
        DEFAULT_MSG 
        MYSQLCONNECTORCPP_INCLUDE_DIR 
        MYSQLCONNECTORCPP_LIBRARY) 

if(MYSQLCONNECTORCPP_FOUND) 
        set(MYSQLCONNECTORCPP_INCLUDE_DIRS "${MYSQLCONNECTORCPP_INCLUDE_DIR}") # Add any dependencies here 
        set(MYSQLCONNECTORCPP_LIBRARIES "${MYSQLCONNECTORCPP_LIBRARY}") # Add any dependencies here 
        mark_as_advanced(MYSQLCONNECTORCPP_ROOT_DIR) 
endif() 