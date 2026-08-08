#include "../lexer/lexer.hpp"
#include "../config/config.hpp"
#include "../ninja/ninja.hpp"
#include "../clangd/clangd.hpp"
#include "../cmd/cmd.hpp"
#include "../cli/cli.hpp"

#include <unordered_map>
#include <iostream>
#include <fstream>

namespace PWMake::CLI 
{
    static inline std::optional<std::vector<std::string>> GetTextLinesInFile(std::string path)
    {
        std::ifstream file(path);

        if(!file.is_open())
        {
            return std::nullopt;
        }

        std::string line;
        std::string get_line;
        auto file_lines = std::vector<std::string>();

        while (std::getline(file, get_line))
        {
            if (!get_line.empty() && get_line[0] == '#')
                continue;
            if (get_line.back() == '\\')
                line += get_line[get_line.size()-1];
            else 
                line = get_line;
            size_t start = 0;
            while (start < line.size() && std::isspace(static_cast<unsigned char>(line[start])))
                start++;
            line.erase(0, start);
            file_lines.push_back(line);
        }
        file.close();
        return file_lines;
    }

    static inline void CreateFileInPath(std::vector<std::string> text_line, std::string file_path)
    {
        std::ofstream file(file_path);

        if(!file.is_open())
        {
            std::cout << "PWMake: Can not open file:" << file_path << std::endl;
            std::exit(1);
        }

        int line_index = 0;
        for (const auto& line: text_line)
        {   
            file << line << std::endl;
            line_index += 1;
        }
        
        file.close();
    }
    PWMake::Lexer::Lexer AnalyzeFile()
    {
        auto config_file = GetTextLinesInFile("./config.pwm");
        auto string_variable = std::unordered_map<std::string, std::string>();
        auto bool_variable = std::unordered_map<std::string, bool>();
        if (config_file.has_value())
        {
            PWMake::Config::Config config = PWMake::Config::Config(config_file.value());
            string_variable = config.string_variable;
            bool_variable = config.bool_variable;
        }

        auto build_file = GetTextLinesInFile("./build.pwm");
        std::printf("   ""\033[1;32m""Start analyzing build.pwm\n""\033[0m");
        if (build_file.has_value())
        {
            Lexer::VariableMap map = {};
            map.bool_variable = bool_variable;
            map.string_variable = string_variable;
            map.files_variable = std::unordered_map<std::string, std::vector<std::filesystem::path>>();
            auto lexer = PWMake::Lexer::Lexer(build_file.value(), map);
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
            StepLine("Creating ninja file\n", (float)count / size);
        }
        std::printf("\r\033[K");
        std::printf("   ""\033[1;32m""Created ninja file successful.\n""\033[0m");
        CreateFileInPath(out.AsFile(), "./build.ninja");
    }

    void CreateClangd(PWMake::Lexer::Lexer lexer)
    {
        auto out = PWMake::Clangd::Clangd();
        out.AddCompilerFlag(lexer.compiler);
        std::printf("   ""\033[1;32m""Created clangd file successful.\n""\033[0m");
        CreateFileInPath(out.AsFile(), "./.clangd");
    }

    void CreateCMD(PWMake::Lexer::Lexer lexer)
    {
        auto out = PWMake::CMD::CMD(lexer.compiler, lexer.project);
        std::printf("   ""\033[1;32m""Created compile_commands.json file successful.\n""\033[0m");
        CreateFileInPath(out.AsFile(), "./compile_commands.json");
    }
}