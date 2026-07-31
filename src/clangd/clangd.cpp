#include "clangd.hpp"
#include <string>
#include <vector>

namespace PWMake::Clangd
{
    Clangd::Clangd()
    {
        this->data = std::vector<std::string>();
        this->data.push_back("# PWMake" + _version);
    }
    void Clangd::AddCompilerFlag(CompilerInfo info)
    {
        this->data.push_back("CompileFlags:");
        this->data.push_back("  CompilationDatabase: .");
        this->data.push_back("  Add:");
        this->data.push_back("   - " + info.compiler_path);
        this->data.push_back("   - -std=" + info.standard_language);
        this->data.push_back("   - -stdlib=" + info.standard_library);

        for (const std::string& lib: info.header)
        {
            this->data.push_back("   - -I" + lib);
        }
    }
}