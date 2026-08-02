#include "../lexer/lexer.hpp"
#include "../config/config.hpp"
#include "../ninja/ninja.hpp"
#include "../clangd/clangd.hpp"
#include "../cmd/cmd.hpp"
#include "../utils/file.hpp"
#include "../utils/utils.hpp"
#include <unordered_map>

namespace PWMake::CLI 
{

    PWMake::Lexer::Lexer AnalyzeFile()
    {
        auto config_file = PWMake::Utils::GetTextLinesInFile("./config.pwm");
        auto string_variable = std::unordered_map<std::string, std::string>();
        auto bool_variable = std::unordered_map<std::string, bool>();
        if (config_file.has_value())
        {
            auto config = PWMake::Config::Config(config_file.value());
            string_variable = config.string_variable;
            bool_variable = config.bool_variable;
        }

        auto build_file = PWMake::Utils::GetTextLinesInFile("./build.pwm");
        std::printf("   ""\033[1;32m""Start analyzing build.pwm\n""\033[0m");
        if (build_file.has_value())
        {
            auto lexer = PWMake::Lexer::Lexer(build_file.value(), bool_variable, string_variable);
            return lexer;
        }
        else 
        {
            std::printf("   ""\033[1;33m""Can not found build file: build.pwm""\033[0m");
            std::exit(1);
        }
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
        PWMake::Utils::CreateFileInPath(out.AsFile(), "./build.ninja");
    }

    void CreateClangd(PWMake::Lexer::Lexer lexer)
    {
        auto out = PWMake::Clangd::Clangd();
        out.AddCompilerFlag(lexer.compiler);
        std::printf("   ""\033[1;32m""Created clangd file successful.\n""\033[0m");
        PWMake::Utils::CreateFileInPath(out.AsFile(), "./.clangd");
    }

    void CreateCMD(PWMake::Lexer::Lexer lexer)
    {
        auto out = PWMake::CMD::CMD(lexer.compiler, lexer.project);
        std::printf("   ""\033[1;32m""Created compile_commands.json file successful.\n""\033[0m");
        PWMake::Utils::CreateFileInPath(out.AsFile(), "./compile_commands.json");
    }
}