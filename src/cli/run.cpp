#include "../lexer/lexer.hpp"
#include "../ninja/ninja.hpp"
#include "../clangd/clangd.hpp"
#include "../cmd/cmd.hpp"
#include "../utils/file.hpp"
#include "../utils/utils.hpp"

namespace PWMake::CLI 
{

    PWMake::Lexer::Lexer AnalyzeFile()
    {
        auto file = PWMake::Lexer::GetTextLinesInFile("./build.pwm");
        std::printf("   ""\033[1;32m""Start analyzing build.pwm\n""\033[0m");
        auto lexer = PWMake::Lexer::Lexer(file);
        return lexer;
    }

    void CreateNinja(PWMake::Lexer::Lexer lexer)
    {
        auto out = PWMake::Ninja::Ninja();
        out.AddCompiler(lexer.compiler);
        int size = lexer.project.size();
        int count = 0;
        for (const auto& project: lexer.project)
        {
            out.AddSource(project);
            count += 1;
            Utils::StepLine("Creating ninja file\n", (float)count / size);
        }
        std::printf("\r\033[K");
        std::printf("   ""\033[1;32m""Created ninja file successful.\n""\033[0m");
        PWMake::Lexer::CreateFileInPath(out.AsFile(), "./build.ninja");
    }

    void CreateClangd(PWMake::Lexer::Lexer lexer)
    {
        auto out = PWMake::Clangd::Clangd();
        out.AddCompilerFlag(lexer.compiler);
        std::printf("   ""\033[1;32m""Created clangd file successful.\n""\033[0m");
        PWMake::Lexer::CreateFileInPath(out.AsFile(), "./.clangd");
    }

    void CreateCMD(PWMake::Lexer::Lexer lexer)
    {
        auto out = PWMake::CMD::CMD(lexer.compiler, lexer.project);
        std::printf("   ""\033[1;32m""Created compile_commands.json file successful.\n""\033[0m");
        PWMake::Lexer::CreateFileInPath(out.AsFile(), "./compile_commands.json");
    }
}