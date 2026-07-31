#pragma once
#include "../Interface.hpp"

#include <string>
#include <vector>
namespace PWMake::Clangd
{
    class Clangd
    {
    private:
        std::vector<std::string> data;
    public:
        Clangd();
        std::vector<std::string> AsFile();
        void AddCompilerFlag(CompilerInfo info);
    };
}