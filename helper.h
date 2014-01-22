/*
 * 
 * @author Adeel Asghar <adeel.asghar@liu.se>
 *
 * Created on: Oct 30, 2013
 *
 */

#ifndef HELPER_H_
#define HELPER_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

void logPrint(FILE *stream, const char *format, ...);
void debugPrint(int debugFlag, FILE *stream, const char *format, ...);

double unparseDoubleResult(const char* data, const char* name, long size);
int unparseIntResult(const char* data, const char* name, long size);

#endif /* HELPER_H_ */
