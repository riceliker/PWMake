#include "../lexer/lexer.hpp"
#include "../ninja/ninja.hpp"
#include "../utils/file.hpp"
#include <string>

namespace PWMake::CLI 
{
    static inline void StepLine(std::string name, float per)
    {
        //std::printf("\r\033[K");
        int num = per * 10.0;
        if (10 >= num && per >= 0)
        {
            std::string sym = "|";
            for (int i = 0; i < num; ++i)
            {
                sym += "=";
            }
            for (int i = 0; i < 10 - num; ++i)
            {
                sym += " ";
            }
            sym += ">";
            std::printf("   ""\033[1;32m""Analyze file""\033[0m""%s|%d%% \n", sym.c_str(), (int)per*100);
        }
    }
    PWMake::Lexer::Lexer AnalyzeFile()
    {
        auto file = PWMake::Lexer::GetTextLinesInFile("./build.pwm");
        std::printf("   ""\033[1;32m""Start analyzing build.pwm\n""\033[0m");
        auto lexer = PWMake::Lexer::Lexer(file);
        return lexer;
    }

    void BuildNinja(PWMake::Lexer::Lexer lexer)
    {
        auto out = PWMake::Ninja::NinjaFile();
        out.AddCompiler(lexer.compiler);
        int size = lexer.project.size();
        int count = 0;
        for (const auto& project: lexer.project)
        {
            out.AddSource(project);
            count += 1;
            StepLine("Creating ninja file\n", (float)count / size);
        }
        //std::printf("\r\033[K");
        std::printf("   ""\033[1;32m""Created ninja file successful\n""\033[0m");
        PWMake::Lexer::CreateFileInPath(out.AsFile(), "./build.ninja");
    }
}