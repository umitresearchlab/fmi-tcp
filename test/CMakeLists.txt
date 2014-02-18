cmake_minimum_required(VERSION 2.8)

# Protobuf
INCLUDE(FindProtobuf)
FIND_PACKAGE(Protobuf REQUIRED)
INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})

FIND_PACKAGE(Threads REQUIRED)

LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../lib)

SET(SRCS main.cpp)
SET(HEADERS)

INCLUDE_DIRECTORIES(../src/fmitcp/include)
ADD_EXECUTABLE(main ${HEADERS} ${SRCS})

IF(WIN32)
    SET(WINLIBS
        fmitcp
        fmilib
        shlwapi
        lacewing
        ws2_32
        mswsock
        crypt32
        secur32
        mpr
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${PROTOBUF_LIBRARY}
        ${CMAKE_THREAD_LIBS_INIT}
    )
    TARGET_LINK_LIBRARIES(main     ${WINLIBS})
ELSE(WIN32)
    SET(LINUXLIBS
        fmitcp
        fmilib
        dl
        lacewing
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${PROTOBUF_LIBRARY}
        ${CMAKE_THREAD_LIBS_INIT}
    )
    TARGET_LINK_LIBRARIES(main     ${LINUXLIBS})
ENDIF(WIN32)
