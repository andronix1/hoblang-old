#include "body.h"
#include "ast/shared/body.h"
#include "core/vec.h"
#include "sema/module.h"
#include "sema/nodes/node.h"

void sema_analyze_body(SemaModule *sema, AstBody *body) {
    sema_module_push_scope(sema, sema_scope_new(NULL));
    for (size_t i = 0; i < vec_len(body->nodes); i++) {
        sema_module_analyze_node(sema, body->nodes[i]);
    }
    sema_module_pop_scope(sema);
}
