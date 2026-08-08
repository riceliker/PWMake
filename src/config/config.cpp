#include "config.hpp"
#include <string>
#include <vector>

namespace PWMake::Config 
{
    static inline std::string GetString(std::string input)
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
    Config::Config(std::vector<std::string> lines)
    {
        for (const auto& _line: lines)
        {
            auto line = GetString(_line);
            if (line[0] == '#')
                continue;
            else
            {
                bool is_record_name = true;
                std::string name = "";
                std::string value = "";
                for (const char& word: line)
                {
                    if (word == '=')
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
                        value += word;
                    }
                }
                if (value == "true" || value == "false")
                {
                    this->bool_variable.insert({name, value=="true"?true:false});
                }
                else 
                {
                    this->string_variable.insert({name, value});
                }
            }
        }
    }
}