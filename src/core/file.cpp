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
    
    std::vector<std::string> SearchFileNameInFolder(std::string folder_path, std::string extension)
    {
        auto files = std::vector<std::string>();
        for (const auto& file: std::filesystem::directory_iterator(folder_path))
        {
            if (file.is_regular_file())
                continue;
            std::string file_path = file.path().string();
            std::string file_extension = file.path().extension().string();
            if (file_extension == extension)
            {
                files.push_back(file_path);
            }
        }
        return files;
    }
}

