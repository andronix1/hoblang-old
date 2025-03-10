#include "lexer/api.h"
#include <malloc.h>
#include "parser/impl.h"

Parser *parser_new(Lexer *lexer) {
    Parser *parser = malloc(sizeof(Parser));
	parser->lexer = lexer;
	parser->failed = false;
	parser->skip_next = false;
	return parser;
}

Parser *parser_from_file(const char *path) {
    Lexer *lexer = lexer_from_file(path);
    if (!lexer) {
        return NULL;
    }
	return parser_new(lexer);
}

bool parser_failed(const Parser *parser) {
    return parser->failed || lexer_failed(parser->lexer);
}

void parser_print_line_error_at(Parser *parser, FileLocation at) {
    lexer_print_line_error_at(parser->lexer, at);
}
