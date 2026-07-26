#include "mod.hpp"

namespace PWMake::Core 
{
    std::tuple<std::string, std::vector<std::string>> Function(std::string text)
    {
        std::string name = "";
        std::vector<std::string> param = std::vector<std::string>();

        bool is_record_name = true;
        bool is_record_param = false;
        int param_index = -1;
        for(const char& word: text)
        {
            // end function name
            if (word == '(')
                is_record_name = false;
            // record function parameter
            if (word == '"')
            {
                if (is_record_name == true)
                {
                    std::printf("\033[31m" "Error: The Error Function Name." "\033[0m" "\n");
                    std::printf("\033[31m" "%s" "\033[0m" "\n", text.c_str());
                    std::exit(1);
                }
                if (is_record_param == false)
                {
                    param_index += 1;
                    param.push_back("");
                }
                is_record_param = ! is_record_param;
            }    
            if (is_record_name)
                name += word;
            if (is_record_param && word != '"')
            {
                std::string old_temp = param[param_index];
                std::string new_temp = old_temp + word;
                param[param_index] = new_temp;
            }
        }
        return std::tuple(name, param);
    }
}
