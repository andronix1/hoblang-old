#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include "fatptr.h"

typedef FILE* File;

typedef struct {
    size_t line;
    size_t character;
} FileLocation;

typedef struct {
    File file;
    FileLocation location;
    char value;
} FileReader;

bool file_open(File *result, const char *filename, const char *rules);
FileReader file_reader_new(File file);
bool file_reader_next(FileReader *reader);
bool file_reader_first(FileReader *reader, bool (*filter)(char));
