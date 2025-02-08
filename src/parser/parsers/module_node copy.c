#include "../parsers.h"

bool parse_ext_func_decl(Parser *parser, AstExtFuncDecl *info);
bool parse_func_decl(Parser *parser, AstFuncDecl *decl);

bool parse_func_info(Parser *parser, AstFuncInfo *info) {
	info->name = PARSER_EXPECT_NEXT(TOKEN_IDENT, "function name")->ident;
	PARSER_EXPECT_NEXT(TOKEN_OPENING_CIRCLE_BRACE, "opening args brace");
	bool parsing_args = true;
	bool was_arg = false;
	info->args = vec_new(AstFuncArg);
	while (parsing_args) {
		Token *token = parser_next(parser);
		switch (token->type) {
			case TOKEN_CLOSING_CIRCLE_BRACE:
				parsing_args = false;
				break;
			case TOKEN_COMMA:
				if (!was_arg) {
					PARSE_ERROR("commas only after args allowed");
					return false;
				}
				was_arg = false;
				break;
			case TOKEN_IDENT: {
				AstFuncArg arg;
				arg.name = token->ident;
				PARSER_EXPECT_NEXT(TOKEN_COLON, "arg type delimeter");
				if (!parse_type(parser, &arg.type)) {
					return false;
				}
				info->args = vec_push(info->args, &arg);
				was_arg = true;
				break;
			}
			default:
				PARSE_ERROR(EXPECTED("function args"));
				return false;
		}
	}

	PARSER_EXPECT_NEXT(TOKEN_FUNC_RETURNS, "returning type");
	parse_type(parser, &info->returning);
	return true;
}

bool parse_module_node(Parser *parser, AstModuleNode *node) {
	parser_step(parser);
	switch (parser_token(parser)->type) {
		case TOKEN_IMPORT:
			node->type = AST_MODULE_NODE_IMPORT;
			char c0 = '\0';
			node->import.path = PARSER_EXPECT_NEXT(TOKEN_STR, "file path")->str = vec_push(parser_token(parser)->str, &c0);
			PARSER_EXPECT_NEXT(TOKEN_AS, "module name");
			node->import.as = PARSER_EXPECT_NEXT(TOKEN_IDENT, "module name")->ident;
			PARSER_EXPECT_NEXT(TOKEN_SEMICOLON, "semicolon");
			return true;
		case TOKEN_USE:
			node->type = AST_MODULE_NODE_USE;
			if (!parse_decl_path(parser, &node->use.path)) {
				return false;
			}
			parser_step(parser);
			node->use.has_alias = false;
			switch (parser_token(parser)->type) {
				case TOKEN_SEMICOLON: break;
				case TOKEN_AS:
					node->use.has_alias = true;
					node->use.alias = PARSER_EXPECT_NEXT(TOKEN_IDENT, "alias")->ident;
					PARSER_EXPECT_NEXT(TOKEN_SEMICOLON, "semicolon");
					break;
				default:
					PARSE_ERROR("expected alias or end of use statement got {tok}", parser_token(parser));
					return false;
			}
			return true;
		case TOKEN_CONST:
			node->type = AST_MODULE_NODE_CONST;
			node->constant.name = PARSER_EXPECT_NEXT(TOKEN_IDENT, "name")->ident;
			PARSER_EXPECT_NEXT(TOKEN_COLON, "colon");
			if (!parse_type(parser, &node->constant.type)) {
				return false;
			}
			PARSER_EXPECT_NEXT(TOKEN_ASSIGN, "assign");
			node->constant.expr = parse_expr(parser, token_semicolon_stop);
			if (!node->constant.expr) {
				return false;
			}
			PARSER_EXPECT_NEXT(TOKEN_SEMICOLON, "semicolon");
			return true;
		case TOKEN_TYPE:
			node->type = AST_MODULE_NODE_TYPE_ALIAS;
			node->type_alias.alias = PARSER_EXPECT_NEXT(TOKEN_IDENT, "alias")->ident;
			PARSER_EXPECT_NEXT(TOKEN_ASSIGN, "assign");
			if (!parse_type(parser, &node->type_alias.type)) {
				return false;
			}
			PARSER_EXPECT_NEXT(TOKEN_SEMICOLON, "semicolon");
			return true;
		case TOKEN_FUN:
			node->type = AST_MODULE_NODE_FUNC;
			return parse_func_decl(parser, &node->func_decl);
		case TOKEN_EXTERN:
			node->type = AST_MODULE_NODE_EXTERNAL_FUNC;
			return parse_ext_func_decl(parser, &node->ext_func_decl);
		default:
			PARSE_ERROR("unexpected `{tok}` while parsing module node", parser_token(parser));
			return false;
	}
}
