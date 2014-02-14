#include "main.h"
#include "utils.h"

int shouldBePrinted(fmi1_import_variable_t * v){
    // Get variability
    fmi1_variability_enu_t variability = fmi1_import_get_variability(v);
    // Don't print parameters
    switch(variability){
        case fmi1_variability_enu_continuous:
        case fmi1_variability_enu_discrete:
            return 1;
        case fmi1_variability_enu_constant:
        case fmi1_variability_enu_parameter:
        case fmi1_variability_enu_unknown:
            return 0;
    }
}

int shouldBePrinted2(fmi2_import_variable_t * v){
    // Get variability
    fmi2_variability_enu_t variability = fmi2_import_get_variability(v);
    // Don't print parameters
    switch(variability){
        case fmi2_variability_enu_continuous:
        case fmi2_variability_enu_discrete:
            return 1;
        case fmi2_variability_enu_constant:
        case fmi2_variability_enu_unknown:
            return 0;
    }
}

void writeCsvHeader1(   FILE* file,
                        char** fmuNames,
                        fmi1_import_t ** fmus,
                        int numFMUs,
                        char separator){
    int k,j;
    fmi1_real_t r;
    fmi1_integer_t i;
    fmi1_boolean_t b;
    fmi1_string_t s;
    fmi1_value_reference_t vr;
    char buffer[32];

    // First column is always time
    fprintf(file, "time");

    for(j=0; j<numFMUs; j++){
        // print all other columns
        fmi1_import_variable_list_t* vl = fmi1_import_get_variable_list(fmus[j]);
        int n = fmi1_import_get_variable_list_size(vl);
        for (k=0; n; k++) {
            fmi1_import_variable_t* v = fmi1_import_get_variable(vl,k);
            if(!v) break;
            //fmi1_import_variable_typedef_t* vt = fmi1_import_get_variable_declared_type(v);
            vr = fmi1_import_get_variable_vr(v);

            if(!shouldBePrinted(v))
                continue;

            const char* s = fmi1_import_get_variable_name(v);
            // output names only
            if (separator==',') {
                // treat array element, e.g. print a[1, 2] as a[1.2]
                fprintf(file, "%c%s.", separator,fmuNames[j]);
                while(*s){
                   if(*s!=' ') fprintf(file, "%c", *s==',' ? '.' : *s);
                   s++;
                }
            } else
                fprintf(file, "%c%s.%s", separator, fmuNames[j], s);
        } // for
    }

    // terminate this row
    fprintf(file, "\n");
}

void writeCsvHeader2(   FILE* file,
                        char** fmuNames,
                        fmi2_import_t ** fmus,
                        int numFMUs,
                        char separator){
    int k,j;
    fmi2_real_t r;
    fmi2_integer_t i;
    fmi2_boolean_t b;
    fmi2_string_t s;
    fmi2_value_reference_t vr;
    char buffer[32];

    // First column is always time
    fprintf(file, "time");

    for(j=0; j<numFMUs; j++){
        // print all other columns
        fmi2_import_variable_list_t* vl = fmi2_import_get_variable_list(fmus[j],0);
        int n = fmi2_import_get_variable_list_size(vl);
        for (k=0; n; k++) {
            fmi2_import_variable_t* v = fmi2_import_get_variable(vl,k);
            if(!v) break;
            //fmi2_import_variable_typedef_t* vt = fmi2_import_get_variable_declared_type(v);
            vr = fmi2_import_get_variable_vr(v);

            if(!shouldBePrinted2(v))
                continue;

            const char* s = fmi2_import_get_variable_name(v);
            // output names only
            if (separator==',') {
                // treat array element, e.g. print a[1, 2] as a[1.2]
                fprintf(file, "%c%s.", separator,fmuNames[j]);
                while(*s){
                   if(*s!=' ') fprintf(file, "%c", *s==',' ? '.' : *s);
                   s++;
                }
            } else
                fprintf(file, "%c%s.%s", separator, fmuNames[j], s);
        } // for
    }

    // terminate this row
    fprintf(file, "\n");
}

void writeCsvRow1(  FILE* file,
                    fmi1_import_t ** fmus,
                    int numFMUs,
                    fmi1_real_t time,
                    char separator){
    int k,j;
    fmi1_real_t r;
    fmi1_integer_t i;
    fmi1_boolean_t b;
    fmi1_string_t s;
    fmi1_value_reference_t vr;
    char buffer[32];

    // First column is always time
    fprintf(file, "%.16g",time);

    for(j=0; j<numFMUs; j++){
        // print all other columns
        fmi1_import_variable_list_t* vl = fmi1_import_get_variable_list(fmus[j]);
        int n = fmi1_import_get_variable_list_size(vl);
        for (k=0; n; k++) {
            fmi1_import_variable_t* v = fmi1_import_get_variable(vl,k);
            if(!v) break;

            if(!shouldBePrinted(v))
                continue;

            //fmi1_import_variable_typedef_t* vt = fmi1_import_get_variable_declared_type(v);
            vr = fmi1_import_get_variable_vr(v);
            const char* s = fmi1_import_get_variable_name(v);

            // output values
            fmi1_base_type_enu_t type = fmi1_import_get_variable_base_type(v);
            fmi1_value_reference_t vr[1];
            vr[0] = fmi1_import_get_variable_vr(v);
            fmi1_real_t rr[1];
            fmi1_boolean_t bb[1];
            fmi1_integer_t ii[1];
            fmi1_string_t ss[1000];
            switch (type){
            case fmi1_base_type_real :
                fmi1_import_get_real(fmus[j], vr, 1, rr);
                if (separator==',')
                    fprintf(file, ",%.16g", rr[0]);
                else {
                    // separator is e.g. ';' or '\t'
                    //doubleToCommaString(buffer, r);
                    fprintf(file, "%c%f", separator, rr[0]);
                }
                break;
            case fmi1_base_type_int:
            case fmi1_base_type_enum:
                fmi1_import_get_integer(fmus[j], vr, 1, ii);
                fprintf(file, "%c%d", separator, ii[0]);
                break;
            case fmi1_base_type_bool:
                fmi1_import_get_boolean(fmus[j], vr, 1, bb);
                fprintf(file, "%c%d", separator, bb[0]);
                break;
            case fmi1_base_type_str:
                fmi1_import_get_string(fmus[j], vr, 1, ss);
                fprintf(file, "%c%s", separator, ss[0]);
                break;
            default:
                fprintf(file, "NoValueForType");
                break;
            }
        }
    }

    // terminate this row
    fprintf(file, "\n");
}


