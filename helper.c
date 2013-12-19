/*
 * 
 * @author Adeel Asghar <adeel.asghar@liu.se>
 *
 * Created on: Oct 30, 2013
 *
 */

#include "helper.h"

void logPrint(FILE *stream, const char *format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stream, format, args);
  va_end(args);
}

/*
 * @brief Parses the result like fmiStepSize=0.200000
 * @param data - represents the full result data.
 * @param name - represents the name in the result data e.g fmiStepSize=
 * @param size - the size of the data.
 *
 * Make a new string with size of the value. Copy the string where name= is finished until value is end.
 * Convert the string to double using atof. Free the temporary str before returning.
 *
 */
double unparseDoubleResult(const char* data, const char* name, long size) {
  int valueLength = strlen(name);
  char* str = (char*)malloc(size - valueLength);
  strncpy(str, data+valueLength, size - valueLength);
  double res = atof(str);
  free(str);
  return res;
}

int unparseIntResult(const char* data, const char* name, long size) {
  int valueLength = strlen(name);
  char* str = (char*)malloc(size - valueLength);
  strncpy(str, data+valueLength, size - valueLength);
  int res = atoi(str);
  free(str);
  return res;
}
