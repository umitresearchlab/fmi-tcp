cmake_minimum_required(VERSION 2.8)

INCLUDE_DIRECTORIES(../ ${FMIL_INCLUDE_DIR} ${LACEWING_INCLUDE_DIR})
LINK_DIRECTORIES(${FMIL_LIBS_DIR} ${LACEWING_LIBS_DIR})

SET(SRCS
    src/main.c
    src/utils.c
    src/parseargs.c
    src/fmiclient.c
    ../helper.c
)
SET(HEADERS
    include/main.h
    include/utils.h
    include/parseargs.h
    include/fmiclient.h
    ../commands.h
    ../helper.h
)

SET(SLAVE_SRCS
    src/main.cpp
    src/Slave.cpp
)
SET(SLAVE_HEADERS
    include/Slave.h
)

SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_LIST_DIR}/../bin")
INCLUDE_DIRECTORIES(include)
ADD_EXECUTABLE(fmi-slave ${HEADERS}         ${SRCS})
ADD_EXECUTABLE(slave     ${SLAVE_HEADERS}   ${SLAVE_SRCS})

IF(WIN32)
    SET(WINLIBS fmilib shlwapi lacewing ws2_32 mswsock crypt32 secur32 mpr)
    TARGET_LINK_LIBRARIES(fmi-slave ${WINLIBS})
    TARGET_LINK_LIBRARIES(slave     ${WINLIBS})
ELSE(WIN32)
    SET(LINUXLIBS fmilib dl lacewing)
    TARGET_LINK_LIBRARIES(fmi-slave ${LINUXLIBS})
    TARGET_LINK_LIBRARIES(slave     ${LINUXLIBS})
ENDIF(WIN32)

INSTALL (TARGETS fmi-slave  DESTINATION bin)
INSTALL (TARGETS slave      DESTINATION bin)
