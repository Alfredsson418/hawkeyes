#include "../../include/other/replace.h"

void replace_substring(char **str, const char *old_sub, const char *new_sub) {
    int count = 0;
    // Count the occurrences of old_sub in str
    for (int i = 0; (*str)[i] != '\0'; i++) {
        if (strstr(&(*str)[i], old_sub) == &(*str)[i]) {
            count++;
        }
    }

    char *result = calloc(strlen(*str) + count * (strlen(new_sub) - strlen(old_sub)) + 1, sizeof(char));
    int result_place = 0;
    for (int i = 0; (*str)[i] != '\0';) {
        if (strstr(&(*str)[i], old_sub) == &(*str)[i]) {
            strcat(result, new_sub);
            result_place += strlen(new_sub);
            i += strlen(old_sub);

        } else {
            result[result_place] = (*str)[i];
            result_place++;
            i++;
        }
    }
    result[result_place] = '\0';

    *str = realloc(*str, strlen(result) + 1);
    strcpy(*str, result);
    free(result);
}

void replace_format(char **format, char *substring, char *replacement) {
    if (replacement == NULL) {
        ERR_PRINT("%s cant be replaced with NULL\n", substring);
        replace_substring(format, substring, "NULL");

    }else if (strstr(*format, substring) != NULL) {
        replace_substring(format, substring, replacement);
    } else {
        replace_substring(format, substring, "NULL");
    }

}
