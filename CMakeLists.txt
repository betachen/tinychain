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
SET(COMMON_WARN "-Wall -Werror -Wstrict-aliasing=2 -Wno-unused-parameter -Wno-unused-variable -Wno-type-limits -Wno-deprecated-register -Wno-deprecated-declarations")
SET(COMMON_FLAGS "-fstrict-aliasing -fvisibility=hidden")
SET(COMMON_LIBS "-pthread")
SET(COMMON_ARGS "${COMMON_FLAGS} ${COMMON_WARN} ${COMMON_LIBS}")

# --------------- Compiler ------------------
IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  SET(CMAKE_C_FLAGS "-std=c11 ${COMMON_ARGS}")
  SET(CMAKE_CXX_FLAGS "-std=c++14 ${COMMON_ARGS} -fno-enforce-eh-specs -fnothrow-opt -Wno-reorder -Wno-ignored-qualifiers -Wno-unused-function")
ELSEIF("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  SET(CMAKE_C_FLAGS "-std=c11 ${COMMON_ARGS}")
  SET(CMAKE_CXX_FLAGS "-std=c++14 ${COMMON_ARGS} -Wno-reorder -Wno-unused-function -Wno-unused-command-line-argument")
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
SET(CMAKE_INSTALL_PREFIX "/usr/local/tinychain")

set(Boost_USE_STATIC_LIBS   ON)
FIND_PACKAGE(Boost 1.56 REQUIRED COMPONENTS date_time)
    #filesystem system program_options)
INCLUDE_DIRECTORIES("${Boost_INCLUDE_DIRS}")

# ------------------ includes --------------------
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/contrib")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/include")

IF(ENABLE_SHARED_LIBS)
  SET(mongoose_LIBRARY mongoose_shared)
  SET(jsoncpp_LIBRARY jsoncpp_shared)
ELSE()
  SET(mongoose_LIBRARY mongoose_static)
  SET(jsoncpp_LIBRARY jsoncpp_static)
ENDIF()

ENABLE_TESTING()

ADD_SUBDIRECTORY(contrib)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(cli-tinychain)

