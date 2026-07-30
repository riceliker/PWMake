#pragma once
#include "../Interface.hpp"

#include <cstddef>
#include <unordered_map>
#include <stack>

namespace PWMake::Lexer 
{
    // lexer.cpp
    struct IfFrame 
    {
        int cond_jump;
        int exit_jump;
        bool met_else;
    };

    // transform.cpp
    CompilerInfo CompilerGroup(std::vector<std::string> group, std::vector<std::string> lines, int pc);
    ProjectInfo ProjectGroup(std::vector<std::string> group, const std::unordered_map<std::string, std::vector<std::filesystem::path>>& dictionary, std::vector<std::string> lines, int pc);
    

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
}