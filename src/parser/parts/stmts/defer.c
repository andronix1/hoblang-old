#include "../../parts.h"
#include "../body.h"

bool parse_defer(Parser *parser, AstDefer *defer) {
	return parse_body_ola(parser, defer->body = malloc(sizeof(AstBody)));
}
