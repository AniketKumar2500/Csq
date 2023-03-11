#if !defined(AST_Csq4_H)
#define AST_Csq4_H

//Imports
#include "../Grammar/grammar.h"
#include "../Tokenizer/tokenizer.h"
#include "../IR/IRcode.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Types of AST
enum NODE_TYPE{
    VALUE_TYPE = 1,
    VAR_DECLARATION = 2,
    VAR_ASSIGNMENT = 3,
    FUN_DEFINITION = 4,
    CLASS_DEFINITION = 5,
    FOR_LOOP = 6,
    WHILE_LOOP = 7,
    BINARY_EXPR = 8,
    FUNCTION_CALL = 9,
    FUNCTION_DECL  = 10,
    BLOCK = 11,
    IF_STATEMENT = 12,
    ELIF_STATEMENT = 13,
    ELSE_STATEMENT = 14,
};

//Base class for all AST.
struct Node{
    NODE_TYPE type;
};


//Inherited structs for different types of AST Node.

//Value struct for value_type AST node.
struct Value;
//BinaryExpr struct for binary expressions.
struct BinaryExpr;
//VarDecl struct for variable declarations.
struct VarDecl;
//VarAssign struct for variable assignments.
struct VarAssign;
//ForLoop struct for for loops.
struct ForLoop;
//WhileLoop struct for while loops.
struct WhileLoop;
//Function struct for functions definitions.
struct FunctionDecl;
//Function struct for functions calls.
struct FunctionCall;
//Class struct for Classes.
struct ClassDef;    
//Block struct for group of different nodes.
struct Block;
//IfStmt struct for if statements
struct IfStmt;
//ElifStmt struct for else if statements
struct ElifStmt;
//ElseStmt for Else statements
struct ElseStmt;

//Body for above AST node types

//Body for Value struct
struct Value : Node{
    NODE_TYPE type = VALUE_TYPE;
    Token value;
};


//Body for BinaryExpr struct
struct BinaryExpr : Node{
    NODE_TYPE type = BINARY_EXPR;
    Token opt;
    Node* value1;
    Node* value2;
};

//Body for VarDecl struct
struct VarDecl : Node{
    NODE_TYPE type = VAR_DECLARATION;
    string name;
    string Dtype;
    Node* value;
};

//Body for VarAssign struct
struct VarAssign : Node{
    NODE_TYPE type = VAR_ASSIGNMENT;
    Node* value;
    string name;
};

//Body for ForLoop struct
struct ForLoop : Node{
    NODE_TYPE type = FOR_LOOP;
    Node* condition;
    string iter_name;
    Block* body;
};

//Body for WhileLoop struct
struct WhileLoop : Node{
    NODE_TYPE type = FOR_LOOP;
    Node* condition;
    Block* body;
};

//Body for FunctionCall struct
struct FunctionCall : Node{
    NODE_TYPE type = FUNCTION_CALL;
    string name;
    vector<Node*> param;
};

//Body for Block struct
struct Block : Node{
    NODE_TYPE type = BLOCK;
    vector<Node*> statements;
};

//Body for FunctionDecl struct
struct FunctionDecl : Node{
    NODE_TYPE type = FUNCTION_DECL;
    string name;
    Block* body;
    vector<VarDecl*> param;
};

//Body for IfStmt struct
struct IfStmt : Node{
    NODE_TYPE type = IF_STATEMENT;
    Node* condition;
    Block* body;
};
//Body for ElifStmt struct
struct ElifStmt : Node{
    NODE_TYPE type = ELIF_STATEMENT;
    Node* condition;
    Block* body;
};
//Body for ElseStmt struct
struct ElseStmt : Node{
    NODE_TYPE type = ELSE_STATEMENT;
    Block* body;
};

//Struct for ClassDef
struct ClassDef : Node{
    NODE_TYPE type = CLASS_DEFINITION;
    string name;
    Block* body;
};

//Help functions to create instances of AST nodes.
void assignValueNode(Value* node, Token token){
    node->type = VALUE_TYPE;
    node->value = token;
    node->value.type = token.type;
}

void assignVarDeclNode(VarDecl* node, string name,string type, Node* value){
    node->type = VAR_DECLARATION;
    node->name = name;
    node->Dtype = type;
    node->value = value;
}

