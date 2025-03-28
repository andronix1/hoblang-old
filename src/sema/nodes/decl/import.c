#include "import.h"
#include "core/not_null.h"
#include "sema/decl.h"
#include "sema/project.h"
#include "sema/module.h"
#include "sema/value.h"
#include "core/slice/api.h"

void sema_module_read_import(SemaModule *sema, AstImport *import, bool public) {
    SemaModule *module = RET_ON_NULL(sema_project_append_module_at(sema->project, slice_to_cstr(&import->file_path)));
    sema_module_push_decl(sema, import->alias_loc, sema_decl_new(
        import->alias,
        sema_value_new_comp_time(sema_value_comp_time_new_module(module))
    ), public);
}
