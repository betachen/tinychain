FILE(GLOB_RECURSE chainbase_SOURCES "*.cpp")

ADD_LIBRARY(chainbase_static STATIC ${chainbase_SOURCES})
SET_TARGET_PROPERTIES(chainbase_static PROPERTIES OUTPUT_NAME chainbase)
TARGET_LINK_LIBRARIES(chainbase_static ${Boost_LIBRARIES})
INSTALL(TARGETS chainbase_static DESTINATION lib)

IF(ENABLE_SHARED_LIBS)
  ADD_LIBRARY(chainbase_shared SHARED ${chainbase_SOURCES})
  SET_TARGET_PROPERTIES(chainbase_shared PROPERTIES OUTPUT_NAME chainbase)
  TARGET_LINK_LIBRARIES(chainbase_shared ${Boost_LIBRARIES})
  INSTALL(TARGETS chainbase_shared DESTINATION lib)
ENDIF()
