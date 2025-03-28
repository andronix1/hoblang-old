#include "ast/interface/behaviour.h"
#include "ast/interface/path.h"
#include "ast/shared/behaviour.h"
#include "core/not_null.h"
#include "core/vec.h"
#include "parser/api.h"
#include "parser/ast/shared/path.h"
#include "parser/interface/parser.h"

AstBehaviour *parse_behaviour(Parser *parser) {
    AstPath **paths = vec_new(AstPath*);
    do {
        paths = vec_push_dir(paths, NOT_NULL(parse_path(parser)));
    } while(parser_next_is(parser, TOKEN_ADD));
    return ast_behaviour_new(paths);
}
