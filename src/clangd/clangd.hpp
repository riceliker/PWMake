#pragma once
#include "../Interface.hpp"

namespace PWMake::Clangd
{
    class Clangd :public IMakeFile
    {
    public:
        Clangd();
        void AddCompilerFlag(CompilerInfo info);
    };
}