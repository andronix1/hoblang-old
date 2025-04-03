#pragma once

#include "sema/interface/project.h"

typedef struct LlvmBackend LlvmBackend;

LlvmBackend *llvm_new_host();
void llvm_emit_project(LlvmBackend *llvm, SemaProject *project);
bool llvm_verify(LlvmBackend *llvm);
bool llvm_write_obj(LlvmBackend *llvm, char *path);
bool llvm_write_ir(LlvmBackend *llvm, char *path);
