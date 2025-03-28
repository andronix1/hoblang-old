#pragma once

#include <malloc.h>
#include "ast/interface/decl.h"
#include "sema/interface/behaviour.h"
#include "sema/interface/const.h"
#include "sema/interface/module.h"
#include "sema/interface/type.h"

typedef enum {
    SEMA_VALUE_COMPTIME_TYPE,
    SEMA_VALUE_COMPTIME_GENERIC_PARAM,
    SEMA_VALUE_COMPTIME_GENERIC,
    SEMA_VALUE_COMPTIME_MODULE,
    SEMA_VALUE_COMPTIME_BEHAVIOUR,
} SemaValueCompTimeKind;

typedef struct {
    SemaValueCompTimeKind kind;
    union {
        SemaType *type;
        SemaBehaviour *generic_param;
        SemaBehaviour *behaviour;
        AstNodeDeclGeneric *generic;
        SemaModule *module;
    };
} SemaValueCompTime;

typedef enum {
    SEMA_VALUE_RUNTIME_FINAL,
    SEMA_VALUE_RUNTIME_VAR,
    SEMA_VALUE_RUNTIME_CONST,
    SEMA_VALUE_RUNTIME_EXT_FUNC_HANDLE,
} SemaValueRuntimeKind;

typedef struct {
    SemaValueRuntimeKind kind;
    SemaType *type;
    union {
        SemaConst *constant;
    };
} SemaValueRuntime;

typedef enum {
    SEMA_VALUE_RUNTIME,
    SEMA_VALUE_COMP_TIME,
} SemaValueKind;

typedef struct SemaValue {
    SemaValueKind kind;
    union {
        SemaValueRuntime runtime;
        SemaValueCompTime comp_time;
    };
} SemaValue;

static inline SemaValueCompTime sema_value_comp_time_new_behaviour(SemaBehaviour *behaviour) {
    SemaValueCompTime result = {
        .kind = SEMA_VALUE_COMPTIME_BEHAVIOUR,
        .behaviour = behaviour
    };
    return result;
}

static inline SemaValueCompTime sema_value_comp_time_new_generic_param(SemaBehaviour *behaviour) {
    SemaValueCompTime result = {
        .kind = SEMA_VALUE_COMPTIME_GENERIC_PARAM,
        .generic_param = behaviour
    };
    return result;
}

static inline SemaValueCompTime sema_value_comp_time_new_generic(AstNodeDeclGeneric *generic) {
    SemaValueCompTime result = {
        .kind = SEMA_VALUE_COMPTIME_GENERIC,
        .generic = generic 
    };
    return result;
}

static inline SemaValueCompTime sema_value_comp_time_new_module(SemaModule *module) {
    SemaValueCompTime result = {
        .kind = SEMA_VALUE_COMPTIME_MODULE,
        .module = module 
    };
    return result;
}

static inline SemaValueCompTime sema_value_comp_time_new_type(SemaType *type) {
    SemaValueCompTime result = {
        .kind = SEMA_VALUE_COMPTIME_TYPE,
        .type = type
    };
    return result;
}

static inline SemaValueRuntime sema_value_runtime_new_ext_func_handle(SemaType *type) {
    SemaValueRuntime result = {
        .kind = SEMA_VALUE_RUNTIME_EXT_FUNC_HANDLE,
        .type = type
    };
    return result;
}

static inline SemaValueRuntime sema_value_runtime_new_var(SemaType *type) {
    SemaValueRuntime result = {
        .kind = SEMA_VALUE_RUNTIME_VAR,
        .type = type
    };
    return result;
}

SemaValueRuntime sema_value_runtime_new_const(SemaConst *constant);

static inline SemaValueRuntime sema_value_runtime_new_final(SemaType *type) {
    SemaValueRuntime result = {
        .kind = SEMA_VALUE_RUNTIME_FINAL,
        .type = type
    };
    return result;
}

static inline SemaValue *sema_value_new_comp_time(SemaValueCompTime comp_time) {
    SemaValue *result = malloc(sizeof(SemaValue));
    result->kind = SEMA_VALUE_COMP_TIME;
    result->comp_time = comp_time;
    return result;
}

static inline SemaValue *sema_value_new_runtime(SemaValueRuntime runtime) {
    SemaValue *result = malloc(sizeof(SemaValue));
    result->kind = SEMA_VALUE_RUNTIME;
    result->runtime = runtime;
    return result;
}
