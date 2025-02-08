#include "parser.h"

bool parser_init(Parser *parser, Lexer *lexer) {
	parser->lexer = lexer;
	parser->failed = false;
	parser->skip_next= false;
	return true;
}

void parser_next_token(Parser *parser) {
	if (parser->skip_next) {
		parser->skip_next = false;
		return;
	}
	if (!lex_next(parser->lexer)) {
		parser->token = NULL;
	} else {
		parser->token = lexer_token(parser->lexer);
	}
}

void parse_skip_to(Parser *parser, ParseStopFunc stop) {
	while (stop(token_type(parser->token)) && token_type(parser->token) != TOKEN_EOI) {
		parser_next_token(parser);
	}
}

