
FILE(GLOB_RECURSE cli-tinychain_SOURCES "*.cpp")

ADD_DEFINITIONS(-DBCX_STATIC=1)

ADD_EXECUTABLE(cli-tinychain ${cli-tinychain_SOURCES})

IF(NOT WIN32)
# for experimental/string_view
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
ENDIF()

IF(ENABLE_SHARED_LIBS)
    TARGET_LINK_LIBRARIES(cli-tinychain ${mongoose_LIBRARY} ${jsoncpp_LIBRARY})
ELSE()
    TARGET_LINK_LIBRARIES(cli-tinychain ${mongoose_LIBRARY} ${jsoncpp_LIBRARY})
ENDIF()

INSTALL(TARGETS cli-tinychain DESTINATION bin)
