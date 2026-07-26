#include "mod.hpp"
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace PWMake::Core
{    
    void Lexer::DivideGroup(std::vector<std::string> line, int pc)
    {
        auto [name, params] = Function(line[pc]);
        auto group = CatchGroup(line, pc);
        if (name == "@files")
        {
            
        }
        else if (name == "@compiler")
        {
            this->compiler = CompilerGroup(group);
        }
        else if (name == "@project")
        {

        }
        else 
        {
            std::printf("\033[31m" "Error:" "\033[0m" " Unknow Group Name!\n");
            std::printf("In build.pwm:%d\n", pc+1);
            std::printf("%d | %s\n", pc+1, line[pc].c_str());
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
    void Lexer::RegistryVariable(std::vector<std::string> line, int pc)
    {
        auto [name, param] = Function(line[pc]);
        auto params = std::move(param);
        if (name == "$string")
        {
            CheckParam(params, 2, line, pc);
            std::string a = GetString(this->string_variable,params->at(1));
            this->string_variable.insert({params->at(0), a});
            return;
        }
        else if (name == "$same") 
        {
            CheckParam(params, 3, line, pc);
            std::string a = GetString(this->string_variable,params->at(1));
            std::string b = GetString(this->string_variable,params->at(2));
            this->bool_variable.insert({params->at(0), a==b});
            return;
        }
        else if (name == "$in") 
        {
            CheckParam(params, 3, line, pc);
            std::string a = GetString(this->string_variable,params->at(1));
            std::string b = GetString(this->string_variable,params->at(2));
            this->bool_variable.insert({params->at(0), a.find(b) != std::string::npos});
            return;
        }

        if (name == "$prints") 
        {
            CheckParam(params, 1, line, pc);
            std::string a = GetString(this->string_variable,params->at(0));
            std::printf("%s\n", a.c_str());
            return;
        }
        else if (name == "$printb")
        {
            CheckParam(params, 1, line, pc);
            bool a = GetBool(this->bool_variable, params->at(0));
            std::printf("%s\n", a?"true":"false");
            return;
        }
        
        if (name == "$bool")
        {
            CheckParam(params, 2, line, pc);
            bool a = GetBool(this->bool_variable, params->at(1));
            this->bool_variable.insert({params->at(0), a});
            return;
        }
        else if (name == "$not") 
        {
            CheckParam(params, 2, line, pc);
            bool a = GetBool(this->bool_variable, params->at(1));
            this->bool_variable.insert({params->at(0), !a});
            return;
        }
        else if (name == "$and") 
        {
            CheckParam(params, 3, line, pc);
            bool a = GetBool(this->bool_variable, params->at(1));
            bool b = GetBool(this->bool_variable, params->at(2));
            this->bool_variable.insert({params->at(0), a&&b});
            return;
        }
        else if (name == "$or") 
        {
            CheckParam(params, 3, line, pc);
            bool a = GetBool(this->bool_variable, params->at(1));
            bool b = GetBool(this->bool_variable, params->at(2));
            this->bool_variable.insert({params->at(0), a||b});
            return;
        }
        else if(name == "$equal")
        {
            CheckParam(params, 3, line, pc);
            bool a = GetBool(this->bool_variable, params->at(1));
            bool b = GetBool(this->bool_variable, params->at(2));
            this->bool_variable.insert({params->at(0), a==b});
            return;
        }

        std::printf("\033[31m" "Error:" "\033[0m" " The Unknown Type Is Not Support!\n");
        std::printf("In build.pwm:%d\n", pc+1);
        std::printf("%d | %s\n", pc+1, line[pc].c_str());
        std::exit(1);
    }
    Lexer::Lexer(std::vector<std::string> lines)
    {
        size_t pc = 0;
        while (pc != lines.size() - 1)
        {
            auto line = lines[pc];
            switch (line[0]) 
            {
            case '#':
            {
                goto step_next_line;
            }
            break;
            case '@':
            {
                this->DivideGroup(lines, pc);
            }
            break;
            case '$':
            {
                this->RegistryVariable(lines, pc);
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
    std::vector<std::vector<std::string>> DivideGroups(std::vector<std::string> lines)
    {
        std::vector<std::vector<std::string>> groups;
        bool is_group = false;
        std::vector<std::string> group;
        for(const auto& line: lines)
        {
            // void
            if (line.length() == 0 && line[0] == '#') 
                continue;
            switch (line[0]) 
            {
            // The start of group
            case '@':
                is_group = true;
                break;
            // The end of group
            case ';':
                is_group = false;
                groups.push_back(group);
                group = std::vector<std::string>();
                break;
            }
            
            if (is_group == true)
            {
                group.push_back(line);
            }
        }
        return groups;
    }
    
}