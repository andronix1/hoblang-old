#pragma once

#include <stdbool.h>
#include "stmts/const.h"
#include "core/location.h"
#include "path.h"
#include "func_info.h"
#include "body.h"

typedef struct {
	AstType type;
	Slice alias;
} AstTypeAlias;

typedef struct {
	AstPath path;
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
	AST_MODULE_NODE_CONST,
	AST_MODULE_NODE_FUNC,
	AST_MODULE_NODE_EXTERNAL_FUNC,
	AST_MODULE_NODE_USE,
	AST_MODULE_NODE_IMPORT,
	AST_MODULE_NODE_TYPE_ALIAS,
	AST_MODULE_NODE_STRUCT_DEF,
} AstModuleNodeType;

typedef struct AstModuleNode {
	AstModuleNodeType type;
    FileLocation loc;
    bool public;
	union {
		AstFuncDecl func_decl;		
		AstExtFuncDecl ext_func_decl;		
		AstImport import;	
		AstTypeAlias type_alias;
		AstStructDef struct_def;
		AstConst constant;	
		AstUse use;	
	};
} AstModuleNode;

void print_ast_module_node(FILE *stream, va_list *list);
