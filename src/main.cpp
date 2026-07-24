#include "core/mod.hpp"
#include <string>

int main()
{
    auto file = PWMake::GetTextLinesInFile("./build.pwm");
    auto out = PWMake::NinjaFile();
    auto lexer = PWMake::Lexer(file);
    out.AddCompiler(lexer.compiler[0]);
    PWMake::CreateFileInPath(out.ptr, "./build/build.ninja");
}