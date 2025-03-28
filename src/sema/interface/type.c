#include "type.h"
#include "core/vec.h"
#include "sema/type/type.h"

bool sema_types_equals(SemaType *type, SemaType *other) {
    if (type->kind != other->kind) {
        return false;
    }
    switch (type->kind) {    
        case SEMA_TYPE_PRIMITIVE:
            if (type->primitive.kind != other->primitive.kind) {
                return false;
            }
            switch (type->primitive.kind) {
                case SEMA_PRIMITIVE_FLOAT:
                    return type->primitive.float_type == other->primitive.float_type;
                case SEMA_PRIMITIVE_INT:
                    return type->primitive.integer == other->primitive.integer;
                case SEMA_PRIMITIVE_VOID:
                case SEMA_PRIMITIVE_BOOL:
                    return true;
            }
            assert(0, "invalid primitive");
        case SEMA_TYPE_GENERIC: return type == other;
        case SEMA_TYPE_OPTIONAL: return sema_types_equals(type->optional_of, other->optional_of);
        case SEMA_TYPE_POINTER: return sema_types_equals(type->pointer_to, other->pointer_to);
        case SEMA_TYPE_FUNCTION:
            if (!sema_types_equals(type->func.returns, other->func.returns)) {
                return false;
            }
            if (vec_len(type->func.args) != vec_len(other->func.args)) {
                return false;
            }
            for (size_t i = 0; i < vec_len(type->func.args); i++) {
                if (!sema_types_equals(type->func.args[i], other->func.args[i])) {
                    return false;
                }
            }
            return true;
        case SEMA_TYPE_STRUCT: return type->struct_decl == other->struct_decl;
        case SEMA_TYPE_SLICE: return sema_types_equals(type->slice_of, other->slice_of);
        case SEMA_TYPE_ARRAY:
            return
                type->array.length == other->array.length &&
                sema_types_equals(type->array.of, other->array.of);
    }
    assert(0, "invalid type");
}

bool sema_type_is_int(SemaType *type) {
    return
        type->kind == SEMA_TYPE_PRIMITIVE &&
        type->primitive.kind == SEMA_PRIMITIVE_INT;
}

bool sema_type_is_float(SemaType *type) {
    return
        type->kind == SEMA_TYPE_PRIMITIVE &&
        type->primitive.kind == SEMA_PRIMITIVE_FLOAT;
}
