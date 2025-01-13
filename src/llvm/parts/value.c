#include "../parts.h"

LLVMValueRef llvm_value(LlvmBackend *llvm, AstValue *value) {
	assert(value->segments[0].type == AST_VALUE_IDENT, "value must be started from ident segment");
	LLVMValueRef val = llvm_resolve_value(llvm, &value->segments[0].ident);
	static bool shown = false;
	if (!shown) { 
		hob_log(LOGW, "todo: ref, deref");
		shown = true;
	}
	return val;
}
