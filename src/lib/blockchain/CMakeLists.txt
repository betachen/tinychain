FILE(GLOB_RECURSE blockchain_SOURCES "*.cpp")

ADD_LIBRARY(blockchain_static STATIC ${blockchain_SOURCES})
SET_TARGET_PROPERTIES(blockchain_static PROPERTIES OUTPUT_NAME blockchain)
TARGET_LINK_LIBRARIES(blockchain_static ${Boost_LIBRARIES} ${tinychain_LIBRARY} )
INSTALL(TARGETS blockchain_static DESTINATION lib)

IF(ENABLE_SHARED_LIBS)
  ADD_LIBRARY(blockchain_shared SHARED ${blockchain_SOURCES})
  SET_TARGET_PROPERTIES(blockchain_shared PROPERTIES OUTPUT_NAME blockchain)
  TARGET_LINK_LIBRARIES(blockchain_shared ${Boost_LIBRARIES} ${tinychain_LIBRARY} )
  INSTALL(TARGETS blockchain_shared DESTINATION lib)
ENDIF()
