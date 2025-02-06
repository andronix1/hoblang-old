#pragma once

typedef enum {
    SEMA_VALUE_CONST,
    SEMA_VALUE_VAR,
    SEMA_VALUE_TYPE,
    SEMA_VALUE_MODULE
} SemaValueType;

typedef struct {
    SemaValueType type;
    struct _SemaType *sema_type;
} SemaValue;

static inline bool sema_value_const(SemaValue *value, struct _SemaType *type) {
    value->type = SEMA_VALUE_CONST;
    value->sema_type = type;
    return true;
}

static inline bool sema_value_type(SemaValue *value, struct _SemaType *type) {
    value->type = SEMA_VALUE_TYPE;
    value->sema_type = type;
    return true;
}
static inline bool sema_value_var(SemaValue *value, struct _SemaType *type) {
    value->type = SEMA_VALUE_VAR;
    value->sema_type = type;
    return true;
}
