#include "config.hpp"
#include "../utils/utils.hpp"
#include <string>
#include <vector>

namespace PWMake::Config 
{
    Config::Config(std::vector<std::string> lines)
    {
        
        for (const auto& _line: lines)
        {
            auto line = Utils::GetStringLite(_line);
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