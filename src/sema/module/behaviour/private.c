#include "sema/module/behaviour/impl.h"
#include "sema/module/behaviour/private.h"
#include <stdlib.h>

SemaBehaviour *sema_behaviour_new_list(SemaBehaviour **list) {
    SemaBehaviour *behaviour = malloc(sizeof(SemaBehaviour));
    behaviour->type = SEMA_BEHAVIOUR_LIST;
    behaviour->list = list;
    return behaviour;
}
