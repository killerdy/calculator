#pragma once
#include <map>
#include <vector>
#include <string>
#include <variant>
namespace rst
{

    enum class TokenType : int
    {
        // clang-format off
  L_I64,L_U64,L_STR,L_DOUBLE,L_CHAR,
  // keywords
  EVAL,BREAK,CASE,CHAR,
  CONST,CONTINUE,DEFAULT,DO,DOUBLE,
  ELSE,ENUM,EXTERN,FLOAT,FOR,
  GOTO,IF,INLINE,INT,LONG,
  REGISTER,RESTRICT,RETURN,SHORT,SIGNED,
  SIZEOF,STATIC,STRUCT,SWITCH,TYPEDEF,
  UNION,UNSIGNED,VOID,VOLATILE,WHILE,_BOOL,
  _COMPLEX,_IMAGINARY,
  // operate and marks
  END_LINE,BEGIN,END,LPAR,RPAR,LSB,RSB,
  COMMA,SEMI,COLON,
  ADD,SUB,MUL,DIV,MOD,INC,DEC,REM,
  ASSIGN,SADD,SSUB,SMUL,SDIV,SLSH,SRSH,SMOD,
  SAND,SOR,SXOR,

  BIT_OR,BIT_AND,BIT_XOR,BIT_NOT,
  AND,OR,NOT,LSH,RSH,
  // ?
  CONDITIONAL,
  EQ,NE,LT,GT,LE,GE,
  //. , ->
  POINT,VISIT,
  // symbols && empty && end file
  SYMBOL, EPT, END_TOKEN
        // clang-format on
    };
    extern std::map<std::string, TokenType> keywords;
    // std::string conv_escape(const std::string &content, size_t &pos);
    class Token
    {
    public:
        using ValueType =
            std::variant<std::monostate, std::string, int64_t, uint64_t, double>;
        Token(const ValueType &val, TokenType _tag) : value(val), tag(_tag) {}
        Token(TokenType _tag) : tag(_tag) {}
        template <typename T>
        T get_value() const { return std::get<T>(value); }
        TokenType get_tag() const { return tag; }
        std::string to_string() const;

    private:
        ValueType value;
        TokenType tag;
    };
    class Scanner
    {
    public:
        Scanner() : filename("noname"), content("") {}
        Scanner(const std::string &_filename, const std::string &_content)
            : filename(_filename), content(_content)
        {
            scan_once();
        }
        void print_tokens();
        void scan_once();
        void scan();
        void warning(const std::string &msg);
        void error(const std::string &err);
        void new_content(const std::string &n_content)
        {
            content += n_content;
            scan();
        }

    public:
        Token parse_number(const std::string &content, size_t &i);
        Token parse_word(const std::string &content, size_t &i);
        void new_line()
        {
            tokens.push_back(TokenType::END_TOKEN);
            cur_line++;
        }
        // char conv_escape(const std::string &content, size_t &i);
        TokenType cur_tag();
        void match(TokenType tag);
        void advance();
        Token this_token();
        int this_token_pos() const { return pos; }
        bool is_terminal() { return cur_tag() == TokenType::END_TOKEN; }
        const std::vector<Token> &get_tokens() const { return tokens; }

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