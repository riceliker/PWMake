#include "ninja.hpp"

namespace PWMake::Ninja
{
    NinjaFile::NinjaFile()
    {
        this->data = std::vector<std::string>();
        this->data.push_back("# PWMake 0.1.0");
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

        // create link task
        std::string link_flags = "link_flags =";
        if (info.standard_library != "")
        {
            link_flags += " -stdlib=" + info.standard_library;
        }
        
        this->data.push_back(link_flags);
        this->data.push_back("rule link");
        this->data.push_back("  command = $compiler_path $link_flags $in -o $out");
    }

    void NinjaFile::AddSource(ProjectInfo info)
    {
        this->data.push_back("# ===== Make Object =====");
        this->data.push_back("obj = ./build/obj");
        this->data.push_back("bin = ./build/" + info.project_name);
        std::string links = "build $bin: link ";
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
        this->data.push_back(links);
        this->data.push_back("default $bin");
    }

}