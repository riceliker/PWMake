/*
 * file.cpp: <build.pwm> -> file -> lexer
 * Any file operaction will be run here.
 */
#include "mod.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

namespace PWMake::Core
{
    std::vector<std::string> GetTextLinesInFile(std::string path)
    {
        std::ifstream file(path);

        if(!file.is_open())
        {
            std::cout << "PWMake: Can not open file:" << path << std::endl;
            std::exit(1);
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

    void CreateFileInPath(std::vector<std::string> text_line, std::string file_path)
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
    
    std::vector<std::filesystem::path> SearchFileInFolder(std::string folder_path, std::string extension)
    {
        auto files = std::vector<std::filesystem::path>();
        if (!std::filesystem::exists(folder_path))
        {
            std::printf("\033[31m" "Error:" "\033[0m" " The Path(%s) Is No Found!\n", folder_path.c_str());
            std::exit(1);
        }
        if (!std::filesystem::is_directory(folder_path))
        {
            std::printf("\033[31m" "Error:" "\033[0m" " The Path(%s) Is No A Folder!\n", folder_path.c_str());
            std::exit(1);
        }
        for (const auto& file: std::filesystem::directory_iterator(folder_path))
        {
            if (!file.is_regular_file())
                continue;
            std::string file_path = file.path().string();
            std::string file_extension = file.path().extension().string();
            if (file_extension == extension)
            {
                files.push_back(file.path());
            }
        }
        return files;
    }

    std::vector<std::filesystem::path> RecursionFileInFolder(std::string folder_path, std::string extension)
    {
        auto files = std::vector<std::filesystem::path>();
        if (!std::filesystem::exists(folder_path))
        {
            std::printf("\033[31m" "Error:" "\033[0m" " The Path(%s) Is No Found!\n", folder_path.c_str());
            std::exit(1);
        }
        if (!std::filesystem::is_directory(folder_path))
        {
            std::printf("\033[31m" "Error:" "\033[0m" " The Path(%s) Is No A Folder!\n", folder_path.c_str());
            std::exit(1);
        }
        std::filesystem::path root = folder_path;
        for (const auto& entry : std::filesystem::recursive_directory_iterator(root))
        {
            if (std::filesystem::is_regular_file(entry))
            {
                auto file = entry.path();
                std::string file_path = file.string();
                std::string file_extension = file.extension().string();
                if (file_extension == extension)
                {
                    files.push_back(file);
                }
            }
        }
        return files;
    }
}

