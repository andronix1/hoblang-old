#include "base.h"

bool parser_init(Parser *parser, Lexer *lexer) {
	parser->lexer = lexer;
	parser->failed = false;
	parser->skip_next= false;
	return true;
}
