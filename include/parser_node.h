#pragma once
#include <string>
#include <memory>
#include <utils.h>
namespace rst
{
    enum class NodeType : int
    {
        UNKOWN,
        BINOP
    };
    struct Object;
    using ObjPtr = std::shared_ptr<Object>;
    class AstNode
    {
    public:
        virtual ~AstNode();
        AstNode() {}
        AstNode(NodeType nt) : node_type(nt) {}

    public:
        NodeType get_node_type() const { return node_type; }
        virtual std::string to_string() const { return "<empty node"; }
        virtual ObjPtr eval()
        {
            rst_error(RUNTIME_ERROR, "internal error!");
            return nullptr;
        }

    private:
        NodeType node_type;
    };
    using AstNodePtr = std::unique_ptr<AstNode>;
}