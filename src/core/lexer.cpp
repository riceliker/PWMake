#include "mod.hpp"
#include <cstddef>
#include <string>
#include <vector>

namespace PWMake::Core
{
    enum LineType
    {
        Define,
        Group,
        Fork,
    };
    static inline std::vector<std::string> CatchGroup(std::vector<std::string> line, int pc)
    {
        auto group = std::vector<std::string>();
        while (line[pc][0] != ';')
        {
            group.push_back(line[pc]);
            pc += 1;
        }
        return group;
    }
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
            case '%':
            {

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