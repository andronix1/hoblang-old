#pragma once

#include <stdbool.h>

typedef struct {
    const char **libs;
} CmdLinker;

typedef enum {
    CMD_FUNCTION_BUILD_EXE,
    CMD_FUNCTION_BUILD_OBJ,
    CMD_FUNCTION_EMIT_LLVM,
    CMD_FUNCTION_HELP,
} CmdFunction;

typedef struct Cmd {
    CmdFunction function;

    union {
        struct {
            const char *input;
            const char *output;
            bool run;
            const char *temp_obj;
            const char **run_args;
            CmdLinker linker;
        } build_exe;

        struct {
            const char *input;
            const char *output;
        } build_obj;

        struct {
            const char *input;
            const char *output; 
        } emit_llvm;
    };
} Cmd;

