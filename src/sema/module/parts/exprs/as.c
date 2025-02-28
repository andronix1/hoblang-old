#include "ast/private/expr/as.h"
#include "core/location.h"
#include "exprs.h"
#include "core/assert.h"
#include "sema/module/parts/type.h"
#include "sema/module/private.h"
#include "sema/type/api.h"
#include "sema/type/private.h"
#include "sema/arch/bits/private.h"
#include "sema/value/private.h"
#include "sema/value/api.h"
#include "sema/const/api.h"

SemaValue *sema_conv_pointer(
	SemaModule *sema,
    FileLocation at,
	SemaValue *source_val,
	SemaType *dest,
	SemaAsConvType *type
) {
    SemaType *source = sema_value_typeof(source_val);
	assert(source->type == SEMA_TYPE_POINTER, "passed non-pointer type {sema::type}", source);
	if (source->ptr_to->type == SEMA_TYPE_ARRAY && dest->type == SEMA_TYPE_SLICE) {
		*type = SEMA_AS_CONV_ARR_PTR_TO_SLICE;
		return sema_value_final(dest);
	}
	if (dest->type == SEMA_TYPE_POINTER) {
		*type = SEMA_AS_CONV_BITCAST;
		return sema_value_final(dest);
	}
	if (sema_type_is_int(dest)) {
		*type = SEMA_AS_CONV_PTR_TO_INT;
		return sema_value_final(dest);
	}
    SEMA_ERROR(at, "pointer {sema::type} can be casted to integer or other pointer only, not {sema::type}", source, dest);
    return NULL;
}

SemaValue *sema_conv_slice(
	SemaModule *sema,
    FileLocation at,
	SemaValue *source_val,
	SemaType *dest,
	SemaAsConvType *type
) {
    SemaType *source = sema_value_typeof(source_val);
	assert(source->type == SEMA_TYPE_SLICE, "passed non-slice type {sema::type}", source);
	if (dest->type == SEMA_TYPE_POINTER) {
		if (!sema_types_equals(source->slice_of, dest->ptr_to)) {
			SEMA_ERROR(at, "cannot cast slice {sema::type} to pointer {sema::type} with different type", source, dest);
			return NULL;
		}
		*type = SEMA_AS_CONV_SLICE_TO_PTR;
		return sema_value_final(dest);
	}
	SEMA_ERROR(at, "slice {sema::type} can be to pointer with similar type only, not {sema::type}", source, dest);
    return NULL;
}

SemaValue *sema_conv_function(
	SemaModule *sema,
    FileLocation at,
	SemaValue *source_val,
	SemaType *dest,
	SemaAsConvType *type
) {
    SemaType *source = sema_value_typeof(source_val);
	assert(source->type == SEMA_TYPE_FUNCTION, "passed non-function type {sema::type}", source);
	if (sema_types_equals(dest, sema_arch_ptr(sema))) {
		*type = SEMA_AS_CONV_PTR_TO_INT;
		return sema_value_final(dest);
	}
	if (dest->type == SEMA_TYPE_POINTER && sema_types_equals(dest->ptr_to, sema_type_primitive_void())) {
		if (!sema_types_equals(source->slice_of, dest->ptr_to)) {
			SEMA_ERROR(at, "cannot cast {sema::type} to {sema::type} because of different inner types", source, dest);
			return NULL;
		}
		*type = SEMA_AS_CONV_BITCAST;
		return sema_value_final(dest);
	}
	SEMA_ERROR(at, "function type {sema::type} can be casted to pointer only, not {sema::type}", source, dest);
    return NULL;
}

SemaValue *sema_conv_optional(
	SemaModule *sema,
    FileLocation at,
	SemaValue *source_val,
	SemaType *dest,
	SemaAsConvType *type
) {
    SemaType *source = sema_value_typeof(source_val);
	assert(source->type == SEMA_TYPE_OPTIONAL, "passed non-optional type {sema::type}", source);

    bool is_const = source_val->type == SEMA_VALUE_CONST;

    if (sema_types_equals(source->optional_of, dest)) {
        *type = SEMA_AS_CONV_OPT_UNWRAP;
        if (is_const) {
            if (source_val->constant.optional.is_null) {
                SEMA_ERROR(at, "trying to unwrap const optional which contains null type");
                return NULL;
            }
            return sema_value_const(*source_val->constant.optional.value);
        }
        return sema_value_final(dest);
    }
	SEMA_ERROR(at, "optional {sema::type} can be only unwrapped to inner type {sema::type}, not {sema::type}", source, source->optional_of, dest);
    return NULL;
}

