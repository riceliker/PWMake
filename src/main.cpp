#include "core/mod.hpp"
#include <string>
#include <vector>

int main(int argc, char** argv)
{
    // auto args = std::span(argv, argc);
    // std::vector<std::string> param;
    // for (auto arg : args)
    // {
    //     param.push_back(std::string(arg));
    // }

    // auto input = PWMake::CLI::Input(param);
    // input.OutputAbout();

    auto file = PWMake::Core::GetTextLinesInFile("./build.pwm");
    auto lexer = PWMake::Core::Lexer(file);
    auto out = PWMake::Core::NinjaFile();
    out.AddCompiler(lexer.compiler);
    out.AddSource(lexer.project[0]);
    PWMake::Core::CreateFileInPath(out.AsFile(), "./build.ninja");
    
}