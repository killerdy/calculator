#include<algorithm>
#include<context.h>
#include<utils.h>
namespace rst{
    AstNodePtr parse_expr(Context &con);
    void Context::init_scanner(){
        if(!filename)
        scanner=std::make_unique<Scanner>();
        else 
        scanner=std::make_unique<Scanner>(*filename,*content);
    }
    void Context::new_content(const std::string &str){
        if(!scanner)
        rst_error(FATAL_ERROR,"scanner not initialized");
        scanner->new_content(str);
    }
    void Context::parse_program(){
        while(cur_tag()!=TokenType::END_TOKEN){
            stmts.push_back(std::move(parse_expr(*this)));
        }
    }
    void Context::execute(){
    
    }
}