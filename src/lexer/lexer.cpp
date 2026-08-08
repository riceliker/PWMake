/*
 * lexer.cpp: file -> lexer -> transform
 * Analyze the PWMake file grammar and run the command.
 */

#include "lexer.hpp"
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <sys/cdefs.h>
#include <vector>

namespace PWMake::Lexer
{    

    /* Only all element in stack is true, return true. */
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

    /* when meet the if-else-fi, control the steam. */
    void Lexer::ForkControl(VariableMap& map, std::unique_ptr<std::stack<bool>>& if_stack, std::string text, int pc)
    {
        auto func = AnalyzeFunction(map, text, pc);
        if (func.name == ":if")
        {
            /*
             * :if(cond: bool)
             */
            ParamCountIsWant(1, func.params.size(), text, pc);
            bool value = ParamGetBool(func.params[0], 0, text, pc);
            if_stack->push(value);
            pc += 1;
        }
        else if (func.name == ":else") 
        {
            bool temp = if_stack->top();
            if_stack->pop();
            if_stack->push(!temp);
            pc += 1;
        }
        else if (func.name == ":fi") 
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
            std::printf("%d | %s\n", pc+1, text.c_str());
            std::exit(1);
        }
    }

    void Lexer::SendGroup(VariableMap& map, std::vector<GroupFunction> group, GroupType status, std::string text, int pc)
    {
        switch (status) 
        {
        case Files:
            CreateFiles(group, map, text, pc);
            break;
        case Compiler:
            this->compiler = CompilerGroup(group, text, pc);
            break;
        case Project:
            this->project.push_back(ProjectGroup(group, map, text, pc));
            break;
        case Global:
            break;
        }
    }

    GroupFunction AnalyzeFunction(VariableMap& map, std::string text, size_t line)
    {
        /* Get function name nnd params */
        bool is_record_name = true;
        std::string name = "";
        std::string param = "";
        std::vector<ParamOption> opt_params;
        for (const auto& word: text)
        {
            if (word == '(')
                is_record_name = false;
            else if (is_record_name)
                name += word;
            else
            {
                switch (word) 
                {
                case ',':
                    opt_params.push_back(FindVariable(map, param, text, line));
                    param = "";
                    break;
                case ')':
                    if(param.size() == 0)
                        break;
                    opt_params.push_back(FindVariable(map, param, text, line));
                    break;
                default:
                    param += word;
                    break;
                }
            }
        }
        /* Create */
        GroupFunction func;
        func.line_text = text;
        func.lines = line;
        func.name = name;
        func.params = opt_params;
        return func;
    }

    /* analyze the command */
    Lexer::Lexer(std::vector<std::string> lines, VariableMap variable)
    {
        this->variable = variable;

        GroupType currect_group_status = Global;
        std::vector<GroupFunction> currect_group;

        size_t pc = 0;
        std::unique_ptr<std::stack<bool>> if_stack(new std::stack<bool>());
        while (pc != lines.size())
        {
            int step = 1;
            auto line = lines[pc];
            size_t start = 0;
            if (line.length() != 0)
            {
                switch (line[0]) 
                {
                /* Anotation*/
                case '#':
                {
                }
                break;
                /* Define Group */
                case '@':
                {
                    if (IsAllTrue(if_stack))
                    {
                        currect_group = std::vector<GroupFunction>();
                        auto func = AnalyzeFunction(this->variable, line, pc);
                        if (currect_group_status != Global)
                        {
                            std::printf("\033[31m" "Error:" "\033[0m" " Can Not Define The Group In Group!\n");
                            std::printf("In build.pwm:%d\n", (int)pc+1);
                            std::printf("%d | %s\n", (int)pc+1, lines[pc].c_str());
                            std::exit(1);
                        }
                        else if (func.name == "@files")
                        {
                            currect_group_status = Files;
                        }
                        else if (func.name == "@compiler")
                        {
                            currect_group_status = Compiler;
                        }
                        else if (func.name == "@project")
                        {
                            currect_group_status = Project;
                        }
                        else 
                        {
                            std::printf("\033[31m" "Error:" "\033[0m" " UnKnown Group Name(%s)!\n", func.name.c_str());
                            std::printf("In build.pwm:%d\n", (int)pc+1);
                            std::printf("%d | %s\n", (int)pc+1, lines[pc].c_str());
                            std::exit(1);
                        }
                        currect_group.push_back(AnalyzeFunction(this->variable, line, pc));
                    }
                }
                break;
                /* Define Variable */
                case '$':
                {
                    if (IsAllTrue(if_stack))
                    {
                        if (currect_group_status != Global)
                        {
                            std::printf("\033[31m" "Error:" "\033[0m" " Can Not Define The Variable In Group!\n");
                            std::printf("In build.pwm:%d\n", (int)pc+1);
                            std::printf("%d | %s\n", (int)pc+1, lines[pc].c_str());
                            std::exit(1);
                        }
                        this->RegistryVariable(line, pc);
                    }
                }
                break;
                /* Fork Structure */
                case ':':
                {
                    this->ForkControl(this->variable, if_stack, line, pc);
                }
                break;
                /* End Group */
                case ';':
                {
                    if (IsAllTrue(if_stack))
                    {
                        this->SendGroup(this->variable, currect_group, currect_group_status, line, pc);
                        currect_group_status = Global;
                    }  
                }
                break;
                /* Common Function */
                default:
                {
                    if (IsAllTrue(if_stack))
                    {
                        if (currect_group_status != Global)
                            currect_group.push_back(AnalyzeFunction(this->variable, line, pc));
                    }     
                }
                break;
                }
            }
            pc += 1;
        }
    }
    
}