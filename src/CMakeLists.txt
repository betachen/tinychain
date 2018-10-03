FILE(GLOB_RECURSE tinychain_SOURCES "*.cpp")

ADD_EXECUTABLE(tinychain ${tinychain_SOURCES})

IF(WIN32)
    TARGET_LINK_LIBRARIES(tinychain ${Boost_LIBRARIES} ${jsoncpp_LIBRARY} ${mongoose_LIBRARY} ${sqlite_LIBRARY})
ELSEIF(ENABLE_SHARED_LIBS)
    TARGET_LINK_LIBRARIES(tinychain ${Boost_LIBRARIES} ${jsoncpp_LIBRARY} ${mongoose_LIBRARY} sqlite3 cryptopp)
ELSE()
    TARGET_LINK_LIBRARIES(tinychain ${Boost_LIBRARIES} ${jsoncpp_LIBRARY} ${mongoose_LIBRARY} sqlite3 cryptopp dl)
ENDIF()

INSTALL(TARGETS tinychain DESTINATION bin)

