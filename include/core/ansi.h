#pragma once

#define __ANSI(cmd) "\033[" #cmd "m"
#define ANSI_RESET __ANSI(0)
#define ANSI_BOLD __ANSI(1)
#define ANSI_UNDERLINE __ANSI(4)
#define ANSI_RED __ANSI(31)
#define ANSI_YELLOW __ANSI(33)
#define ANSI_GREEN __ANSI(32)
#define ANSI_BLUE __ANSI(34)
#define ANSI_BRIGHT_WHITE __ANSI(97)
