#include <iostream>
#include <lexer.h>
#include <vector>
namespace rst
{
    namespace
    {
        int oct_digit(char ch)
        {
            if ('0' <= ch && '7' >= ch)
                return ch - '0';
            return -1;
        }
        int hex_digit(char ch)
        {
            if (isdigit(ch))
                return ch - '0';
            if ('a' <= tolower(ch) && 'f' >= tolower(ch))
                return 10 + tolower(ch) - 'a';
            return -1;
        }
        inline double parse_float(int base, double val, const std::string &str,
                                  size_t &pos, Scanner &scanner)
        {
            if (str[pos] == '.')
            {
                pos++;
                double tmp = 1.0;
                uint64_t float_part = 0;
                uint64_t exp = 1;
                while (pos < str.size())
                {
                    int dig = hex_digit(str[pos]);
                    if (dig == -1 || dig >= base)
                        break;
                    exp *= base;
                    if (float_part < 1e16)
                        float_part = float_part * base + dig;
                    pos++;
                }
                val += (double)float_part / exp;
            }
            if (pos < str.size())
            {
                int exp = 0;
                bool sign_flag = false;
                double idx = 10;
                if (tolower(str[pos] == 'e'))
                {
                    pos++;
                    if (pos < str.size() && (str[pos] == '-' || str[pos] == '+'))
                        sign_flag = str[pos] == '-', pos++;
                    while (pos < str.size() && isdigit(str[pos]))
                        exp = exp * 10 + str[pos] - '0', pos++;
                }
                else if (tolower(str[pos]) == 'p')
                {
                    pos++;
                    idx = 2;
                    if (pos < str.size() && (str[pos] == '-') || str[pos] == '+')
                        sign_flag = str[pos] == '-', pos++;
                    while (pos < str.size() && isdigit(str[pos]))
                        exp = exp * 10 + str[pos] - '0', pos++;
                }
                double ans = 1;
                while (exp)
                {
                    if (exp & 1)
                        ans *= idx;
                    idx *= idx;
                    exp /= 2;
                }
                return sign_flag ? val / ans : val * ans;
            }
            //
            return val;
        }
    }
    std::map<std::string, TokenType> keywords = {
        // clang-format off
    {"eval", TokenType::EVAL},{"break", TokenType::BREAK},{"case", TokenType::CASE},
    {"char", TokenType::CHAR},{"const", TokenType::CONST},{"continue", TokenType::CONTINUE},
    {"default", TokenType::DEFAULT},{"do", TokenType::DO},{"double", TokenType::DOUBLE},
    {"else", TokenType::ELSE},{"enum", TokenType::ENUM},{"extern", TokenType::EXTERN},
    {"float", TokenType::FLOAT},{"for", TokenType::FOR},{"goto", TokenType::GOTO},
    {"if", TokenType::IF},{"inline", TokenType::INLINE},{"int", TokenType::INT},
    {"long", TokenType::LONG},{"register", TokenType::REGISTER},{"restrict", TokenType::RESTRICT},
    {"return", TokenType::RETURN},{"short", TokenType::SHORT},{"signed", TokenType::SIGNED},
    {"sizeof", TokenType::SIZEOF},{"static", TokenType::STATIC},{"struct", TokenType::STRUCT},
    {"switch", TokenType::SWITCH},{"typedef", TokenType::TYPEDEF},{"union", TokenType::UNION},
    {"unsigned", TokenType::UNSIGNED},{"void", TokenType::VOID},{"volatile", TokenType::VOLATILE},
    {"while", TokenType::WHILE},{"_Bool", TokenType::_BOOL},{"_Complex", TokenType::_COMPLEX},
    {"_Imaginary", TokenType::_IMAGINARY},
    {"{", TokenType::BEGIN}, {"}", TokenType::END},{"(", TokenType::LPAR}, {")", TokenType::RPAR}, {",", TokenType::COMMA},
    {"[",TokenType::LSB},{"]",TokenType::RSB},
    {";",TokenType::SEMI},{":",TokenType::COLON},{"=",TokenType::ASSIGN},
    {"+",TokenType::ADD},{"-",TokenType::SUB},{"*",TokenType::MUL},{"/",TokenType::DIV},{"%",TokenType::MOD},
    {"!",TokenType::NOT},{"~",TokenType::BIT_NOT},{"&",TokenType::BIT_AND},{"|",TokenType::BIT_OR},{"^",TokenType::BIT_XOR},
    {"+=",TokenType::SADD},{"-=",TokenType::SSUB},{"/=",TokenType::SDIV},{"*=",TokenType::SMUL},{"%=",TokenType::SMOD},
    {">>=",TokenType::SRSH},{"<<=",TokenType::SLSH},{"^=",TokenType::SOR},{"&=",TokenType::SAND},{"|=",TokenType::SOR},
    {"++",TokenType::INC},{"--",TokenType::DEC},
    {"<<",TokenType::LSH},{">>",TokenType::RSH},{"<",TokenType::LT},{">",TokenType::GT},{"<=",TokenType::LE},{">=",TokenType::GE},
    {"==",TokenType::EQ},{"!=",TokenType::NE},
    {"&&",TokenType::AND},{"||",TokenType::OR}

};
// replace rex (copy content from keywords and transfer): \{"(.+)",TokenType::([A-Z_]+)\} => {TokenType::$2,"$1"}
    std::map<TokenType, std::string> rkeywords={
    {TokenType::EVAL,"eval"},{TokenType::BREAK,"break"},{TokenType::CASE,"case"},{TokenType::END_LINE,"\n"},
    {TokenType::CHAR,"char"},{TokenType::CONST,"const"},{TokenType::CONTINUE,"continue"},
    {TokenType::DEFAULT,"default"},{TokenType::DO,"do"},{TokenType::DOUBLE,"double"},
    {TokenType::ELSE,"else"},{TokenType::ENUM,"enum"},{TokenType::EXTERN,"extern"},
    {TokenType::FLOAT,"float"},{TokenType::FOR,"for"},{TokenType::GOTO,"goto"},
    {TokenType::IF,"if"},{TokenType::INLINE,"inline"},{TokenType::INT,"int"},
    {TokenType::LONG,"long"},{TokenType::REGISTER,"register"},{TokenType::RESTRICT,"restrict"},
    {TokenType::RETURN,"return"},{TokenType::SHORT,"short"},{TokenType::SIGNED,"signed"},
    {TokenType::SIZEOF,"sizeof"},{TokenType::STATIC,"static"},{TokenType::STRUCT,"struct"},
    {TokenType::SWITCH,"switch"},{TokenType::TYPEDEF,"typedef"},{TokenType::UNION,"union"},
    {TokenType::UNSIGNED,"unsigned"},{TokenType::VOID,"void"},{TokenType::VOLATILE,"volatile"},
    {TokenType::WHILE,"while"},{TokenType::_BOOL,"_Bool"},{TokenType::_COMPLEX,"_Complex"},
    {TokenType::_IMAGINARY,"_Imaginary"},
    {TokenType::BEGIN,"{"}, { TokenType::END,"}"},{TokenType::LPAR,"("}, {TokenType::RPAR,")"}, {TokenType::COMMA,","},
    {TokenType::LSB,"["},{TokenType::RSB,"]"},
    {TokenType::SEMI,";"},{TokenType::COLON,":"},{TokenType::ASSIGN,"="},
    {TokenType::ADD,"+"},{TokenType::SUB,"-"},{TokenType::MUL,"*"},{TokenType::DIV,"/"},{TokenType::MOD,"%"},
    {TokenType::NOT,"!"},{TokenType::BIT_NOT,"~"},{TokenType::BIT_AND,"&"},{TokenType::BIT_OR,"|"},{TokenType::BIT_XOR,"^"},
    {TokenType::SADD,"+="},{TokenType::SSUB,"-="},{TokenType::SDIV,"/="},{TokenType::SMUL,"*="},{TokenType::SMOD,"%="},
    {TokenType::SRSH,">>="},{TokenType::SLSH,"<<="},{TokenType::SOR,"^="},{TokenType::SAND,"&="},{TokenType::SOR,"|="},
    {TokenType::INC,"++"},{TokenType::DEC,"--"},
    {TokenType::LSH,"<<"},{TokenType::RSH,">>"},{TokenType::LT,"<"},{TokenType::GT,">",},{TokenType::LE,"<="},{TokenType::GE,">="},
    {TokenType::EQ,"=="},{TokenType::NE,"!="},
    {TokenType::AND,"&&"},{TokenType::OR,"||"}
        // clang-format on
    };
    void Scanner::scan_once()
    {
        scan();
        tokens.push_back(TokenType::END_TOKEN);
        scan_fin = true;
        cur_line = 1;
    }

