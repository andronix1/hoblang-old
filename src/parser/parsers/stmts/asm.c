#include "../../parsers.h"

bool token_is_asm_arg_end(TokenType type) {
    return type == TOKEN_COMMA || type == TOKEN_SEMICOLON;
}

bool parse_asm_lang_arg(Parser *parser, AstInlineAsmLangArg *arg) {
    parser_next_token(parser);
    switch (token_type(parser->token)) {
        case TOKEN_IDENT: {
            AstValue value;
            parser->skip_next = true;
            if (!parse_value(parser, &value)) {
                return false;
            }
            ast_inline_arg_init_value(arg, value);
            return true;
        }
        
        case TOKEN_OPENING_CIRCLE_BRACE: {
            parser->skip_next = true;
            AstExpr *expr = parse_expr_before(parser, token_is_asm_arg_end);
            parser->skip_next = true;
            if (!expr) {
                return false;
            }
            ast_inline_arg_init_expr(arg, expr);
            return true;
        }

        default:
            parse_err(EXPECTED("asm lang arg"));
            return false;
    }
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
                AstInlineAsmArg arg = ast_asm_reg(parser->token->ident);
                mnem->args = vec_push(mnem->args, &arg);
                break;
            }

            case TOKEN_INTEGER: {
                AstInlineAsmArg arg = ast_asm_int(parser->token->integer);
                mnem->args = vec_push(mnem->args, &arg);
                break;
            }

            case TOKEN_STR: {
                Slice constr = {
                    .str = parser->token->str,
                    .len = vec_len(parser->token->str)
                };
                AstInlineAsmArg arg = {
                    .type = AST_INLINE_ASM_ARG_LANG
                };
                if (!parse_asm_lang_arg(parser, &arg.lang)) {
                    return false;
                }
                arg.lang.has_constraint = true;
                arg.lang.constraint = constr;
                mnem->args = vec_push(mnem->args, &arg);
                break;
            }

            case TOKEN_IDENT:
            case TOKEN_OPENING_CIRCLE_BRACE: {
                parser->skip_next = true;
                AstInlineAsmArg arg = {
                    .type = AST_INLINE_ASM_ARG_LANG
                };
                if (!parse_asm_lang_arg(parser, &arg.lang)) {
                    return false;
                }
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
    parse_exp_next(TOKEN_OPENING_CIRCLE_BRACE, "asm clobers opening");
    inline_asm->clobbers = vec_new(Slice);
    bool reading = true;
    parser_next_token(parser);
    if (token_type(parser->token) == TOKEN_CLOSING_CIRCLE_BRACE) {
        return true;
    }
    parser->skip_next = true;
    while (reading) {
        parse_exp_next(TOKEN_IDENT, "register name");
        inline_asm->clobbers = vec_push(inline_asm->clobbers, &parser->token->ident);
        parser_next_token(parser);
        switch (token_type(parser->token)) {
            case TOKEN_CLOSING_CIRCLE_BRACE:
                reading = false;
                break;
            case TOKEN_COMMA:
                break;
            default:
                parse_err("expected clobber break");
                return false;
        }
    }
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

