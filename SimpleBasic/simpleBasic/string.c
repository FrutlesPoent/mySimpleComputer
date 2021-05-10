#include "string.h"
#include <string.h>
#include <malloc.h>

String *initString(char *data)
{
    String *string = malloc(sizeof(*string));
    if (string == NULL)
        return string;
    int dataLength = strlen(data);
    string->string = malloc(dataLength * sizeof(char));
    strcpy(string->string, data);
    string->length = dataLength;
    return string;
}

String *stringAppend(String *self, char *string)
{
    int length = strlen(string);
    if (self->string == NULL)
    {
        self->string = malloc(sizeof(char) * length);
        self->length = 0;
    }
    else
    {
        self->string = realloc(self->string, (self->length + length) * sizeof(char));
    }
    memcpy(self->string + self->length, string, sizeof(char) * length);
    self->length += length;
    return self;
}

String *stringPrepend(String *self, char *string)
{
    if (self->string == NULL)
    {
        return stringAppend(self, string);
    }
    int length = strlen(string);
    char *buff = malloc(sizeof(char) * (self->length + length));
    memcpy(buff, string, length * sizeof(char));
    memcpy(buff + length, self->string, self->length * sizeof(char));
    free(self->string);
    self->string = buff;
    self->length += length;
    return self;
}
