#include "core/log.h"
#include "print.h"
#include "sema/arch/info.h"
#include "sema/interface/project.h"

int main() {
    print_setup();

    SemaArchInfo arch_info = sema_arch_info_new(
        SEMA_INT_8 | SEMA_INT_16 | SEMA_INT_32 | SEMA_INT_64,
        SEMA_FLOAT_32 | SEMA_FLOAT_64
    );
    SemaProject *project = sema_project_from_file("../examples/test.hob", arch_info);
    if (!project) return 1;
    if (!sema_project_analyze(project)) return 1;
    hob_log(LOGI, "finished!");
    return 0;
}
