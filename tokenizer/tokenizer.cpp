#include "tokenizer.hpp"

// set the file string
Tokenizer& Tokenizer::readFileStr(const std::string fstr) {
    this->filestr = std::move(fstr);
    return *this;
};

// get the list of tokens
std::vector<Token> Tokenizer::getTokens() {
    return this->tokens;
}

// check for new token state
bool Tokenizer::check(char c) {
    if (this->ltoken.empty() && this->state == NUL_STATE) {
        this->ltoken += c;
        // if the new token starts with # then it enters comment state and ltoken is reset
        if (c == '#') {
            this->ltoken.clear();
            this->state = COMMENT_STATE;
        }
        // if the token starts with a character, it enters the name state
        else if (isChar(c)) this->state = NAME_STATE;
        // if the token starts with a number, it enters the number state
        else if (isNum(c)) this->state = NUMBER_STATE;
        // if the token starts with an operator, it enters the operator state
        else if (isOP(c)) this->state = OPERATOR_STATE;
        // if the token starts with a punctuator, it enters the punctuator state
        else if (isPunc(c)) this->state = PUNCTUATOR_STATE;
        else if (c == '"') this->state = STRING_STATE;
        // if the token is a whitespace, ltoken is reset and it goes into nul state
        else if (isWS(c)) {
            this->ltoken.clear();
            this->state = NUL_STATE;
        }
        // if it encounters an unkown symbol the check fails
        else {
            this->ltoken.clear();
            return false;
        }
        // if it doesnt fail, it returns true stating that the char is checked successfully
        return true;
    }
    // if token state is not in nul state or if ltoken is not empty, the check fails and return false
    return false;
}

// get the token type of the lexeme with the state and the lexeme
TokenType Tokenizer::getTokenType(std::string lexeme, TokenState tstate) {
    // if the state of the lexeme is number state or string state, the token type is literal
    if (tstate == NUMBER_STATE || tstate == STRING_STATE) return LITERAL;
    // if the state of the lexeme is operator state and the lexeme is a valid operator, the token type is operator
    if (tstate == OPERATOR_STATE && getOperator(lexeme) != UNKNOWNOP) return OPERATOR;
    // if the state of the lexeme is punctuator state and the lexeme is a valid punctuator, the token type is punctuator
    if (tstate == PUNCTUATOR_STATE) return PUNCTUATOR;
    // since the name state could be a boolean, an identifier or a keyword, we handle this here
    if (tstate == NAME_STATE) {
        // if the lexeme is true or false, then the token type is boolean
        if (lexeme == "true" || lexeme == "false") return BOOLEAN;
        // if the lexeme is not a valid keyword, then the token type is identifier
        if (getKeyWord(lexeme.c_str()) == UNKNOWNKW) return IDENTIFIER;
        // if both of the above conditions fail, then the token type is keyword
        return KEYWORD;
    }
    // if it fails to predict the token type of the lexeme, it returns unkown token type enum
    return UNKNOWNTT;
}

