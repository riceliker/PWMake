#include "ninja.hpp"
#include "../utils/utils.hpp"

namespace PWMake::Ninja
{
    Ninja::Ninja()
    {
        this->data = std::vector<std::string>();
        this->data.push_back("# PWMake " + _version);
    }

    void Ninja::AddCompiler(CompilerInfo info)
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

        for (const auto& header: info.header)
        {
            compiler_flag += " -I" + header + " ";
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

        this->data.push_back(compiler_flag);
        this->data.push_back("rule compile");
        this->data.push_back("  command = $compiler_path $compiler_flags -c $in -o $out -MD -MF $out.d");
        this->data.push_back("  depfile = $out.d");
        this->data.push_back("  deps = gcc");
        this->data.push_back("rule compile_lib");
        this->data.push_back("  command = $compiler_path $compiler_flags -fPIC -c $in -o $out -MD -MF $out.d");
        this->data.push_back("  depfile = $out.d");
        this->data.push_back("  deps = gcc");

        // create link task
        std::string link_flags = "link_flags =";
        if (info.standard_library != "")
        {
            link_flags += " -stdlib=" + info.standard_library;
        }
        
        this->data.push_back(link_flags);
        this->data.push_back("rule binary");
        this->data.push_back("  command = $compiler_path $in -o $out $link_flags");

        // create static library
        this->data.push_back("ar = ar");
        this->data.push_back("rule static_library");
        this->data.push_back("  command = $ar rc $out $in");

        // create dynamic library
        this->data.push_back("rule shared_library");
        this->data.push_back("  command = $compiler_path -shared $in -o $out $link_flags");
    }

    void Ninja::AddSource(ProjectInfo info)
    {
        this->data.push_back("# ===== Make Object =====");
        this->data.push_back("obj = ./build/obj");
        
        std::string links = "build $bin:";
        std::string name_lib = "";
        std::string name_ext = "";

        // project name
        if (info.project_type == "binary")
        {
            links += " binary ";
            name_ext = "";
        }
        else if (info.project_type == "static")
        {
            links += " static_library ";
            name_ext = ".a";
            name_lib = "lib";
        }
        else if (info.project_type == "shared")
        {
            links += " shared_library ";
            name_lib = "lib";
            if (platform == "windows")
            {
                name_ext = ".dll";
            }
            else if (platform == "macos")
            {
                name_ext = ".dylib";
            }
            else if (platform == "linux")
            {
                name_ext = ".so";
            }
            else 
            {
                std::printf("\033[31m" "Error:" "\033[0m" " Unknown Project Platform.\n");
                std::exit(1);
            }
        }
        else 
        {
            std::printf("\033[31m" "Error:" "\033[0m" " Unknown Project Build Type(%s).\n", info.project_type.c_str());
            std::exit(1);
        }

        this->data.push_back("bin = ./build/" + name_lib + info.project_name + name_ext);
        // source file
        for (const auto& file: info.source_files)
        {
            std::filesystem::path out = file;
            out.replace_extension(".o");
            std::string out_file = "$obj/" + info.project_name + "/" + out.string();
            this->data.push_back(
                "build " + out_file + ": " + "compile " + file.string()
            );
            links.append(out_file + " ");
        }

        for (const auto& lib: info.library)
        {
            links.append(lib + " ");
        }

        if (platform == "windows")
        {
            links.append(" -static -static-libgcc -static-libstdc++");
        }


        this->data.push_back(links);
        this->data.push_back("default $bin");
    }

}