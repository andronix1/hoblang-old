#pragma once

#include "mod_path.h"
#include "func_info.h"
#include "body.h"

struct _AstModule;

typedef struct {
	AstType type;
	Slice alias;
} AstTypeAlias;

typedef struct {
	AstModPath path;
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

typedef enum {
	AST_MODULE_NODE_FUNC,
	AST_MODULE_NODE_EXTERNAL_FUNC,
	AST_MODULE_NODE_USE,
	AST_MODULE_NODE_IMPORT,
	AST_MODULE_NODE_TYPE_ALIAS,
} AstModuleNodeType;

typedef struct {
	AstModuleNodeType type;
	union {
		AstFuncDecl func_decl;		
		AstExtFuncDecl ext_func_decl;		
		AstImport import;	
		AstTypeAlias type_alias;	
		AstUse use;	
	};
} AstModuleNode;

void print_ast_module_node(FILE *stream, va_list *list);
