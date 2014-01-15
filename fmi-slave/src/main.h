#ifndef MAIN_H
#define MAIN_H

#include <fmilib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>
#define lw_import
#include <lacewing.h>

#define EXECUTABLE_NAME "fmi-slave"
#define VERSION "0.1.3"

enum FILEFORMAT {
    csv
} fileformat;

#endif /* MAIN_H */
