#include <fmilib.h>
#include <stdio.h>

#ifdef WIN32
#include <stdlib.h>
#include <errno.h>
#endif

#ifndef UTILS_H
#define UTILS_H

/**
 * @brief Indicates if the variable in question should be printed to the out file. It basically checks the variable type.
 * @return Nonzero if the variable should be printed
 */
int shouldBePrinted(fmi1_import_variable_t * v);

/**
 * @brief Writes header data in CSV format to file.
 * E.g. time,fmuName.variableName1,fmuName.variableName2,... It always prints an extra "time" variable first.
 * @param file
 * @param fmuNames
 * @param fmus
 * @param numFMUs
 * @param separator CSV separator character to use
 */
void writeCsvHeader1(FILE* file,
                     char** fmuNames,
                     fmi1_import_t ** fmus,
                     int numFMUs,
                     char separator);
/**
 * @see writeCsvHeader1
 */
void writeCsvHeader2(FILE* file,
                     char** fmuNames,
                     fmi2_import_t ** fmus,
                     int numFMUs,
                     char separator);

/**
 * @brief Writes a single CSV row to an outfile.
 * @param file File to write to
 * @param fmus
 * @param numFMUs
 * @param time Current simulation time. This value will be printed first.
 * @param separator CSV separator character to use
 */
void writeCsvRow1(FILE* file,
                  fmi1_import_t ** fmus,
                  int numFMUs,
                  fmi1_real_t time,
                  char separator);
/**
 * @see writeCsvRow1
 */
void writeCsvRow2(FILE* file,
                  fmi2_import_t ** fmus,
                  int numFMUs,
                  fmi2_real_t time,
                  char separator);

/**
 * Transfer values in a connection
 * @todo should transfer more cleverly
 * @todo apparently fmi1_import_get_variable_list() allocates new list objects. Reuse somehow?
 */
void fmi1TransferConnectionValues(  connection c,
                                    fmi1_import_t ** fmus,
                                    fmi1_import_variable_list_t** variables);

/**
 * @see fmi1TransferConnectionValues
 */
void fmi2TransferConnectionValues(  connection c,
                                    fmi2_import_t ** fmus,
                                    fmi2_import_variable_list_t** variables);

#ifdef WIN32
char *realpath(const char * file_name,
		       char * resolved_name);
#endif

#endif /* UTILS_H */
