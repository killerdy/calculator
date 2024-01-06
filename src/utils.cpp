#include <fstream>
#include <utils.h>
namespace rst
{
    void rst_error(ErrorType error_type, const std::string &msg)
    {
        switch (error_type)
        {
        case FATAL_ERROR:
        case SYNTAX_ERROR:
        case OPTION_ERROR:
            std::cout << "error\n"
                      << msg << std::endl;
            exit(1);
        default:
            std::cout << "unknown error\n"
                      << std::endl;
            exit(1);
        }
    }
    std::optional<std::string> read_file(const std::string &path)
    {
        using namespace std;
        ifstream ifs(path);
        if (!ifs)
        {
            return std::nullopt;
        }
        std::string file_content((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
        return file_content;
    }
    void debug_print(const std::string &msg) { std::cout << msg << "\n"; }
}
