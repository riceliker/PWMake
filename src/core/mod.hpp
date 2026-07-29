#pragma once
#include <cstddef>
#include <filesystem>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>
#include <stack>

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
        std::vector<std::filesystem::path> source_files;
    };

    // utils.cpp
    std::tuple<std::string, std::unique_ptr<std::vector<std::string>>> Function(std::string text);
    std::vector<std::string> CatchGroup(std::vector<std::string> line, int pc);
    std::string GetString(std::unordered_map<std::string, std::string> map, std::string input, std::vector<std::string> lines, int pc);
    bool GetBool(std::unordered_map<std::string, bool> map, std::string input,  std::vector<std::string> lines, int pc);
    std::string GetStringLite(std::string input);
    void CheckParam(std::unique_ptr<std::vector<std::string>>& params, int count, std::vector<std::string> line, int pc);
    
    // file.cpp
    std::vector<std::string> GetTextLinesInFile(std::string path);
    void CreateFileInPath(std::vector<std::string> move_text_line, std::string folder_path);
    std::vector<std::filesystem::path> SearchFileInFolder(std::string folder_path, std::string extension);
    std::vector<std::filesystem::path> RecursionFileInFolder(std::string folder_path, std::string extension);
   

    // lexer.cpp
    struct IfFrame 
    {
        int cond_jump;
        int exit_jump;
        bool met_else;
    };

    class Lexer
    {
    private:
        void DivideGroup(std::vector<std::string> line, int pc);
        void RegistryVariable(std::vector<std::string> line, int pc);
        void ForkControl(std::unique_ptr<std::stack<bool>>& if_stack, std::vector<std::string> lines, int pc);
        void CreateFiles(std::vector<std::string> group, std::vector<std::string> lines, int pc);
        std::unordered_map<std::string, bool> bool_variable;
        std::unordered_map<std::string, std::string> string_variable;
        std::unordered_map<std::string, std::vector<std::filesystem::path>> files_variable;
    public:
        Lexer(std::vector<std::string> lines);

        CompilerInfo compiler;
        std::vector<ProjectInfo> project;
    };
    

    // transform.cpp
    CompilerInfo CompilerGroup(std::vector<std::string> group, std::vector<std::string> lines, int pc);
    ProjectInfo ProjectGroup(std::vector<std::string> group, const std::unordered_map<std::string, std::vector<std::filesystem::path>>& dictionary, std::vector<std::string> lines, int pc);
    
    // ninja.cpp
    class NinjaFile
    {
    private:
        std::vector<std::string> data;
    public:
        NinjaFile();
        std::vector<std::string> AsFile();
        void AddCompiler(CompilerInfo info);
        void AddSource(ProjectInfo info);
    };

    
}