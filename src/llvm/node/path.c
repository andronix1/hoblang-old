#include "path.h"
#include "ast/shared/path.h"
#include "core/vec.h"
#include "sema/decl.h"
#include "llvm/slice.h"
#include "llvm/value.h"

LLVMValueRef llvm_path(LlvmBackend *llvm, AstPath *path) {
    LLVMValueRef value = NULL;
    for (size_t i = 0; i < vec_len(path->segments); i++) {
        AstPathSegment *segment = &path->segments[i];
        switch (segment->sema.kind) {
            case SEMA_PATH_SEG_DECL:
                value = segment->sema.decl->llvm.value;
                break;
            case SEMA_PATH_SEG_EXT_FUNC_HANDLE_DIRECT:
                path->llvm.ext_handle = llvm_value_get(llvm, value, segment->sema.from_value);
                value = segment->sema.ext_func_decl->llvm.value;
                break;
            case SEMA_PATH_SEG_SLICE_PTR:
                return llvm_slice_ptr(llvm, segment->sema.from_value, value);
            case SEMA_PATH_SEG_SLICE_LENGTH:
                return llvm_slice_length(llvm, segment->sema.from_value, value);
            case SEMA_PATH_SEG_SIZEOF_TYPE:
            case SEMA_PATH_SEG_STRUCT_FIELD:
                assert(0, "NIY!");
        }
    }
    assert(value, "invalid path");
    return value;
}
