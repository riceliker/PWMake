/**
 * lexer.cpp -> transform.cpp -> ninja.cpp
 * This file will read the group type and transform the function name as
 * a struct, then send then to ninja.cpp.
 */

#include "lexer.hpp"
#include "../utils/utils.hpp"

namespace PWMake::Lexer
{
    CompilerInfo Lexer::CompilerGroup(std::vector<std::string> group, std::vector<std::string> lines, int pc)
    {
        CompilerInfo info = {};
        info.is_warning = false;
        info.is_debug = false;
        for (const auto& line: group)
        {
            auto [name, _params] = Utils::Function(line);
            auto params = std::move(_params);
            if (name[0] == '#')
            {
                continue;
            }
            else if (name == "@compiler")
            {
                Utils::CheckParam(params, 1, lines, pc);
                info.compiler_path = Utils::GetString(this->string_variable,params->at(0), lines, pc);
            }
            else if (name == "language_version")
            {
                Utils::CheckParam(params, 1, lines, pc);
                info.standard_language = Utils::GetString(this->string_variable,params->at(0), lines, pc);
            }
            else if (name == "standard_library")
            {
                Utils::CheckParam(params, 1, lines, pc);
                info.standard_library = Utils::GetString(this->string_variable,params->at(0), lines, pc);
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
                Utils::CheckParam(params, 1, lines, pc);
                info.header.push_back(Utils::GetString(this->string_variable,params->at(0), lines, pc));
            }
            else if (name == "link")
            {
                Utils::CheckParam(params, 1, lines, pc);
                info.links.push_back(Utils::GetString(this->string_variable,params->at(0), lines, pc));
            }
            else if (name == "framework")
            {
                Utils::CheckParam(params, 1, lines, pc);
                info.frameworks.push_back(Utils::GetString(this->string_variable,params->at(0), lines, pc));
            }
            else if (name == "find_library")
            {
                Utils::CheckParam(params, 1, lines, pc);
                info.find_library.push_back(Utils::GetString(this->string_variable,params->at(0), lines, pc));
            }
            else if (name == "link_param")
            {
                std::string link_param = "";
                for (const auto& param: *params)
                { 
                    link_param += param + ",";
                }
                link_param = link_param.substr(1, link_param.size()-3);
                info.link_params.push_back(link_param);
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

    ProjectInfo Lexer::ProjectGroup(std::vector<std::string> group, const std::unordered_map<std::string, std::vector<std::filesystem::path>>& dictionary, std::vector<std::string> lines, int pc)
    {
        ProjectInfo info = {};
        for (const auto& line: group)
        {
            auto [name, _params] = Utils::Function(line);
            auto params = std::move(_params);
            if (name[0] == '#')
            {
                continue;
            }
            else if (name == "@project")
            {
                Utils::CheckParam(params, 2, lines, pc);
                info.project_name = Utils::GetString(this->string_variable,params->at(0), lines, pc);
                info.project_type = Utils::GetString(this->string_variable,params->at(1), lines, pc);
            }
            else if (name == "source_files") 
            {
                Utils::CheckParam(params, 1, lines, pc);
                std::string files = Utils::GetString(this->string_variable,params->at(0), lines, pc);
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
                Utils::CheckParam(params, 1, lines, pc);
                info.library.push_back(Utils::GetString(this->string_variable,params->at(0), lines, pc));
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