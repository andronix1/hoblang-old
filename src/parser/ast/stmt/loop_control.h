#pragma once

#include "ast/node/stmt/loop_control.h"
#include "parser/interface/parser.h"

AstLoopControl *parse_loop_control(Parser *parser);
