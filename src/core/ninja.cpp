#include "mod.hpp"
#include <vector>

namespace PWMake
{
    NinjaFile::NinjaFile()
    {
        this->ptr = new std::vector<std::string>();
        this->ptr->push_back("mod_cache = ./build/mods");
        this->ptr->push_back("obj_dir = ./build/obj");
        this->ptr->push_back("target_bin = ./build/app");

        this->ptr->push_back("rule clean");
        this->ptr->push_back("  command = rm -rf $mod_cache $obj_dir $target_bin");
        this->ptr->push_back("  description = Clean all build artifacts");
    }

    void NinjaFile::AddCompiler(CompilerInfo info)
    {
        if (info.compiler_name == "clang")
        {
            // link compiler
            this->ptr->push_back("compiler_path = " + info.compiler_path);
            this->ptr->push_back("clangxx = $llvm_root/bin/clang++");
            this->ptr->push_back("cxxflags = -std=" + info.language_version +" -stdlib=" + info.standard_library);
            this->ptr->push_back("ldflags = -stdlib=" + info.standard_library);
            // init source compiler rule
            this->ptr->push_back("rule src_compile");
            this->ptr->push_back("  command = $clangxx $cxxflags -c $in -o $out");
            this->ptr->push_back("  command = $clangxx $cxxflags -c $in -o $out");
            this->ptr->push_back("  description = Build source $in");
            // init link compiler rule
            this->ptr->push_back("rule link_bin");
            this->ptr->push_back("  command = $clangxx $ldflags $in -o $out");
            this->ptr->push_back("  description = Link executable $out");

        }
    }

}