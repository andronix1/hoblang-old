#include "llvm.h"

bool llvm_init(LlvmBackend *llvm) {
	LLVMInitializeNativeTarget();
	LLVMInitializeNativeAsmPrinter();

	LLVMTargetRef target = LLVMGetFirstTarget();
	// const char *target_name = "aarch64";
	//LLVMTargetRef target = LLVMGetTargetFromName(target_name);
	if (!target) {
		hob_log(LOGE, "failed to initialize target");
		return false;
	}
	const char *features = LLVMGetHostCPUFeatures();
	const char *cpu = LLVMGetHostCPUName();
	const char *triple = LLVMGetDefaultTargetTriple();
	hob_log(LOGD, "target: %s - %s", LLVMGetTargetName(target), LLVMGetTargetDescription(target));
	hob_log(LOGD, "triple: %s", triple);
	hob_log(LOGD, "with features %s", features);
	hob_log(LOGD, "cpu: %s", cpu);
	llvm->machine = LLVMCreateTargetMachine(target, triple, cpu, features, LLVMCodeGenLevelNone, LLVMRelocDefault, LLVMCodeModelDefault);
	if (!llvm->machine) {
		hob_log(LOGE, "failed to create target machine");
		return false;
	}
	llvm->builder = LLVMCreateBuilder();
	llvm->scopes = vec_new(LlvmScope*);
	return true;	
}

LLVMTypeRef llvm_resolve_type(Type *type) {
	switch (type->type) {
		case TYPE_PRIMITIVE:
			switch (type->primitive) {
				case PRIMITIVE_I8: case PRIMITIVE_U8: return LLVMInt8Type();
				case PRIMITIVE_I16: case PRIMITIVE_U16: return LLVMInt16Type();
				case PRIMITIVE_I32: case PRIMITIVE_U32: return LLVMInt32Type();
				case PRIMITIVE_I64: case PRIMITIVE_U64: return LLVMInt64Type();
				case PRIMITIVE_BOOL: return LLVMInt1Type();
				case PRIMITIVE_VOID: return LLVMVoidType();
			}
			assert(0 && "invalid primitive");
			return NULL;
		case TYPE_FUNCTION: {
			LLVMTypeRef *params = malloc(sizeof(LLVMTypeRef) * type->func.args.len);
			for (size_t i = 0; i < type->func.args.len; i++) {
				AstFuncArg *arg = vec_at(&type->func.args, i);
				params[i] = llvm_resolve_type(&arg->type.sema);
			}
			return LLVMFunctionType(llvm_resolve_type(type->func.returning), params, type->func.args.len, false /* IsVarArg */);
		}
	}
	hob_log(LOGE, "invalid type' type - %d", type->type);
	assert(0);
	return NULL;
}

LlvmValue *llvm_resolve(LlvmBackend *llvm, FatPtr *name) {
	foreach (&llvm->scopes, LlvmScope*, scope) {
		foreach(*scope, LlvmValue, value) {
			if (fatptr_eq(name, value->name)) {
				return value;
			}
		}
	}
	hob_log(LOGE, "value `%P` wasn't registered", name);
	assert(0);
	return NULL;
}

LLVMTypeRef llvm_resolve_type_of(LlvmBackend *llvm, FatPtr *name) {
	return llvm_resolve(llvm, name)->type;
}

LLVMValueRef llvm_resolve_value(LlvmBackend *llvm, FatPtr *name) {
	return llvm_resolve(llvm, name)->value;
}

LLVMValueRef llvm_expr(LlvmBackend *llvm, Expr *expr);

LLVMValueRef llvm_funcall(LlvmBackend *llvm, Funcall *funcall) {
	LLVMValueRef *params = malloc(sizeof(LLVMValueRef) * funcall->args.len);
	for (size_t i = 0; i < funcall->args.len; i++) {
		params[i] = llvm_expr(llvm, vec_at(&funcall->args, i));
	}
	return LLVMBuildCall2(llvm->builder, llvm_resolve_type_of(llvm, &funcall->name), llvm_resolve_value(llvm, &funcall->name), params, funcall->args.len, "");
}

