#pragma once

typedef struct {
	size_t line;
	size_t column;
} FileLocation;

static inline FileLocation file_loc_new() {
	FileLocation result = { 0, 0 };
	return result;
}
