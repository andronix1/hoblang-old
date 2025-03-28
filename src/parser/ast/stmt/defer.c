#include "defer.h"
#include "ast/node/stmt/defer.h"
#include "core/not_null.h"
#include "parser/ast/shared/body.h"

AstDefer *parse_defer(Parser *parser) {
    return ast_defer_new(NOT_NULL(parse_body(parser)));
}
