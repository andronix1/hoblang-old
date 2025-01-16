#pragma once

#include "mod_path.h"
#include "func_info.h"
#include "body.h"

struct _AstModule;

typedef struct {
	AstModPath path;
	struct _AstModule *module;
} AstUse;

typedef struct {
	Slice path;
	struct _AstModule *module;
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
} AstModuleNodeType;

typedef struct {
	AstModuleNodeType type;
	union {
		AstFuncDecl func_decl;		
		AstExtFuncDecl ext_func_decl;		
		AstImport import;	
		AstUse use;	
	};
} AstModuleNode;

void print_ast_module_node(FILE *stream, va_list *list);
