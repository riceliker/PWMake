#pragma once
#include "../Interface.hpp"

#include <cstddef>
#include <unordered_map>
#include <stack>
#include <vector>

namespace PWMake::Lexer 
{
    struct VariableMap
    {
        std::unordered_map<std::string, bool> bool_variable;
        std::unordered_map<std::string, std::string> string_variable;
        std::unordered_map<std::string, std::vector<std::filesystem::path>> files_variable;
    };

    enum ParamOptionType
    {
        Bool,
        String
    };

    struct ParamOption
    {
        ParamOptionType type;
        std::string str;
        bool boolean;
    };

    struct GroupFunction
    {
        std::string name;
        std::vector<ParamOption> params;
        std::string line_text;
        size_t lines;
    };

    enum GroupType
    {
        Global,
        Files,
        Compiler,
        Project
    };
    
    class Lexer
    {
    private:
        void SendGroup(VariableMap& map, std::vector<GroupFunction> group, GroupType status, std::string text, int pc);
        void RegistryVariable(std::string text, int pc);
        void ForkControl(VariableMap& map, std::unique_ptr<std::stack<bool>>& if_stack, std::string text, int pc);
        
    public:
        Lexer(std::vector<std::string> lines, VariableMap variable);

        VariableMap variable;
        CompilerInfo compiler;
        std::vector<ProjectInfo> project;
    };  

    GroupFunction AnalyzeFunction(VariableMap& map, std::string text, size_t line);

    ParamOption FindVariable(VariableMap& map, std::string var, std::string text, int line);

    void CreateFiles(std::vector<GroupFunction> funcs, VariableMap& map, std::string text, int pc);
    CompilerInfo CompilerGroup(std::vector<GroupFunction> funcs, std::string text, int pc);
    ProjectInfo ProjectGroup(std::vector<GroupFunction> funcs, VariableMap& map, std::string text, int pc);
    
    inline std::string ParamGetString(ParamOption opt, int index, std::string text, int pc)
    {
        if (opt.type != String)
        {
            std::printf("\033[31m" "Error:" "\033[0m" " This Function Param In %d need String Type!\n", index);
            std::printf("In build.pwm:%d\n", pc+1);
            std::printf("%d | %s\n", pc+1, text.c_str());
            std::exit(1);
        }
        return opt.str;
    }

    inline bool ParamGetBool(ParamOption opt, int index, std::string text, int pc)
    {
        if (opt.type != Bool)
        {
            std::printf("\033[31m" "Error:" "\033[0m" " This Function Param In %d need Boolean Type!\n", index);
            std::printf("In build.pwm:%d\n", pc+1);
            std::printf("%d | %s\n", pc+1, text.c_str());
            std::exit(1);
        }
        return opt.boolean;
    }
        
    static inline void ParamCountIsWant(int need, int real, std::string text, int pc)
    {
        if (need != real)
        {
            std::printf("\033[31m" "Error:" "\033[0m" " This Function Need %d, Not %d!\n", need, real);
            std::printf("In build.pwm:%d\n", pc+1);
            std::printf("%d | %s\n", pc+1, text.c_str());
            std::exit(1);
        }
    }

    /* Remove the block in the from of text*/
    inline std::string RemoveBlock(std::string text)
    {
        size_t start = 0;
        while (start < text.size() && std::isspace(static_cast<unsigned char>(text[start])))
            start++;
        text.erase(0, start);
        return text;
    }
}