#pragma once
#include <map>
#include <vector>
#include <string>
#include <variant>
namespace rst
{

    enum class TokenType : int
    {
        END_TOKEN,
    };
    extern std::map<std::string, TokenType> keywords;

    class Token
    {
    public:
        using ValueType =
            std::variant<std::monostate, std::string, int64_t, uint64_t, double>;
        Token(const ValueType &val, TokenType _tag) : value(val), tag(_tag) {}
        Token(TokenType _tag) : tag(_tag) {}
        // template<typename T> T get_value() const{return std::get<T>(value);}
        // TokenType get_tag() const{return tag;}
        // std::string to_string() const;

    private:
        ValueType value;
        TokenType tag;
    };
    class Scanner
    {
    public:
        Scanner() : filename("noname"), content("") {}
        Scanner(const std::string &_filename, const std::string &_content)
            : filename(_filename), content(_content){
            scan_once();
        }
        void scan_once();
        void scan();
        void warning(const std::string &msg);
        void error(const std::string &err);
        void new_content(const std::string &n_content){
            content+=n_content;
            scan();
        }
    public:
        
    private:
        std::string filename;
        std::string content;
        size_t content_pos = 0;
        bool scan_fin = false;
        int cur_line = 1;
        std::vector<Token> tokens;
        int pos = 0;
    };
}