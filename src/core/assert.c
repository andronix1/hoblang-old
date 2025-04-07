#include <valgrind/valgrind.h>

void print_backtrace() {
    VALGRIND_PRINTF_BACKTRACE("\n");
}
