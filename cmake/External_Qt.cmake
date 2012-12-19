# The Qt external project for Avogadro

message(STATUS "Building Qt as part of Avogadro.")
set(qt_source "${CMAKE_CURRENT_BINARY_DIR}/Qt")
if(WIN32)
  # if jom is in the path use it as it will be faster
  find_program(JOM jom)
  mark_as_advanced(JOM)
  if(JOM)
    set(qt_build_program "${JOM}")
  else()
    set(qt_build_program nmake)
  endif()
  set(qt_install_dir ${qt_source})
  configure_file(${AvogadroSquared_SOURCE_DIR}/cmake/win_config_qt.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/win_config_qt.cmake )
  set(qt_configure ${CMAKE_COMMAND}
    -P ${CMAKE_CURRENT_BINARY_DIR}/win_config_qt.cmake)
  set(qt_build ${qt_build_program})
  set(qt_install "")
else()
  set(qt_install_dir "${CMAKE_CURRENT_BINARY_DIR}/Qt-install")
  set(qt_configure echo yes | sh ./configure -release
    -nomake examples -nomake demos --prefix=${qt_install_dir} -opensource)
  if ("-m32" STREQUAL "${CMAKE_CXX_FLAGS}")
    set(qt_configure echo yes | sh ./configure -release
      -nomake examples -nomake demos --prefix=${qt_install_dir} -opensource
      -platform linux-g++-32)
  endif ()
  set(qt_build ${MAKE})
  set(qt_install make install)
  if(APPLE)
    exec_program(${CMAKE_C_COMPILER} ARGS --version OUTPUT_VARIABLE
        _gcc_version_info)
    string (REGEX MATCH "[345]\\.[0-9]\\.[0-9]"
        _gcc_version "${_gcc_version_info}")
    if(NOT _gcc_version)
      string (REGEX REPLACE ".*\\(GCC\\).* ([34]\\.[0-9]) .*" "\\1.0"
        _gcc_version "${_gcc_version_info}")
    endif()
    if(${_gcc_version} VERSION_GREATER 4.2.0)
      # Then Qt should be built 64 bit
      message(STATUS "Building 64 bit Qt using cocoa.")
      set(qt_configure ${qt_configure} -arch x86_64 -cocoa)
    else()
      # Then Qt should be built 32 bit
      message(STATUS "Building 32 bit Qt using carbon.")
      set(qt_configure ${qt_configure} -arch x86 -carbon)
    endif()
  endif()
endif()

ExternalProject_Add(Qt
  DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}
  URL ${qt_file}
  URL_MD5 ${qt_md5}
  UPDATE_COMMAND ""
  SOURCE_DIR ${qt_source}
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND ${qt_configure}
  BUILD_COMMAND ${qt_build}
  INSTALL_COMMAND "${qt_install}"
  )
set(QT_QMAKE_EXECUTABLE "${qt_install_dir}/bin/qmake"
  CACHE FILEPATH "Path to qmake executable" FORCE)
