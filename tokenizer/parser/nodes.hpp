#pragma once
#include <string>
#include <vector>
#include "../token/token.hpp"

enum NodeType {
    DEFAULT,
    VAR_DEC,
    VAR_REASSIGN,
    VAR_ACCESS,
    CONDITION,
    FUNC_DEF,
    FUNC_CALL,
    EVENT_DEF,
    LIT,
    BLOCK,
    BINARY_OP,
    UNARY_OP,
    ID
};

class ASTNode {
public:    
    NodeType Type = DEFAULT;
    virtual ~ASTNode() {};
};

class VarDecNode : public ASTNode {
public:
    VarDecNode() {this->Type = VAR_DEC;}
    ASTNode* Var;
    ASTNode* Value;
};

class VarReassignNode : public ASTNode {
public:
    VarReassignNode() {this->Type = VAR_REASSIGN;}
    ASTNode* Var;
    Operator AssignmentOp;
    ASTNode* Value;
};

class VarAccessNode : public ASTNode {};

class IdNode : public ASTNode {
public:
    IdNode() {this->Type = ID;}
    std::string Name;
};

class LiteralNode : public ASTNode {
public:
    LiteralNode() {this->Type = LIT;}
    LiteralToken Literal;
};

class BinaryOpNode : public ASTNode {
public:
    BinaryOpNode() {this->Type = BINARY_OP;}
    ASTNode* lOp;
    OperatorToken Op;
    ASTNode* rOp;
};

class UnaryOpNode : public ASTNode {
public:
    UnaryOpNode() {this->Type = UNARY_OP;}
    OperatorToken Opr;
    ASTNode* Oprnd;
};

class BlockNode : public ASTNode {
public:
    BlockNode() {this->Type = BLOCK;}
    std::vector<ASTNode*> Statements;
};

class ConditionNode : public ASTNode {
public:
    ConditionNode() {this->Type = CONDITION;}
    ASTNode* Condition;
    ASTNode* TrueBlock;
    ASTNode* FalseBlock;
};

class EventNode : public ASTNode {
public:
    EventNode() {this->Type = EVENT_DEF;}
    IdNode* EventID;
    BlockNode* EventBlock;
};