#pragma once
#include "../tokenizer/token/tokenType.hpp"
#include <map>
#include <string>

class SymbolTable {
private:
    std::map<std::string, Symbol> Table;
public:
    Symbol set(std::string name, Symbol symbol) {
        Table[name] = symbol;
        return symbol;
    }
    Symbol get(std::string name) {
        return Table[name];
    }
    Symbol del(std::string name) {
        Symbol symbol = Table[name];
        Table.erase(name);
        return symbol;
    }
};

static struct Symbol {
    std::string SymbolName;
    void* SymbolToken;
    TokenType SymbolTokenType;
} symbol;