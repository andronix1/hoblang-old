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

void sema_conv_function(
	SemaModule *sema,
	SemaType *source,
	SemaType *dest,
	SemaAsConvType *type
) {
	assert(source->type == SEMA_TYPE_FUNCTION, "passed non-function type {sema::type}", source);
	if (sema_types_equals(dest, sema_type_primitive_i64())) {
		*type = SEMA_AS_CONV_PTR_TO_INT;
		return;
	}
	if (dest->type == SEMA_TYPE_POINTER && sema_types_equals(dest->ptr_to, sema_type_primitive_void())) {
		if (!sema_types_equals(source->slice_of, dest->ptr_to)) {
			sema_err("cannot cast {sema::type} to {sema::type} because of different inner types", source, dest);
			return;
		}
		*type = SEMA_AS_CONV_BITCAST;
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
	switch (source->primitive.type) {
		case SEMA_PRIMITIVE_INT: {
			switch (dest->type) {
				case SEMA_TYPE_PRIMITIVE: {
					if (dest->primitive.type != SEMA_PRIMITIVE_INT) {
						sema_err("{sema::type} cannot be casted to void", source);
						break;
					}
					int src_size = sizes[source->primitive.integer];
					int dst_size = sizes[dest->primitive.integer];
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
					if (source->primitive.integer != SEMA_PRIMITIVE_UINT64 && source->primitive.integer != SEMA_PRIMITIVE_INT64) {
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
		case SEMA_PRIMITIVE_BOOL:
		case SEMA_PRIMITIVE_VOID:
			sema_err("{sema::type} cannot be casted to {sema::type}", source, dest);
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

			case SEMA_TYPE_FUNCTION: sema_conv_function(sema, expr_type, as_type, &as->conv_type); break;
			case SEMA_TYPE_STRUCT:
				sema_err("unknown conversion from {sema::type} to {sema::type}", expr_type, as_type);
				return false;
		}
	}
	return sema_value_const(as_type);
}
