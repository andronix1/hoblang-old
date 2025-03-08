#include "sema/module/scopes/api.h"
#include "sema/module/scopes/scope.h"
#include "sema/module/scopes/impl.h"
#include "core/vec.h"

SemaScopeStack sema_ss_new() {
    SemaScopeStack result = {
        .scopes = vec_new(SemaScope),
        .loops = vec_new(SemaLoop*)
    };
    return result;
}
