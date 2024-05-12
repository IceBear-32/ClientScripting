#include "parser.hpp"
#include <iostream>

Token Parser::getPrevTok() {
    return tokens[idx-1];
}

Token Parser::getCurTok() {
    return tokens[idx];
}

Token Parser::getNextTok() {
    return tokens[idx+1];
}

void Parser::next() {
    ++idx;
}

void Parser::back() {
    --idx;
}

Parser& Parser::readTokens(std::vector<Token> toks) {
    this->tokens = toks;
    return *this;
}

ASTNode* Parser::parseAtom() {
    auto tok = getCurTok();
    LiteralNode* LitNode = new LiteralNode();
    if (tok.Type == PUNCTUATOR) {
        this->next();
        Punctuator Punc = getPunctuator(tok.Value);
        if (Punc != LPAR) return nullptr;
        auto Expr = parseExpr();
        if (getCurTok().Type == PUNCTUATOR && getPunctuator(getCurTok().Value) == RPAR) {
            return Expr;
        }
        else {
            return nullptr;
        }
    }
    if (tok.Type == LITERAL){
        if (tok.Value.at(0)== '"') {
            LitTok.Type = STRING;
            LitTok.Value = tok.Value.substr(1, tok.Value.size()-2);
        }
        else {
            auto np = std::string::npos;
            if (tok.Value.find('.') != np) {
                LitTok.Type = FLOAT;
                LitTok.Value = tok.Value;
            }
            else {
                LitTok.Type = INT;
                LitTok.Value = tok.Value;
            }
        }
        LitNode->Literal = LitTok;
        return LitNode;
    }
    return nullptr;
}

ASTNode* Parser::parseExpo() {
    auto lOp = parseAtom();
    this->next();
    auto tok = getCurTok();
    Operator Op = getOperator(tok.Value);
    while (tok.Type == OPERATOR && Op == POW) {
        OpTok.Value = Op;
        BinaryOpNode* BinNode = new BinaryOpNode();
        BinNode->lOp = lOp;
        BinNode->Op = OpTok;
        this->next();
        BinNode->rOp = parseFac();
        lOp = BinNode;
        tok = getCurTok();
        Op = getOperator(tok.Value);
    }
    return lOp;
}

ASTNode* Parser::parseFac() {
    auto tok = getCurTok();
    if (tok.Type == OPERATOR) {
        Operator Opr = getOperator(tok.Value);
        if (Opr == DOT) {
            this->next();
            this->tokens[idx].Value = "0." + getCurTok().Value;
            return parseExpo();
        }
        this->next();
        if (Opr != ADD && Opr != SUB) return nullptr;
        LiteralNode* Oprnd = (LiteralNode*)parseFac();
        UnaryOpNode* UnOpNode = new UnaryOpNode();
        OpTok.Value = Opr;
        UnOpNode->Opr = OpTok;
        UnOpNode->Oprnd = Oprnd;
        return UnOpNode;
    }
    return parseExpo();
}

ASTNode* Parser::parseTerm() {
    auto lOp = parseFac();
    auto tok = getCurTok();
    Operator Op = getOperator(tok.Value);
    while (tok.Type == OPERATOR && (Op == MUL || Op == DIV || Op == MOD)) {
        OpTok.Value = Op;
        BinaryOpNode* BinNode = new BinaryOpNode();
        BinNode->lOp = lOp;
        BinNode->Op = OpTok;
        this->next();
        BinNode->rOp = parseFac();
        lOp = BinNode;
        tok = getCurTok();
        Op = getOperator(tok.Value);
    }
    return lOp;
}

ASTNode* Parser::parseArithExpr() {
    auto lOp = parseTerm();
    auto tok = getCurTok();
    Operator Op = getOperator(tok.Value);
    while (tok.Type == OPERATOR && (Op == ADD || Op == SUB)) {
        OpTok.Value = Op;
        BinaryOpNode* BinNode = new BinaryOpNode();
        BinNode->lOp = lOp;
        BinNode->Op = OpTok;
        this->next();
        BinNode->rOp = parseTerm();
        lOp = BinNode;
        tok = getCurTok();
        Op = getOperator(tok.Value);
    }
    return lOp;
}

ASTNode* Parser::parseRelExpr() {
    if (getCurTok().Type == OPERATOR && getOperator(getCurTok().Value) == NOT) {
        UnaryOpNode* UOpNode = new UnaryOpNode();
        OpTok.Value = getOperator(getCurTok().Value);
        UOpNode->Opr = OpTok;
        this->next();
        UOpNode->Oprnd = parseRelExpr();
        this->next();
        return UOpNode;
    }
    auto lOp = parseArithExpr();
    auto tok = getCurTok();
    Operator Op = getOperator(tok.Value);
    BinaryOpNode* BinNode = new BinaryOpNode();
    while (tok.Type == OPERATOR && isRelationalOp(Op)) {
        OpTok.Value = Op;
        BinNode->lOp = lOp;
        BinNode->Op = OpTok;
        this->next();
        BinNode->rOp = parseArithExpr();
        lOp = BinNode;
        tok = getCurTok();
        Op = getOperator(tok.Value);
    }
    return lOp;
}

ASTNode* Parser::parseExpr() {
    auto lOp = parseRelExpr();
    auto tok = getCurTok();
    Operator Op = getOperator(tok.Value);
    BinaryOpNode* BinNode = new BinaryOpNode();
    while (tok.Type == OPERATOR && (Op == AND || Op == OR)) {
        OpTok.Value = Op;
        BinNode->lOp = lOp;
        BinNode->Op = OpTok;
        this->next();
        BinNode->rOp = parseRelExpr();
        lOp = BinNode;
        tok = getCurTok();
        Op = getOperator(tok.Value);
    }
    return lOp;
}

ASTNode* Parser::parseVarDec() {
    this->next();
    auto tok = getCurTok();
    if(tok.Type != IDENTIFIER) return nullptr;
    this->next();
    auto opTok = getCurTok();
    if(opTok.Type != OPERATOR && getOperator(opTok.Value) != ASSIGN && opTok.Type != EOL) return nullptr;
    this->next();
    VarDecNode* vdNode = new VarDecNode();
    IdNode* idNode = new IdNode();
    idNode->Name = tok.Value;
    vdNode->Var = idNode;
    if (opTok.Type == EOL) {
        LiteralNode* lnode = new LiteralNode();
        LitTok.Signal = FALSE;
        LitTok.Type = BOOL;
        LitTok.Value = "0";
        lnode->Literal = LitTok;
        vdNode->Value = lnode;
        return vdNode;
    }
    auto Expr = parseExpr();
    vdNode->Value = Expr;
    return vdNode;
}

std::vector<ASTNode*> Parser::getAST() {
    return this->AST;
}

Parser& Parser::parse() {
    while (getCurTok().Type != EOFILE) {
        auto tok = getCurTok();
        if (tok.Type == OPERATOR && getOperator(tok.Value) == VAR) {
            this->AST.push_back(parseVarDec());
            continue;
        }
    }
    return *this;
}

Parser parser;