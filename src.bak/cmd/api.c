#include <string.h>
#include "cmd/api.h"
#include "cmd/impl.h"
#include "cmd/private.h"

#define CMD_DEF(name) \
    void cmd_usage_##name(); \
    bool cmd_parse_##name(Cmd *output, const char **args, size_t len);

CMD_DEF(build_exe);
CMD_DEF(build_obj);
CMD_DEF(emit_llvm);
CMD_DEF(list_targets);
CMD_DEF(help);

typedef struct {
    const char *name;
    CmdFunction function;
    bool (*parse)(Cmd *output, const char **args, size_t len);
    void (*usage)();
} CmdParseInfo;

#define CMD(_name, _func, _parse, _usage) { .name = _name, .function = _func, .parse = _parse, .usage = _usage }

const CmdParseInfo cmds[] = {
    CMD("build-exe", CMD_FUNCTION_BUILD_EXE, cmd_parse_build_exe, cmd_usage_build_exe),
    CMD("build-obj", CMD_FUNCTION_BUILD_OBJ, cmd_parse_build_obj, cmd_usage_build_obj),
    CMD("emit-llvm", CMD_FUNCTION_EMIT_LLVM, cmd_parse_emit_llvm, cmd_usage_emit_llvm),
    CMD("list-targets", CMD_FUNCTION_LIST_TARGETS, cmd_parse_list_targets, cmd_usage_emit_llvm),
    CMD("help", CMD_FUNCTION_HELP, cmd_parse_help, NULL),
};

void cmd_help() {
    for (size_t i = 0; i < sizeof(cmds) / sizeof(cmds[0]); i++) {
        const CmdParseInfo *cmd = &cmds[i];
        if (cmd->usage) {
            if (i != 0) {
                printf("\n");
            }
            cmd->usage();
        }
    }
}

void cmd_no_args() {
    printf("missing command name! use `help` for documentation\n");
}

bool cmd_parse(Cmd *output, const char **args, size_t len) {
    memset(output, 0, sizeof(Cmd));
    const char *command = POP_REQ_ARG("command name", cmd_no_args);
    for (size_t i = 0; i < sizeof(cmds) / sizeof(cmds[0]); i++) {
        const CmdParseInfo *cmd = &cmds[i];
        if (!strcmp(command, cmd->name)) {
            output->function = cmd->function;
            if (!cmd->parse(output, args, len)) {
                if (cmd->usage) {
                    cmd->usage();
                }
                return false;
            }
            return true;
        }
    }
    hob_log(LOGE, "unknown command `{cstr}`", command);
    return false;   
}