// this is the main function which separates each lexical units
Tokenizer& Tokenizer::tokenize() {
    // start iterating the file string
    for (char c : this->filestr) {
        // if it passes the check, that it is a start of a new token, it continues to the next iteration to prevent duplication of the same char in upcoming conditions
        if (check(c)) continue;
        // if the char is a new line char, it pushes the ltoken to the tokens list and reset the ltoken and the state to tokenize a new token
        if (c == '\n') {
            // if ltoken is not empty, push the values
            if(!this->ltoken.empty()) {
                Tok.Value = ltoken;
                Tok.Type = getTokenType(ltoken, state);
                this->tokens.push_back(Tok);
                this->ltoken.clear();
            }
            // push newline character and the token type as EOL (END OF LINE)
            Tok.Value = "\n";
            Tok.Type = EOL;
            this->tokens.push_back(Tok);
            this->state = NUL_STATE;
        }
        // if the token state is in comment state, continue until the newline condition resets the ltoken value and the token state
        if (this->state == COMMENT_STATE) continue;
        // if the token state is in string state, continue to gather characters and append them to ltoken until the string is closed with "
        // btw '\"' <- this this is also handled here (yeah quotes inside string yk)
        if (this->state == STRING_STATE) {
            // if the state is in string state and the character is ", it checks if the last character is backslash
            // so that it knows it should not mistake \" as string close
            // and then it pushes the value between the "'s and pushes a " and resets the ltoken and the token state to continue with new token
            if (c == '"' && ltoken.back() != '\\') {
                Tok.Value = ltoken + c;
                Tok.Type = getTokenType(ltoken+c, state);
                this->tokens.push_back(Tok);
                this->ltoken.clear();
                this->state = NUL_STATE;
            }
            // if it is not " append the character to ltoken
            else {
                this->ltoken += c;
            }
            // continue this iteration and check the next iteration to prevent malfunction
            continue;
        }
        // if the token is in operator state, then continue gathering the trailing operators
        if (this->state == OPERATOR_STATE) {
            // if the state is in operator state but the current char is not an operator, then we push the ltoken
            // and the token type of the lexeme into tokens list and reset the ltoken and state
            if (!isOP(c)) {
                Tok.Value = ltoken;
                Tok.Type = getTokenType(ltoken, state);
                this->tokens.push_back(Tok);
                this->ltoken.clear();
                this->state = NUL_STATE;
                // after resetting, we check the current char so that it is not missed on next iteration
                if(check(c)) continue;
            }
            // if the current char is an operator too, then we append the char
            else {
                this->ltoken += c;
            }
        }
        // if the token state is in name state or number state, we continue with default chars until a symbol or whitespace occurs
        if (this->state == NAME_STATE) {
            // if current char is not a number as well as a character, we push the ltoken value
            // and the token type and reset ltoken and the token state
            if (!isChar(c) && !isNum(c)) {
                Tok.Value = ltoken;
                Tok.Type = getTokenType(ltoken, state);
                this->tokens.push_back(Tok);
                this->ltoken.clear();
                this->state = NUL_STATE;
                // again we check the current char so it's not missed on the next iteration
                if(check(c)) continue;
            }
            // if it is a number or a character, it is appended
            else {
                this->ltoken += c;
            }
        }
        if (this->state == NUMBER_STATE) {
            if(!isNum(c) && c != 'e' && c != 'E' && c != '.') {
                if (ltoken.back() == 'e' || ltoken.back() == 'E') if (c == '-' || c == '+') {
                    this->ltoken += c;
                    continue;
                }
                Tok.Value = ltoken;
                Tok.Type = getTokenType(ltoken, state);
                this->tokens.push_back(Tok);
                this->ltoken.clear();
                this->state = NUL_STATE;
                // again we check the current char so it's not missed on the next iteration
                if(check(c)) continue;
            }
            else this->ltoken += c;
        }
        // same as other state handling but punctuators are single char so it doesnt append trailong punctuators and resets the ltoken and state after 1 punctuator
        if (this->state == PUNCTUATOR_STATE) {
            Tok.Value = ltoken;
            Tok.Type = getTokenType(ltoken, state);
            this->tokens.push_back(Tok);
            this->ltoken.clear();
            this->state = NUL_STATE;
            if(check(c)) continue;
        }
    }
    // at the end of the iteration, the last token is in the ltoken but is not pushed to the tokens list
    // so we check if ltoken is not empty and if it is empty, then we push the values and reset the ltoken
    // and set the state as EOFILE (END OF FILE)
    if (!this->ltoken.empty()) {
        Tok.Value = ltoken;
        Tok.Type = getTokenType(ltoken, state);
        this->tokens.push_back(Tok);
        this->ltoken.clear();
    }
    Tok.Value = "\0";
    Tok.Type = EOFILE;
    this->tokens.push_back(Tok);
    this->state = EOF_STATE;
    this->filestr.clear();
    // and we return the class itself for chaining the functions
    return *this;
}
Tokenizer tokenizer;