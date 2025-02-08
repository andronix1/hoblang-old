#include "impl.h"

Lexer *lexer_from_file(const char *path) {
	Slice content;
	if (!slice_read_from_file(&content, path)) {
		hob_log(LOGE, "failed to read file `{cstr}`: {errno}\n", path);
		return NULL;
	}
    return lexer_new(content, path);
}

Lexer *lexer_new(Slice content, const char *path) {
    Lexer *lexer = malloc(sizeof(Lexer));
	lexer->remain = lexer->full = content;
	lexer->file = path;
	lexer->failed = false;
	lexer->location = lexer->start_location = file_loc_new();
	lexer->line_offset = 0;
	lexer->delta = 0;
    return lexer;
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

bool lexer_finished(const Lexer *lexer) {
	return lexer->remain.len == 0;
}

Token *lexer_token(Lexer *lexer) {
	return &lexer->token;
}

LexerPosition lexer_position(const Lexer *lexer) {
    LexerPosition result;
    result.file = lexer->file;
    result.location = lexer->location;
    return result;
}

bool lexer_failed(const Lexer *lexer) {
	return lexer->failed;
}
