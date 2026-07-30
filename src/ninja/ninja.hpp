#pragma once
#include "../Interface.hpp"

namespace PWMake::Ninja
{
    // ninja.cpp
    class NinjaFile
    {
    private:
        std::vector<std::string> data;
    public:
        NinjaFile();
        std::vector<std::string> AsFile();
        void AddCompiler(CompilerInfo info);
        void AddSource(ProjectInfo info);
    };
}