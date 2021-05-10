#include "string.h"

// String* newString() { // create empty string
//     String* str = malloc(sizeof(*str));
//     if (str == NULL)
//         return str;
//     str->string = NULL;
//     str->length = 0;
//     return str;
// } useless

String* newStringFirst(char* data) {
    String* str = malloc(sizeof(*str));
    if (str == NULL) 
        return str;
    int dataLength = strlen(data); // char length
    str->string = malloc(dataLength * sizeof(char));
    strcpy(str->string, data); // copy data into str string
    str->length = dataLength; // lenght of data
    return str;
}

String* stringAppendOne(String* str1, char const* str2) {
    int stringLenght = strlen(str2); // size of str2 
    if (str1->string == NULL) {
        str1->string = malloc(sizeof(char) * stringLenght);
        str1->length = 0;
    } else {
        str1->string = realloc(str1->string, (str1->length + stringLenght) * sizeof(char));

    }
    memcpy(str1->string + str1->length, str2, sizeof(char) * stringLenght); // copy into str1 from str 2
    str1->length += stringLenght;
    return str1;

}

String* stringPrependOne(String* str1, char const* str2) {
    if (str1->string == NULL) // if we have empty string then just create new one
        return stringAppendOne(str1, str2);

    int stringLenght = strlen(str2);
    char* buff = malloc(sizeof(char) * (str1->length + stringLenght));
    memcpy(buff, str2, stringLenght * sizeof(char));
    memcpy(buff + stringLenght, str1->string, str1->length + sizeof(char));
    free(str1->string);
    str1->string = buff;
    str1->length += stringLenght;
    return str1;
}