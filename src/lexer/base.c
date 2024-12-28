#include "base.h"

Lexer lexer_new(Tokens *tokens) {
	Lexer result = {
		.tokens = tokens
	};
	return result;
}
