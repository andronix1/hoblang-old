#include "lexer.h"

bool lexer_init(Lexer *lexer, const char *path) {
	FILE *file = fopen(path, "r");
	if (!file) {
		hob_log(LOGE, "failed to open file `%s`: %s\n", path, strerror(errno));
		return false;
	}
	if (!fatptr_read_all(&lexer->full, file)) {
		hob_log(LOGE, "failed to read file `%s`: %s\n", path, strerror(errno));
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

char *lexer_str(Lexer *lexer) {
	return lexer->remain.str - 1;
}

bool lexer_finished(Lexer *lexer) {
	return lexer->remain.size == 0;
}

char lexer_next_char(Lexer *lexer) {
	if (lexer_finished(lexer)) {
		return EOF;
	}
	lexer->delta++;
	lexer->remain.size--;
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
	lexer->remain.size += lexer->delta;
	lexer->line_offset -= lexer->delta;
	lexer->delta = 0;
	lexer->location = lexer->start_location;
}

void lexer_skip_whitespace(Lexer *lexer) {
	while (char_is_whitespace(lexer_future_char(lexer))) {
		lexer_next_char(lexer);
	}
}

void lexer_free(Lexer *lexer) {
	fatptr_free(&lexer->full);
}
