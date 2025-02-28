#include <stdio.h>
#include "cmd/impl.h"
#include "core/log.h"
#include "core/process.h"
#include "core/vec.h"
#include "print.h"
#include "sema/arch/bits/bits.h"
#include "sema/arch/bits/api.h"
#include "sema/project/api.h"
#include "llvm/api.h"
#include "cmd/api.h"

LlvmBackend *llvm_create_from_file(const char *input, CmdArchFlags *arch) {
    SemaIntBits ints = SEMA_INT_8;
    SemaFloatBits floats = 0;
    switch (arch->bits) {
        case ARCH_MODE_16:
            ints = SEMA_INT_8 | SEMA_INT_16;
            break;
        case ARCH_MODE_32:
            ints = SEMA_INT_8 | SEMA_INT_16 | SEMA_INT_32;
            floats = SEMA_FLOAT_32;
            break;
        case ARCH_MODE_64:
            ints = SEMA_INT_8 | SEMA_INT_16 | SEMA_INT_32 | SEMA_INT_64;
            floats = SEMA_FLOAT_32 | SEMA_FLOAT_64;
            break;
    }
    SemaProject *project = sema_project_new(sema_arch_info_new(ints, floats));
    if (!sema_project_add_module_at(project, input)) {
        return NULL;
    }
    if (!sema_project_analyze(project)) {
        return NULL;
    }
    LlvmBackend *llvm = llvm_create();
    if (!llvm) {
        return NULL;
    }
    if (!llvm_emit_project(llvm, project)) {
        return NULL;
    }
    char *temp = "/tmp/hobtmp.o";
    if (!llvm_write_module(llvm, temp)) {
        return NULL;
    }
    return llvm;
}

int main(int argc, char **argv) {
	print_setup();

    Cmd cmd;
    if (!cmd_parse(&cmd, (const char**)&argv[1], (size_t)argc - 1)) {
        return 1;
    }

    switch (cmd.function) {
        case CMD_FUNCTION_BUILD_EXE: {
            LlvmBackend *llvm = llvm_create_from_file(cmd.build_exe.input, &cmd.build_exe.arch);
            if (!llvm) {
                return 1;
            }
            if (!llvm_write_module(llvm, (char*)cmd.build_exe.temp_obj)) {
                return 1;
            }
            const char **args = vec_new(const char*);
            const char *default_args[] = {
                cmd.build_exe.temp_obj,
                "-o",
                cmd.build_exe.output,
            };
            for (size_t i = 0; i < sizeof(default_args) / sizeof(default_args[0]); i++) {
                args = vec_push(args, &default_args[i]);
            }
            for (size_t i = 0; i < vec_len(cmd.build_exe.linker.libs); i++) {
                const char *flag = "-l";
                args = vec_push(args, &flag);
                args = vec_push(args, &cmd.build_exe.linker.libs[i]);
            }
            int status = 1;
            if (!process_run(cmd.build_exe.linker.path, (char**)args, &status)) {
                hob_log(LOGE, "failed to run linker {errno}");
                return 1;
            }
            if (status != 0) {
                hob_log(LOGE, "linker failed with exit code {int}", status);
                return 1;
            }

            if (cmd.build_exe.run) {
                hob_log(LOGI, "running...");
                if (!process_run(cmd.build_obj.output, (char**)cmd.build_exe.run_args, &status)) {
                    hob_log(LOGE, "failed to run program {errno}");
                    return 1;
                }
                hob_log(LOGI, "program finished with exit code {int}", status);
            } else {
                hob_log(LOGI, "finished");
            }
            
            return 0;
        }
        case CMD_FUNCTION_BUILD_OBJ: {
            LlvmBackend *llvm = llvm_create_from_file(cmd.build_obj.input, &cmd.build_obj.arch);
            if (!llvm) {
                return 1;
            }
            if (!llvm_write_module(llvm, (char*)cmd.build_obj.output)) {
                return 1;
            }
            return 0;
        }
        case CMD_FUNCTION_EMIT_LLVM: {
            LlvmBackend *llvm = llvm_create_from_file(cmd.emit_llvm.input, &cmd.emit_llvm.arch);
            if (!llvm) {
                return 1;
            }
		    if (!llvm_write_module_ir(llvm, (char*)cmd.emit_llvm.output)) {
                return 1;
            }
            return 0;
        }
        case CMD_FUNCTION_HELP: {
            cmd_help();
            return 0;
        }
    }

    assert(0, "unknown cmd function");
}
