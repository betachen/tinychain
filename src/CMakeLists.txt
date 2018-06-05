FILE(GLOB_RECURSE tinychain_SOURCES "*.cpp")

ADD_EXECUTABLE(tinychain ${tinychain_SOURCES})

IF(ENABLE_SHARED_LIBS)
    TARGET_LINK_LIBRARIES(tinychain ${Boost_LIBRARIES} ${jsoncpp_LIBRARY} ${mongoose_LIBRARY})
ELSE()
    TARGET_LINK_LIBRARIES(tinychain ${Boost_LIBRARIES} ${jsoncpp_LIBRARY} ${mongoose_LIBRARY})
ENDIF()

INSTALL(TARGETS tinychain DESTINATION bin)