void assignBinaryExprNode(BinaryExpr* node, Node* val1, Node* val2, Token opt){
    node->opt= opt;
    node->type = BINARY_EXPR;
    node->value1 = val1;
    node->value2 = val2;
}

void assignVarAssignNode(VarAssign* node, string name, Node* val){
    node->type = VAR_ASSIGNMENT;
    node->name = name;
    node->value = val;
}

void assignIfStmtNode(IfStmt* node, Node* condition, Block* body){
    node->type = IF_STATEMENT;
    node->condition = condition;
    node->body = body;
}

void assignElifStmtNode(ElifStmt* node, Node* condition, Block* body){
    node->type = ELIF_STATEMENT;
    node->condition = condition;
    node->body = body;
}

void assignElseStmtNode(ElseStmt* node, Block* body){
    node->type = ELSE_STATEMENT;
    node->body = body;
}

void assignForLoop(ForLoop* node, Block* body, Node* condition, string iter_name){
    node->body = body;
    node->condition = condition;
    node->iter_name = iter_name;
    node->type = FOR_LOOP;
}

void assignWhileLoop(WhileLoop* node, Block* body, Node* condition){
    node->body = body;
    node->condition = condition;
    node->type = WHILE_LOOP;
}

void assignFunctionDecl(FunctionDecl* node, string name, Block* body, vector<VarDecl*> param){
    node->body = body;
    node->name = name;
    node->param = param;
    node->type = FUNCTION_DECL;
}

void assignFunctionCall(FunctionCall* node, string name, vector<Node*> param){
    node->name = name;
    node->param = param;
    node->type = FUNCTION_CALL;
}

void assignClassDef(ClassDef* node, string name, Block* body){
    node->name = name;
    node->body = body;
    node->type = CLASS_DEFINITION; 
}

//Help functions for Block node
Block newBlock(){
    Block block;
    block.type = BLOCK;
    return block;
}
void addStatement(Block block, Node* statement){
    block.statements.push_back(statement);
}


void printNode(Node* node){
    switch (node->type) {
        case VALUE_TYPE: {
            Value* valueNode = static_cast<Value*>(node);
            printf("|Value : %s\n",valueNode->value.token.c_str());
            break;
        }
        case VAR_DECLARATION: {
            VarDecl* varDeclNode = static_cast<VarDecl*>(node);
            printf("|Variable Declaration: %s\n",varDeclNode->name.c_str());
            printNode(varDeclNode->value);
            break;
        }
        case VAR_ASSIGNMENT: {
            VarAssign* varAssignNode = static_cast<VarAssign*>(node);
            printf("|Variable Assignment : %s\n",varAssignNode->name.c_str());
            printNode(varAssignNode->value);
            break;
        }
        case BINARY_EXPR: {
            BinaryExpr* binaryExprNode = static_cast<BinaryExpr*>(node);
            printf("|Binary Expression : %s\n",binaryExprNode->opt.token.c_str());
            printNode(binaryExprNode->value1);
            printNode(binaryExprNode->value2);
            break;
        }
        case FOR_LOOP:{
            ForLoop* forLoopNode = static_cast<ForLoop*>(node);
            printf("|For Loop: %s over ",forLoopNode->iter_name.c_str());
            printNode(forLoopNode->condition);
        }
        case WHILE_LOOP:{
            WhileLoop* whileLoopNode = static_cast<WhileLoop*>(node);
            printf("|While Loop for ");
            printNode(whileLoopNode->condition);
        }
        case FUNCTION_CALL:{
            FunctionCall* functionCallNode = static_cast<FunctionCall*>(node);
            printf("Function Call : Name : %s \n",functionCallNode->name.c_str());
            if(functionCallNode->param.size() > 0)
                for(int paramCount = 1; paramCount <= functionCallNode->param.size();paramCount++){
                    printf("|Argument %d: \n\t",paramCount);
                    printNode(functionCallNode->param[paramCount-1]);
                    printf("\n");
                }
            else
                ignore;
        }
    }
}

