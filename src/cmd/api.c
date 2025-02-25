#include <string.h>
#include "cmd/api.h"
#include "cmd/impl.h"
#include "cmd/private.h"

bool cmd_parse_build_exe(Cmd *output, const char **args, size_t len);
bool cmd_parse_build_obj(Cmd *output, const char **args, size_t len);
bool cmd_parse_emit_llvm(Cmd *output, const char **args, size_t len);

typedef struct {
    const char *name;
    CmdFunction function;
    bool (*parse)(Cmd *output, const char **args, size_t len);
} CmdParseInfo;

#define CMD(_name, _func, _parse) { .name = _name, .function = _func, .parse = _parse }

const CmdParseInfo cmds[] = {
    CMD("build-exe", CMD_FUNCTION_BUILD_EXE, cmd_parse_build_exe),
    CMD("build-obj", CMD_FUNCTION_BUILD_OBJ, cmd_parse_build_obj),
    CMD("emit-llvm", CMD_FUNCTION_EMIT_LLVM, cmd_parse_emit_llvm),
};

void cmd_usage() {
    printf(HOBLANG" build-exe|build-obj|emit-llvm");
}

bool cmd_parse(Cmd *output, const char **args, size_t len) {
    memset(output, 0, sizeof(Cmd));
    const char *command = POP_REQ_ARG("command name", cmd_usage);
    for (size_t i = 0; i < sizeof(cmds) / sizeof(cmds[0]); i++) {
        const CmdParseInfo *cmd = &cmds[i];
        if (!strcmp(command, cmd->name)) {
            output->function = cmd->function;
            return cmd->parse(output, args, len);
        }
    }
    hob_log(LOGE, "unknown command `{cstr}`", command);
    return false;   
}
