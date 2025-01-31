#include "../parsers.h"

bool parse_ext_func_decl(Parser *parser, AstExtFuncDecl *info);
bool parse_func_decl(Parser *parser, AstFuncDecl *decl);

bool parse_func_info(Parser *parser, AstFuncInfo *info) {
	parse_exp_next(TOKEN_IDENT, "function name");
	info->name = parser->token->ident;

	parse_exp_next(TOKEN_OPENING_CIRCLE_BRACE, "opening args brace");
	bool parsing_args = true;
	bool was_arg = false;
	info->args = vec_new(AstFuncArg);
	while (parsing_args) {
		parser_next_token(parser);
		switch (token_type(parser->token)) {
			case TOKEN_CLOSING_CIRCLE_BRACE:
				parsing_args = false;
				break;
			case TOKEN_COMMA:
				if (!was_arg) {
					parse_err("commas only after args allowed");
					return false;
				}
				was_arg = false;
				break;
			case TOKEN_IDENT: {
				AstFuncArg arg;
				arg.name = parser->token->ident;
				parse_exp_next(TOKEN_COLON, "arg type delimeter");
				if (!parse_type(parser, &arg.type)) {
					return false;
				}
				info->args = vec_push(info->args, &arg);
				was_arg = true;
				break;
			}
			default:
				parse_err(EXPECTED("function args"));
				return false;
		}
	}

	parse_exp_next(TOKEN_COLON, "returning type");
	parse_type(parser, &info->returning);
	return true;
}

bool parse_module_node(Parser *parser, AstModuleNode *node) {
	parser_next_token(parser);
	switch (token_type(parser->token)) {
		case TOKEN_IMPORT:
			parse_exp_next(TOKEN_STR, "file path");
			node->type = AST_MODULE_NODE_IMPORT;
			char c0 = '\0';
			node->import.path = parser->token->str = vec_push(parser->token->str, &c0);
			parse_exp_next(TOKEN_AS, "module name");
			parse_exp_next(TOKEN_IDENT, "module name");
			node->import.as = parser->token->ident;
			parse_exp_next(TOKEN_SEMICOLON, "semicolon");
			return true;
		// case TOKEN_USE:
		// 	node->type = AST_MODULE_NODE_USE;
		// 	if (!parse_mod_path(parser, &node->use.path)) {
		// 		return false;
		// 	}
		// 	parser_next_token(parser);
		// 	node->use.has_alias = false;
		// 	switch (token_type(parser->token)) {
		// 		case TOKEN_SEMICOLON: break;
		// 		case TOKEN_AS:
		// 			parse_exp_next(TOKEN_IDENT, "alias");
		// 			node->use.has_alias = true;
		// 			node->use.alias = parser->token->ident;
		// 			parse_exp_next(TOKEN_SEMICOLON, "semicolon");
		// 			break;
		// 		default:
		// 			parse_err("expected alias or end of use statement got {tok}", parser->token);
		// 			return false;
		// 	}
		// 	return true;
		case TOKEN_TYPE:
			node->type = AST_MODULE_NODE_TYPE_ALIAS;
			parse_exp_next(TOKEN_IDENT, "alias");
			node->type_alias.alias = parser->token->ident;
			parse_exp_next(TOKEN_ASSIGN, "assign");
			if (!parse_type(parser, &node->type_alias.type)) {
				return false;
			}
			parse_exp_next(TOKEN_SEMICOLON, "semicolon");
			return true;
		case TOKEN_FUN:
			node->type = AST_MODULE_NODE_FUNC;
			return parse_func_decl(parser, &node->func_decl);
		case TOKEN_EXTERN:
			node->type = AST_MODULE_NODE_EXTERNAL_FUNC;
			return parse_ext_func_decl(parser, &node->ext_func_decl);
		default:
			parse_err("unexpected `{tok}` while parsing module node", parser->token);
			return false;
	}
}
