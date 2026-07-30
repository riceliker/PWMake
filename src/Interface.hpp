#pragma once

#include <string>
#include <vector>
#include <filesystem>

namespace PWMake
{
    struct CompilerInfo
    {
        std::string compiler_path;
        std::string standard_language;
        std::string standard_library;
        bool is_warning;
        bool is_extra_warning;
        bool is_debug;
        std::vector<std::string> header;
    };

    struct ProjectInfo
    {
        std::string project_name;
        std::string project_type;
        std::vector<std::filesystem::path> source_files;
        std::vector<std::string> library;
    };
}
    