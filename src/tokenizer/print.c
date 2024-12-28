#include "print.h"

void tokenizer_print_line(Tokenizer *tokenizer) {
	char c;
	for (int i = -tokenizer->line_offset; (c = *(tokenizer->remain.str + i)) != '\n'; i++) {
		putchar(c);
	}
}

void tokenizer_print_column_ptr(Tokenizer *tokenizer) {
	for (int i = -tokenizer->line_offset; i < (int)tokenizer->location.column - 1; i++) {
		if (*(tokenizer->remain.str + i) == '\t') {
			putchar('\t');
		} else {
			putchar(' ');
		}
	}
	putchar('^');
}

void tokenizer_print_line_at(Tokenizer *tokenizer) {
	printf("| ");
	tokenizer_print_line(tokenizer);
	printf("\n");
	printf("| ");
	tokenizer_print_column_ptr(tokenizer);
	printf("\n");
}
