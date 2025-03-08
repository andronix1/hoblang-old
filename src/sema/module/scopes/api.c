#include "sema/module/scopes/api.h"
#include "sema/module/scopes/scope.h"
#include "sema/module/scopes/impl.h"
#include "core/vec.h"

SemaScopeStack sema_ss_new(SemaType *returns) {
    SemaScopeStack result = {
        .scopes = vec_new(SemaScope),
        .loops = vec_new(SemaLoop*),
        .returning = returns
    };
    return result;
}
