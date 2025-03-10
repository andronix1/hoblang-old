#include "ast/private/module_node.h"
#include "lexer/token.h"
#include "parser/parts/module_node.h"
#include "parser/parts/type.h"
#include "parser/parts/path.h"
#include "parser/parts/expr.h"
#include "parser/parts/val_decl.h"
#include "parser/private.h"
#include "parser/token_stops.h"

bool parse_ext_func_decl(Parser *parser, Slice name, AstExtFuncDecl *info);
bool parse_ext_var_decl(Parser *parser, Slice name, AstExtVarDecl *info);
bool parse_func_decl(Parser *parser, AstFuncDecl *decl);

bool parse_module_node_decl(Parser *parser, AstModuleNode *node) {
	Token *token = parser_next(parser);
    node->loc = token->location;
	switch (token->type) {
		case TOKEN_IMPORT: {
			Token *path_token = PARSER_EXPECT_NEXT(TOKEN_STR, "file path");
			node->type = AST_MODULE_NODE_IMPORT;
			char c0 = '\0';
			node->import.path = path_token->str = vec_push(path_token->str, &c0);
			PARSER_EXPECT_NEXT(TOKEN_AS, "module name");
			node->import.as = PARSER_EXPECT_NEXT(TOKEN_IDENT, "module name")->ident;
			PARSER_EXPECT_NEXT(TOKEN_SEMICOLON, "semicolon");
			return true;
		}
		case TOKEN_STRUCT: {
			node->type = AST_MODULE_NODE_STRUCT_DEF;
			return parse_ast_struct_def(parser, &node->struct_def);
		}
		case TOKEN_USE:
			node->type = AST_MODULE_NODE_USE;
			if (!(node->use.path = parse_path(parser))) {
				return false;
			}
			node->use.has_alias = false;
			switch (parser_next(parser)->type) {
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
		case TOKEN_VAR:
		case TOKEN_FINAL:
            parser_skip_next(parser);
			node->type = AST_MODULE_NODE_VAL_DECL;
			return parse_val_decl(parser, &node->val_decl);
		case TOKEN_TYPE:
			node->type = AST_MODULE_NODE_TYPE_ALIAS;
			node->type_alias.alias = PARSER_EXPECT_NEXT(TOKEN_IDENT, "alias")->ident;
			PARSER_EXPECT_NEXT(TOKEN_ASSIGN, "assign");
			if (!(node->type_alias.type = parse_type(parser))) {
				return false;
			}
			PARSER_EXPECT_NEXT(TOKEN_SEMICOLON, "semicolon");
			return true;
		case TOKEN_FUN:
			node->type = AST_MODULE_NODE_FUNC;
			return parse_func_decl(parser, &node->func_decl);
		case TOKEN_EXTERN:
            PARSER_EXPECT_NEXT(TOKEN_OPENING_CIRCLE_BRACE, "external name opening");
            Slice ext_name = PARSER_EXPECT_NEXT(TOKEN_IDENT, "external name")->ident;
            PARSER_EXPECT_NEXT(TOKEN_CLOSING_CIRCLE_BRACE, "external name closing");
            switch (parser_next(parser)->type) {
                case TOKEN_FUN:
                    node->type = AST_MODULE_NODE_EXTERNAL_FUNC;
                    return parse_ext_func_decl(parser, ext_name, &node->ext_func_decl);
                case TOKEN_VAR:
                    node->type = AST_MODULE_NODE_EXTERNAL_VAR;
                    return parse_ext_var_decl(parser, ext_name, &node->ext_var_decl);
                default:
                    PARSE_ERROR(EXPECTED("ext declaration"));
                    return NULL;
            }
		default:
			PARSE_ERROR("unexpected {tok} while parsing module node", parser_token(parser));
			return false;
	}
}

bool parse_module_node(Parser *parser, AstModuleNode *node) {
    node->public = parser_next_is(parser, TOKEN_PUBLIC);
    return parse_module_node_decl(parser, node);
}
