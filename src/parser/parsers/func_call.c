#include "../parsers.h"

bool parse_func_call_args(Parser *parser, AstFuncCall *func_call) {
	parser_next_token(parser);
	func_call->args = vec_new(AstExpr);
	bool first = true;
	while (token_type(parser->token) != TOKEN_CLOSING_CIRCLE_BRACE) {
		AstExpr expr;
		parser->skip_next = first;
		first = false;
		if (!parse_expr(parser, &expr, token_funcall_arg_stop)) {
			return false;
		}
		func_call->args = vec_push(func_call->args, &expr);
	}
	return true;
}
