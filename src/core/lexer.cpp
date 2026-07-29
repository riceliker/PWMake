#include "mod.hpp"
#include <cstddef>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>
#include <stack>

namespace PWMake::Core
{    
    void Lexer::DivideGroup(std::vector<std::string> lines, int pc)
    {
        auto line = lines[pc];
        size_t start = 0;
        while (start < line.size() && std::isspace(static_cast<unsigned char>(line[start])))
            start++;
        line.erase(0, start);
        auto [name, params] = Function(line);
        auto group = CatchGroup(lines, pc);
        if (name == "@files")
        {
            this->CreateFiles(group, lines, pc);
        }
        else if (name == "@compiler")
        {
            this->compiler = CompilerGroup(group);
        }
        else if (name == "@project")
        {
            this->project.push_back(ProjectGroup(group));
        }
        else 
        {
            std::printf("\033[31m" "Error:" "\033[0m" " Unknow Group Name!\n");
            std::printf("In build.pwm:%d\n", pc+1);
            std::printf("%d | %s\n", pc+1, lines[pc].c_str());
            std::exit(1);
        }
    }
    static inline void CheckParam(std::unique_ptr<std::vector<std::string>>& params, int count, std::vector<std::string> line, int pc)
    {
        if (params->size() != count)
        {
            std::printf("\033[31m" "Error:" "\033[0m" " Defined A Variable Must Need Two Parameters!\n");
            std::printf("In build.pwm:%d\n", pc+1);
            std::printf("%d | %s\n", pc+1, line[pc].c_str());
            std::exit(1);
        }
    }
    void Lexer::RegistryVariable(std::vector<std::string> lines, int pc)
    {
        auto line = lines[pc];
        size_t start = 0;
        while (start < line.size() && std::isspace(static_cast<unsigned char>(line[start])))
            start++;
        line.erase(0, start);
        auto [name, param] = Function(line);
        auto params = std::move(param);
        if (name == "$string")
        {
            CheckParam(params, 2, lines, pc);
            std::string a = GetString(this->string_variable,params->at(1), lines, pc);
            this->string_variable.insert({params->at(0), a});
            return;
        }
        else if (name == "$same") 
        {
            CheckParam(params, 3, lines, pc);
            std::string a = GetString(this->string_variable,params->at(1), lines, pc);
            std::string b = GetString(this->string_variable,params->at(2), lines, pc);
            this->bool_variable.insert({params->at(0), a==b});
            return;
        }
        else if (name == "$in") 
        {
            CheckParam(params, 3, lines, pc);
            std::string a = GetString(this->string_variable,params->at(1), lines, pc);
            std::string b = GetString(this->string_variable,params->at(2), lines, pc);
            this->bool_variable.insert({params->at(0), a.find(b) != std::string::npos});
            return;
        }
        else if (name == "$add")
        {
            CheckParam(params, 3, lines, pc);
            std::string a = GetString(this->string_variable,params->at(1), lines, pc);
            std::string b = GetString(this->string_variable,params->at(2), lines, pc);
            this->string_variable.insert({params->at(0), a + b});
            return;
        }
        

        if (name == "$prints") 
        {
            CheckParam(params, 1, lines, pc);
            std::string a = GetString(this->string_variable,params->at(0), lines, pc);
            std::printf("%s\n", a.c_str());
            return;
        }
        else if (name == "$printb")
        {
            CheckParam(params, 1, lines, pc);
            bool a = GetBool(this->bool_variable, params->at(0), lines, pc);
            std::printf("%s\n", a?"true":"false");
            return;
        }
        
        if (name == "$bool")
        {
            CheckParam(params, 2, lines, pc);
            bool a = GetBool(this->bool_variable, params->at(1), lines, pc);
            this->bool_variable.insert({params->at(0), a});
            return;
        }
        else if (name == "$not") 
        {
            CheckParam(params, 2, lines, pc);
            bool a = GetBool(this->bool_variable, params->at(1), lines, pc);
            this->bool_variable.insert({params->at(0), !a});
            return;
        }
        else if (name == "$and") 
        {
            CheckParam(params, 3, lines, pc);
            bool a = GetBool(this->bool_variable, params->at(1), lines, pc);
            bool b = GetBool(this->bool_variable, params->at(2), lines, pc);
            this->bool_variable.insert({params->at(0), a&&b});
            return;
        }
        else if (name == "$or") 
        {
            CheckParam(params, 3, lines, pc);
            bool a = GetBool(this->bool_variable, params->at(1), lines, pc);
            bool b = GetBool(this->bool_variable, params->at(2), lines, pc);
            this->bool_variable.insert({params->at(0), a||b});
            return;
        }
        else if(name == "$equal")
        {
            CheckParam(params, 3, lines, pc);
            bool a = GetBool(this->bool_variable, params->at(1), lines, pc);
            bool b = GetBool(this->bool_variable, params->at(2), lines, pc);
            this->bool_variable.insert({params->at(0), a==b});
            return;
        }

        std::printf("\033[31m" "Error:" "\033[0m" " The Unknown Type Is Not Support! Name:%s\n", name.c_str());
        std::printf("In build.pwm:%d\n", pc+1);
        std::printf("%d | %s\n", pc+1, lines[pc].c_str());
        std::exit(1);
    }


