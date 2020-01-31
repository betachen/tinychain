
FILE(GLOB_RECURSE tinychain_cli_SOURCES "*.cpp")

ADD_DEFINITIONS(-DBCX_STATIC=1)

ADD_EXECUTABLE(tinychain-cli ${tinychain_cli_SOURCES})

IF(NOT WIN32)
# for experimental/string_view
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
ENDIF()

IF(ENABLE_SHARED_LIBS)
    TARGET_LINK_LIBRARIES(tinychain-cli ${Boost_LIBRARIES} ${jsoncpp_LIBRARY})
ELSE()
    TARGET_LINK_LIBRARIES(tinychain-cli ${Boost_LIBRARIES} ${jsoncpp_LIBRARY})
ENDIF()

INSTALL(TARGETS tinychain-cli DESTINATION bin)
