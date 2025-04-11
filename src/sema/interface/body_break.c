#include "body_break.h"
#include "core/vec.h"
#include "sema/body_break.h"
#include "sema/module.h"
#include <sys/types.h>

SemaBodyBreak *sema_module_bb_collect_until(SemaModule *sema, AstBody *until) {
    SemaBodyBreak *result = sema_body_break();
    if (!sema->ss) {
        return result;
    }
    for (ssize_t i = vec_len(sema->ss->scopes) - 1; i >= 0; i--) {
        SemaScope *scope = sema->ss->scopes[i];
        for (ssize_t j = vec_len(scope->defers) - 1; j >= 0; j--) {
            result->defers = vec_push_dir(result->defers, scope->defers[j]);
        }
        if (until && scope->body == until) {
            return result;
        }
    }
    return result;
}
