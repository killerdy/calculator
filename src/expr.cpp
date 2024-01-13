#include <expr.h>
#include <utils.h>
#include <vector>
#include <memory>
#include <context.h>
#include<object.h>
#include <map>
namespace rst
{
    AstNodePtr parse_unit(Context &con)
    {
        auto cur_tok = con.this_token();
        switch (cur_tok.get_tag())
        {
        case TokenType::LPAR:
        {
            con.match(TokenType::LPAR);
            auto ret = parse_expr(con);
            con.match(TokenType::RPAR);
            return ret;
        }
        case TokenType::L_I64:
        {
            con.advance();
            return std::make_unique<Literal>(cur_tok);
        }
        default:
            rst_error(SYNTAX_ERROR, "unsupport unit");
        }
    }
    AstNodePtr parse_expr(Context &con)
    {
        return parse_normal_binary(con, 15);
    }
    AstNodePtr parse_normal_binary(Context &con, int ppred)
    {
        if (ppred == 1)
            return parse_unit(con);
        static std::map<TokenType, std::pair<int, bool>> pred_tab = {
            {TokenType::ADD, {4, true}},
            {TokenType::SUB, {4, true}},
            {TokenType::MUL, {3, true}},
            {TokenType::DIV, {3, true}},
        };
        auto get_pred = [&](TokenType tag) -> std::pair<int, bool>
        {
            auto it = pred_tab.find(tag);
            if (it == pred_tab.end())
                return {100, false};
            return it->second;
        };
        auto left = parse_normal_binary(con, ppred - 1);
        int is_l2r = -1;
        std::vector<AstNodePtr> nodes;
        nodes.push_back(std::move(left));
        std::vector<TokenType> ops;
        while (!con.get_scanner()->is_terminal() && get_pred(con.cur_tag()).first == ppred)
        {
            if (is_l2r == -1)
                is_l2r = get_pred(con.cur_tag()).second;
            auto op = con.cur_tag();
            con.advance();
            ops.push_back(op);
            auto right = parse_normal_binary(con, ppred - 1);
            nodes.emplace_back(std::move(right));
        }
        if (nodes.size() == 1)
            return std::move(nodes.front());
        if (is_l2r)
        {
            AstNodePtr root = std::move(nodes.front());
            for (int i = 1; i < nodes.size(); i++)
            {
                root = std::make_unique<BinaryNode>(ops[i - 1], std::move(root), std::move(nodes[i]));
                // ops.pop_back();
            }
            return root;
        }
        else
        {
            AstNodePtr root = std::move(nodes.back());
            for (int i = 0; i < nodes.size() - 1; i++)
            {
                root = std::make_unique<BinaryNode>(ops.back(), std::move(nodes[i]), std::move(root));
                ops.pop_back();
            }
            return root;
        }
    }
    Literal::Literal(Token tok)
    {
        if (tok.get_tag() == TokenType::L_I64)
            val=std::make_shared<Integer>(tok.get_value<int64_t>());
        else
            rst_error(SYNTAX_ERROR, "unknown literal" + tok.to_string());
    }
}