#include "sema/type/matches.h"
#include "core/vec.h"
#include "sema/type/type.h"

bool sema_type_matches_generic(SemaModule *sema, SemaType *generic, SemaType *other, SemaTypeMatches *output) {
    assert(generic->kind == SEMA_TYPE_GENERIC, "first type is not a generic");
    if (!generic->generic.behaviour || sema_type_behaves_like(sema, other, generic->generic.behaviour)) {
        if (output) {
            output->matches = vec_push_dir(output->matches, sema_type_match_new(generic, other));
        }
        return true;
    }
    return false;
}

bool sema_type_matches(SemaModule *sema, SemaType *type, SemaType *other, SemaTypeMatches *output) {
    if (type->kind == SEMA_TYPE_GENERIC) {
        return sema_type_matches_generic(sema, type, other, output);
    }
    if (other->kind == SEMA_TYPE_GENERIC) {
        return sema_type_matches_generic(sema, other, type, output);
    }
    if (type->kind != other->kind) {
        return false;
    }
    switch (type->kind) {    
        case SEMA_TYPE_GENERIC: break;
        case SEMA_TYPE_PRIMITIVE: return type == other;
        case SEMA_TYPE_OPTIONAL: return sema_type_matches(sema, type->optional_of, other->optional_of, output);
        case SEMA_TYPE_POINTER: return sema_type_matches(sema, type->pointer_to, other->pointer_to, output);
        case SEMA_TYPE_FUNCTION:
            if (!sema_type_matches(sema, type->func.returns, other->func.returns, NULL)) {
                return false;
            }
            if (vec_len(type->func.args) != vec_len(other->func.args)) {
                return false;
            }
            for (size_t i = 0; i < vec_len(type->func.args); i++) {
                if (!sema_type_matches(sema, type->func.args[i], other->func.args[i], output)) {
                    return false;
                }
            }
            return true;
        case SEMA_TYPE_STRUCT: return type->struct_decl == other->struct_decl;
        case SEMA_TYPE_SLICE: return sema_type_matches(sema, type->slice_of, other->slice_of, output);
        case SEMA_TYPE_ARRAY:
            return
                type->array.length == other->array.length &&
                sema_type_matches(sema, type->array.of, other->array.of, output);
    }
    assert(0, "invalid type"); 
}

SemaType *sema_type_generate(SemaType *type, SemaTypeMatches *matches) {
    if (type->kind == SEMA_TYPE_GENERIC) {
        for (size_t i = 0; i < vec_len(matches->matches); i++) {
            if (type == matches->matches[i].generic) {
                return matches->matches[i].match;
            }
        }
        assert(0, "failed to generate type");
    }
    switch (type->kind) {    
        case SEMA_TYPE_GENERIC:
        case SEMA_TYPE_PRIMITIVE:
        case SEMA_TYPE_STRUCT:
            return type;
        case SEMA_TYPE_SLICE: return sema_type_new_slice(sema_type_generate(type->slice_of, matches));
        case SEMA_TYPE_OPTIONAL: return sema_type_new_optional(sema_type_generate(type->optional_of, matches));
        case SEMA_TYPE_POINTER: return sema_type_new_pointer(sema_type_generate(type->pointer_to, matches));
        case SEMA_TYPE_ARRAY: return sema_type_new_array(type->array.length, sema_type_generate(type->array.of, matches));
        case SEMA_TYPE_FUNCTION: {
            SemaType **args = vec_new(SemaType*);
            for (size_t i = 0; i < vec_len(type->func.args); i++) {
                args = vec_push_dir(args, sema_type_generate(type->func.args[i], matches));
            }
            return sema_type_new_func(args, sema_type_generate(type->func.returns, matches));
        }
    }
    assert(0, "invalid type"); 
}
