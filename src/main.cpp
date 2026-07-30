#include "cli/cli.hpp"

#include <span>
#include <string>
#include <vector>

int main(int argc, char** argv)
{
    auto args = std::span(argv, argc);
    std::vector<std::string> param;
    for (auto arg : args)
    {
        param.push_back(std::string(arg));
    }

    auto input = PWMake::CLI::Input(param);
}