#pragma once


#include <unordered_map>
#include <vector>
namespace PWMake::Config 
{
    class Config
    {
    public:
        std::unordered_map<std::string, std::string> string_variable;
        std::unordered_map<std::string, bool> bool_variable;
        Config(std::vector<std::string> lines);
    };
}