    void Lexer::ForkControl(std::unique_ptr<std::stack<bool>>& if_stack, std::vector<std::string> lines, int pc)
    {
        auto line = lines[pc];
        size_t start = 0;
        while (start < line.size() && std::isspace(static_cast<unsigned char>(line[start])))
            start++;
        line.erase(0, start);
        auto [name, param] = Function(line);
        auto params = std::move(param);
        if (name == ":if")
        {
            CheckParam(params, 1, lines, pc);
            bool value = GetBool(this->bool_variable, params->at(0), lines, pc);
            if_stack->push(value);
            pc += 1;
        }
        else if (name == ":else") 
        {
            CheckParam(params, 0, lines, pc);
            bool temp = if_stack->top();
            if_stack->pop();
            if_stack->push(!temp);
            pc += 1;
        }
        else if (name == ":endif") 
        {
            CheckParam(params, 0, lines, pc);
            if (!if_stack->empty())
            {
                if_stack->pop();  
            }  
            pc += 1;
        }
        else 
        {
            std::printf("\033[31m" "Error:" "\033[0m" " Error If Structure!\n");
            std::printf("In build.pwm:%d\n", pc+1);
            std::printf("%d | %s\n", pc+1, lines[pc].c_str());
            std::exit(1);
        }
    }
    static inline bool IsAllTrue(const std::unique_ptr<std::stack<bool>>& src)
    {
        bool res = true;
        std::stack<bool> tmp = *src;
        while (!tmp.empty())
        {
            bool val = tmp.top();
            res = val && res;
            tmp.pop();
        }
        return res;
    }
    void Lexer::CreateFiles(std::vector<std::string> group, std::vector<std::string> lines, int pc)
    {
        auto list = std::vector<std::string>();
        for (const auto& _line: group)
        {
            std::string line = _line;
            size_t start = 0;
            while (start < line.size() && std::isspace(static_cast<unsigned char>(line[start])))
                start++;
            line.erase(0, start);
            auto [name, param] = Function(line);
            auto params = std::move(param);
            if (name == "@files")
            {
                std::string file_name = GetStringLite(params->at(0));
                if (this->files_variable.find(file_name) != this->files_variable.end())
                {
                    std::printf("\033[31m" "Error:" "\033[0m" " The Files Variable Is Repeat Define!\n");
                    std::exit(1);
                }
                if (params->size() == 2)
                {
                    std::string extend_files = GetStringLite(params->at(1));
                    if (this->files_variable.find(extend_files) != this->files_variable.end())
                    {
                        std::printf("\033[31m" "Error:" "\033[0m" " Can Not Found The Extend Files!\n");
                        std::exit(1);
                    }
                }
            }
            else if (name == "add_file")
            {
                CheckParam(params, 1, lines, pc);
                list.push_back(GetStringLite(params->at(0)));
            }
            else if (name == "remove_file")
            {
                CheckParam(params, 1, lines, pc);
                std::string file = GetStringLite(params->at(0));
                int index = 0;
                auto pos = find(list.begin(), list.end(), file);
                list.erase(pos);
            }
            else if (name == "foreach_folder")
            {
                CheckParam(params, 2, lines, pc);
                std::string folder_path = GetStringLite(params->at(0));
                std::string file_extension = GetStringLite(params->at(1));
                auto temp_list = SearchFileInFolder(folder_path, file_extension);
                for (const auto& file: temp_list)
                {
                    list.push_back(file);
                }
            }
            else if (name == "recursion_folder")
            {
                CheckParam(params, 2, lines, pc);
                std::string folder_path = GetStringLite(params->at(0));
                std::string file_extension = GetStringLite(params->at(1));
                auto temp_list = RecursionFileInFolder(folder_path, file_extension);
                for (const auto& file: temp_list)
                {
                    list.push_back(file);
                }
            }
            else if (name == "print")
            {
                CheckParam(params, 1, lines, pc);
                std::string msg = GetStringLite(params->at(0));
                std::printf("\033[35m" "%s\n" "\033[0m", msg.c_str());
                int i = 0;
                for (const auto& file: list)
                {
                    std::printf("\033[32m" "%d|" "\033[0m" " %s\n"  , i, file.c_str());
                    i += 1;
                }
            }
            else 
            {
                std::printf("\033[31m" "Error:" "\033[0m" " Can Not Found The Extend Files!\n");
                std::exit(1);
            }
        }
        this->files_variable.insert({});
    }
    Lexer::Lexer(std::vector<std::string> lines)
    {
        size_t pc = 0;
        std::unique_ptr<std::stack<bool>> if_stack(new std::stack<bool>());
        while (pc != lines.size() - 1)
        {
            auto line = lines[pc];
            size_t start = 0;
            while (start < line.size() && std::isspace(static_cast<unsigned char>(line[start])))
                start++;
            line.erase(0, start);
            if (line.length() == 0)
            {
                goto step_next_line;
            }
            switch (line[0]) 
            {
            case '#':
            {
                goto step_next_line;
            }
            break;
            case '@':
            {
                if (IsAllTrue(if_stack))
                    this->DivideGroup(lines, pc);
            }
            break;
            case '$':
            {
                if (IsAllTrue(if_stack))
                    this->RegistryVariable(lines, pc);
            }
            break;
            case ':':
            {
                this->ForkControl(if_stack, lines, pc);
            }
            break;
            default:
            {

            }
            break;
            }
            step_next_line:
            pc += 1;
        }
    }
    
}