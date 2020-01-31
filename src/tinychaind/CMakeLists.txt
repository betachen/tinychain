FILE(GLOB_RECURSE tinychain_SOURCES "*.cpp")

ADD_EXECUTABLE(tinychaind ${tinychain_SOURCES})

IF(ENABLE_SHARED_LIBS)
    TARGET_LINK_LIBRARIES(tinychaind ${Boost_date_time} ${jsoncpp_LIBRARY} ${sqlite_LIBRARY} ${utils_LIBRARY} cryptopp)

ELSE()
    TARGET_LINK_LIBRARIES(tinychaind ${Boost_LIBRARIES} ${jsoncpp_LIBRARY} ${sqlite_LIBRARY} ${utils_LIBRARY} cryptopp dl)

ENDIF()

INSTALL(TARGETS tinychaind DESTINATION bin)

