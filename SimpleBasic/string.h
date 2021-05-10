#ifndef STRING_H
#define STRING_H
#include <string.h>
#include <malloc.h>

typedef struct _String {
    char* string;
    int length;
} String;

String* newString();
String* newStringFirst(char* data);
String* stringAppendOne(String* str1, char const* str2);
String* stringPrependOne(String* str1, char const* str2);

#endif