LLVMValueRef llvm_expr(LlvmBackend *llvm, Expr *expr) {
	switch (expr->type) {
		case EXPR_IDENT: return llvm_resolve_value(llvm, &expr->ident);
		case EXPR_INTEGER: return LLVMConstInt(LLVMInt32Type(), expr->integer, true);
		case EXPR_BOOL: return LLVMConstInt(LLVMInt1Type(), expr->boolean, false);
		case EXPR_BINOP: {
			LLVMValueRef right = llvm_expr(llvm, expr->binop.right);
			LLVMValueRef left = llvm_expr(llvm, expr->binop.left);
			switch (expr->binop.type) {
				case BINOP_ADD: return LLVMBuildAdd(llvm->builder, left, right, "");
				case BINOP_SUB: return LLVMBuildSub(llvm->builder, left, right, "");
				case BINOP_MUL: return LLVMBuildMul(llvm->builder, left, right, "");
				case BINOP_DIV: return LLVMBuildSDiv(llvm->builder, left, right, "");
				case BINOP_EQ: return LLVMBuildICmp(llvm->builder, LLVMIntEQ, left, right, "");
				case BINOP_NEQ: return LLVMBuildICmp(llvm->builder, LLVMIntNE, left, right, "");
				case BINOP_GT: return LLVMBuildICmp(llvm->builder, LLVMIntSGT, left, right, "");
				case BINOP_GE: return LLVMBuildICmp(llvm->builder, LLVMIntSGE, left, right, "");
				case BINOP_LT: return LLVMBuildICmp(llvm->builder, LLVMIntSLT, left, right, "");
				case BINOP_LE: return LLVMBuildICmp(llvm->builder, LLVMIntSLE, left, right, "");
			}
			assert(0 && "invalid binop");
			return NULL;
		}
		case EXPR_FUNCALL: return llvm_funcall(llvm, &expr->funcall);
	}
	assert(0 && "invalid expr");
	return NULL;
}

void llvm_body(LlvmBackend *llvm, AstBody *body) {
	LlvmScope scope = vec_new(LlvmValue);
	LlvmScope *scope_of = &scope;
	vec_push(&llvm->scopes, &scope_of);
	foreach (&body->stmts, AstStmt, stmt) {
		switch (stmt->type) {
			case AST_STMT_VAR: {
				assert(stmt->var.typed);
				LLVMTypeRef type = llvm_resolve_type(&stmt->var.type.sema);
				LLVMValueRef var = LLVMBuildAlloca(llvm->builder, type, fatptr_to_cstr(&stmt->var.name));
				LlvmValue value = {
					.type = type,
					.value = var,
					.name = &stmt->var.name
				};
				vec_push(scope_of, &value);
				if (stmt->var.initializes) {
					LLVMSetInitializer(var, llvm_expr(llvm, &stmt->var.expr));
				}
				break;
			}
			case AST_STMT_RETURN: {
				if (stmt->ret.has_value) {
					LLVMBuildRet(llvm->builder, llvm_expr(llvm, &stmt->ret.expr));
				} else {
					LLVMBuildRetVoid(llvm->builder);
				}
				break;
			}
			case AST_STMT_FUNCALL: {
				llvm_funcall(llvm, &stmt->funcall);
				break;
			}
			case AST_STMT_IF: {
				LLVMBasicBlockRef if_block = LLVMAppendBasicBlock(llvm->func, "");
				LLVMBasicBlockRef else_block = LLVMAppendBasicBlock(llvm->func, "");
				LLVMBasicBlockRef end_block = LLVMAppendBasicBlock(llvm->func, "");
				LLVMValueRef expr = llvm_expr(llvm, &stmt->if_else.main.expr);
				LLVMBuildCondBr(llvm->builder, expr, if_block, else_block);
				LLVMPositionBuilderAtEnd(llvm->builder, if_block);
				llvm_body(llvm, &stmt->if_else.main.body);
				LLVMBuildBr(llvm->builder, end_block);
				assert(stmt->if_else.else_ifs.len == 0 && "if else NYI");
				LLVMPositionBuilderAtEnd(llvm->builder, else_block);
				if (stmt->if_else.has_else) {
					llvm_body(llvm, &stmt->if_else.else_body);
				}
				LLVMBuildBr(llvm->builder, end_block);
				LLVMPositionBuilderAtEnd(llvm->builder, end_block);
				break;
			}
		}
	}
	vec_pop(&llvm->scopes);
}

