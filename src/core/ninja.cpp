#include "mod.hpp"
#include <string>
#include <vector>

namespace PWMake::Core
{
    NinjaFile::NinjaFile()
    {
        this->data = std::vector<std::string>();
        this->data.push_back("# ===== The Template File =====");
        this->data.push_back("mod_cache = ./build/mods");
        this->data.push_back("obj_dir = ./build/obj");
        this->data.push_back("target_bin = ./build/app");

        this->data.push_back("rule clean");
        this->data.push_back("  command = rm -rf $mod_cache $obj_dir $target_bin");
        this->data.push_back("  description = Clean template file ");
    }

    std::vector<std::string> NinjaFile::AsFile()
    {
        return this->data;
    }

    void NinjaFile::AddCompiler(CompilerInfo info)
    {
        this->data.push_back("# ===== Compiler =====");
        std::string compiler = "compiler_path = " + info.compiler_path;
        this->data.push_back(compiler);

        // create compile task
        std::string compiler_flag = "compiler_flags =";
        if (info.standard_language != "")
        {
            compiler_flag += " -std=" + info.standard_language;    
        }
        if (info.standard_library != "")
        {
            compiler_flag += " -stdlib=" + info.standard_library;
        }
        if (info.is_debug)
        {
            compiler_flag += " -g -O0";
        } 
        if (info.is_warning)
        {
            compiler_flag += " -Wall";
        }
        if (info.is_extra_warning)
        {
            compiler_flag += " -Wextra";
        }
        

        this->data.push_back("# ===== Compile Task =====");
        this->data.push_back(compiler_flag);
        this->data.push_back("rule compile");
        this->data.push_back("  command = $compiler_path $compiler_flags -c $in -o $out -MD -MF $out.d");
        this->data.push_back("  depfile = $out.d");
        this->data.push_back("  deps = gcc");

        // create link task
        std::string link_flags = "link_flags =";
        if (info.standard_library != "")
        {
            link_flags += " -stdlib=" + info.standard_library;
        }
            
        this->data.push_back("# ===== Link Task =====");
        this->data.push_back("rule link");
        this->data.push_back("  command = $compiler_path $ldflags $in -o $out");

    }

}