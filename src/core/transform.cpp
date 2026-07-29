/**
 * lexer.cpp -> transform.cpp -> ninja.cpp
 * This file will read the group type and transform the function name as
 * a struct, then send then to ninja.cpp.
 */

#include "mod.hpp"
#include <filesystem>
#include <string>
#include <unordered_map>

namespace PWMake::Core 
{
    CompilerInfo CompilerGroup(std::vector<std::string> group, std::vector<std::string> lines, int pc)
    {
        CompilerInfo info = {};
        info.is_warning = false;
        info.is_debug = false;
        for (const auto& line: group)
        {
            auto [name, _params] = Function(line);
            auto params = std::move(_params);
            if (name == "@compiler")
            {
                CheckParam(params, 1, lines, pc);
                info.compiler_path = GetStringLite(params->at(0));
            }
            else if (name == "language_version")
            {
                CheckParam(params, 1, lines, pc);
                info.standard_language = GetStringLite(params->at(0));
            }
            else if (name == "standard_library")
            {
                CheckParam(params, 1, lines, pc);
                info.standard_library = GetStringLite(params->at(0));
            }
            else if (name == "compiler_warning")
            {
                info.is_warning = true;
            }
            else if (name == "compiler_extra_warning")
            {
                info.is_extra_warning = true;
            }
            else if (name == "compiler_debug")
            {
                info.is_debug = true;
            }
            else if (name == "library_header")
            {
                CheckParam(params, 1, lines, pc);
                info.header.push_back(GetStringLite(params->at(0)));
            }
            else 
            {
                std::printf("\033[31m" "Error:" "\033[0m" " Unknow Function Name(%s) In Compiler Group!\n", name.c_str());
                std::printf("In build.pwm:%d\n", pc+1);
                std::printf("%d | %s\n", pc+1, lines[pc].c_str());
                std::exit(1);
            }
        }
        return info;
    }

    ProjectInfo ProjectGroup(std::vector<std::string> group, const std::unordered_map<std::string, std::vector<std::filesystem::path>>& dictionary, std::vector<std::string> lines, int pc)
    {
        ProjectInfo info = {};
        for (const auto& line: group)
        {
            auto [name, _params] = Function(line);
            auto params = std::move(_params);
            if (name == "@project")
            {
                CheckParam(params, 2, lines, pc);
                info.project_name = GetStringLite(params->at(0));
                info.project_type = GetStringLite(params->at(1));
            }
            else if (name == "source_files") 
            {
                CheckParam(params, 1, lines, pc);
                std::string files = GetStringLite(params->at(0));
                if (dictionary.find(files) != dictionary.end())
                {
                    info.source_files = dictionary.at(files);
                }
                else 
                {
                    std::printf("\033[31m" "Error:" "\033[0m" " The Files Name(%s) Is Not Defined.\n", files.c_str());
                    std::printf("In build.pwm:%d\n", pc+1);
                    std::printf("%d | %s\n", pc+1, lines[pc].c_str());
                    std::exit(1);
                }
            }
            else if (name == "add_library")
            {
                CheckParam(params, 1, lines, pc);
                info.library.push_back(GetStringLite(params->at(0)));
            }
            else 
            {
                std::printf("\033[31m" "Error:" "\033[0m" " Unknow Function Name(%s) In Project Group.\n", name.c_str());
                std::printf("In build.pwm:%d\n", pc+1);
                std::printf("%d | %s\n", pc+1, lines[pc].c_str());
                std::exit(1);
            }
        }
        return info;
    }

}