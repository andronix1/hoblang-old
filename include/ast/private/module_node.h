#pragma once

#include <stdbool.h>
#include "ast/private/decls/behaviour.h"
#include "ast/private/val_decl.h"
#include "sema/module/decls/decls.h"
#include "core/location.h"
#include "path.h"
#include "func_info.h"
#include "body.h"

typedef struct {
    AstType *type;
	Slice alias;
} AstTypeAlias;

typedef enum {
    AST_FROM_USE_ALL,
    AST_FROM_USE_LIST,
} AstFromUseType;

typedef struct {
    FileLocation loc;
    Slice what;
	bool has_alias;
	Slice alias;
} AstFromUseListItem;

typedef struct {
    AstFromUseType type;
	AstPath *module_path;
    union {
        AstFromUseListItem *items;
    };
} AstFromUse;

typedef struct {
	AstPath *path;
	bool has_alias;
	Slice alias;
} AstUse;

typedef struct {
	const char *path;
	Slice as;
} AstImport;

typedef struct {
	AstFuncInfo info;
	AstBody body;
} AstFuncDecl;

typedef struct {
    AstType *type;
	Slice ext_name;
	Slice public_name;

    struct {
        SemaDecl *decl;
    } sema;
} AstExtVarDecl;

typedef struct {
	AstFuncInfo info;
	Slice ext_name;
} AstExtFuncDecl;

typedef struct AstStructMember {
    FileLocation loc;
	Slice name;
    bool public;
	AstType *type;
} AstStructMember;

typedef struct AstStructDef {
	Slice name;
	AstStructMember *members;

    SemaModule *module;
} AstStructDef;

typedef enum {
	AST_MODULE_NODE_VAL_DECL,
    AST_MODULE_NODE_BEHAVIOUR_DECL,
	AST_MODULE_NODE_FUNC,

	AST_MODULE_NODE_EXTERNAL_FUNC,
	AST_MODULE_NODE_EXTERNAL_VAR,

	AST_MODULE_NODE_FROM_USE,
	AST_MODULE_NODE_IMPORT,
	AST_MODULE_NODE_USE,

	AST_MODULE_NODE_TYPE_ALIAS,
	AST_MODULE_NODE_STRUCT_DEF,
} AstModuleNodeType;

typedef struct AstModuleNode {
	AstModuleNodeType type;
    FileLocation loc;
    bool public;
	union {
        AstFromUse from_use;
		AstFuncDecl func_decl;		
		AstExtFuncDecl ext_func_decl;		
		AstExtVarDecl ext_var_decl;		
		AstImport import;	
		AstTypeAlias type_alias;
		AstStructDef struct_def;
        AstValDecl val_decl;
		AstUse use;	
        AstDeclBehaviour behaviour_decl;
	};
} AstModuleNode;

void print_ast_module_node(FILE *stream, va_list *list);
