FILE(GLOB_RECURSE node_SOURCES "*.cpp")

ADD_LIBRARY(node_static STATIC ${node_SOURCES})
SET_TARGET_PROPERTIES(node_static PROPERTIES OUTPUT_NAME node)
TARGET_LINK_LIBRARIES(node_static ${Boost_LIBRARIES} ${chainbase_LIBRARY}
      ${blockchain_LIBRARY} ${database_LIBRARY} ${network_LIBRARY})
INSTALL(TARGETS node_static DESTINATION lib)

IF(ENABLE_SHARED_LIBS)
  ADD_LIBRARY(node_shared SHARED ${node_SOURCES})
  SET_TARGET_PROPERTIES(node_shared PROPERTIES OUTPUT_NAME node)
  TARGET_LINK_LIBRARIES(node_shared ${Boost_LIBRARIES} ${chainbase_LIBRARY}
      ${blockchain_LIBRARY} ${database_LIBRARY} ${network_LIBRARY})
  INSTALL(TARGETS node_shared DESTINATION lib)
ENDIF()
