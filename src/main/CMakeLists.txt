FILE(GLOB_RECURSE tinychain_SOURCES "*.cpp")

ADD_EXECUTABLE(tinychain ${tinychain_SOURCES})

IF(ENABLE_SHARED_LIBS)
TARGET_LINK_LIBRARIES(tinychain ${Boost_LIBRARIES} ${network_LIBRARY} ${database_LIBRARY} ${consensus_LIBRARY} 
    ${blockchain_LIBRARY} ${tinychain_LIBRARY} ${mongoose_LIBRARY} ${node_LIBRARY} ${protocol_LIBRARY})
ELSE()
TARGET_LINK_LIBRARIES(tinychain ${Boost_LIBRARIES} ${network_LIBRARY} ${database_LIBRARY} ${consensus_LIBRARY} 
    ${blockchain_LIBRARY} ${tinychain_LIBRARY} ${mongoose_LIBRARY} ${node_LIBRARY} ${protocol_LIBRARY})
ENDIF()

INSTALL(TARGETS tinychain DESTINATION bin)
