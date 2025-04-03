#pragma once

#include "ast/interface/global_decl.h"
#include "ast/node/decl/behaviour.h"
#include "ast/node/decl/global.h"
#include "ast/node/decl/import.h"
#include "ast/node/decl/val.h"
#include "ast/node/decl/func.h"
#include "ast/node/decl/use.h"
#include "ast/node/decl/from_use.h"
#include "ast/node/decl/type_alias.h"
#include "ast/node/decl/struct.h"
#include "ast/interface/global.h"
#include "ast/interface/generic.h"
#include "ast/shared/global.h"
#include <stdlib.h>

typedef enum {
    AST_NODE_DECL_GENERIC_FUNC,
    AST_NODE_DECL_GENERIC_STRUCT,
    AST_NODE_DECL_GENERIC_TYPE_ALIAS,
    AST_NODE_DECL_GENERIC_BEHAVIOUR,
} AstNodeDeclGenericKind;

typedef struct AstNodeDeclGeneric {
    AstNodeDeclGenericKind kind;
    AstGenerics *generics;
    union {
        AstFuncDecl *func_decl;
        AstStructDecl struct_decl;
        AstTypeAlias type_alias;
        AstBehaviourDecl behaviour;
    };
} AstNodeDeclGeneric;

typedef enum {
    AST_NODE_DECL_GENERIC,
    AST_NODE_DECL_VAL,
    AST_NODE_DECL_EXTERNAL,
    AST_NODE_DECL_GLOBAL,
    AST_NODE_DECL_USE,
    AST_NODE_DECL_FROM_USE,
    AST_NODE_DECL_IMPORT,
} AstNodeDeclKind;

typedef struct AstNodeDecl {
    AstNodeDeclKind kind;
    FileLocation decl_loc;
    bool public;
    union {
        AstValDecl *val;
        AstGlobalDeclInfo *external;
        AstGlobalDecl global_decl;
        AstUse use;
        AstFromUse from_use;
        AstNodeDeclGeneric generic;
        AstImport import;
    };
} AstNodeDecl;

static inline AstNodeDeclGeneric ast_node_decl_generic_new_behaviour(AstGenerics *generics, AstBehaviourDecl behaviour) {
    AstNodeDeclGeneric result = {
        .kind = AST_NODE_DECL_GENERIC_BEHAVIOUR,
        .generics = generics,
        .behaviour = behaviour 
    };
    return result;
}

static inline AstNodeDeclGeneric ast_node_decl_generic_new_struct(AstGenerics *generics, AstStructDecl struct_decl) {
    AstNodeDeclGeneric result = {
        .kind = AST_NODE_DECL_GENERIC_STRUCT,
        .generics = generics,
        .struct_decl = struct_decl
    };
    return result;
} 

static inline AstNodeDeclGeneric ast_node_decl_generic_new_func(AstGenerics *generics, AstFuncDecl *func_decl) {
    AstNodeDeclGeneric result = {
        .kind = AST_NODE_DECL_GENERIC_FUNC,
        .generics = generics,
        .func_decl = func_decl
    };
    return result;
}

static inline AstNodeDeclGeneric ast_node_decl_generic_new_type_alias(AstGenerics *generics, AstTypeAlias alias) {
    AstNodeDeclGeneric result = {
        .kind = AST_NODE_DECL_GENERIC_TYPE_ALIAS,
        .generics = generics,
        .type_alias = alias
    };
    return result;
}

static inline AstNodeDecl *ast_node_decl_new_import(FileLocation decl_loc, AstImport import) {
    AstNodeDecl *result = malloc(sizeof(AstNodeDecl));
    result->kind = AST_NODE_DECL_IMPORT;
    result->decl_loc = decl_loc;
    result->import = import;
    return result;
}

static inline AstNodeDecl *ast_node_decl_new_from_use(FileLocation decl_loc, AstFromUse from_use) {
    AstNodeDecl *result = malloc(sizeof(AstNodeDecl));
    result->kind = AST_NODE_DECL_FROM_USE;
    result->decl_loc = decl_loc;
    result->from_use = from_use;
    return result;
}

static inline AstNodeDecl *ast_node_decl_new_use(FileLocation decl_loc, AstUse use) {
    AstNodeDecl *result = malloc(sizeof(AstNodeDecl));
    result->kind = AST_NODE_DECL_USE;
    result->decl_loc = decl_loc;
    result->use = use;
    return result;
}

static inline AstNodeDecl *ast_node_decl_new_global(AstGlobalDecl decl) {
    AstNodeDecl *result = malloc(sizeof(AstNodeDecl));
    result->kind = AST_NODE_DECL_GLOBAL;
    result->decl_loc = ast_global_decl_decl_loc(&decl);
    result->global_decl = decl;
    return result;
}

static inline AstNodeDecl *ast_node_decl_new_external(AstGlobalDeclInfo *external) {
    AstNodeDecl *result = malloc(sizeof(AstNodeDecl));
    result->kind = AST_NODE_DECL_EXTERNAL;
    result->decl_loc = ast_global_decl_info_decl_loc(external);
    result->external = external;
    return result;
}

static inline AstNodeDecl *ast_node_decl_new_val(FileLocation decl_loc, AstValDecl *val) {
    AstNodeDecl *result = malloc(sizeof(AstNodeDecl));
    result->kind = AST_NODE_DECL_VAL;
    result->decl_loc = decl_loc;
    result->val = val;
    return result;
}

static inline AstNodeDecl *ast_node_decl_new_generic(FileLocation decl_loc, AstNodeDeclGeneric generic) {
    AstNodeDecl *result = malloc(sizeof(AstNodeDecl));
    result->kind = AST_NODE_DECL_GENERIC;
    result->decl_loc = decl_loc;
    result->generic = generic;
    return result;
}

