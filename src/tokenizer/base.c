#include "base.h"

Tokenizer tokenizer_new(FatPtr content) {
    Tokenizer result = {
		.remain = content,
		.location = { 0, 0 },
		.start_location = { 0, 0 },
		.line_offset = 0,
		.delta = 0,
    };
    return result;
}

char *tokenizer_str(Tokenizer *tokenizer) {
	return tokenizer->remain.str - 1;
}

bool tokenizer_finished(Tokenizer *tokenizer) {
	return tokenizer->remain.size == 0;
}

char tokenizer_next_char(Tokenizer *tokenizer) {
	if (tokenizer_finished(tokenizer)) {
		return EOF;
	}
	tokenizer->delta++;
	tokenizer->remain.size--;
	char c = *(tokenizer->remain.str++);
	if (c == '\n') {
		tokenizer->line_offset = 0;
		tokenizer->location.line++;
		tokenizer->location.column = 0;
	} else {
		tokenizer->line_offset++;
		tokenizer->location.column++;
	}
	return c;
}

char tokenizer_future_char(Tokenizer *tokenizer) {
	if (tokenizer_finished(tokenizer)) {
		return EOF;
	}
	return *tokenizer->remain.str;
}

void tokenizer_begin(Tokenizer *tokenizer) {
	tokenizer->delta = 0;
	tokenizer->start_location = tokenizer->location;
}

void tokenizer_rollback(Tokenizer *tokenizer) {
	tokenizer->remain.str -= tokenizer->delta;
	tokenizer->remain.size += tokenizer->delta;
	tokenizer->line_offset -= tokenizer->delta;
	tokenizer->delta = 0;
	tokenizer->location = tokenizer->start_location;
}

void tokenizer_skip_whitespace(Tokenizer *tokenizer) {
	while (char_is_whitespace(tokenizer_future_char(tokenizer))) {
		tokenizer_next_char(tokenizer);
	}
}

