
#include <string>
#include <vector>

namespace PWMake 
{
    std::vector<std::string> GetTextLinesInFile(std::string path);
    void CreateFileInPath(const std::vector<std::string>* const move_text_line, std::string folder_path);
    std::vector<std::string> SearchFileNameInFolder(std::string folder_path, std::string extension);

    struct CompilerInfo
    {
        std::string compiler_name;
        std::string compiler_path;
        std::string language_version;
        std::string standard_library;
    };

    struct PWMListInfo
    {

    };

    class PWMFunc
    {
    public:
        std::string name;
        std::vector<std::string> param;
        PWMFunc(std::string text, int line);
    };

    class Lexer
    {
    private:
        std::vector<std::vector<std::string>> DivideGroup(std::vector<std::string> lines);
    public:
        std::vector<CompilerInfo> compiler;
        Lexer(std::vector<std::string> lines);
        
    };

    class NinjaFile
    {
    public:
        std::vector<std::string>* ptr;
    
        NinjaFile();
        void AddCompiler(CompilerInfo info);
    };

    CompilerInfo CompilerGroup(std::string text, int line);
}