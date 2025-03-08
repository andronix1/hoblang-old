#include <stdarg.h>
#include "ast/private/expr.h"
#include "ast/private/expr/as.h"
#include "ast/private/path.h"
#include "core/vec.h"
#include "core/print.h"

void print_ast_binop_type(FILE *stream, va_list list) {
	AstBinopType binop = va_arg(list, AstBinopType);
	const char *strs[] = {
		[AST_BINOP_ADD] = "+",
		[AST_BINOP_SUB] = "-",
		[AST_BINOP_MUL] = "*",
		[AST_BINOP_BITAND] = "&",
		[AST_BINOP_BITOR] = "|",
		[AST_BINOP_SHR] = ">>",
		[AST_BINOP_SHL] = "<<",
		[AST_BINOP_XOR] = "^",
		[AST_BINOP_DIV] = "/",
		[AST_BINOP_EQ] = "==",
		[AST_BINOP_NEQ] = "!=",
		[AST_BINOP_GT] = ">",
		[AST_BINOP_GE] = ">=",
		[AST_BINOP_LT] = "<",
		[AST_BINOP_LE] = "<="
	};
	print_to(stream, strs[binop]);
}

void print_ast_path(FILE *stream, va_list list) {
	AstPath *path = va_arg(list, AstPath*);
	for (size_t i = 0; i < vec_len(path->segments); i++) {
        if (i != 0) {
            print_to(stream, ".");
        }
		AstPathSegment *seg = &path->segments[i];
		switch (seg->type) {
			case AST_PATH_SEG_NULL:
				print_to(stream, "null");
				break;
			case AST_PATH_SEG_IDENT:
				print_to(stream, "{slice}", &seg->ident);
				break;
			case AST_PATH_SEG_SIZEOF:
				print_to(stream, "sizeof");
				break;
			case AST_PATH_SEG_DEREF:
				print_to(stream, "*", &seg->ident);
				break;
		}
	}
}

void print_ast_expr(FILE *stream, va_list list) {
	AstExpr *expr = va_arg(list, AstExpr*);
	switch (expr->type) {
		case AST_EXPR_ANON_FUN: {
            print_to(stream, "fun (");
            for (size_t i = 0; i < vec_len(expr->anon_fun.args); i++) {
                AstFuncArg *arg = &expr->anon_fun.args[i];
                if (i != 0) {        
                    fprintf(stream, ", ");
                }
                print_to(stream, "{slice}: {ast::type}", &arg->name, &arg->type);
            }
            print_to(stream, ") -> {ast::type}", &expr->anon_fun.returning);
            fprintf(stream, "{...}");

			break;
		}
		case AST_EXPR_STRUCT: {
            print_to(stream, "struct {ast::path}", &expr->structure.path);

			break;
		}
		case AST_EXPR_UNWRAP: {
			print_to(stream, "{ast::expr} unwrap {slice}", expr->unwrap.expr, &expr->unwrap.name); break;
			break;
		}
		case AST_EXPR_RET_ON_NULL: {
			print_to(stream, "{ast::expr}?", &expr->ret_on_null.expr); break;
			break;
		}
		case AST_EXPR_GET_LOCAL_PATH: {
			print_to(stream, "{ast::path}", &expr->get_local.path); break;
			break;
		}
		case AST_EXPR_GET_INNER_PATH: {
			print_to(stream, "({ast::expr}).{ast::path}", expr->get_inner.of, &expr->get_inner.path); break;
			break;
		}
		case AST_EXPR_FLOAT: {
			fprintf(stream, "%Lf", expr->float_value); break;
			break;
		}
		case AST_EXPR_IDX: {
			print_to(stream, "({ast::expr})[ast::expr]", expr->idx.of, &expr->idx.idx); break;
			break;
		}
		case AST_EXPR_UNARY: {
			switch (expr->unary.type) {
				case AST_UNARY_MINUS: print_to(stream, "-{ast::expr}", expr->unary.expr); break;
				case AST_UNARY_BITNOT: print_to(stream, "~{ast::expr}", expr->unary.expr); break;
			}
			break;
		}
		case AST_EXPR_REF: print_to(stream, "&{ast::expr}", &expr->ref_expr); break;
		case AST_EXPR_NULL: print_to(stream, "null"); break;
		case AST_EXPR_NOT: print_to(stream, "!({ast::expr})", &expr->not_expr); break;
		case AST_EXPR_INTEGER: print_to(stream, "{long}", expr->integer); break;
		case AST_EXPR_CHAR: fprintf(stream, "'%c'", expr->character); break;
		case AST_EXPR_BOOL: print_to(stream, "{bool}", &expr->boolean); break;
		case AST_EXPR_BINOP: print_to(stream, "({ast::expr} {ast::binop} {ast::expr})", expr->binop.left, expr->binop.type, expr->binop.right); break;
		case AST_EXPR_ARRAY:
			print_to(stream, "[");
			for (size_t i = 0; i < vec_len(expr->array); i++) {
				print_to(stream, i == 0 ? "{ast::expr}" : ", {ast::expr}", &expr->array[i]);
			}
			print_to(stream, "]");
			break;
		case AST_EXPR_STR:
			print_to(stream, "\"{slice}\"", &expr->str);
			break;
		case AST_EXPR_CALL:
			print_to(stream, "{ast::expr}(", expr->call.callable);
			for (size_t i = 0; i < vec_len(expr->call.args); i++) {
				if (i != 0) print_to(stream, ", ");
				print_to(stream, "{ast::expr}", expr->call.args[i]);
			}
			print_to(stream, ")");
			break;
		case AST_EXPR_AS:
            switch (expr->as.type) {
                case AST_EXPR_AS_TYPE:
			        print_to(stream, "{ast::expr} as {ast::type}", expr->as.expr, &expr->as.as_type);
                    break;
                case AST_EXPR_AS_AUTO:
			        print_to(stream, "{ast::expr} as _", expr->as.expr);
                    break;
            }
			break;
	}
}
