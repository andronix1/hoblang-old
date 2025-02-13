#include "exprs.h"
#include "core/assert.h"
#include "sema/module/parts/type.h"
#include "sema/type/private.h"

void sema_conv_pointer(
	SemaModule *sema,
	SemaType *source,
	SemaType *dest,
	SemaAsConvType *type
) {
	assert(source->type == SEMA_TYPE_POINTER, "passed non-pointer type {sema::type}", source);
	if (source->ptr_to->type == SEMA_TYPE_ARRAY && dest->type == SEMA_TYPE_SLICE) {
		*type = SEMA_AS_CONV_ARR_PTR_TO_SLICE;
		return;
	}
	if (dest->type == SEMA_TYPE_POINTER) {
		*type = SEMA_AS_CONV_BITCAST;
		return;
	}
	if (sema_types_equals(dest, sema_type_primitive_i64())) {
		*type = SEMA_AS_CONV_PTR_TO_INT;
		return;
	}
	sema_err("{sema::type} cannot be casted to {sema::type}", source, dest);
}

void sema_conv_slice(
	SemaModule *sema,
	SemaType *source,
	SemaType *dest,
	SemaAsConvType *type
) {
	assert(source->type == SEMA_TYPE_SLICE, "passed non-slice type {sema::type}", source);
	if (dest->type == SEMA_TYPE_POINTER) {
		if (!sema_types_equals(source->slice_of, dest->ptr_to)) {
			sema_err("cannot cast {sema::type} to {sema::type} because of different inner types", source, dest);
			return;
		}
		*type = SEMA_AS_CONV_SLICE_TO_PTR;
		return;
	}
	sema_err("{sema::type} cannot be casted to {sema::type}", source, dest);
}

void sema_conv_array(
	SemaModule *sema,
	SemaType *source,
	SemaType *dest,
	SemaAsConvType *type
) {
	assert(source->type == SEMA_TYPE_ARRAY, "passed non-array type {sema::type}", source);
	/*if (dest->type == SEMA_TYPE_SLICE) {
		if (!sema_types_equals(source->array.of, dest->slice_of)) {
			sema_err("cannot cast {semas::type} to {semas::type} because of different inner types", source, dest);
			return;
		}
		*type = SEMA_AS_CONV_ARR_TO_SLICE;
		return;
	}*/
	sema_err("{sema::type} cannot be casted to {sema::type}", source, dest);
}

void sema_conv_primitive(
	SemaModule *sema,
	SemaType *source,
	SemaType *dest,
	SemaAsConvType *type
) {
	assert(source->type == SEMA_TYPE_PRIMITIVE, "passed non-primitive type {sema::type}", source);
	Primitive primitive = source->primitive;
	const int sizes[] = {
		[PRIMITIVE_I8] = 1,
		[PRIMITIVE_I16] = 2,
		[PRIMITIVE_I32] = 3,
		[PRIMITIVE_I64] = 4,
		[PRIMITIVE_U8] = 1,
		[PRIMITIVE_U16] = 2,
		[PRIMITIVE_U32] = 3,
		[PRIMITIVE_U64] = 4,
		[PRIMITIVE_BOOL] = 1,
	};
	switch (primitive) {
		case PRIMITIVE_I8:
		case PRIMITIVE_U8:
		case PRIMITIVE_I16:
		case PRIMITIVE_U16:
		case PRIMITIVE_I32:
		case PRIMITIVE_U32:
		case PRIMITIVE_I64:
		case PRIMITIVE_U64:
		case PRIMITIVE_BOOL: {
			switch (dest->type) {
				case SEMA_TYPE_PRIMITIVE: {
					if (dest->primitive == PRIMITIVE_VOID) {
						sema_err("{sema::type} cannot be casted to void", source);
					break;
					}
					int src_size = sizes[primitive];
					int dst_size = sizes[dest->primitive];
					if (src_size > dst_size) {
						*type = SEMA_AS_CONV_TRUNC;
					} else if (src_size == dst_size) {
						*type = SEMA_AS_CONV_BITCAST;
					} else {
						*type = SEMA_AS_CONV_EXTEND;
					}
					break;
				}

				case SEMA_TYPE_POINTER: {
					if (primitive != PRIMITIVE_I64 && primitive != PRIMITIVE_U64) {
						sema_err("only 64-bit values(not {sema::type}) can be casted to pointer", source);
						break;
					}
					*type = SEMA_AS_CONV_INT_TO_PTR;
					break;
				}

				case SEMA_TYPE_ARRAY:
				case SEMA_TYPE_FUNCTION:
				case SEMA_TYPE_SLICE:
				case SEMA_TYPE_STRUCT:
					sema_err("{sema::type} cannot be casted to {sema::type}", source, dest);
					break;
			}
			break;
		}
		case PRIMITIVE_VOID:
			sema_err("void cannot be casted to {sema::type}", dest);
			break;
	}
}

SemaValue *sema_analyze_expr_as(SemaModule *sema, AstExprAs *as, SemaExprCtx ctx) { 
	SemaType *as_type = sema_ast_type(sema, &as->type);
	if (!as_type) {
		return false;
	}
	SemaType *expr_type = sema_value_expr_type(sema, as->expr, sema_expr_ctx_expect(ctx, sema_type_primitive_i64()));
	if (!expr_type) {
		return false;
	}
	if (sema_types_equals(as_type, expr_type)) {
		as->conv_type = SEMA_AS_CONV_IGNORE;
	} else {
		switch (expr_type->type) {
			case SEMA_TYPE_PRIMITIVE: sema_conv_primitive(sema, expr_type, as_type, &as->conv_type); break;
			case SEMA_TYPE_ARRAY: sema_conv_array(sema, expr_type, as_type, &as->conv_type); break;
			case SEMA_TYPE_POINTER: sema_conv_pointer(sema, expr_type, as_type, &as->conv_type); break;
			case SEMA_TYPE_SLICE: sema_conv_slice(sema, expr_type, as_type, &as->conv_type); break;

			case SEMA_TYPE_FUNCTION:
			case SEMA_TYPE_STRUCT:
				sema_err("unknown conversion from {sema::type} to {sema::type}", expr_type, as_type);
				return false;
		}
	}
	return sema_value_const(as_type);
}
