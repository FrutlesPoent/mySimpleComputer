#ifndef STRING_H
#define STRING_H

typedef struct _String
{
    char *string;
    int length;
} String;

String *initString(char *data);
String *stringAppend(String *self, char *string);
String *stringPrepend(String *self, char *string);

#endif