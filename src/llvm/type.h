#pragma once

#include <llvm-c/Types.h>
#include "sema/interface/type.h"

LLVMTypeRef llvm_type(SemaType* type);
