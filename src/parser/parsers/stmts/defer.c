bool parse_defer(Parser *parser, AstDefer *defer) {
	return parse_body(parser, defer->body = malloc(sizeof(AstBody)));
}
