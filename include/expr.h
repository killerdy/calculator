#pragma once
#include <lexer.h>
#include <parser_node.h>
#include <string>
#include <vector>
namespace rst
{
    class Context;
    class Function;
    class BinaryNode : public AstNode
    {
    public:
        BinaryNode(TokenType _op, AstNodePtr _lhs, AstNodePtr _rhs)
            : op(_op), lhs(std::move(_lhs)), rhs(std::move(_rhs)) {}
        std::string to_string() const override;
        ObjPtr eval() override;

    private:
        TokenType op;
        AstNodePtr lhs;
        AstNodePtr rhs;
    };
    class Literal : public AstNode
    {
    public:
        Literal(Token _tok);
        Literal(ObjPtr _val) : val(_val) {}
        std::string to_string() const override;
        ObjPtr eval() override;

    private:
        ObjPtr val;
    };
    AstNodePtr parse_expr(Context &con);
    AstNodePtr parse_normal_binary(Context &con,int pred);
}