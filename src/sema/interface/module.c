#include <malloc.h>
#include "sema/module.h"
#include "core/not_null.h"
#include "core/vec.h"
#include "parser/interface/parser.h"
#include "ast/module.h"
#include "sema/decl.h"
#include "sema/interface/project.h"
#include "sema/nodes/node.h"
#include "sema/project.h"
#include "sema/scope.h"
#include "sema/module.h"
#include "sema/value.h"
#include "core/slice/api.h"
#include "sema/type/type.h"

#define PUSH_PRIMITIVE(_name) sema_module_push_decl(sema, file_loc_new(), sema_decl_new(slice_from_cstr(#_name), sema_value_new_comp_time(sema_value_comp_time_new_type(sema_type_primitive_##_name()))), false)

void sema_module_push_primitives(SemaModule *sema, SemaArchInfo *info) {
    if (info->ints & SEMA_INT_8) {
        PUSH_PRIMITIVE(i8);
        PUSH_PRIMITIVE(u8);
    }
    if (info->ints & SEMA_INT_16) {
        PUSH_PRIMITIVE(i16);
        PUSH_PRIMITIVE(u16);
    }
    if (info->ints & SEMA_INT_32) {
        PUSH_PRIMITIVE(i32);
        PUSH_PRIMITIVE(u32);
    }
    if (info->ints & SEMA_INT_64) {
        PUSH_PRIMITIVE(i64);
        PUSH_PRIMITIVE(u64);
    }
    if (info->floats & SEMA_FLOAT_32) {
        PUSH_PRIMITIVE(f32);
    }
    if (info->floats & SEMA_FLOAT_64) {
        PUSH_PRIMITIVE(f64);
    }
    PUSH_PRIMITIVE(void);
    PUSH_PRIMITIVE(bool);
}

SemaModule *sema_module_from_file(const char *path, SemaProject *project) {
    Parser *parser = NOT_NULL(parser_from_file(path));
    AstModule *module = NOT_NULL(parse_module(parser));
	SemaModule *result = malloc(sizeof(SemaModule));
	result->path = path;
	result->ast = module;
	result->project = project;
	result->ss = NULL;
    result->private = sema_scope_new(NULL);
    result->public = sema_scope_new(NULL);
	result->failed = false;
    sema_module_push_primitives(result, &project->arch_info);
	return result; 
}

bool sema_module_read(SemaModule *sema) {
    for (size_t i = 0; i < vec_len(sema->ast->nodes); i++) {
        AstNode *node = sema->ast->nodes[i];
        sema_module_read_node(sema, node);
    }
    return !sema->failed;
}

bool sema_module_analyze(SemaModule *sema) {
    for (size_t i = 0; i < vec_len(sema->ast->nodes); i++) {
        AstNode *node = sema->ast->nodes[i];
        sema_module_analyze_node(sema, node);
    }
    return !sema->failed;
}
