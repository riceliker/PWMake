#include "cli.hpp"
#include <cstddef>
#include <cstdio>

namespace PWMake::CLI
{   
    Input::Input(std::vector<std::string> params)
    {
        if (params.size() == 1)
        {
            auto lexer = AnalyzeFile();
            BuildNinja(lexer);
            return;
        }
        else if (params[1] == "--version")
        {
            std::printf("0.1.0");
            return;
        }
        else if (params[1] == "--about")
        {
            this->OutputAbout();
            return;
        }
        else if (params[1] == "build")
        {
            auto lexer = AnalyzeFile();
            BuildNinja(lexer);
            return;
        }
        else if (params[1] == "clangd")
        {
            return;
        }
        else if (params[1] == "cmd")
        {
            return;
        }
        else 
        {
            std::printf("   ""\033[1;31m""PWMake CLI Command Help:\n""\033[0m");
            std::printf("   ""\033[1;36m""--version""\033[1;34m""         Get PWMake Version.\n""\033[0m");
            std::printf("   ""\033[1;36m""--about""\033[1;34m""          Get Infomation.\n""\033[0m");
            std::printf("   ""\033[1;36m""clangd""\033[1;34m""          Create clangd file.\n""\033[0m");
            std::printf("   ""\033[1;36m""build""\033[1;34m""          Create clangd file.\n""\033[0m");
            std::printf("   ""\033[1;36m""cmd""\033[1;34m""         Create compiler_command.json\n""\033[0m");
            return;
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
        std::printf("   Visit source code: https://www.github.com/riceliker/PWMake. \n");
        std::printf("   ""\033[1;31m""PWMake CLI Command Help:\n""\033[0m");
        std::printf("   ""\033[1;36m""--version""\033[1;34m""         Get PWMake Version.\n""\033[0m");
        std::printf("   ""\033[1;36m""--about""\033[1;34m""          Get Infomation.\n""\033[0m");
        std::printf("   ""\033[1;36m""clangd""\033[1;34m""          Create clangd file.\n""\033[0m");
        std::printf("   ""\033[1;36m""build""\033[1;34m""          Create clangd file.\n""\033[0m");
        std::printf("   ""\033[1;36m""cmd""\033[1;34m""         Create compiler_command.json\n""\033[0m");
        
    }
}