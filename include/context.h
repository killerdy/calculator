#pragma once
#include <lexer.h>
#include <map>
#include <memory>
#include <optional>
#include <utils.h>
#include <parser_node.h>
namespace rst
{
    class Context
    {
    public:
        Context(const std::string &_filename, const std::string &_content)
            : filename(_filename), content(_content) {}

    public:
        void init_scanner();
        void new_content(const std::string &str);
        Scanner *get_scanner() { return scanner.get(); }
        Token this_token() { return scanner->this_token(); }
        void match(TokenType tag) { scanner->match(tag); }
        void advance(){scanner->advance();}
        TokenType cur_tag(){return scanner->cur_tag();}
        void parse_program();
        void execute();
    private:
        std::optional<std::string> filename = std::nullopt;
        std::optional<std::string> content = std::nullopt;
        std::unique_ptr<Scanner> scanner;
        std::vector<AstNodePtr> stmts;
    };
}