cmake_minimum_required(VERSION 2.8)

project(CyclusSuperbuild)

set(CMAKE_MODULE_PATH "${CyclusSuperbuild_SOURCE_DIR}/cmake"
  ${CMAKE_MODULE_PATH})

# Include the relevant CMake module and tarball file
include(ExternalProject)
include(cyclus_tarballs)
include(library_suffix) # TODO necessary ? 

# Add options to use system libraries for each of the dependencies
option(USE_SYSTEM_ZLIB "Should we use the system zlib?" OFF)
option(USE_SYSTEM_ZLIB "Should we use the system zlib?" OFF)
option(USE_SYSTEM_LIBXML2 "Should we use the system LIBXML2?" OFF)
option(USE_SYSTEM_Boost "Should we use the system LIBXML2?" OFF)
option(USE_SYSTEM_LIBXiML2 "Should we use the system LIBXML2?" OFF)

# External Qt build if requested
if(USE_SYSTEM_QT)
  # Include the boost header files and the program_options library
  SET(Boost_USE_STATIC_LIBS       OFF)
  SET(Boost_USE_STATIC_RUNTIME    OFF)
  FIND_PACKAGE( Boost COMPONENTS program_options filesystem system REQUIRED)
  SET(CYCLUS_INCLUDE_DIR ${CYCLUS_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
  SET(LIBS ${LIBS} ${Boost_PROGRAM_OPTIONS_LIBRARY})
  SET(LIBS ${LIBS} ${Boost_SYSTEM_LIBRARY})
  SET(LIBS ${LIBS} ${Boost_FILESYSTEM_LIBRARY})
else()
  include(External_Boost)
endif()
set(cyclus_build_vars "-DBoost_INCLUDE_DIR:PATH=${Boost_INCLUDE_DIR}")
list(APPEND cyclus_build_vars
  "-DBoost_INCLUDE_DIR:PATH=${Boost_INCLUDE_DIR}")
  SET(LIBS ${LIBS} ${Boost_PROGRAM_OPTIONS_LIBRARY})
  SET(LIBS ${LIBS} ${Boost_SYSTEM_LIBRARY})
  SET(LIBS ${LIBS} ${Boost_FILESYSTEM_LIBRARY})

if(CMAKE_BUILD_TYPE)
  list(APPEND cyclus_build_vars
    "-DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}")
endif()

# Now build zlib
if(USE_SYSTEM_ZLIB)
  find_package(ZLIB REQUIRED)
else()
  include(External_zlib)
endif()
# Append the relevant variables to our initial cache
list(APPEND cyclus_build_vars
  "-DZLIB_INCLUDE_DIR:PATH=${ZLIB_INCLUDE_DIR}"
  "-DZLIB_LIBRARY:PATH=${ZLIB_LIBRARY}")

# Now build sqlite3
if(USE_SYSTEM_Sqlite3)
  find_package(Sqlite3 REQUIRED)
else()
  include(External_Sqlite3)
endif()
# Append the relevant variables to our initial cache
list(APPEND cyclus_build_vars
  "-DSQLITE3_INCLUDE_DIR:PATH=${SQLITE3_INCLUDE_DIR}"
  "-DSQLITE3_LIBRARIES:PATH=${SQLITE3_LIBRARIS}")

# Now build libxml2
if(USE_SYSTEM_LIBXML2)
  find_package(LIBXML2 REQUIRED)
else()
  include(External_libxml2)
endif()
list(APPEND cyclus_build_vars
  "-DLIBXML2_INCLUDE_DIR:PATH=${LIBXML2_INCLUDE_DIR}"
  "-DLIBXML2_LIBRARIES:PATH=${LIBXML2_LIBRARIES}")

# Now build libxml++
if(USE_SYSTEM_LIBXML++)
  find_package(LIBXML2 REQUIRED)
else()
  include(External_libxml++)
endif()
list(APPEND cyclus_build_vars
  "-DLibXML++_INCLUDE_DIR:PATH=${LIBXML++_INCLUDE_DIR}"
  "-DLibXML++Config_INCLUDE_DIR:PATH=${LibXML++Config_INCLUDE_DIR}"
  "-DGlibmm_INCLUDE_DIR:PATH=${Glibmm_INCLUDE_DIR}"
  "-DLIBXML2_LIBRARIES:PATH=${LIBXML2_LIBRARIES}")

# Problems compiling with Eigen, just don't pass it in for now
include(External_openbabel)

list(APPEND cyclus_build_vars
  "-DEIGEN2_INCLUDE_DIR:PATH=${CyclusSuperbuild_SOURCE_DIR}/eigen"
  "-DOPENBABEL2_INCLUDE_DIR:PATH=${OPENBABEL2_INCLUDE_DIR}"
  "-DOPENBABEL2_LIBRARIES:FILEPATH=${OPENBABEL2_LIBRARIES}"
  "-DOPENBABEL2_VERSION_MET:BOOL=TRUE"
  "-DOPENBABEL_IS_NEWER_THAN_2_2_99:BOOL=TRUE")

include(External_openqube)

list(APPEND cyclus_build_vars
  "-DOpenQube_DIR:PATH=${CyclusSuperbuild_BINARY_DIR}/openqube"
  "-DCMAKE_PREFIX_PATH:PATH=${CyclusSuperbuild_BINARY_DIR}/prefix")

include(External_cyclus)

list(APPEND cyclus_build_vars
  "-DCyclus_DIR:PATH=${CyclusSuperbuild_BINARY_DIR}/cyclus")

# Should we use the optional CYCAMORE components? If so, find them and pass it down
option(Cyclus_USE_CYCAMORE "Use optional CYCAMORE components?" OFF)
if(Cyclus_USE_CYCAMORE)
  find_package(CYCAMORE REQUIRED)
  list(APPEND cyclus_build_vars
    "-DCYCAMORE_DIR:PATH=${CYCAMORE_DIR}")
  message("cyclus_build_vars: ${cyclus_build_vars}")

  include(External_cycluscycamore)
endif()
