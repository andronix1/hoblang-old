#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "sema/sema.h"
#include "llvm/llvm.h"

char *args_shift(int *argc, char ***argv) {
    *argc -= 1;
    char *cached = (*argv)[0];
    *argv = &(*argv)[1];
    return cached;
}

void usage() {
	printf("hoblang compile <source> <output> - compile source file to machine code\n");
	printf("hoblang help                      - print this\n");
}

int main(int argc, char **argv) {
	if (argc != 4) {
		usage();
		return 0;
	}
	fatptr_register_printf();
	token_register_printf();
    args_shift(&argc, &argv);
    const char *command = args_shift(&argc, &argv);
    if (!strcmp(command, "compile")) {
		const char *src_path = args_shift(&argc, &argv);
		char *output_path = args_shift(&argc, &argv);
        Lexer lexer;
		if (!lexer_init(&lexer, src_path)) {
			return 1;
		}
        hob_log(LOGI, "lexed!");
		Parser parser;
		if (!parser_init(&parser, &lexer)) {
			return 1;
		}
		AstModule module = {
			.name = fatptr_from_cstr("main") // todo: module keyword
		};
		if (!parse_module(&parser, &module)) {
			return 1;
		}
		// ast_print_module(&module);
        hob_log(LOGI, "parsed!");
		Sema sema = sema_new();
		sema_module(&sema, &module);
		if (sema.failed) {
			return 1;
		}
		hob_log(LOGI, "analyzed!");
		LlvmBackend llvm;
		if (!llvm_init(&llvm)) {
			return 1;
		}
		llvm_module(&llvm, &module, output_path);
		hob_log(LOGI, "finished!");
    } else {
		usage();
    }
    return 0;
}
