#include "cmd/private.h"
#include "cmd/usage.h"
#include "cmd/shared/arch.h"
#include "core/vec.h"

void cmd_usage_build_exe() {
    CMD_USAGE_MAIN("build-exe", "<input> <output>")
    CMD_USAGE_FLAG("--temp-obj <path>", "temporary object that will be linked later");
    CMD_USAGE_FLAG("--run", "temporary object that will be linked later");
    CMD_USAGE_FLAG("--link <name>", "link with library");
    CMD_USAGE_FLAG("--linker <path>", "use custom linker");
    CMD_USAGE_FLAG("--args [...args]", "pass arguments to running program");
    cmd_usage_arch_flags();
}

bool cmd_parse_build_exe(Cmd *output, const char **args, size_t len) {
    output->build_exe.linker.libs = vec_new(const char*);
    output->build_exe.run_args = vec_new(const char*);
    CMD_PARSE {
        const char *arg = POP_ARG();
        if (CMD_IS_FLAG) {
            CMD_FLAG_APPEND("link", output->build_exe.linker.libs);
            CMD_FLAG_STR("linker", output->build_exe.linker.path);
            CMD_FLAG_STR("temp-obj", output->build_exe.temp_obj);
            CMD_FLAG_ON("run", output->build_exe.run);
            CMD_FLAG_APPEND_ALL("args", output->build_exe.run_args);
            CMD_ARCH_FLAGS(output->build_exe.arch);
            CMD_FLAGS_END;
        } else {
            CMD_POS_ARG(0, output->build_exe.input);
            CMD_POS_ARG(1, output->build_exe.output);
            CMD_POS_ARGS_END;
        }
    }
    CMD_END(2);
    CMD_DEFAULT(output->build_exe.temp_obj, "/tmp/hobtmp.o");
    CMD_DEFAULT(output->build_exe.linker.path, "/usr/bin/ld");
    return true;
}
