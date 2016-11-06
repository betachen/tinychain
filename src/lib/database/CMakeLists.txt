FILE(GLOB_RECURSE database_SOURCES "*.cpp")

ADD_LIBRARY(database_static STATIC ${database_SOURCES})
SET_TARGET_PROPERTIES(database_static PROPERTIES OUTPUT_NAME database)
TARGET_LINK_LIBRARIES(database_static ${Boost_LIBRARIES} ${tinychain_LIBRARY})
INSTALL(TARGETS database_static DESTINATION lib)

IF(ENABLE_SHARED_LIBS)
  ADD_LIBRARY(database_shared SHARED ${database_SOURCES})
  SET_TARGET_PROPERTIES(database_shared PROPERTIES OUTPUT_NAME database)
  TARGET_LINK_LIBRARIES(database_shared ${Boost_LIBRARIES} ${tinychain_LIBRARY})
  INSTALL(TARGETS database_shared DESTINATION lib)
ENDIF()
