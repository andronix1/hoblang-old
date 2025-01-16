#include "log.h"

#define ANSI(cmd) "\033[" #cmd "m"
#define ANSI_RESET ANSI(0)
#define ANSI_RED ANSI(31)
#define ANSI_YELLOW ANSI(33)
#define ANSI_GREEN ANSI(32)
#define ANSI_BLUE ANSI(34)

const char *log_level_str[] = {
	[LOGD] = ANSI_BLUE   "debug"   ANSI_RESET,
	[LOGI] = ANSI_GREEN  "info"    ANSI_RESET,
	[LOGW] = ANSI_YELLOW "warning" ANSI_RESET,
	[LOGE] = ANSI_RED    "error"   ANSI_RESET,
	[LOGF] = ANSI_RED    "fatal"   ANSI_RESET,
};

void hob_log(LogLevel level, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	print("{cstr}: ", log_level_str[level]);
	print_va_list(fmt, args);
	putchar('\n');
	va_end(args);
}

void hob_log_at(LogLevel level, const char *path, FileLocation location, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	print("{cstr}:{size}:{size} - {cstr}: ", path, location.line + 1, location.column + 1, log_level_str[level]);
	print_va_list(fmt, args);
	print("\n");
	va_end(args);
}

