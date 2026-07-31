#include "cmd.hpp"
#include <filesystem>
#include <string>

namespace PWMake::CMD 
{
    CMD::CMD(CompilerInfo cinfo, std::vector<ProjectInfo> pinfos)
    {
        this->data = std::vector<std::string>();
        this->data.push_back("[");

        std::string dictionary = std::filesystem::current_path().string();
        std::string command = "";
        command += cinfo.compiler_path;
        command += " -std=" + cinfo.standard_language;
        command += " -stdlib=" + cinfo.standard_library;
        for (const auto lib: cinfo.header)
        {
            command += " -I" + lib;
        }
        if (cinfo.is_debug)
        {
            command += "-g -O0";
        }

        for (const auto& proj: pinfos)
        {
            for (const auto& _file: proj.source_files)
            {
                std::filesystem::path file = _file;
                std::string cfile = file.string();
                std::string ofile = "build/obj/" + proj.project_name + "/" + file.replace_extension(".o").string();
                std::string dfile = file.replace_extension(".o.d");

                this->data.push_back("  {");

                this->data.push_back("    \"dictionary\": \"" + dictionary + "\",");
                this->data.push_back("    \"command\": \"" + command + 
                    " -c " + cfile + " -o " + ofile + " -MD -MF " + dfile + "\","
                );
                this->data.push_back("    \"file\": \"" + cfile + "\",");
                this->data.push_back("    \"output\": \"" + ofile + "\"");

                this->data.push_back("  },");

            }
        }
        this->data[this->data.size()-1] = "  }";

        this->data.push_back("]");
    }
}