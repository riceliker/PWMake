#pragma once
#include "../lexer/lexer.hpp"

#include <string>
#include <vector>

namespace PWMake::CLI
{
    class Input
    {
    public:
        Input(std::vector<std::string> params);
        void OutputHelp();
        void OutputVersion();
        void OutputAbout();
    };

    PWMake::Lexer::Lexer AnalyzeFile();
    void CreateNinja(PWMake::Lexer::Lexer lexer);
    void CreateClangd(PWMake::Lexer::Lexer lexer);
    void CreateCMD(PWMake::Lexer::Lexer lexer);

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