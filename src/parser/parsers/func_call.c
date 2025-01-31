#include "../parsers.h"

AstExpr **parse_call_args(Parser *parser) {
	parser_next_token(parser);
	AstExpr** args = vec_new(AstExpr*);
	bool first = true;
	while (token_type(parser->token) != TOKEN_CLOSING_CIRCLE_BRACE) {
		parser->skip_next = first;
		first = false;
		AstExpr *expr = parse_expr(parser, token_funcall_arg_stop);
		if (!expr) {
			return false;
		}
		args = vec_push(args, &expr);
	}
	return args;
}
