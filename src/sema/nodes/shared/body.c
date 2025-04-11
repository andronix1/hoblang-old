#include "body.h"
#include "ast/node.h"
#include "ast/shared/body.h"
#include "core/vec.h"
#include "sema/module.h"
#include "sema/nodes/node.h"

bool sema_analyze_body(SemaModule *sema, AstBody *body) {
    sema_module_push_scope(sema, sema_scope_new_in_body(NULL, body));
    body->sema.breaks = false;
    for (size_t i = 0; i < vec_len(body->nodes); i++) {
        AstNode *node = body->nodes[i];
        if (body->sema.breaks) {
            SEMA_ERROR(node->loc, "unreachable code");
            break;
        }
        if (!sema_module_analyze_node(sema, node)) {
            body->sema.breaks = true;
        }
    }
    body->sema.body_break = sema_module_bb_collect_until(sema, body);
    sema_module_pop_scope(sema);
    return !body->sema.breaks;
}
