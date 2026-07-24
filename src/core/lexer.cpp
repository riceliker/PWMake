#include "mod.hpp"
#include <string>
#include <vector>

namespace PWMake 
{
    PWMFunc::PWMFunc(std::string text, int line)
    {
        int index = 0;
        bool is_record_name = true;
        bool is_record_param = false;
        int param_index = -1;
        for(const char& word: text)
        {
            // end function name
            if (word == '(')
                is_record_name = false;
            // record function parameter
            if (word == '"')
            {
                if (is_record_name == true)
                {
                    std::printf("\033[31m" "Error: The Error Function Name. In line: %d" "\033[0m" "\n", line);
                    std::printf("\033[31m" "%s" "\033[0m" "\n", text.c_str());
                    std::exit(1);
                }
                if (is_record_param == false)
                {
                    param_index += 1;
                    this->param.push_back("");
                }
                is_record_param = ! is_record_param;
            }    
            if (is_record_name)
                this->name += word;
            if (is_record_param && word != '"')
            {
                std::string old_temp = this->param[param_index];
                std::string new_temp = old_temp + word;
                this->param[param_index] = new_temp;
            }
                
            
            index += 1;
        }
    }
    enum GroupType
    {
        None,
        Compiler,
        Project
    };
    static inline std::vector<std::string> GetVectorFromParamText(std::string param)
    {
        auto res = std::vector<std::string>();
        std::string a_var = "";
        for(const char& word: param)
        {
            int status = 0;
            if (word == '"')
                status += 1;
            else
                a_var += word;
            if (status == 2)
            {
                status = 0;
                res.push_back(a_var);
            }
            res.push_back(a_var);
        }
        return res;
    }
    std::string PeekGroupType(std::vector<std::string> group)
    {
        auto func = PWMFunc(group[0], -1);
        if (func.name == "@compiler")
        {
            return "compiler";
        }
        std::printf("\033[31m" "Error: The Error Function Name." "\033[0m" "\n");
        std::printf("\033[31m" "%s" "\033[0m" "\n", group[0].c_str());
        std::exit(1);
    }

    CompilerInfo CompilerGroup(std::vector<std::string> group)
    {
        CompilerInfo info = {};
        for (const auto& line: group)
        {
            auto func = PWMFunc(line, -1);
            if (func.name == "@compiler")
            {
                info.compiler_name = func.param[0];
            }
            if (func.name == "compiler_path")
            {
                info.compiler_path = func.param[0];
            }
            if (func.name == "language_version")
            {
                info.language_version = func.param[0];
            }
            if (func.name == "standard_library")
            {
                info.standard_library = func.param[0];
            }
        }
        return info;
    }

    Lexer::Lexer(std::vector<std::string> lines)
    {
        auto groups = this->DivideGroup(lines);
        for (const auto& group: groups)
        {
            if (PeekGroupType(group) == "compiler")
            {
                this->compiler.push_back(CompilerGroup(group));
            }
        }
    }
    std::vector<std::vector<std::string>> Lexer::DivideGroup(std::vector<std::string> lines)
    {
        int line_index = 0;
        std::vector<std::vector<std::string>> groups;
        int group_count = 0;
        bool is_group = false;
        std::vector<std::string> group;
        for(const auto& line: lines)
        {
            line_index += 1;
            if (line.length() == 0 && line[0] == '#') 
                continue;
            
            if (line[0] == '@')
            {
                is_group = true;
            }
            if (line[0] == ';')
            {
                is_group = false;
                groups.push_back(group);
                group = std::vector<std::string>();
            }
            if (is_group == true)
            {
                group.push_back(line);
            }
        }
        return groups;
    }
}