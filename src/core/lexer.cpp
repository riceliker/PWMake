/*
 * lexer.cpp: file -> lexer -> transform
 * Analyze the PWMake file grammar and run the command.
 */

#include "mod.hpp"
#include <cstdio>

namespace PWMake::Core
{    
    /* only all element in stack is true, return true. */
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

    /* find the group, and call other function. */
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
            this->compiler = CompilerGroup(group, lines, pc);
        }
        else if (name == "@project")
        {
            this->project.push_back(ProjectGroup(group, this->files_variable, lines, pc));
        }
        else 
        {
            std::printf("\033[31m" "Error:" "\033[0m" " Unknow Group Name!\n");
            std::printf("In build.pwm:%d\n", pc+1);
            std::printf("%d | %s\n", pc+1, lines[pc].c_str());
            std::exit(1);
        }
    }

    /* define a new variable or calculate. */
    void Lexer::RegistryVariable(std::vector<std::string> lines, int pc)
    {
        auto line = lines[pc];
        auto [name, _params] = Function(line);
        auto params = std::move(_params);

        /* string variable */
        if (name == "$string")
        {
            /*
             * $string(name: def<string>, value: string)
             */
            CheckParam(params, 2, lines, pc);
            std::string a = GetString(this->string_variable,params->at(1), lines, pc);
            this->string_variable.insert({params->at(0), a});
            return;
        }
        else if (name == "$same") 
        {
            /*
             * $same(result: def<bool>, first: string, second: string)
             */
            CheckParam(params, 3, lines, pc);
            std::string a = GetString(this->string_variable,params->at(1), lines, pc);
            std::string b = GetString(this->string_variable,params->at(2), lines, pc);
            this->bool_variable.insert({params->at(0), a==b});
            return;
        }
        else if (name == "$in") 
        {
            /*
             * $same(result: def<bool>, bigger: string, smaller: string)
             */
            CheckParam(params, 3, lines, pc);
            std::string a = GetString(this->string_variable,params->at(1), lines, pc);
            std::string b = GetString(this->string_variable,params->at(2), lines, pc);
            this->bool_variable.insert({params->at(0), a.find(b) != std::string::npos});
            return;
        }
        else if (name == "$add")
        {
            /*
             * $add(result: def<string>, forward: string, backend: string)
             */
            CheckParam(params, 3, lines, pc);
            std::string a = GetString(this->string_variable,params->at(1), lines, pc);
            std::string b = GetString(this->string_variable,params->at(2), lines, pc);
            this->string_variable.insert({params->at(0), a + b});
            return;
        }
        if (name == "$prints") 
        {
            /*
             * $prints(msg: def<string>)
             */
            CheckParam(params, 1, lines, pc);
            std::string a = GetString(this->string_variable,params->at(0), lines, pc);
            std::printf("%s\n", a.c_str());
            return;
        }
        else if (name == "$printb")
        {
            /*
             * $prints(msg: def<bool>)
             */
            CheckParam(params, 1, lines, pc);
            bool a = GetBool(this->bool_variable, params->at(0), lines, pc);
            std::printf("%s\n", a?"true":"false");
            return;
        }
        
        /* bool variable */
        if (name == "$bool")
        {
            /*
             * $bool(name: def<bool>, value: bool)
             */
            CheckParam(params, 2, lines, pc);
            bool a = GetBool(this->bool_variable, params->at(1), lines, pc);
            this->bool_variable.insert({params->at(0), a});
            return;
        }
        else if (name == "$not") 
        {
            /*
             * $not(name: def<bool>, value: bool)
             */
            CheckParam(params, 2, lines, pc);
            bool a = GetBool(this->bool_variable, params->at(1), lines, pc);
            this->bool_variable.insert({params->at(0), !a});
            return;
        }
        else if (name == "$and") 
        {
            /*
             * $and(name: def<bool>, first: bool, second: bool)
             */
            CheckParam(params, 3, lines, pc);
            bool a = GetBool(this->bool_variable, params->at(1), lines, pc);
            bool b = GetBool(this->bool_variable, params->at(2), lines, pc);
            this->bool_variable.insert({params->at(0), a&&b});
            return;
        }
        else if (name == "$or") 
        {
            /*
             * $or(name: def<bool>, first: bool, second: bool)
             */
            CheckParam(params, 3, lines, pc);
            bool a = GetBool(this->bool_variable, params->at(1), lines, pc);
            bool b = GetBool(this->bool_variable, params->at(2), lines, pc);
            this->bool_variable.insert({params->at(0), a||b});
            return;
        }
        else if(name == "$equal")
        {
            /*
             * $and(name: def<bool>, first: bool, second: bool)
             */
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

    /* when meet the if-else-fi, control the steam. */
    void Lexer::ForkControl(std::unique_ptr<std::stack<bool>>& if_stack, std::vector<std::string> lines, int pc)
    {
        auto line = lines[pc];
        auto [name, _params] = Function(line);
        auto params = std::move(_params);

        if (name == ":if")
        {
            /*
             * :if(cond: bool)
             */
            CheckParam(params, 1, lines, pc);
            bool value = GetBool(this->bool_variable, params->at(0), lines, pc);
            if_stack->push(value);
            pc += 1;
        }
        else if (name == ":else") 
        {
            bool temp = if_stack->top();
            if_stack->pop();
            if_stack->push(!temp);
            pc += 1;
        }
        else if (name == ":fi") 
        {
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
    
    /* when meet @files to create new files group. */
    void Lexer::CreateFiles(std::vector<std::string> group, std::vector<std::string> lines, int pc)
    {
        auto list = std::vector<std::filesystem::path>();
        std::string _name = "";
        for (const auto& _line: group)
        {
            std::string line = _line;
            auto [name, _params] = Function(line);
            auto params = std::move(_params);

            if (name == "@files")
            {
                /*
                 * @file(name: string, [optional] extend: string)
                 */
                std::string file_name = GetString(this->string_variable, params->at(0), lines, pc);
                _name = file_name;
                if (this->files_variable.find(file_name) != this->files_variable.end())
                {
                    std::printf("\033[31m" "Error:" "\033[0m" " The Files Variable Is Repeat Define!\n");
                    std::exit(1);
                }
                // if it need to extend
                if (params->size() == 2)
                {
                    std::string extend_files = GetString(this->string_variable, params->at(1), lines, pc);
                    if (this->files_variable.find(extend_files) != this->files_variable.end())
                    {
                        std::printf("\033[31m" "Error:" "\033[0m" " Can Not Found The Extend Files!\n");
                        std::exit(1);
                    }
                    for (const auto& file: this->files_variable[extend_files])
                    {
                        list.push_back(file);
                    }
                }
            }
            else if (name == "add_file")
            {
                /*
                 * add_file(direct<string>)
                 */
                CheckParam(params, 1, lines, pc);
                list.push_back(GetString(this->string_variable, params->at(0), lines, pc));
            }
            else if (name == "remove_file")
            {
                /*
                 */
                CheckParam(params, 1, lines, pc);
                std::string file = GetString(this->string_variable, params->at(0), lines, pc);;
                int index = 0;
                auto pos = find(list.begin(), list.end(), file);
                list.erase(pos);
            }
            else if (name == "foreach_folder")
            {
                CheckParam(params, 2, lines, pc);
                std::string folder_path = GetString(this->string_variable, params->at(0), lines, pc);
                std::string file_extension = GetString(this->string_variable, params->at(1), lines, pc);
                auto temp_list = SearchFileInFolder(folder_path, file_extension);
                for (const auto& file: temp_list)
                {
                    list.push_back(file);
                }
            }
            else if (name == "recursion_folder")
            {
                CheckParam(params, 2, lines, pc);
                std::string folder_path = GetString(this->string_variable, params->at(0), lines, pc);;
                std::string file_extension = GetString(this->string_variable, params->at(1), lines, pc);;
                auto temp_list = RecursionFileInFolder(folder_path, file_extension);
                for (const auto& file: temp_list)
                {
                    list.push_back(file);
                }
            }
            else if (name == "print")
            {
                CheckParam(params, 1, lines, pc);
                std::string msg = GetString(this->string_variable, params->at(0), lines, pc);
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
        this->files_variable.insert({_name, list});
    }

    /* analyze the command */
    Lexer::Lexer(std::vector<std::string> lines)
    {
        size_t pc = 0;
        std::unique_ptr<std::stack<bool>> if_stack(new std::stack<bool>());
        while (pc != lines.size() - 1)
        {
            auto line = lines[pc];
            size_t start = 0;
            if (line.length() != 0)
            {
                switch (line[0]) 
                {
                case '#':
                {
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
                }
            }
            pc += 1;
        }
    }
    
}