
# For mongoose.c including
#INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/include/restful-cpp/contrib") 

#SET(CMAKE_C_FLAGS "-std=c11 ${COMMON_WARN} -Wnodeprecated-register")
SET(CMAKE_C_FLAGS "-std=c99 -Wno-deprecated-register")
FILE(GLOB_RECURSE restful_cpp_SOURCES "*.cpp")

ADD_LIBRARY(restful_cpp_static STATIC ${restful_cpp_SOURCES})
SET_TARGET_PROPERTIES(restful_cpp_static PROPERTIES OUTPUT_NAME restful_cpp)
#TARGET_LINK_LIBRARIES(restful_cpp_static ${Boost_LIBRARIES})
INSTALL(TARGETS restful_cpp_static DESTINATION lib)

IF(ENABLE_SHARED_LIBS)
    ADD_LIBRARY(restful_cpp_shared SHARED ${restful_cpp_SOURCES})
    SET_TARGET_PROPERTIES(restful_cpp_shared PROPERTIES OUTPUT_NAME restful_cpp)
    #TARGET_LINK_LIBRARIES(restful_cpp_shared ${Boost_LIBRARIES})
    INSTALL(TARGETS restful_cpp_shared DESTINATION lib)
ENDIF()
