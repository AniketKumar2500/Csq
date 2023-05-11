#if !defined(AST_Csq4_H)
#define AST_Csq4_H

//Imports
#include "../Grammar/grammar.h"
#include "../Tokenizer/tokenizer.h"
#include "../Memory/stack.h"
#include <memory>
using namespace std;

//Node types
typedef enum {
    VAR_DECLARATION,
    VAR_ASSIGNMENT,
    CLASS_DEFINITION,
    FOR_LOOP,
    WHILE_LOOP,
    EXPR_TYPE,
    FUNCTION_DECL,
    BLOCK,
    IF_STATEMENT,
    ELIF_STATEMENT,
    ELSE_STATEMENT,
    FUN_ARGUMENT,
    BREAK,
    GROUP,
    ONE_LINER,
    RETURN_STMT,
    IMPORT,
    PROGRAM, //The basic node type is Program which doesn't needs to be traversed.
} NODE_TYPE;

//Node struct
struct Node;
//body struct for node type
struct Node{
    NODE_TYPE type;
    Node(NODE_TYPE nodetype){
        type = nodetype;
    }
    Node(){}
};

//Declaration for Node types.
struct Expr;
struct VarDecl;
struct VarAssign;
struct Block;
struct FunctionDecl;
struct ClassDecl;
struct ForLoop;
struct WhileLoop;
struct IfStmt;
struct ElifStmt;
struct ElseStmt;
struct FunArg;
struct Break;
struct Group;
struct Import;
struct OneLiner;
struct ReturnStmt;
// Definitions for above Node types
//Expr node type is the combination of different types of statements like
//*Function call
//*Value
//*Binary and Unary expressions
struct Expr : Node{
    string expr;
    Expr(){type = EXPR_TYPE; expr = "";}
    Expr(string express){type = EXPR_TYPE; expr = express;}
};

struct VarDecl : Node{
    string name;
    Expr value;
    bool type_infr;
    string type_;
    VarDecl(string name_, string value_, string type_ = ""){
        name = name_;
        value = value_;
        type = VAR_DECLARATION;
        if(type_ == ""){
            type_infr = true;
        }
        else{
            type_infr = false;
        }
    }
    VarDecl(){
        name = "";
        value = Expr();
        type = VAR_DECLARATION;

    }
};

struct ReturnStmt : Node{
    Expr expr;
    ReturnStmt(){
        type = RETURN_STMT;
    }
    ReturnStmt(Expr exp){
        type = RETURN_STMT;
        expr = exp;
    }
};

struct Import : Node{
    string path;
    string alias;
    string code;
    Import(){
        type = IMPORT;
    }
    Import(string path_, string code_, string alias_ = ""){
        code = code_;
        path = path_;
        alias = alias_;
        type = IMPORT;
    }   
};

struct Break : Node{
    Break(){
        type = BREAK;
    }
};

struct VarAssign : Node{
    string name;
    Expr value;
    VarAssign(string name_, string value_){
        name = name_;
        value = value_;
        type = VAR_ASSIGNMENT;
    }
    VarAssign(){
        name = "";
        value = Expr();
        type = VAR_ASSIGNMENT;
    }
};

struct Block : Node{
    vector<string> statements;
    Block(){type = BLOCK;}
    Block(vector<string> statement){statements = statement;type = BLOCK;}
};

struct FunctionDecl : Node{
    string name;
    string return_type;
    bool return_type_infr;
    vector<VarDecl> params = {};
    Block body;
    FunctionDecl(){
        type = FUNCTION_DECL;
        name = "";
    }
    FunctionDecl(string name_, vector<VarDecl> params_, string return_type_=""){
        type = FUNCTION_DECL;
        name = name_;
        params = params_;
        if(return_type_ == ""){
            return_type = "";
            return_type_infr = 1;
        }
        else{
            return_type = return_type_;
            return_type_infr = 0;
        }
    }
};

struct ForLoop : Node{
    Expr condition;
    string iter_name;
    Block body;
    ForLoop(){type = FOR_LOOP;}
    ForLoop(string itername, Expr cond){type = FOR_LOOP;iter_name = itername;condition = cond;}
};

struct WhileLoop : Node{
    Expr condition;
    Block body;
    WhileLoop(){type = WHILE_LOOP;}
    WhileLoop(Expr cond){type = WHILE_LOOP;condition = cond;}
};

struct IfStmt : Node{
    Expr condition;
    Block body;
    IfStmt(){type = IF_STATEMENT;}
    IfStmt(Expr cond){type = IF_STATEMENT;condition = cond;}
};

struct ElifStmt : Node{
    Expr condition;
    Block body;
    ElifStmt(){type = ELIF_STATEMENT;}
    ElifStmt(Expr cond){type = ELIF_STATEMENT;condition = cond;}
};

struct ElseStmt : Node{
    Block body;
    ElseStmt(){type = ELSE_STATEMENT;}
};

struct ClassDecl : Node{
    string name;
    string inherit_class;
    ClassDecl(){
        type = CLASS_DEFINITION;
    }
    ClassDecl(string name_, string inherit_class_){
        name = name_;
        inherit_class = inherit_class_;
        type = CLASS_DEFINITION;
    }
};

struct Group : Node{
    string name;
    Group(){type = GROUP;}
    Group(string name_){
        name = name_;
        type = GROUP;
    }
};

struct OneLiner : Node{
    string code;
    vector<vector<Token>> lines;
    OneLiner(){type = ONE_LINER;}      
    OneLiner(string code_){
        type = ONE_LINER;
        code = code_;
    }      
};

//Importing visitor containing all utilities to visit AST.
#include "visitor.h"

#endif // AST_Csq4_H
