#include "tokenizer/tokenizer.hpp"
#include "tokenizer/parser/parser.hpp"
#include "fs/filereader.hpp"
#include "interpreter/visitor.hpp"
#include <iostream>

void walk(ASTNode* node) {
    if (node == nullptr) {
        return;
    }

    switch (node->Type) {
        case VAR_DEC: {
            VarDecNode* varDecNode = dynamic_cast<VarDecNode*>(node);
            // Handle VarDecNode
            std::cout << "Variable Declaration" << std::endl;
            walk(varDecNode->Var);
            walk(varDecNode->Value);
            break;
        }
        case VAR_REASSIGN: {
            VarReassignNode* varReassignNode = dynamic_cast<VarReassignNode*>(node);
            // Handle VarReassignNode
            std::cout << "Variable Reassignment" << std::endl;
            walk(varReassignNode->Var);
            walk(varReassignNode->Value);
            break;
        }
        case ID: {
            IdNode* idNode = dynamic_cast<IdNode*>(node);
            // Handle IdNode
            std::cout << "Identifier: " << idNode->Name << std::endl;
            break;
        }
        case LIT: {
            LiteralNode* litNode = dynamic_cast<LiteralNode*>(node);
            // Handle LiteralNode
            std::cout << "Literal: " << litNode->Literal.Value << std::endl;
            break;
        }
        case BINARY_OP: {
            BinaryOpNode* binaryOpNode = dynamic_cast<BinaryOpNode*>(node);
            // Handle BinaryOpNode
            std::cout << "Binary Operation: " << binaryOpNode->Op.Value << std::endl;
            walk(binaryOpNode->lOp);
            walk(binaryOpNode->rOp);
            break;
        }
        case UNARY_OP: {
            UnaryOpNode* unaryOpNode = dynamic_cast<UnaryOpNode*>(node);
            // Handle UnaryOpNode
            std::cout << "Unary Operation: " << unaryOpNode->Opr.Value << std::endl;
            walk(unaryOpNode->Oprnd);
            break;
        }
        case BLOCK: {
            BlockNode* blockNode = dynamic_cast<BlockNode*>(node);
            // Handle BlockNode
            std::cout << "Block" << std::endl;
            for (auto statement : blockNode->Statements) {
                walk(statement);
            }
            break;
        }
        // Add cases for other node types as needed
        default:
            std::cerr << "Unhandled node type" << std::endl;
            break;
    }
}


int main(int argc, char* argv[]) {
    std::string path = argv[1];
    std::string file = getFileString(path);
    std::cout << "[";
    auto tokens = tokenizer.readFileStr(file).tokenize().getTokens();
    auto parse = parser.readTokens(tokens).parse().getAST();
    auto node = (BinaryOpNode*)(((VarDecNode*)parse.at(0))->Value);
    Visitor vis;
    walk(parse[0]);
    // node.rOp = node.lOp.rOp and node.lOp.lOp is none
    std::cout << vis.visitBinaryOpNode(node).Value;
    // for (auto tok : tokens) {
    //     std::cout << tok.Value << ", ";
    // }
    std::cout << "]";
}