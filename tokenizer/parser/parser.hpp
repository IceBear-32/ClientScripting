#pragma once
#include "../tokenizer.hpp"
#include "nodes.hpp"

class Parser {
private:
    std::vector<Token> tokens;
    size_t idx;
    std::vector<ASTNode*> AST;
    Token getPrevTok();
    Token getCurTok();
    Token getNextTok();
    ASTNode* parseExpr();
    ASTNode* parseRelExpr();
    ASTNode* parseArithExpr();
    ASTNode* parseTerm();
    ASTNode* parseFac();
    ASTNode* parseExpo();
    ASTNode* parseAtom();
    ConditionNode* parseConditionalStatements();
    BlockNode* parseBlock();
    ASTNode* parseVarDec();
    EventNode* parseEventReg();
    void next();
    void back();
public:
    Parser& readTokens(std::vector<Token>);
    std::vector<ASTNode*> getAST();
    Parser& parse();
};

extern Parser parser;