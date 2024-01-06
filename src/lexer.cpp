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
            }
            //
            return val;
        }
    }
    std::map<std::string, TokenType> keywords={
        
    };
    std::map<TokenType,std::string> rkeywords={
        
    };
    void Scanner::scan_once(){
        scan();
        tokens.push_back(TokenType::END_TOKEN);
        scan_fin=true;
        cur_line=1;
    }
    
    void Scanner::scan(){
        size_t &i=content_pos;
        
    }
    
    void Scanner::error(const std::string &err){
        
    }
    
}