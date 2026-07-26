#pragma once
#include <string>
#include <unordered_map>
#include <vector>

namespace PWMake::Core 
{
    // Global API
    struct CompilerInfo
    {
        std::string compiler_path;
        std::string standard_language;
        std::string standard_library;
        bool is_warning;
        bool is_extra_warning;
        bool is_debug;
    };

    struct ProjectInfo
    {
        std::string project_name;
        std::string project_type;
        std::vector<std::string> project_header;
    };
    
    // file.cpp
    std::vector<std::string> GetTextLinesInFile(std::string path);
    void CreateFileInPath(std::vector<std::string> move_text_line, std::string folder_path);
    std::vector<std::string> SearchFileNameInFolder(std::string folder_path, std::string extension);

    // function.cpp
    std::tuple<std::string, std::vector<std::string>> Function(std::string text);

    // lexer.cpp
    class Lexer
    {
    private:
        void DivideGroup(std::vector<std::string> line, int pc);
        std::unordered_map<std::string, bool> bool_variable;
        std::unordered_map<std::string, std::string> string_variable;
        std::unordered_map<std::string, std::vector<std::string>> files_variable;
    public:
        Lexer(std::vector<std::string> lines);

        CompilerInfo compiler;
        
    };
    

    // transform.cpp
    CompilerInfo CompilerGroup(std::vector<std::string> group);
    
    // ninja.cpp
    class NinjaFile
    {
    private:
        std::vector<std::string> data;
    public:
        NinjaFile();
        std::vector<std::string> AsFile();
        void AddCompiler(CompilerInfo info);
    };

    
}