#include "../core/mod.hpp"

namespace PWMake::CLI 
{
    void run()
    {
        auto file = PWMake::Core::GetTextLinesInFile("./build.pwm");
        auto lexer = PWMake::Core::Lexer(file);
        auto out = PWMake::Core::NinjaFile();
        out.AddCompiler(lexer.compiler);
        for (const auto& project: lexer.project)
        {
            out.AddSource(project);
        }
        PWMake::Core::CreateFileInPath(out.AsFile(), "./build.ninja");
    }
}