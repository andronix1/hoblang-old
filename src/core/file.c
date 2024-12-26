#include "file.h"

bool file_open(File *result, const char *filename, const char *rules) {
    *result = fopen(filename, rules);
    if (*result == NULL) {
        printf("failed to open file: %s\n", strerror(errno));
        return false;
    }
    return true;
}

FileReader file_reader_new(File file) {
    FileReader result = {
        .file = file,
        .location = { 1, 1 }
    };
    return result;
}

bool file_reader_next(FileReader *reader) {
    char c = fgetc(reader->file);
	if (c == 0xFF) {
		return false;
	}
    reader->value = c;
    if (c == '\n') {
        reader->location.line++;
        reader->location.character = 0;
    }
    reader->location.character++;
    return true;
}

bool file_reader_first(FileReader *reader, bool (*filter)(char)) {
    while (true) {
        if (!file_reader_next(reader)) {
            return false;
        }
        if (filter(reader->value)) {
            break;
        }
    }
    return true;
}
