#include "ast/interface/module.h"
#include "ast/interface/node.h"
#include "ast/module.h"
#include "core/vec.h"
#include "lexer/interface/lexer.h"
#include "parser/api.h"
#include "parser/ast/node.h"
#include <malloc.h>
#include "parser/parser.h"

Parser *parser_new(Lexer *lexer) {
    Parser *parser = malloc(sizeof(Parser));
	parser->lexer = lexer;
	parser->failed = false;
	parser->skip_next = false;
	return parser;
}

Parser *parser_from_file(const char *path) {
    Lexer *lexer = lexer_new_from_file(path);
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

AstModule *parse_module(Parser *parser) {
    AstNode **nodes = vec_new(AstNode*);
    while (!lexer_finished(parser->lexer)) {
        if (parser_next_is(parser, TOKEN_EOI)) {
            break;
        }
        AstNode *node = parse_node(parser);
        if (node) {
            nodes = vec_push_dir(nodes, node);
        }
    }
    return ast_module_new(nodes, parser);
}