SemaValue *sema_conv_array(
	SemaModule *sema,
    FileLocation at,
	SemaValue *source_val,
	SemaType *dest,
	SemaAsConvType *type
) {
    SemaType *source = sema_value_typeof(source_val);
	assert(source->type == SEMA_TYPE_ARRAY, "passed non-array type {sema::type}", source);
	SEMA_ERROR(at, "array {sema::type} cannot be casted to {sema::type}", source, dest);
    return NULL;
}

bool sema_int_is_signed(SemaPrimitiveIntType type) {
	return
		type == SEMA_PRIMITIVE_INT8 ||
		type == SEMA_PRIMITIVE_INT16 ||
		type == SEMA_PRIMITIVE_INT32 ||
		type == SEMA_PRIMITIVE_INT64;
}

SemaValue *sema_conv_primitive(
	SemaModule *sema,
    FileLocation at,
	SemaValue *source_val,
	SemaType *dest,
	SemaAsConvType *type
) {
    SemaType *source = sema_value_typeof(source_val);
	assert(source->type == SEMA_TYPE_PRIMITIVE, "passed non-primitive type {sema::type}", source);

    bool is_const = source_val->type == SEMA_VALUE_CONST;

	switch (source->primitive.type) {
		case SEMA_PRIMITIVE_FLOAT: {
			if (sema_type_is_int(dest)) {
				if (sema_int_is_signed(dest->primitive.integer)) {
					*type = SEMA_AS_CONV_FLOAT_TO_INT;
				} else {
					*type = SEMA_AS_CONV_FLOAT_TO_UINT;
				}
                if (is_const) {
                    return sema_value_const(sema_const_int(dest, source_val->constant.fp));
                }
                return sema_value_final(dest);
			}
			if (sema_type_is_float(dest)) {
				const int sizes[] = {
					[SEMA_PRIMITIVE_FLOAT32] = 1,
					[SEMA_PRIMITIVE_FLOAT64] = 2,
				};
				int src_size = sizes[source->primitive.float_type];
				int dst_size = sizes[dest->primitive.float_type];
				if (src_size > dst_size) {
					*type = SEMA_AS_CONV_FTRUNC;
				} else if (src_size == dst_size) {
					*type = SEMA_AS_CONV_BITCAST;
				} else {
					*type = SEMA_AS_CONV_FEXTEND;
				}
                if (is_const) {
                    return sema_value_const(sema_const_float(dest, source_val->constant.fp));
                }
                return sema_value_final(dest);
			}
			SEMA_ERROR(at, "float {sema::type} can be casted to floats and integers only, not {sema::type}", source, dest);
            return NULL;
		}
		case SEMA_PRIMITIVE_INT: {
			switch (dest->type) {
				case SEMA_TYPE_PRIMITIVE: {
					switch (dest->primitive.type) {
						case SEMA_PRIMITIVE_FLOAT:
							if (sema_int_is_signed(dest->primitive.integer)) {
								*type = SEMA_AS_CONV_INT_TO_FLOAT;
							} else {
								*type = SEMA_AS_CONV_UINT_TO_FLOAT;
							}
                            if (is_const) {
                                return sema_value_const(
                                    sema_const_float(dest, source_val->constant.integer)
                                );
                            }
                            return sema_value_final(dest);
						case SEMA_PRIMITIVE_INT: {
							const int sizes[] = {
								[SEMA_PRIMITIVE_INT8] = 1,
								[SEMA_PRIMITIVE_INT16] = 2,
								[SEMA_PRIMITIVE_INT32] = 3,
								[SEMA_PRIMITIVE_INT64] = 4,
								[SEMA_PRIMITIVE_UINT8] = 1,
								[SEMA_PRIMITIVE_UINT16] = 2,
								[SEMA_PRIMITIVE_UINT32] = 3,
								[SEMA_PRIMITIVE_UINT64] = 4,
							};
							int src_size = sizes[source->primitive.integer];
							int dst_size = sizes[dest->primitive.integer];
							if (src_size > dst_size) {
								*type = SEMA_AS_CONV_TRUNC;
							} else if (src_size == dst_size) {
								*type = SEMA_AS_CONV_BITCAST;
							} else {
								*type = SEMA_AS_CONV_EXTEND;
							}
                            if (is_const) {
                                return sema_value_const(
                                    sema_const_int(dest, source_val->constant.integer)
                                );
                            }
                            return sema_value_final(dest);
						}
						default:
							SEMA_ERROR(at, "integer {sema::type} can be casted to floats, integers and pointers only, not {sema::type}", source, dest);
                            return NULL;
					}
					break;
				}

				case SEMA_TYPE_POINTER: {
					if (source->primitive.integer != SEMA_PRIMITIVE_UINT64 && source->primitive.integer != SEMA_PRIMITIVE_INT64) {
						SEMA_ERROR(at, "only i64 integers can be casted to pointer {sema::type}, not {sema::type}", dest, source);
                        return NULL;
					}
					*type = SEMA_AS_CONV_INT_TO_PTR;
                    return sema_value_final(dest);
				}

				case SEMA_TYPE_ARRAY:
				case SEMA_TYPE_FUNCTION:
				case SEMA_TYPE_SLICE:
				case SEMA_TYPE_OPTIONAL:
				case SEMA_TYPE_STRUCT:
					SEMA_ERROR(at, "integer {sema::type} can be casted to integers, pointers and floats, not {sema::type}", source, dest);
                    return NULL;
			}
			break;
		}
		case SEMA_PRIMITIVE_BOOL:
		case SEMA_PRIMITIVE_VOID:
			SEMA_ERROR(at, "{sema::type} cannot be casted to anything, including {sema::type}", source, dest);
			return NULL;
	}
    assert(0, "invalid source or dest type!");
}

