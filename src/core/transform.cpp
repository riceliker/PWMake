/**
 * lexer.cpp -> transform.cpp -> ninja.cpp
 * This file will read the group type and transform the function name as
 * a struct, then send then to ninja.cpp.
 */

#include "mod.hpp"

namespace PWMake::Core 
{
    CompilerInfo CompilerGroup(std::vector<std::string> group)
    {
        CompilerInfo info = {};
        info.is_warning = false;
        info.is_debug = false;
        for (const auto& line: group)
        {
            auto [name, params] = Function(line);
            if (name == "@compiler")
            {
                info.compiler_path = params->at(0);
            }
            if (name == "compiler_standard_language")
            {
                info.standard_language = params->at(0);
            }
            if (name == "compiler_standard_library")
            {
                info.standard_library = params->at(0);
            }
            if (name == "compiler_warning")
            {
                info.is_warning = true;
            }
            if (name == "compiler_extra_warning")
            {
                info.is_extra_warning = true;
            }
            if (name == "compiler_debug")
            {
                info.is_debug = true;
            }
        }
        return info;
    }

}