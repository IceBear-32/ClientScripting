#pragma once
#include "../tokenizer/parser/parser.hpp"
#include "Eval.hpp"

class Visitor {
public:
    LiteralToken visitLiteralNode(LiteralNode*);
    LiteralToken visitBinaryOpNode(BinaryOpNode*);
    LiteralToken visitUnaryOpNode(UnaryOpNode*);
    void visitBlockNode(BlockNode*);
    void visitVarDecNode(VarDecNode*);
    void visitConditionNode(ConditionNode*);
public:
    void visitNode(ASTNode*);
    void visit(std::vector<ASTNode*>);
};