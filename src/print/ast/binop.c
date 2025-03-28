#include "ast/interface/path.h"
#include "ast/shared/path.h"
#include "core/vec.h"
#include <stdarg.h>
#include <stdio.h>

void print_ast_path(FILE *stream, va_list list) {
	AstPath *path = va_arg(list, AstPath*);
    for (size_t i = 0; i < vec_len(path->segments); i++) {
        if (i != 0) fprintf(stream, ".");
        AstPathSegment *segment = &path->segments[i];
        switch (segment->kind) {
            case AST_PATH_SEGMENT_IDENT: print_to(stream, "{slice}", &segment->ident); break;
            case AST_PATH_SEGMENT_DEREF: print_to(stream, "*"); break;
            case AST_PATH_SEGMENT_SIZEOF: print_to(stream, "sizeof"); break;
            case AST_PATH_SEGMENT_NULL: print_to(stream, "null"); break;
            case AST_PATH_SEGMENT_GENERIC:
                print_to(stream, "<");
                for (size_t j = 0; j < vec_len(segment->generic.params); j++) {
                    if (j != 0) print_to(stream, ", ");
                    print_to(stream, "{ast::type}", segment->generic.params[j]);
                }
                print_to(stream, ">");
                break;
        }
    }
}
