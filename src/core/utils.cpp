#include "mod.hpp"
#include <memory>
#include <print>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#if defined(__unix__)
std::string _platform = "unix";
#endif

#if defined(__linux__)
std::string _platform = "linux";
#endif

#if defined(__APPLE__)
std::string _platform = "macos";
#endif

#if defined(_WIN64)
std::string _platform = "window";
#endif

#if defined (__x86_64__)
std::string _arch = "amd64";
#endif

#if defined (__aarch64__)
std::string _arch = "arm64";
#endif

namespace PWMake::Core 
{
    std::tuple<std::string, std::unique_ptr<std::vector<std::string>>> Function(std::string text)
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

    std::string GetString(std::unordered_map<std::string, std::string> map, std::string input, std::vector<std::string> lines, int pc)
    {
        size_t start = 0;
        while (start < input.size() && std::isspace(static_cast<unsigned char>(input[start])))
            start++;
        input.erase(0, start);
        // special string
        if (input == "__platform__")
        {
            return _platform;
        }
        if (input == "__arch__")
        {
            return _arch;
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

    bool GetBool(std::unordered_map<std::string, bool> map, std::string input, std::vector<std::string> lines, int pc)
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

    std::vector<std::string> CatchGroup(std::vector<std::string> line, int pc)
    {
        auto group = std::vector<std::string>();
        while (line[pc][0] != ';')
        {
            group.push_back(line[pc]);
            pc += 1;
        }
        return group;
    }
}