/// @brief 
/// @param node 
/// @return 
//Another visitor function to visit the AST nodes but this time it will also generate IR codes
string generateCode(Node* node) {
    switch (node->type) {
        case VALUE_TYPE: {
            Value* valueNode = static_cast<Value*>(node);
            return valueNode->value.token; // return the value as C++ code
        }
        case VAR_DECLARATION: {
            VarDecl* varDeclNode = static_cast<VarDecl*>(node);
            return  "VAR " + varDeclNode->name + " = " + generateCode(varDeclNode->value) + ";";
        }
        case VAR_ASSIGNMENT: {
            VarAssign* varAssignNode = static_cast<VarAssign*>(node);
            return varAssignNode->name + " = " + generateCode(varAssignNode->value) + ";";
        }
        case BINARY_EXPR: {
            BinaryExpr* binaryExprNode = static_cast<BinaryExpr*>(node);
            string left = generateCode(binaryExprNode->value1);
            string right = generateCode(binaryExprNode->value2);
            return "(" + left + " " + binaryExprNode->opt.token + " " + right + ")";
        }
        case FOR_LOOP:{
            ForLoop* forLoopNode = static_cast<ForLoop*>(node);
            Block* bodyNode = static_cast<Block*>(forLoopNode->body);
            string condition = generateCode(forLoopNode->condition);
            string bodyCode = "";
            for (int i = 0; i < bodyNode->statements.size(); i++) {
                bodyCode += generateCode(bodyNode->statements[i]);
                bodyCode += "\n";
            }
            return "FOR (VAR " + forLoopNode->iter_name + " : " + condition + ") {\n"+ bodyCode + "}";
        }

        case WHILE_LOOP:{
            WhileLoop* whileLoopNode = static_cast<WhileLoop*>(node);
            string condition = generateCode(whileLoopNode->condition);
            Block* bodyNode = static_cast<Block*>(whileLoopNode->body);
            string bodyCode = "";
            for (int i = 0; i < bodyNode->statements.size(); i++) {
                bodyCode += generateCode(bodyNode->statements[i]);
                bodyCode += "\n";
            }
            return "WHILE (" + condition + ") {\n" + bodyCode + "}";
        }
        case IF_STATEMENT:{
            IfStmt* ifNode = static_cast<IfStmt*>(node);
            string condition = generateCode(ifNode->condition);
            Block* bodyNode = static_cast<Block*>(ifNode->body);
            string bodyCode = "";
            for (int i = 0; i < bodyNode->statements.size(); i++) {
                bodyCode += generateCode(bodyNode->statements[i]);
                bodyCode += "\n";
            }
            return "IF (" + condition + ") {\n" + bodyCode + "}\n";
        }
        case ELIF_STATEMENT:{
            ElifStmt* elifNode = static_cast<ElifStmt*>(node);
            string condition = generateCode(elifNode->condition);
            Block* bodyNode = static_cast<Block*>(elifNode->body);
            string bodyCode = "";
            for (int i = 0; i < bodyNode->statements.size(); i++) {
                bodyCode += generateCode(bodyNode->statements[i]);
                bodyCode += "\n";
            }
            return "ELIF (" + condition + ") {\n" + bodyCode + "}\n";
        }
        case ELSE_STATEMENT:{
            ElseStmt* elseNode = static_cast<ElseStmt*>(node);
            Block* bodyNode = static_cast<Block*>(elseNode->body);
            string bodyCode = "";
            for (int i = 0; i < bodyNode->statements.size(); i++) {
                bodyCode += generateCode(bodyNode->statements[i]);
                bodyCode += "\n";
            }
            return "ELSE{" + bodyCode + "}\n";
        }
        case FUNCTION_CALL:{
            FunctionCall* functionCallNode = static_cast<FunctionCall*>(node);
            string name = functionCallNode->name;
            string params = "";
            for (int i = 0; i < functionCallNode->param.size(); i++) {
                params += generateCode(functionCallNode->param[i]);
                if (i != functionCallNode->param.size() - 1) {
                    params += ", ";
                }
            }
            return name + "(" + params + ")";
        }
        case BLOCK:{
            Block* blockNode = static_cast<Block*>(node);
            string code = "{\n";
            for (int i = 0; i < blockNode->statements.size(); i++) {
                code += generateCode(blockNode->statements[i]);
                code += "\n";
            }
            code += "}";
            return code;
        }
        case FUNCTION_DECL:{
            FunctionDecl* funNode = static_cast<FunctionDecl*>(node);
            string params;
            for(VarDecl* var : funNode->param)
                params += var->name + ",";
            return "DEF " + funNode->name + " (" + params + ") {\n" + generateCode(funNode->body) + "}\n";
        }
        default: {
            return ""; // return empty string for unknown node types
        }
    }
}

#endif // AST_Csq4_H
