#pragma once

#include <string.h>
#include "core/log.h"
#include "core/slice.h"
#include "impl.h"

#define POP_ARG() ({ \
        len--; \
        const char *arg = args[0]; \
        args = &args[1]; \
        arg; \
    })

#define POP_REQ_ARG(name, usage) ({ \
        if (len == 0) { \
            hob_log(LOGE, "missing " name); \
            usage(); \
            return false; \
        } \
        POP_ARG(); \
    })

#define CMD_PARSE size_t __pos_arg = 0; while (len > 0)

#define CMD_POS_ARG(idx, output) ({ \
            if (idx == __pos_arg) { \
                output = arg; \
                __pos_arg++; \
                continue; \
            } \
        })

#define CMD_POS_ARGS_END \
    hob_log(LOGE, "extra arg {cstr}", arg); \
    return false; \

#define CMD_FLAG (&arg[2])

#define CMD_FLAG_SET(name, output, value) { \
            if (!strcmp(name, CMD_FLAG)) { \
                output = value; \
                continue; \
            } \
        }

#define CMD_FLAG_ON(name, output) CMD_FLAG_SET(name, output, true)

#define CMD_FLAG_STR(name, output) { \
            if (!strcmp(name, CMD_FLAG)) { \
                const char *arg = POP_ARG(); \
                output = arg;\
                continue; \
            } \
        }

#define CMD_FLAG_APPEND_ALL(name, output) { \
            if (!strcmp(name, CMD_FLAG)) { \
                while (len > 0) { \
                    const char *arg = POP_ARG(); \
                    output = vec_push(output, &arg);\
                } \
                continue; \
            } \
        }

#define CMD_FLAG_APPEND(name, output) { \
            if (!strcmp(name, CMD_FLAG)) { \
                const char *arg = POP_ARG(); \
                output = vec_push(output, &arg);\
                continue; \
            } \
        }

#define CMD_IS_FLAG (strlen(arg) > 2 && arg[0] == '-' && arg[1] == '-')

#define CMD_FLAGS_END { \
        hob_log(LOGE, "unknown flag `{cstr}`", CMD_FLAG); \
        return false; \
    }

#define CMD_END(pos_args) \
    if (__pos_arg != pos_args) { \
        hob_log(LOGE, "missing one or multiple positional args"); \
        return false; \
    }

#define CMD_DEFAULT(output, _default) if (!(output)) { (output) = _default; }