    void Scanner::scan()
    {
        size_t &i = content_pos;
        for (; i < content.size(); i++)
        {
            switch (content[i])
            {
            case ' ':
            case '\r':
                continue;
            case '#':
                while (i + 1 < content.size() && content[i + 1] != '\n')
                    i++;
                break;
            case '\n':
                new_line();
                break;
            case '+':
            case '-':
            case '*':
            case '/':
            case '(':
            case ')':
            case '=':
            case ',':
            case ';':
                tokens.push_back(keywords[std::string(1, content[i])]);
                continue;
            default:
                if (content[i] == '_' || isalpha(content[i]))
                    tokens.push_back(parse_word(content, i));
                else if (isdigit(content[i]))
                {
                    tokens.push_back(parse_number(content, i));
                    i--;
                }
                else
                    error("unexpected character " + std::string(1, content[i]));
                break;
            }
        }
    }

    void Scanner::error(const std::string &err)
    {
        std::cout << filename << ":" << cur_line << ": \033[31merror: \033[0m"
                  << err << "\n";
        exit(1);
    }
    Token Scanner::parse_word(const std::string &content, size_t &i)
    {
        std::string symbol;
        while (i < content.size() && (content[i] == '_' || isalnum(content[i])))
            symbol += content[i], i++;
        i--;
        auto it = keywords.find(symbol);
        if (it == keywords.end())
            return Token(symbol, TokenType::SYMBOL);
        return Token({}, it->second);
    }
    Token Scanner::parse_number(const std::string &str, size_t &pos)
    {
        unsigned long long ret = 0;
        int base = 10;
        int is_uint = 0;
        if (pos < str.size() && str[pos] == '0')
        {
            pos++;
            if (pos < str.size() && tolower(str[pos]) == 'x')
            {
                base = 16, pos++;
            }
            else if (str[pos] != '.')
                base = 8;
        }
        for (; pos < str.size(); pos++)
        {
            if (isdigit(str[pos]))
            {
                ret = ret * base + (str[pos] - 48);
            }
            else
            {
                if (isalpha(str[pos]))
                {
                    if (tolower(str[pos]) == 'u')
                        is_uint++;
                    else if (tolower(str[pos] == 'l'))
                        continue;
                    else if (base == 16 && tolower(str[pos] <= 'f'))
                        ret = ret * base + (tolower(str[pos]) - 'a' + 10);
                    else
                        break;
                }
                else
                    break;
            }
        }
        if (str[pos] == '.' || tolower(str[pos] == 'e') || tolower(str[pos]) == 'p')
        {
            double val = 0;
            val = parse_float(base, ret, str, pos, *this);
            if (pos < str.size() && (tolower(str[pos]) == 'f' || tolower(str[pos]) == 'l'))
                pos++;
            return Token(val, TokenType::L_DOUBLE);
        }
        if (is_uint)
            return Token((uint64_t)ret, TokenType::L_U64);
        else
            return Token((int64_t)ret, TokenType::L_I64);
    }
    Token Scanner::this_token()
    {
        while (pos < tokens.size() &&
               tokens[pos].get_tag() == TokenType::END_LINE)
        {
            pos++;
        }
        if (pos == tokens.size())
            error("to the end of token_stream");
        return tokens[pos];
    }
    TokenType Scanner::cur_tag() { return this_token().get_tag(); }
    void Scanner::advance()
    {
        pos++;
        if (pos >= tokens.size())
        {
            tokens.push_back(TokenType::END_TOKEN);
            error("end");
        }
    }
    std::string Token::to_string() const
    {
        switch (tag)
        {
        case TokenType::SYMBOL:
        case TokenType::L_STR:
            return get_value<std::string>();
        case TokenType::L_DOUBLE:
            return std::to_string(get_value<double>());
        case TokenType::L_I64:
            return std::to_string(get_value<int64_t>());
        default:
            return rkeywords[tag];
        }
    }
    void Scanner::print_tokens(){
        // std::cout<<"ok"<<std::endl;
        // std::cout<<int(TokenType::L_CHAR);
        for(auto it:tokens)
            printf("<%s,%d> ",it.to_string().c_str(),int(it.get_tag()));
    }
}