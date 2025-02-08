#include "../parsers.h"

AstExpr **parse_call_args(Parser *parser) {
	AstExpr** args = vec_new(AstExpr*);
	if (parser_next_is(parser, TOKEN_CLOSING_CIRCLE_BRACE)) {
		return args;
	}
	while (true) {
		AstExpr *expr = parse_expr(parser, token_funcall_arg_stop);
		if (!expr) {
			return false;
		}
		args = vec_push(args, &expr);
		switch (parser_next(parser)->type) {
			case TOKEN_COMMA: break;
			case TOKEN_CLOSING_CIRCLE_BRACE: return args;
			default:
				PARSE_ERROR(EXPECTED("arg stop"));
				return NULL;
		}
	}
}
