#pragma once

#include <memory>
#include <print>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#if defined(__unix__)
inline std::string platform = "unix";
#endif

#if defined(__linux__)
inline std::string platform = "linux";
#endif

#if defined(__APPLE__)
inline std::string platform = "macos";
#endif

#if defined(_WIN64)
inline std::string platform = "windows";
#endif

#if defined (__x86_64__)
inline std::string arch = "amd64";
#endif

#if defined (__aarch64__)
inline std::string arch = "arm64";
#endif

#if defined (__clang__)
inline std::string compiler = "clang";
#elif defined (__GNUC__)
inline std::string compiler = "gcc";
#endif

namespace PWMake::Utils
{
    inline void CheckParam(std::unique_ptr<std::vector<std::string>>& params, int count, std::vector<std::string> line, int pc)
    {
        if (params->size() != count)
        {
            std::printf("\033[31m" "Error:" "\033[0m" " Defined A Variable Must Need Two Parameters!\n");
            std::printf("In build.pwm:%d\n", pc+1);
            std::printf("%d | %s\n", pc+1, line[pc].c_str());
            std::exit(1);
        }
    }

    inline std::tuple<std::string, std::unique_ptr<std::vector<std::string>>> Function(std::string text)
    {
        std::string name = "";
        std::unique_ptr<std::vector<std::string>> params(new std::vector<std::string>());

        bool is_record_name = true;
        std::string param = "";
        for(const char& word: text)
        {
            // end function name
            if (word == '(')
            {
                is_record_name = false;
                continue;
            }
            if (is_record_name)
            {
                name += word;
            }
            else
            {
                if(word == ',')
                {
                    params->push_back(param);
                    param = "";
                }
                else if (word == ')')
                {
                    params->push_back(param);
                    break;
                }
                else 
                {
                    param += word;
                }
            }
        }
        return std::tuple(name, std::move(params));
    }

    inline std::string GetString(std::unordered_map<std::string, std::string> map, std::string input, std::vector<std::string> lines, int pc)
    {
        size_t start = 0;
        while (start < input.size() && std::isspace(static_cast<unsigned char>(input[start])))
            start++;
        input.erase(0, start);
        // special string
        if (input == "__platform__")
        {
            return platform;
        }
        if (input == "__arch__")
        {
            return arch;
        }
        if (input == "__compiler__")
        {
            return compiler;
        }
        // if not literal string, find from dictionary
        if (input[0] == '"' && input[input.size()-1] == '"')
        {
            return input.substr(1, input.size()-2);
        }
        else 
        {
            if (map.find(input) != map.end())
            {
                return map[input];
            }
            else 
            {
                std::printf("\033[31m" "Error:" "\033[0m" " The String Variable(%s) Is Not Define!\n", input.c_str());
                std::printf("In build.pwm:%d\n", pc+1);
                std::printf("%d | %s\n", pc+1, lines[pc].c_str());
                std::exit(1);
            }
        }
    }

    inline std::string GetStringLite(std::string input)
    {
        size_t start = 0;
            while (start < input.size() && std::isspace(static_cast<unsigned char>(input[start])))
                start++;
            input.erase(0, start);
        if (input[0] == '"' && input[input.size()-1] == '"')
        {
            return input.substr(1, input.size()-2);
        }
        return input;
    }

    inline bool GetBool(std::unordered_map<std::string, bool> map, std::string input, std::vector<std::string> lines, int pc)
    {
        size_t start = 0;
        while (start < input.size() && std::isspace(static_cast<unsigned char>(input[start])))
            start++;
        input.erase(0, start);
        if (input == "true" || input == "false")
        {
            return input == "true" ? true : false;
        }
        else if (map.find(input) != map.end())
        {
            return map[input];
        }
        else 
        {
            std::printf("\033[31m" "Error:" "\033[0m" " The Bool Variable(%s) Is Not Define!\n", input.c_str());
            std::printf("In build.pwm:%d\n", pc+1);
            std::printf("%d | %s\n", pc+1, lines[pc].c_str());
            std::exit(1);
        }
    }

    inline std::vector<std::string> CatchGroup(std::vector<std::string> line, int pc)
    {
        auto group = std::vector<std::string>();
        while (line[pc][0] != ';')
        {
            group.push_back(line[pc]);
            pc += 1;
        }
        return group;
    }

    inline void StepLine(std::string name, float per)
    {
        //std::printf("\r\033[K");
        int num = per * 10.0;
        if (10 >= num && per >= 0)
        {
            std::string sym = "|";
            for (int i = 0; i < num; ++i)
            {
                sym += "=";
            }
            for (int i = 0; i < 10 - num; ++i)
            {
                sym += " ";
            }
            sym += ">";
            std::printf("   ""\033[1;32m""Analyze file""\033[0m""%s|%d%% \n", sym.c_str(), (int)per*100);
        }
    }
}
