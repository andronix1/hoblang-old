#include <llvm-c/Core.h>
#include "llvm/private.h"
#include "core/vec.h"
#include "ast/private/path.h"
#include "ast/private/module_node.h"
#include "sema/module/behaviour/impl.h"
#include "sema/module/behaviour/table/path.h"
#include "sema/value/api.h"
#include "sema/value/private.h"
#include "sema/module/decls/impl.h"
#include "llvm/parts/type.h"
#include "llvm/parts/types/slice.h"
#include "llvm/parts/types/optional.h"
#include "llvm/utils/member.h"
#include "core/assert.h"

LLVMValueRef llvm_resolve_path(LlvmBackend *llvm, LLVMValueRef value, AstPath *path, SemaValue *from) {
    for (size_t i = 0; i < vec_len(path->segments); i++) {
        SemaPath *segment = &path->segments[i].sema;
        switch (segment->type) {
            case SEMA_PATH_DECL: {
                value = segment->decl->llvm.value;
                break;
            }
            case SEMA_PATH_SIZEOF:
                value = llvm_type_sizeof(llvm, llvm_resolve_type(segment->sizeof_op.type), llvm_resolve_type(segment->sizeof_op.output_type));
                break;
            case SEMA_PATH_DEREF:
                if (segment->value->type == SEMA_VALUE_VAR) {
                    value = LLVMBuildLoad2(
                        llvm_builder(llvm),
                        LLVMPointerType(llvm_resolve_type(segment->deref_type), 0),
                        value,
                        "deref"
                    );
                }
                break;
            case SEMA_PATH_EXT_FUNC_DIRECT: {
                if (segment->value->type == SEMA_VALUE_VAR) {
                    value = from->ext_func_handle = LLVMBuildLoad2(
                        llvm_builder(llvm),
                        llvm_resolve_type(segment->ext_func_decl->in_type),
                        value,
                        ""
                    );
                } else {
                    from->ext_func_handle = value;
                }
                value = segment->ext_func_decl->llvm.value;
                break;
            }
            case SEMA_PATH_EXT_FUNC_REF: {
                assert(segment->value->type == SEMA_VALUE_VAR, "trying to call ext func ref from non-var value");
                from->ext_func_handle = value;
                value = segment->ext_func_decl->llvm.value;
                break;
            }
            case SEMA_PATH_STRUCT_MEMBER: {
                                              /*
                switch (segment->struct_member.member->type) {
                    case SEMA_STRUCT_MEMBER_EXT_FUNC:
                        break;
                    case SEMA_STRUCT_MEMBER_FIELD: {
                    */
                size_t mid = segment->struct_member.idx;
                value = llvm_get_member(
                    llvm,
                    llvm_resolve_type(segment->struct_member.of),
                    llvm_resolve_type(segment->struct_member.of->struct_def->members[mid].type->sema),
                    value,
                    mid,
                    false
                );
                break;
            }
            case SEMA_PATH_IS_NULL:
                value = llvm_opt_is_null(llvm, llvm_resolve_type(segment->optional_type), value, false);
                break;
            case SEMA_PATH_ARRAY_LEN:
                value = LLVMConstInt(llvm_resolve_type(sema_value_typeof(segment->value)), segment->array_length, false);
                break;
            case SEMA_PATH_SLICE_RAW:
                value = llvm_slice_ptr(llvm, llvm_resolve_type(segment->slice_type), value, false);
                break;
            case SEMA_PATH_SLICE_LEN:
                value = llvm_slice_len(llvm, llvm_resolve_type(segment->slice_type), value, false);
                break;
            case SEMA_PATH_BUILD_GENERIC: {
                value = segment->generic.table->llvm.value;
                break;
            }
            case SEMA_PATH_BTABLE_PATH: {
                SemaType *type = segment->btable_path.generic;
                if (LLVMGetTypeKind(LLVMTypeOf(value)) == LLVMPointerTypeKind) {
                    value = LLVMBuildLoad2(
                        llvm_builder(llvm),
                        llvm_resolve_type(type),
                        value,
                        ""
                    );
                }
                SemaBehaviourTable *table = type->generic.table;
                SemaBehaviourTablePathElement *path = segment->btable_path.path;
                for (size_t i = 0; i < vec_len(path); i++) {
                    assert((int)path[i].kind == table->kind, "btable and behaviour doesn't matches at {size} iteration!", i);
                    switch (path[i].kind) {
                        case SEMA_BEHAVIOUR_DECL:
                            from->ext_func_handle = value;
                            value = table->decl[path[i].idx].decl->llvm.value;
                            break;
                        case SEMA_BEHAVIOUR_LIST:
                            table = table->list[path[i].idx];
                            break;
                    }
                }
                break;
            }
        }
    }
    return value;
}
