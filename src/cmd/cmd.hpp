#pragma once
#include "../Interface.hpp"
#include <vector>

namespace PWMake::CMD
{
    class CMD :public IMakeFile
    {  
    public:
        CMD(CompilerInfo cinfo, std::vector<ProjectInfo> pinfos);
    };
}
