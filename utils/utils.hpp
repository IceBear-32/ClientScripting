#pragma once
#include "token/token.hpp"
#include <string>

bool isWS(char);
bool isChar(char);
bool isNum(char);
bool isOP(char);
bool isPunc(char);

Keyword getKeyWord(std::string);
Operator getOperator(std::string);
Punctuator getPunctuator(std::string);

bool isRelationalOp(Operator);

int getCharAsNum(char);

bool long_long_addition_overflow(long long, long long);