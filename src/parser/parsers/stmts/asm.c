#include "../../parsers.h"

bool token_is_asm_arg_end(TokenType type) {
    return type == TOKEN_COMMA || type == TOKEN_SEMICOLON;
}

bool parse_asm_mnemonic(Parser *parser, AstInlineAsmMnemonic *mnem) {
    parse_exp_next(TOKEN_IDENT, "asm mnemonic");
    mnem->name = parser->token->ident;
    mnem->args = vec_new(AstInlineAsmArg);
    parser_next_token(parser);
    if (token_type(parser->token) == TOKEN_SEMICOLON) {
        return true;
    }
    parser->skip_next = true;
    while (true) {
        parser_next_token(parser);
        switch (token_type(parser->token)) {
            case TOKEN_DOLLAR: {
                parse_exp_next(TOKEN_IDENT, "register name");
                AstInlineAsmArg arg = ast_inline_arg_new_reg(parser->token->ident);
                mnem->args = vec_push(mnem->args, &arg);
                break;
            }

            case TOKEN_IDENT: {
                AstValue value;
                parser->skip_next = true;
                if (!parse_value(parser, &value)) {
                    return false;
                }
                AstInlineAsmArg arg = ast_inline_arg_new_value(value);
                mnem->args = vec_push(mnem->args, &arg);
                break;
            }
            
            case TOKEN_OPENING_CIRCLE_BRACE: {
                parser->skip_next = true;
                AstExpr *expr = parse_expr_before(parser, token_is_asm_arg_end);
                parser->skip_next = true;
                if (!expr) {
                    return false;
                }
                AstInlineAsmArg arg = ast_inline_arg_new_expr(expr);
                mnem->args = vec_push(mnem->args, &arg);
                break;
            }

            default:
                parse_err(EXPECTED("asm arg"));
                break;
        }
        parser_next_token(parser);
        switch (token_type(parser->token)) {
            case TOKEN_COMMA:
                break;
            case TOKEN_SEMICOLON:
                return true;
            default:
                parse_err(EXPECTED("asm mnemonic arg break"));
                return false;
        }
    }
}

bool parse_asm_body(Parser *parser, AstInlineAsm *inline_asm) {
    parse_exp_next(TOKEN_OPENING_FIGURE_BRACE, "asm body opening");
    inline_asm->mnems = vec_new(AstInlineAsmMnemonic);
    while (true) {
        parser_next_token(parser);
        if (token_type(parser->token) == TOKEN_CLOSING_FIGURE_BRACE) {
            return true;
        }
        parser->skip_next = true;
        AstInlineAsmMnemonic mnem;
        if (!parse_asm_mnemonic(parser, &mnem)) {
            return false;
        }
        inline_asm->mnems = vec_push(inline_asm->mnems, &mnem);
    }
}
