#include "parser/private.h"
#include "parser/token_stops.h"
#include "parser/parts/body.h"
#include "ast/private/stmts/defer.h"
#include "ast/private/body.h"

bool parse_defer(Parser *parser, AstDefer *defer) {
	return parse_body_ola(parser, defer->body = malloc(sizeof(AstBody)));
}
