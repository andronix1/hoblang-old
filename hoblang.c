#include "core/log.h"
#include "print.h"
#include "sema/arch/info.h"
#include "sema/interface/project.h"
#include "llvm/interface/llvm.h"

int main() {
    print_setup();

    SemaArchInfo arch_info = sema_arch_info_new(
        SEMA_INT_8 | SEMA_INT_16 | SEMA_INT_32 | SEMA_INT_64,
        SEMA_FLOAT_32 | SEMA_FLOAT_64
    );
    SemaProject *project = sema_project_from_file("../examples/test.hob", arch_info);
    if (!project) return 1;
    if (!sema_project_analyze(project)) return 1;
    LlvmBackend *llvm = llvm_new_host();
    if (!llvm) return 1;
    llvm_emit_project(llvm, project);
    if (!llvm_write_ir(llvm, "./test.ll")) return 1;
    if (!llvm_verify(llvm)) return 1;
    if (!llvm_write_obj(llvm, "./test.o")) return 1;
    hob_log(LOGI, "finished!");
    return 0;
}
