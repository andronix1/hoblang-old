#include <stdio.h>
#include <stdbool.h>
#include "slice.h"

bool slice_read_from_file(Slice *output, const char *path);
size_t slice_write_to(const Slice *slice, FILE *stream);
