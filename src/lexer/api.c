#include "lexer/api.h"
#include "core/location.h"
#include "lexer/impl.h"

InFilePosition lexer_position(const Lexer *lexer) {
    InFilePosition result;
    result.file = lexer->file;
    result.location = lexer->location;
    return result;
}

bool lexer_failed(const Lexer *lexer) {
	return lexer->failed;
}

void lexer_print_line_error_at(Lexer *lexer, FileLocation at) {
    size_t line = 0;
    size_t len = 0;
    size_t i = 0;
    for (; line < at.line && i < lexer->full.len; i++) {
        if (lexer->full.str[i] == '\n') {
            line++;
        }
    }
    size_t j = i;
    while (j < lexer->full.len && lexer->full.str[j] != '\n') {
        j++;
        len++;
    }
    Slice slice = slice_new(&lexer->full.str[i], len);
    print("| {slice}\n| ", &slice);
    for (i = 0; i < at.column; i++) {
        print(" ");
    }
    print("^\n");
}

Token *lexer_next(Lexer *lexer) {
    while (!lexer_try_next(lexer));
    return &lexer->token;
}

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


