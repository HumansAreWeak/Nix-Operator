//
// Created by ich on 13.07.23.
//

#ifndef NIX_OPERATOR_STRINGPLUS_H
#define NIX_OPERATOR_STRINGPLUS_H
#include <stdlib.h>
#include <string.h>

/**
 * You must free the result if result is non-NULL.
 * @param orig The string you want to change
 * @param rep Substring that you are searching for
 * @param with Replacement string
 */
char *str_replace(const char *oOrig, const char *oRep, const char *oWith) {
    char* orig = malloc(sizeof(char) * strlen(oOrig));
    char* rep = malloc(sizeof(char) * strlen(oRep));
    char* with = malloc(sizeof(char) * strlen(oWith));

    strcpy(orig, oOrig);
    strcpy(rep, oRep);
    strcpy(with, oWith);

    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    unsigned long len_rep;  // length of rep (the string to remove)
    unsigned long len_with; // length of with (the string to replace rep with)
    unsigned long len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);

    // TODO: Stop memory leakage
    //free(orig);
    //free(rep);
    //free(with);

    return result;
}

#endif //NIX_OPERATOR_STRINGPLUS_H