void writeCsvRow2(  FILE* file,
                    fmi2_import_t ** fmus,
                    int numFMUs,
                    fmi2_real_t time,
                    char separator){
    int k,j;
    fmi2_real_t r;
    fmi2_integer_t i;
    fmi2_boolean_t b;
    fmi2_string_t s;
    fmi2_value_reference_t vr;
    char buffer[32];

    // First column is always time
    fprintf(file, "%.16g",time);

    for(j=0; j<numFMUs; j++){
        // print all other columns
        fmi2_import_variable_list_t* vl = fmi2_import_get_variable_list(fmus[j],0);
        int n = fmi2_import_get_variable_list_size(vl);
        for (k=0; n; k++) {
            fmi2_import_variable_t* v = fmi2_import_get_variable(vl,k);
            if(!v) break;

            if(!shouldBePrinted2(v))
                continue;

            //fmi2_import_variable_typedef_t* vt = fmi2_import_get_variable_declared_type(v);
            vr = fmi2_import_get_variable_vr(v);
            const char* s = fmi2_import_get_variable_name(v);

            // output values
            fmi2_base_type_enu_t type = fmi2_import_get_variable_base_type(v);
            fmi2_value_reference_t vr[1];
            vr[0] = fmi2_import_get_variable_vr(v);
            fmi2_real_t rr[1];
            fmi2_boolean_t bb[1];
            fmi2_integer_t ii[1];
            fmi2_string_t ss[1000];
            switch (type){
            case fmi2_base_type_real :
                fmi2_import_get_real(fmus[j], vr, 1, rr);
                if (separator==',')
                    fprintf(file, ",%.16g", rr[0]);
                else {
                    // separator is e.g. ';' or '\t'
                    //doubleToCommaString(buffer, r);
                    fprintf(file, "%c%f", separator, rr[0]);
                }
                break;
            case fmi2_base_type_int:
            case fmi2_base_type_enum:
                fmi2_import_get_integer(fmus[j], vr, 1, ii);
                fprintf(file, "%c%d", separator, ii[0]);
                break;
            case fmi2_base_type_bool:
                fmi2_import_get_boolean(fmus[j], vr, 1, bb);
                fprintf(file, "%c%d", separator, bb[0]);
                break;
            case fmi2_base_type_str:
                fmi2_import_get_string(fmus[j], vr, 1, ss);
                fprintf(file, "%c%s", separator, ss[0]);
                break;
            default:
                fprintf(file, "NoValueForType");
                break;
            }
        }
    }

    // terminate this row
    fprintf(file, "\n");
}

#ifdef WIN32
char *realpath(const char *name, char *resolved)
{
  char *retname = NULL;  /* we will return this, if we fail */

  /* SUSv3 says we must set `errno = EINVAL', and return NULL,
   * if `name' is passed as a NULL pointer.
   */

  if( name == NULL )
    errno = EINVAL;

  /* Otherwise, `name' must refer to a readable filesystem object,
   * if we are going to resolve its absolute path name.
   */

  else if( access( name, 4 ) == 0 )
  {
    /* If `name' didn't point to an existing entity,
     * then we don't get to here; we simply fall past this block,
     * returning NULL, with `errno' appropriately set by `access'.
     *
     * When we _do_ get to here, then we can use `_fullpath' to
     * resolve the full path for `name' into `resolved', but first,
     * check that we have a suitable buffer, in which to return it.
     */

    if( (retname = resolved) == NULL )
    {
      /* Caller didn't give us a buffer, so we'll exercise the
       * option granted by SUSv3, and allocate one.
       *
       * `_fullpath' would do this for us, but it uses `malloc', and
       * Microsoft's implementation doesn't set `errno' on failure.
       * If we don't do this explicitly ourselves, then we will not
       * know if `_fullpath' fails on `malloc' failure, or for some
       * other reason, and we want to set `errno = ENOMEM' for the
       * `malloc' failure case.
       */

      retname = malloc( _MAX_PATH );
    }

    /* By now, we should have a valid buffer.
     * If we don't, then we know that `malloc' failed,
     * so we can set `errno = ENOMEM' appropriately.
     */

    if( retname == NULL )
      errno = ENOMEM;

    /* Otherwise, when we do have a valid buffer,
     * `_fullpath' should only fail if the path name is too long.
     */

    else if( (retname = _fullpath( retname, name, _MAX_PATH )) == NULL )
      errno = ENAMETOOLONG;
  }

  /* By the time we get to here,
   * `retname' either points to the required resolved path name,
   * or it is NULL, with `errno' set appropriately, either of which
   * is our required return condition.
   */

  return retname;
}
#endif