void llvm_add_module_node(LlvmBackend *llvm, AstModuleNode *node) {
	switch (node->type) {
		case AST_MODULE_NODE_FUNC: {
			LLVMTypeRef *params = malloc(sizeof(LLVMTypeRef) * node->func_decl.args.len);
			for (size_t i = 0; i < node->func_decl.args.len; i++) {
				AstFuncArg *arg = vec_at(&node->func_decl.args, i);
				params[i] = llvm_resolve_type(&arg->type.sema);
			}
			LLVMTypeRef type = LLVMFunctionType(llvm_resolve_type(&node->func_decl.returning.sema), params, node->func_decl.args.len, false);
			LlvmScope **scope = vec_top(&llvm->scopes);
			LLVMValueRef value = LLVMAddFunction(llvm->module, fatptr_to_cstr(&node->func_decl.name), type);
			LlvmValue val = {
				.name = &node->func_decl.name,
				.type = type,
				.value = value
			};
			vec_push(*scope, &val);
			break;
		}
		case AST_MODULE_NODE_VAR: {
			hob_log(LOGE, "var nodes are not implemented yet!");
			break;
		}
	}
}

void llvm_module_node(LlvmBackend *llvm, AstModuleNode *node) {
	switch (node->type) {
		case AST_MODULE_NODE_FUNC: {
			LlvmScope scope = vec_new(LlvmValue);
			LlvmScope *scope_of = &scope;
			vec_push(&llvm->scopes, &scope_of);
			llvm->func = llvm_resolve_value(llvm, &node->func_decl.name);
			LLVMPositionBuilderAtEnd(llvm->builder, LLVMAppendBasicBlock(llvm->func, "entry"));
			for (size_t i = 0; i < node->func_decl.args.len; i++) {
				AstFuncArg *arg = vec_at(&node->func_decl.args, i);
				LlvmValue value = {
					.type = llvm_resolve_type(&arg->type.sema),
					.value = LLVMGetParam(llvm->func, i),
					.name = &arg->name
				};
				vec_push(scope_of, &value);
			}
			llvm_body(llvm, &node->func_decl.body);
			if (types_equals(&node->func_decl.returning.sema, &primitives[PRIMITIVE_VOID])) {
				LLVMBuildRetVoid(llvm->builder);
			}
			vec_pop(&llvm->scopes);
			break;
		}
		case AST_MODULE_NODE_VAR: {
			hob_log(LOGE, "var nodes are not implemented yet!");
			assert(0);
			break;
		}
	}
}

void llvm_module(LlvmBackend *llvm, AstModule *module, char *output_path) {
	LlvmScope scope = vec_new(LlvmValue);
	LlvmScope *scope_of = &scope;
	vec_push(&llvm->scopes, &scope_of);
	llvm->module = LLVMModuleCreateWithName(module->name.ptr);
	foreach (&module->nodes, AstModuleNode, node) {
		llvm_add_module_node(llvm, node);
	}
	foreach (&module->nodes, AstModuleNode, node) {
		llvm_module_node(llvm, node);
	}
	// LLVMDumpModule(llvm->module);
	char *error;
	if (LLVMTargetMachineEmitToFile(llvm->machine, llvm->module, output_path, LLVMObjectFile, &error) == 1) {
		hob_log(LOGE, "failed to emit to file: %s", error);
		return;
	}
}
