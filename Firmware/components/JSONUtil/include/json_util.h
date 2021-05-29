#ifndef JSON_UTIL
#define JSON_UTIL

#include "cJSON.h"

char* json_create(char *value1);
char* json_create_attributes(char *value1, char *value2, char *value3, char *value4, char *value5);
char* json_update_telemetry(char *value1, char *value2, char *value3, char *value4, char *value5, char *value6);

#endif /* JSON_UTIL  */
