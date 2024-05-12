#pragma once
#include <string>
#include <vector>
#include "../utils/utils.hpp"
#include "token/tokenState.hpp"

class Tokenizer {
    std::string filestr;
    std::vector<Token> tokens;
    std::string ltoken = "";
    TokenState state = NUL_STATE;
public:
    Tokenizer& readFileStr(const std::string fstr);
    Tokenizer& tokenize();
    std::vector<Token> getTokens();
    TokenType getTokenType(std::string, TokenState);
    bool check(char);
};

extern Tokenizer tokenizer;