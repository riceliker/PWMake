#include "mod.hpp"
#include <cstddef>

namespace PWMake::CLI
{   
    Input::Input(std::vector<std::string> params)
    {
        if (params[0] == "--version")
        {
            std::printf("0.1.0");
        }
        if (params[0] == "--about")
        {
            this->OutputAbout();
        }
    }
    void Input::OutputAbout()
    {
        std::printf("    ""\033[1;31m"" _____""\033[1;33m""__          __""\033[1;32m""__  __      ""\033[1;34m"" _\n""\033[0m");       
        std::printf("    ""\033[1;31m""|  __ ""\033[1;33m""\\ \\        / /""\033[1;32m""  \\/  |     ""\033[1;34m""| |      \n""\033[0m");
        std::printf("    ""\033[1;31m""| |__) ""\033[1;33m""\\ \\  /\\  / /""\033[1;32m""| \\  / | __ _""\033[1;34m""| | _____\n""\033[0m"); 
        std::printf("    ""\033[1;31m""|  ___/""\033[1;33m"" \\ \\/  \\/ / ""\033[1;32m""| |\\/| |/ _` ""\033[1;34m""| |/ / _ \\\n""\033[0m");
        std::printf("    ""\033[1;31m""| |    ""\033[1;33m""  \\  /\\  /  ""\033[1;32m""| |  | | (_| ""\033[1;34m""|   <  __/\n""\033[0m");
        std::printf("    ""\033[1;31m""|_|    ""\033[1;33m""   \\/  \\/   ""\033[1;32m""|_|  |_|\\__,_""\033[1;34m""|_|\\_\\___|\n""\033[0m");
        std::printf("   Welcome to use ""\033[1;34m""PWMake""\033[0m"". It will save your time to make program.\n");
        std::printf("   PWMake is a ""\033[1;32m""Free Software""\033[0m"". Use ""\033[1;32m""GPLv3.0 ""\033[0m""License.\n");
        std::printf("   Visit source code: https://www.github.com/riceliker/PWMake. ");
        std::printf("   Command:");
        std::printf("   --version       Get PWMake Version.");
        std::printf("   --about         Get Infomation.");
        
    }
}