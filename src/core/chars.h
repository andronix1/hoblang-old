#pragma once

#include <stdbool.h>

inline static bool char_is_whitespace(char c) {
	return c == '\n' || c == ' ' || c == '\t';
}

inline static bool char_is_latin(char c) {
	return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

inline static bool char_is_digit(char c) {
	return '0' <= c && c <= '9';
}

inline static bool char_is_hex(char c) {
	return char_is_digit(c) ||
		('a' <= c && c <= 'f') || 
		('A' <= c && c <= 'F');
}

inline static bool char_is_ident_start(char c) {
	return char_is_latin(c) || c == '_' || c == '$' || c == '@';
}

inline static bool char_is_ident(char c) {
	return char_is_ident_start(c) || char_is_digit(c);
}

