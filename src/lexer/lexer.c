#include "lexer.h"

bool lexer_init(Lexer *lexer, const char *path) {
	FILE *file = fopen(path, "r");
	if (!file) {
		hob_log(LOGE, "failed to open file `{cstr}`: {errno}\n", path);
		return false;
	}
	fseek(file, 0, SEEK_END);
	lexer->full.len = ftell(file);
	fseek(file, 0, SEEK_SET);
	lexer->full.str = malloc(lexer->full.len);
	if (fread((char*)lexer->full.str, 1, lexer->full.len, file) != lexer->full.len) {
		hob_log(LOGE, "failed to read file `{cstr}`: {errno}\n", path);
		fclose(file);
		return false;
	}
	lexer->remain = lexer->full;
	lexer->file = path;
	lexer->failed = false;
	lexer->location = lexer->start_location = file_loc_new();
	lexer->line_offset = 0;
	lexer->delta = 0;
    return true;
}

const char *lexer_str(Lexer *lexer) {
	return lexer->remain.str - 1;
}

bool lexer_finished(Lexer *lexer) {
	return lexer->remain.len == 0;
}

char lexer_next_char(Lexer *lexer) {
	if (lexer_finished(lexer)) {
		return EOF;
	}
	lexer->delta++;
	lexer->remain.len--;
	char c = *(lexer->remain.str++);
	if (c == '\n') {
		lexer->line_offset = 0;
		lexer->location.line++;
		lexer->location.column = 0;
	} else {
		lexer->line_offset++;
		lexer->location.column++;
	}
	return c;
}

char lexer_future_char(Lexer *lexer) {
	if (lexer_finished(lexer)) {
		return EOF;
	}
	return *lexer->remain.str;
}

void lexer_begin(Lexer *lexer) {
	lexer->delta = 0;
	lexer->start_location = lexer->location;
}

void lexer_rollback(Lexer *lexer) {
	lexer->remain.str -= lexer->delta;
	lexer->remain.len += lexer->delta;
	lexer->line_offset -= lexer->delta;
	lexer->delta = 0;
	lexer->location = lexer->start_location;
}

void lexer_skip_whitespace(Lexer *lexer) {
	while (char_is_whitespace(lexer_future_char(lexer))) {
		lexer_next_char(lexer);
	}
}
