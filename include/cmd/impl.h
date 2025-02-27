#pragma once

#include <stdbool.h>

typedef enum {
    ARCH_MODE_8,
    ARCH_MODE_16,
    ARCH_MODE_32,
    ARCH_MODE_64 = 0
} CmdArchBits;

typedef struct {
    CmdArchBits bits;
} CmdArchFlags;

typedef struct {
    const char *path;
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
            CmdArchFlags arch;
        } build_exe;

        struct {
            const char *input;
            const char *output;
            CmdArchFlags arch;
        } build_obj;

        struct {
            const char *input;
            const char *output; 
            CmdArchFlags arch;
        } emit_llvm;
    };
} Cmd;

