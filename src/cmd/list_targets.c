#include "cmd/private.h"
#include "cmd/usage.h"
#include "cmd/shared/arch.h"
#include "core/vec.h"

void cmd_usage_list_targets() {
    CMD_USAGE_MAIN("list-targets", "")
}

bool cmd_parse_list_targets(Cmd *output, const char **args, size_t len) {
    CMD_PARSE {
        const char *arg = POP_ARG();
    }
    CMD_END(0);
    return true;
}
