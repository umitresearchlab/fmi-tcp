cmake_minimum_required(VERSION 2.8)

INCLUDE_DIRECTORIES(../ ${FMIL_INCLUDE_DIR} ${LACEWING_INCLUDE_DIR})
LINK_DIRECTORIES(${FMIL_LIBS_DIR} ${LACEWING_LIBS_DIR})

SET(SRCS
    src/main.c
    src/utils.c
    src/parseargs.c
    src/jacobi.c
    src/gs.c
    src/fmiclient.c
    ../helper.c
)
SET(HEADERS
    src/main.h
    src/utils.h
    src/parseargs.h
    src/jacobi.h
    src/gs.h
    src/fmiclient.h
    ../commands.h
    ../helper.h
)

SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_LIST_DIR}/../bin")

ADD_EXECUTABLE(fmi-slave ${HEADERS} ${SRCS})

IF(WIN32)
TARGET_LINK_LIBRARIES(fmi-slave fmilib shlwapi lacewing ws2_32 mswsock crypt32 secur32 mpr)
ELSE(WIN32)
TARGET_LINK_LIBRARIES(fmi-slave fmilib dl lacewing)
ENDIF(WIN32)

INSTALL (TARGETS fmi-slave DESTINATION bin)
INSTALL (FILES ${CMAKE_CURRENT_LIST_DIR}/man/fmi-slave.1 DESTINATION man/man1)
