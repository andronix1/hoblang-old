#pragma once

#include <stdio.h>
#include "core/ansi.h"

#define HOBLANG ANSI_BOLD "hoblang" ANSI_RESET

#define __CMD_PRINT(text) printf(text "\n");
#define CMD_USAGE_MAIN(name, args) __CMD_PRINT(HOBLANG " " ANSI_UNDERLINE name ANSI_RESET " " args)
#define CMD_USAGE_FLAG(flag, desc) __CMD_PRINT("    " flag ANSI_DIM " — " desc ANSI_RESET)

