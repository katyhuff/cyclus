# modeled after FindCOIN.cmake in the lemon project

IF(NOT DEFINED COIN_ROOT_DIR)
    SET(COIN_ROOT_DIR "$ENV{COIN_ROOT_DIR}")
ENDIF(NOT DEFINED COIN_ROOT_DIR)

MESSAGE(STATUS "COIN_ROOT_DIR hint is : ${COIN_ROOT_DIR}")

# if installed in a non-standard location,
# the include directory must be added to the PATH variable
FIND_PATH(COIN_INCLUDE_DIR coin/CbcModel.hpp
  HINTS "${COIN_INCLUDE_DIR}"
  HINTS "${COIN_ROOT_DIR}/include"
  HINTS /usr/
  HINTS /usr/include/
  HINTS /usr/local/
  HINTS /usr/local/include/
  HINTS /usr/coin/
  HINTS /usr/coin-Cbc/
  HINTS /usr/local/coin/
  HINTS /usr/local/coin-Cbc/
)
#MESSAGE("\tCOIN Include Dir: ${COIN_INCLUDE_DIR}")

set(COIN_INCLUDE_DIR ${COIN_INCLUDE_DIR}/coin)
#MESSAGE("\tCOIN Include Dir: ${COIN_INCLUDE_DIR}")

FIND_LIBRARY(COIN_CBC_LIBRARY
  NAMES Cbc libCbc
  HINTS "${COIN_INCLUDE_DIR}/../../lib/"
  HINTS "${COIN_ROOT_DIR}/lib"
)
#MESSAGE("\tCOIN CBC: ${COIN_CBC_LIBRARY}")

FIND_LIBRARY(COIN_CBC_SOLVER_LIBRARY
  NAMES CbcSolver libCbcSolver
  HINTS ${COIN_INCLUDE_DIR}/../../lib/
  HINTS "${COIN_ROOT_DIR}/lib"
)
#MESSAGE("\tCOIN CBC solver: ${COIN_CBC_SOLVER_LIBRARY}")

FIND_LIBRARY(COIN_CGL_LIBRARY
  NAMES Cgl libCgl
  HINTS ${COIN_INCLUDE_DIR}/../../lib/
  HINTS "${COIN_ROOT_DIR}/lib"
)
#MESSAGE("\tCOIN CGL: ${COIN_CGL_LIBRARY}")

FIND_LIBRARY(COIN_CLP_LIBRARY
  NAMES Clp libClp
  HINTS ${COIN_INCLUDE_DIR}/../../lib/
  HINTS "${COIN_ROOT_DIR}/lib"
)
#MESSAGE("\tCOIN CLP: ${COIN_CLP_LIBRARY}")

FIND_LIBRARY(COIN_COIN_UTILS_LIBRARY
  NAMES CoinUtils libCoinUtils
  HINTS ${COIN_INCLUDE_DIR}/../../lib/
  HINTS "${COIN_ROOT_DIR}/lib"
)
#MESSAGE("\tCOIN UTILS: ${COIN_COIN_UTILS_LIBRARY}")

FIND_LIBRARY(COIN_OSI_LIBRARY
  NAMES Osi libOsi
  HINTS ${COIN_INCLUDE_DIR}/../../lib/
  HINTS "${COIN_ROOT_DIR}/lib"
)
#MESSAGE("\tCOIN OSI: ${COIN_OSI_LIBRARY}")

FIND_LIBRARY(COIN_OSI_CBC_LIBRARY
  NAMES OsiCbc libOsiCbc
  HINTS ${COIN_INCLUDE_DIR}/../../lib/
  HINTS "${COIN_ROOT_DIR}/lib"
)
#MESSAGE("\tCOIN OSI CBC: ${COIN_OSI_CBC_LIBRARY}")

FIND_LIBRARY(COIN_OSI_CLP_LIBRARY
  NAMES OsiClp libOsiClp
  HINTS ${COIN_INCLUDE_DIR}/../../lib/
  HINTS "${COIN_ROOT_DIR}/lib"
)
#MESSAGE("\tCOIN OSI CLP: ${COIN_OSI_CLP_LIBRARY}")

FIND_LIBRARY(COIN_ZLIB_LIBRARY
  NAMES z libz
  HINTS ${COIN_ROOT_DIR}/lib
  HINTS "${COIN_ROOT_DIR}/lib"
)
#MESSAGE("\tCOIN ZLIP: ${COIN_ZLIB_LIBRARY}")

FIND_LIBRARY(COIN_BZ2_LIBRARY
  NAMES bz2 libz2
  HINTS ${COIN_ROOT_DIR}/lib
  HINTS "${COIN_ROOT_DIR}/lib"
)
# #MESSAGE("\tCOIN BZ2: ${COIN_BZ2_LIBRARY}")
 
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(COIN DEFAULT_MSG
  COIN_INCLUDE_DIR
  COIN_CBC_LIBRARY
  COIN_CBC_SOLVER_LIBRARY
  COIN_CGL_LIBRARY
  COIN_CLP_LIBRARY
  COIN_COIN_UTILS_LIBRARY
  COIN_OSI_LIBRARY
  COIN_OSI_CBC_LIBRARY
  COIN_OSI_CLP_LIBRARY
  COIN_ZLIB_LIBRARY
  COIN_BZ2_LIBRARY
)

IF(COIN_FOUND)
  SET(COIN_INCLUDE_DIRS ${COIN_INCLUDE_DIR})
  SET(COIN_CLP_LIBRARIES "${COIN_CLP_LIBRARY};${COIN_COIN_UTILS_LIBRARY};${COIN_ZLIB_LIBRARY}")
  IF(COIN_ZLIB_LIBRARY)
    SET(COIN_CLP_LIBRARIES "${COIN_CLP_LIBRARIES};${COIN_ZLIB_LIBRARY}")
  ENDIF(COIN_ZLIB_LIBRARY)
  IF(COIN_BZ2_LIBRARY)
    SET(COIN_CLP_LIBRARIES "${COIN_CLP_LIBRARIES};${COIN_BZ2_LIBRARY}")
  ENDIF(COIN_BZ2_LIBRARY)
  SET(COIN_CBC_LIBRARIES "${COIN_CBC_LIBRARY};${COIN_CBC_SOLVER_LIBRARY};${COIN_CGL_LIBRARY};${COIN_OSI_LIBRARY};${COIN_OSI_CBC_LIBRARY};${COIN_OSI_CLP_LIBRARY};${COIN_CLP_LIBRARIES}")
  SET(COIN_LIBRARIES ${COIN_CBC_LIBRARIES})
ENDIF(COIN_FOUND)

# let us know if we found it
IF(COIN_INCLUDE_DIRS)
  MESSAGE("\tFound COIN Include Dirs: ${COIN_INCLUDE_DIRS}")
ELSE(COIN_INCLUDE_DIRS)
  MESSAGE("\tCOIN Include Dirs NOT FOUND")
ENDIF(COIN_INCLUDE_DIRS)