FILE(GLOB_RECURSE consensus_SOURCES "*.cpp")

ADD_LIBRARY(consensus_static STATIC ${consensus_SOURCES})
SET_TARGET_PROPERTIES(consensus_static PROPERTIES OUTPUT_NAME consensus)
TARGET_LINK_LIBRARIES(consensus_static ${Boost_LIBRARIES} )
INSTALL(TARGETS consensus_static DESTINATION lib)

IF(ENABLE_SHARED_LIBS)
  ADD_LIBRARY(consensus_shared SHARED ${consensus_SOURCES})
  SET_TARGET_PROPERTIES(consensus_shared PROPERTIES OUTPUT_NAME consensus)
  TARGET_LINK_LIBRARIES(consensus_shared ${Boost_LIBRARIES})
  INSTALL(TARGETS consensus_shared DESTINATION lib)
ENDIF()
