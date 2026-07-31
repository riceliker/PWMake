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
}