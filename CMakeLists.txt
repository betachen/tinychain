CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(tinychain)

SET(CMAKE_VERBOSE_MAKEFILE 1)

SET(ENABLE_SHARED_LIBS OFF CACHE BOOL   "Enable shared libs.")
SET(MG_ENABLE_DEBUG    OFF CACHE BOOL   "Enable Mongoose debug.")

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE DEBUG)
  #SET(CMAKE_BUILD_TYPE RELEASE)
  #SET(CMAKE_BUILD_TYPE RELWITHDEBINFO)
  #SET(CMAKE_BUILD_TYPE MINSIZEREL)
ENDIF()

# Enable for use with clang-tidy.
IF(NOT CMAKE_EXPORT_COMPILE_COMMANDS)
  SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
ENDIF()

STRING(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE)

# ----------------- common Definitions ------
SET(COMMON_WARN "-Wall -Werror -Wextra -Wstrict-aliasing=2 -Wno-unused-parameter -Wno-unused-variable -Wno-type-limits")
SET(COMMON_FLAGS "-fstrict-aliasing -fvisibility=hidden")
#SET(COMMON_LIBS "-pthread") chenhao
SET(COMMON_ARGS "${COMMON_FLAGS} ${COMMON_WARN} ${COMMON_LIBS}")

# --------------- Compiler ------------------
IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  SET(CMAKE_C_FLAGS "-std=c11 ${COMMON_ARGS}")
  #libtinychain has too many ignored-qualifiers, and TODOs
  SET(CMAKE_CXX_FLAGS "-std=c++11 ${COMMON_ARGS} -fno-enforce-eh-specs -fnothrow-opt -Wno-reorder -Wno-ignored-qualifiers -Wno-unused-function")
ELSEIF("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  SET(CMAKE_C_FLAGS "-std=c11 ${COMMON_ARGS}")
  SET(CMAKE_CXX_FLAGS "-std=c++11 ${COMMON_ARGS} -Wno-reorder")
ENDIF()
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fPIC")

# --------------- Definitions ------------------
ADD_DEFINITIONS(-DBOOST_NO_AUTO_PTR=1 -DBOOST_NO_TYPEID=1)

IF(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
  ADD_DEFINITIONS(-DMVS_DEBUG=1)
ENDIF()

SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
SET(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

# --------------- REQUIREMENTS ---------------------
FILE(MAKE_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}")
FILE(MAKE_DIRECTORY "${LIBRARY_OUTPUT_PATH}")

SET(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${PROJECT_SOURCE_DIR}/etc")
#SET(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR})
SET(CMAKE_INSTALL_PREFIX "/usr/local/mvs")

set(Boost_USE_STATIC_LIBS   ON)
FIND_PACKAGE(Boost 1.56 REQUIRED COMPONENTS date_time filesystem system program_options)
#FIND_PACKAGE(Boost 1.56 REQUIRED COMPONENTS date_time filesystem system program_options regex thread)
#FIND_PACKAGE(secp256k1 REQUIRED)

# ------------------ includes --------------------
INCLUDE_DIRECTORIES("${Boost_INCLUDE_DIRS}")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/contrib")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/include")

IF(ENABLE_SHARED_LIBS)
  SET(mongoose_LIBRARY mongoose_shared)
  SET(network_LIBRARY network_shared)
  SET(consensus_LIBRARY consensus_shared)
  SET(database_LIBRARY database_shared)
  SET(node_LIBRARY node_shared)
  SET(protocol_LIBRARY protocol_shared)
ELSE()
  SET(mongoose_LIBRARY mongoose_static)
  SET(network_LIBRARY network_static)
  SET(consensus_LIBRARY consensus_static)
  SET(database_LIBRARY database_static)
  SET(blockchain_LIBRARY blockchain_static)
  SET(node_LIBRARY node_static)
  SET(protocol_LIBRARY protocol_static)
ENDIF()

ENABLE_TESTING()

ADD_SUBDIRECTORY(contrib)
ADD_SUBDIRECTORY(src)
