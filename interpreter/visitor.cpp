#include "visitor.hpp"

LiteralToken Visitor::visitBinaryOpNode(BinaryOpNode* BinOpNode) {
    LiteralToken lOp;
    LiteralToken rOp;
    switch(BinOpNode->lOp->Type) {
        case LIT:
        lOp = visitLiteralNode((LiteralNode*)BinOpNode->lOp);
        break;
        case BINARY_OP:
        lOp = visitBinaryOpNode((BinaryOpNode*)BinOpNode->lOp);
        break;
        case UNARY_OP:
        lOp = visitUnaryOpNode((UnaryOpNode*)BinOpNode->lOp);
        break;
    }
    switch(BinOpNode->rOp->Type) {
        case LIT:
        rOp = visitLiteralNode((LiteralNode*)BinOpNode->rOp);
        break;
        case BINARY_OP:
        rOp = visitBinaryOpNode((BinaryOpNode*)BinOpNode->rOp);
        break;
        case UNARY_OP:
        rOp = visitUnaryOpNode((UnaryOpNode*)BinOpNode->rOp);
        break;
    }
    auto Op = BinOpNode->Op;

    Literal rType;
    std::string rValue;
    Boolean rSignal;

    rType = (lOp.Type == FLOAT || rOp.Type == FLOAT) ? FLOAT : INT;
    if (lOp.Type == STRING || rOp.Type ==  STRING) rType = STRING;

    switch (Op.Value)
    {
    case ADD:
    if (rType == STRING) {
        rValue = Eval::ConcatenateString(lOp.Value, rOp.Value);
    }
    else {
        rValue = Eval::AddNumbers(lOp.Value, rOp.Value, rType);
    }
    break;
    case SUB:
    if (rType == STRING) break;
    else {
        rValue = Eval::SubNumbers(lOp.Value, rOp.Value, rType);
    }
    break;
    case MUL:
    if (rType == STRING) {
        rValue = Eval::RepeatString(lOp.Value, rOp.Value);
    }
    else {
        rValue = Eval::MulNumbers(lOp.Value, rOp.Value, rType);
    }
    break;
    case DIV:
    if (rType == STRING) break;
    else {
        rValue = Eval::DivNumbers(lOp.Value, rOp.Value, rType);
    }
    break;
    case POW:
    if (rType == STRING) break;
    else {
        rValue = Eval::PowNumbers(lOp.Value, rOp.Value, rType);
    }
    break;
    case GREAT:
    if (rType == STRING) break;
    else {
        rType = BOOL;
        rValue = std::to_string(Eval::IsGreater(lOp.Value, rOp.Value));
    }
    break;
    case GREAT_EQ:
    if (rType == STRING) break;
    else {
        rType = BOOL;
        rValue = std::to_string(Eval::IsGreatOrEq(lOp.Value, rOp.Value));
    }
    break;
    case LESS:
    if (rType == STRING) break;
    else {
        rType = BOOL;
        rValue = std::to_string(Eval::IsLesser(lOp.Value, rOp.Value));
    }
    break;
    case LESS_EQ:
    if (rType == STRING) break;
    else {
        rType = BOOL;
        rValue = std::to_string(Eval::IsLessOrEq(lOp.Value, rOp.Value));
    }
    break;
    case EQUAL:
    rType = BOOL;
    rValue = std::to_string(Eval::IsEqual(lOp.Value, rOp.Value));
    break;
    case NOT_EQ:
    rType = BOOL;
    rValue = std::to_string(Eval::IsNotEqual(lOp.Value, rOp.Value));
    break;
    }
    
    rSignal = (rValue.empty() || rValue == "0") ? FALSE : TRUE;
    LitTok.Type = rType;
    LitTok.Value = rValue;
    LitTok.Signal = rSignal;
    return LitTok;
}

LiteralToken Visitor::visitUnaryOpNode(UnaryOpNode* UnOpNode) {
    OperatorToken Op = UnOpNode->Opr;
    auto Oprnd = UnOpNode->Oprnd;
    LiteralToken newTok;
    switch (Oprnd->Type) {
        case BINARY_OP:
        newTok = visitBinaryOpNode((BinaryOpNode*)Oprnd);
        break;
        case LIT:
        newTok = visitLiteralNode((LiteralNode*)Oprnd);
        break;
        case UNARY_OP:
        newTok = visitUnaryOpNode((UnaryOpNode*)Oprnd);
        break;
    }

    //if (LitTok.Type == STRING) return;
    if(Op.Value == SUB){
        newTok.Value = "-"+newTok.Value;
        return newTok;
    } else if(Op.Value == ADD) return newTok;
}

LiteralToken Visitor::visitLiteralNode(LiteralNode* node) {
    if (node->Literal.Type == INT || node->Literal.Type == FLOAT) {
        node->Literal.Value = Eval::CheckInfinity(node->Literal.Value);
    }
    return node->Literal;
}