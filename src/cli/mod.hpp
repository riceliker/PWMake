#pragma once

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
}