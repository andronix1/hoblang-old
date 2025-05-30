#pragma once

#include <stdbool.h>
#include "llvm.h"
#include "ast/api/module.h"
#include "sema/project/project.h"

LlvmBackend *llvm_create();
void llvm_module(LlvmBackend *llvm, AstModule *module);
void llvm_module_init(LlvmBackend *llvm, AstModule *module);
void llvm_print_targets();
bool llvm_write_module(LlvmBackend *llvm, bool verify, const char *target_name, char *output_path);
bool llvm_write_module_ir(LlvmBackend *llvm, char *output_path);
bool llvm_emit_project(LlvmBackend *llvm, SemaProject *project);
