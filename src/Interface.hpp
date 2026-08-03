#pragma once

#include <string>
#include <vector>
#include <filesystem>

inline const std::string _version = "0.1.0";

namespace PWMake
{
    class IMakeFile
    {
    protected:
        std::vector<std::string> data;
    public:
        virtual std::vector<std::string> AsFile()
        {
            return this->data;
        }
    };

    struct CompilerInfo
    {
        std::string compiler_path;
        std::string standard_language;
        std::string standard_library;
        bool is_warning;
        bool is_extra_warning;
        bool is_debug;
        std::vector<std::string> header;
        std::vector<std::string> links;
        std::vector<std::string> frameworks;
    };

    struct ProjectInfo
    {
        std::string project_name;
        std::string project_type;
        std::vector<std::filesystem::path> source_files;
        std::vector<std::string> library;

    };
}
    