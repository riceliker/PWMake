#pragma once
#include "../Interface.hpp"

namespace PWMake::Ninja
{
    // ninja.cpp
    class Ninja :public IMakeFile
    {
    public:
        Ninja();
        void AddCompiler(CompilerInfo info);
        void AddSource(ProjectInfo info);
    };
}