SemaValue *sema_analyze_expr_as(SemaModule *sema, AstExprAs *as, SemaExprCtx ctx) { 
	SemaType *as_type = NULL, *req_type = NULL;
    ctx.loc = as->loc;
    switch (as->type) {
        case AST_EXPR_AS_TYPE:
            as_type = sema_ast_type(sema, &as->as_type);
            break;
        case AST_EXPR_AS_AUTO:
            if (!ctx.expectation) {
                SEMA_ERROR(ctx.loc, "as-auto operator can be used only then expression destination expect type is specified");
                return NULL;
            }
            as_type = req_type = ctx.expectation;
            break;
    }
	if (!as_type) {
		return false;
	}
    as->sema_type = as_type;
	SemaType *expr_type = sema_value_expr_type(sema, as->expr, sema_expr_ctx_expect(ctx, as_type));
	if (!expr_type) {
		return false;
	}
	if (sema_types_equals(as_type, expr_type)) {
		as->conv_type = SEMA_AS_CONV_IGNORE;
        return sema_value_final(as_type);
	} else if (as_type->type == SEMA_TYPE_OPTIONAL) {
        if (!sema_types_equals(expr_type, as_type->optional_of)) {
            SEMA_ERROR(ctx.loc, "cannot wrap {sema::type} with optional {sema::type} because of different inner types", expr_type, as_type);
            return NULL;
        }
        as->conv_type = SEMA_AS_CONV_OPT_WRAP;
        return sema_value_final(as_type);
    } else {
		switch (expr_type->type) {
			case SEMA_TYPE_PRIMITIVE: return sema_conv_primitive(sema, ctx.loc, as->expr->value, as_type, &as->conv_type); break;
			case SEMA_TYPE_ARRAY: return sema_conv_array(sema, ctx.loc, as->expr->value, as_type, &as->conv_type); break;
			case SEMA_TYPE_POINTER: return sema_conv_pointer(sema, ctx.loc, as->expr->value, as_type, &as->conv_type); break;
			case SEMA_TYPE_SLICE: return sema_conv_slice(sema, ctx.loc, as->expr->value, as_type, &as->conv_type); break;

			case SEMA_TYPE_FUNCTION: return sema_conv_function(sema, ctx.loc, as->expr->value, as_type, &as->conv_type); break;
			case SEMA_TYPE_OPTIONAL: return sema_conv_optional(sema, ctx.loc, as->expr->value, as_type, &as->conv_type); break;
			case SEMA_TYPE_STRUCT:
				SEMA_ERROR(ctx.loc, "{sema::type} cannot be casted to anything, including {sema::type}", expr_type, as_type);
				return false;
		}
        assert(0, "passed through");
	}
}
