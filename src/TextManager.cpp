#include <dwrite.h>

#include "structs.h"
#include "TextManager.hpp"

void GetQuotedContent(char* data, int offset, int maxLen) {
    int identifierStart;
    int identifierEnd;

    for (int j = 1; data[offset+j] != '\"'; j++) identifierStart = j + 1;
    for (int j = 1; data[offset+identifierStart+j] != '\"'; j++) identifierEnd = j;

    char *identifier = (char *)malloc(identifierEnd + 1);
    for (int j = 0; j < identifierEnd; j++) {
        identifier[j] = data[offset + 1 + identifierStart + j];
    }
    identifier[identifierEnd] = '\0';
}

void JsonScriptParser(unsigned char* jsonFilePath) {
    int window_size = 15;

    FILE *fp = fopen((const char*)jsonFilePath, "r");

    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *buffer = (char *)malloc(fileSize + 1);

    size_t readSize = fread(buffer, sizeof(char), fileSize, fp);
    buffer[readSize] = '\0';

    int step = 0;

    for (int i = 0; i < readSize; i++) {
        if (buffer[i] == '[') step = 1;
        if (buffer[i] == ']') step = 0;
        if (buffer[i] == '{') step ++;
        if (buffer[i] == '}') step --;

        if (step == 1) {
            GetQuotedContent(buffer, i, fileSize);
        }

        if (step == 2) {
            if (buffer[i] == '}') step--;
        }
    }

    printf("%c", buffer[0]);

    fclose(fp);
    free(buffer);
}