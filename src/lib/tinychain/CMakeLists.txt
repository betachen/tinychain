FILE(GLOB_RECURSE tinychain_SOURCES "*.cpp")

ADD_LIBRARY(tinychain_static STATIC ${tinychain_SOURCES})
SET_TARGET_PROPERTIES(tinychain_static PROPERTIES OUTPUT_NAME tinychain)
TARGET_LINK_LIBRARIES(tinychain_static ${Boost_LIBRARIES} libsecp256k1.a )
INSTALL(TARGETS tinychain_static DESTINATION lib)

IF(ENABLE_SHARED_LIBS)
  ADD_LIBRARY(tinychain_shared SHARED ${tinychain_SOURCES})
  SET_TARGET_PROPERTIES(tinychain_shared PROPERTIES OUTPUT_NAME tinychain)
  TARGET_LINK_LIBRARIES(tinychain_shared ${Boost_LIBRARIES} secp256k1 )
  INSTALL(TARGETS tinychain_shared DESTINATION lib)
ENDIF()
