/**
 * lexer.cpp -> transform.cpp -> ninja.cpp
 * This file will read the group type and transform the function name as
 * a struct, then send then to ninja.cpp.
 */

#include "lexer.hpp"
#include <cstddef>
#include <string>
#include <unordered_map>

namespace PWMake::Lexer
{
    inline std::vector<std::filesystem::path> SearchFileInFolder(std::string folder_path, std::string extension)
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

    inline std::vector<std::filesystem::path> RecursionFileInFolder(std::string folder_path, std::string extension)
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

    /* when meet @files to create new files group. */
    void CreateFiles(std::vector<GroupFunction> funcs, VariableMap& map, std::string text, int pc)
    {
        auto list = std::vector<std::filesystem::path>();
        std::string name = "";
        for (const auto& func: funcs)
        {
            if (func.name == "@files")
            {
                /*
                 * @file(name: string)
                 */
                ParamCountIsWant(1, func.params.size(), text, pc);
                name = ParamGetString(func.params[0], 0, text, pc);
            }
            else if (func.name == "add_file")
            {
                /*
                 * add_file(name: string)
                 */
                ParamCountIsWant(1, func.params.size(), text, pc);
                list.push_back(ParamGetString(func.params[0], 0, text, pc));
            }
            else if (func.name == "remove_file")
            {
                /*
                 * remove_file(name: string)
                 */
                ParamCountIsWant(1, func.params.size(), text, pc);
                std::string file = ParamGetString(func.params[0], 0, text, pc);
                int index = 0;
                auto pos = find(list.begin(), list.end(), file);
                list.erase(pos);
            }
            else if (func.name == "foreach_folder")
            {
                ParamCountIsWant(2, func.params.size(), text, pc);
                std::string folder_path = ParamGetString(func.params[0], 0, text, pc);
                std::string file_extension = ParamGetString(func.params[1], 0, text, pc);
                auto temp_list = SearchFileInFolder(folder_path, file_extension);
                for (const auto& file: temp_list)
                {
                    list.push_back(file);
                }
            }
            else if (func.name == "recursion_folder")
            {
                ParamCountIsWant(2, func.params.size(), text, pc);
                std::string folder_path = ParamGetString(func.params[0], 0, text, pc);
                std::string file_extension = ParamGetString(func.params[1], 0, text, pc);
                auto temp_list = RecursionFileInFolder(folder_path, file_extension);
                for (const auto& file: temp_list)
                {
                    list.push_back(file);
                }
            }
            else if (func.name == "print")
            {
                ParamCountIsWant(1, func.params.size(), text, pc);
                std::string msg = ParamGetString(func.params[0], 0, text, pc);
                std::printf("\033[35m" "%s\n" "\033[0m", msg.c_str());
                int i = 0;
                for (const auto& file: list)
                {
                    std::printf("\033[32m" "%d|" "\033[0m" " %s\n"  , i, file.c_str());
                    i += 1;
                }
            }
            else 
            {
                std::printf("\033[31m" "Error:" "\033[0m" " Can Not Found The Extend Files!\n");
                std::printf("In build.pwm:%d\n", pc+1);
                std::printf("%d | %s\n", pc+1, text.c_str());
                std::exit(1);
            }
        }
        map.files_variable.insert({name, list});
    }
    
    CompilerInfo CompilerGroup(std::vector<GroupFunction> funcs, std::string text, int pc)
    {
        CompilerInfo info = {};
        info.is_warning = false;
        info.is_debug = false;

        int offset = 0;
        for (const auto& func: funcs)
        {
            if (func.name == "@compiler")
            {
                ParamCountIsWant(1, func.params.size(), text, pc);
                info.compiler_path = ParamGetString(func.params[0], 0, text, pc);
            }
            else if (func.name == "language_version")
            {
                ParamCountIsWant(1, func.params.size(), text, pc);
                info.standard_language = ParamGetString(func.params[0], 0, text, pc);
            }
            else if (func.name == "standard_library")
            {
                ParamCountIsWant(1, func.params.size(), text, pc);
                info.standard_library = ParamGetString(func.params[0], 0, text, pc);
            }
            else if (func.name == "compiler_warning")
            {
                ParamCountIsWant(0, func.params.size(), text, pc);
                info.is_warning = true;
            }
            else if (func.name == "compiler_extra_warning")
            {
                ParamCountIsWant(0, func.params.size(), text, pc);
                info.is_extra_warning = true;
            }
            else if (func.name == "compiler_debug")
            {
                ParamCountIsWant(0, func.params.size(), text, pc);
                info.is_debug = true;
            }
            else if (func.name == "library_header")
            {
                ParamCountIsWant(1, func.params.size(), text, pc);
                info.header.push_back(ParamGetString(func.params[0], 0, text, pc));
            }
            else if (func.name == "link")
            {
                ParamCountIsWant(1, func.params.size(), text, pc);
                info.links.push_back(ParamGetString(func.params[0], 0, text, pc));
            }
            else if (func.name == "framework")
            {
                ParamCountIsWant(1, func.params.size(), text, pc);
                info.frameworks.push_back(ParamGetString(func.params[0], 0, text, pc));
            }
            else if (func.name == "find_library")
            {
                ParamCountIsWant(1, func.params.size(), text, pc);
                info.find_library.push_back(ParamGetString(func.params[0], 0, text, pc));
            }
            else if (func.name == "link_param")
            {
                std::string link_param = "";
                for (int i = 0; i < func.params.size(); ++i)
                { 
                    link_param += ParamGetString(func.params[i], i, text, pc) + ",";
                }
                link_param.pop_back();
                info.link_params.push_back(link_param);
            }
            else 
            {
                std::printf("\033[31m" "Error:" "\033[0m" " Unknow Function Name(%s) In Compiler Group!\n", func.name.c_str());
                std::printf("In build.pwm:%d\n", pc+1);
                std::printf("%d | %s\n", pc+1, text.c_str());
                std::exit(1);
            }
            offset += 1;
        }
        return info;
    }

    ProjectInfo ProjectGroup(std::vector<GroupFunction> funcs, VariableMap& map, std::string text, int pc)
    {
        ProjectInfo info = {};
        for (const auto& func: funcs)
        {
            if (func.name == "@project")
            {
                ParamCountIsWant(2, func.params.size(), text, pc);
                info.project_name = ParamGetString(func.params[0], 0, text, pc);
                info.project_type = ParamGetString(func.params[1], 0, text, pc);
            }
            else if (func.name == "source_files") 
            {
                ParamCountIsWant(1, func.params.size(), text, pc);
                std::string files = ParamGetString(func.params[0], 0, text, pc);
                if (map.files_variable.find(files) != map.files_variable.end())
                {
                    info.source_files = map.files_variable.at(files);
                }
                else 
                {
                    std::printf("\033[31m" "Error:" "\033[0m" " The Files Name(%s) Is Not Defined.\n", files.c_str());
                    std::printf("In build.pwm:%d\n", pc+1);
                    std::printf("%d | %s\n", pc+1, text.c_str());
                    std::exit(1);
                }
            }
            else if (func.name == "add_library")
            {
                ParamCountIsWant(1, func.params.size(), text, pc);
                info.library.push_back(ParamGetString(func.params[0], 0, text, pc));
            }
            else 
            {
                std::printf("\033[31m" "Error:" "\033[0m" " Unknow Function Name(%s) In Project Group.\n", func.name.c_str());
                std::printf("In build.pwm:%d\n", pc+1);
                std::printf("%d | %s\n", pc+1, text.c_str());
                std::exit(1);
            }
        }
        return info;
    }

}