#include <lexer.h>
#include <utils.h>

int main()
{
    // using namespace std;
    // auto code = rst::read_file("../test.rst");
    // if (!code)
    //     rst::rst_error(RUNTIME_ERROR, "read file failed!");
    // // using namespace rst;
    // Scanner("dy", *code).print_tokens();
    // // auto tokens=Scanner("dy",*code).this_token();

    std::string s;
    while (std::getline(std::cin, s))
    {
        rst::Scanner("dy",s).print_tokens();
        std::cout<<std::endl;
    }
}