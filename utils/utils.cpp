#include "utils.hpp"

bool isWS(char c) {
    return  c == ' '    || \
            c == '\t'   || \
            c == '\r'   || \
            c == '\f'   || \
            c == '\v'   ;
}

bool isChar(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool isNum(char c) {
    return (c >= '0' && c <= '9');
}

bool isOP(char c) {
    return  c == '!' || \
            c == '@' || \
            c == '$' || \
            c == '%' || \
            c == '^' || \
            c == '&' || \
            c == '*' || \
            c == '-' || \
            c == '=' || \
            c == '+' || \
            c == '<' || \
            c == '>' || \
            c == '/' || \
            c == '|' || \
            c == ':' || \
            c == '?' || \
            c == '.' ;
}

bool isPunc(char c) {
    return  c == '(' || \
            c == ')' || \
            c == '[' || \
            c == ']' || \
            c == '{' || \
            c == '}' || \
            c == ',' ;
}

Keyword getKeyWord(std::string kw) {
    if (kw == "if") return IF;
    if (kw == "else") return ELSE;
    if (kw == "func") return FUNC;
    if (kw == "del") return DEL;
    if (kw == "loop") return LOOP;
    if (kw == "null") return NULLKW;
    if (kw == "return") return RETURN;
    return UNKNOWNKW;
}

Operator getOperator(std::string op) {
    if (op == "+") return ADD;
    if (op == "-") return SUB;
    if (op == "*") return MUL;
    if (op == "/") return DIV;
    if (op == "%") return MOD;
    if (op == "^") return POW;
    if (op == "!") return NOT;
    if (op == "@") return AT;
    if (op == "$") return VAR;
    if (op == "=") return ASSIGN;
    if (op == ":") return COL;
    if (op == "?") return QS;
    if (op == "<") return LESS;
    if (op == ">") return GREAT;
    if (op == ".") return DOT;
    if (op == "==") return EQUAL;
    if (op == "!=") return NOT_EQ;
    if (op == "<=") return LESS_EQ;
    if (op == ">=") return GREAT_EQ;
    if (op == "&&") return AND;
    if (op == "||") return OR;
    if (op == "+=") return ADD_EQ;
    if (op == "-=") return SUB_EQ;
    if (op == "*=") return MUL_EQ;
    if (op == "/=") return DIV_EQ;
    if (op == "%=") return MOD_EQ;
    if (op == "^=") return POW_EQ;
    return UNKNOWNOP;
}

Punctuator getPunctuator(std::string Punc) {
    if (Punc == "(") return LPAR;
    if (Punc == ")") return RPAR;
    if (Punc == "[") return LBOX;
    if (Punc == "]") return RBOX;
    if (Punc == "{") return LCUR;
    if (Punc == "}") return RCUR;
    if (Punc == ",") return COMMA;
    return UNKNOWNPU;
}

bool isRelationalOp(Operator Op) {
    return  (Op == EQUAL)   || \
            (Op == NOT_EQ)  || \
            (Op == GREAT)   || \
            (Op == GREAT_EQ)|| \
            (Op == LESS)    || \
            (Op == LESS_EQ) ;
}

int getCharAsNum(char n) {
    return n - '0';
}

bool long_long_addition_overflow(long long a, long long b) {
    if (b > 0 && a > (LLONG_MAX - b)) return true;
    if (b < 0 && a < (LLONG_MIN - b)) return true;
    return false;
}
