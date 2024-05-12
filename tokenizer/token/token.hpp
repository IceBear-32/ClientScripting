#pragma once
#include "tokenType.hpp"
#include <string>

static struct Token {
    TokenType Type;
    std::string Value;
} Tok;

enum Literal {
    STRING,
    INT,
    FLOAT,
    BOOL
};

enum Punctuator {
    LPAR,
    RPAR,
    LBOX,
    RBOX,
    LCUR,
    RCUR,
    COMMA,
    UNKNOWNPU
};

enum Boolean {
    FALSE,
    TRUE
};

enum Keyword {
    IF,
    ELSE,
    FUNC,
    DEL,
    NULLKW,
    LOOP,
    UNKNOWNKW,
    RETURN
};

enum Operator {
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    POW,
    NOT,
    AT,
    VAR,
    AND,
    OR,
    EQUAL,
    NOT_EQ,
    GREAT,
    GREAT_EQ,
    LESS,
    LESS_EQ,
    ASSIGN,
    ADD_EQ,
    SUB_EQ,
    MUL_EQ,
    DIV_EQ,
    MOD_EQ,
    POW_EQ,
    COL,
    QS,
    DOT,
    UNKNOWNOP
};

static struct LiteralToken {
    Literal Type;
    std::string Value;
    Boolean Signal;
} LitTok;

static struct OperatorToken {
    Operator Value;
} OpTok;

static struct KeywordToken {
    Keyword Value;
} KwTok;

static struct PunctuatorToken {
    Punctuator Value;
} PuncTok;