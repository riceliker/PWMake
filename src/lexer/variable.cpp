#include "lexer.hpp"

#if defined(__linux__)
inline std::string _platform = "linux";
#endif

#if defined(__APPLE__)
inline std::string _platform = "macos";
#endif

#if defined(_WIN64)
inline std::string _platform = "windows";
#endif

#if defined (__x86_64__)
inline std::string _arch = "amd64";
#endif

#if defined (__aarch64__)
inline std::string _arch = "arm64";
#endif


namespace PWMake::Lexer
{
    ParamOption FindVariable(VariableMap& map, std::string var, std::string text, int line)
    {
        ParamOption param = {};
        var = RemoveBlock(var);
        if (var[0] == '"' && var[var.length()-1] == '"')
        {
            std::string param_str = var.substr(1, var.length()-2);
            param.type = String;
            param.str = param_str;
        }
        else if (var == "true")
        {
            param.type = Bool;
            param.boolean = true;
        }
        else if (var == "false")
        {
            param.type = Bool;
            param.boolean = false;
        }
        else if (map.bool_variable.find(var) != map.bool_variable.end())
        {
            param.type = Bool;
            param.boolean = map.bool_variable.at(var);
        }
        else if (map.string_variable.find(var) != map.string_variable.end())
        {
            param.type = String;
            param.str = map.string_variable.at(var);
        }
        else if (var == "__platform__") 
        {
            param.type = String;
            param.str = _platform;
        }
        else if (var == "__arch__") 
        {
            param.type = String;
            param.str = _arch;
        }
        else if (var[0] == '$')
        {
            
            param.type = String;
            param.str = var.substr(1, var.length()-1);
        }
        else 
        {
            std::printf("\033[31m" "Error:" "\033[0m" " The Variable Is Not Define(%s)!\n", var.c_str());
            std::printf("In build.pwm:%d\n", line);
            std::printf("%d | %s\n", line, text.c_str());
            std::exit(1);
  
        }
        return param;
    }

    /* define a new variable or calculate. */
    void Lexer::RegistryVariable(std::string text, int pc)
    {
        auto func = AnalyzeFunction(this->variable, text, pc);

        /* string variable */
        if (func.name == "$string")
        {
            /*
             * $string(name: def<string>, value: string)
             */
            ParamCountIsWant(2, func.params.size(), text, pc);
            std::string key = ParamGetString(func.params[0], 0, text, pc);
            std::string value = ParamGetString(func.params[1], 1, text, pc);
            this->variable.string_variable.insert({key, value});
        }
        else if (func.name == "$same") 
        {
            /*
             * $same(result: def<bool>, first: string, second: string)
             */
            ParamCountIsWant(3, func.params.size(), text, pc);
            std::string key = ParamGetString(func.params[0], 0, text, pc);
            std::string a = ParamGetString(func.params[1], 0, text, pc);
            std::string b = ParamGetString(func.params[2], 0, text, pc);
            this->variable.bool_variable.insert({key, a==b});
        }
        else if (func.name == "$print") 
        {
            /*
             * $prints(msg: def<string>)
             */
            ParamCountIsWant(1, func.params.size(), text, pc);
            std::string a = ParamGetString(func.params[0], 0, text, pc);
            std::printf("%s\n", a.c_str());
        }
        else if (func.name == "$bool")
        {
            /*
             * $bool(name: def<bool>, value: bool)
             */
            ParamCountIsWant(2, func.params.size(), text, pc);
            std::string key = ParamGetString(func.params[0], 0, text, pc);
            bool value = ParamGetBool(func.params[1], 0, text, pc);
            this->variable.bool_variable.insert({key, value});
        }
        else if (func.name == "$not") 
        {
            /*
             * $not(name: def<bool>, value: bool)
             */
            ParamCountIsWant(2, func.params.size(), text, pc);
            std::string key = ParamGetString(func.params[0], 0, text, pc);
            bool a = ParamGetBool(func.params[1], 0, text, pc);
            this->variable.bool_variable.insert({key, !a});
        }
        else if (func.name == "$and") 
        {
            /*
             * $and(name: def<bool>, first: bool, second: bool)
             */
            ParamCountIsWant(3, func.params.size(), text, pc);
            std::string key = ParamGetString(func.params[0], 0, text, pc);
            bool a = ParamGetBool(func.params[1], 0, text, pc);
            bool b = ParamGetBool(func.params[2], 0, text, pc);
            this->variable.bool_variable.insert({key, a&&b});
        }
        else if (func.name == "$or") 
        {
            /*
             * $or(name: def<bool>, first: bool, second: bool)
             */
            ParamCountIsWant(3, func.params.size(), text, pc);
            std::string key = ParamGetString(func.params[0], 0, text, pc);
            bool a = ParamGetBool(func.params[1], 0, text, pc);
            bool b = ParamGetBool(func.params[2], 0, text, pc);
            this->variable.bool_variable.insert({key, a||b});
        }
        else if(func.name == "$equal")
        {
            /*
             * $and(name: def<bool>, first: bool, second: bool)
             */
            ParamCountIsWant(3, func.params.size(), text, pc);
            std::string key = ParamGetString(func.params[0], 0, text, pc);
            bool a = ParamGetBool(func.params[1], 0, text, pc);
            bool b = ParamGetBool(func.params[2], 0, text, pc);
            this->variable.bool_variable.insert({key, a==b});
        }
        else 
        {
            std::printf("\033[31m" "Error:" "\033[0m" " The Unknown Type Is Not Support! Name:%s\n", func.name.c_str());
            std::printf("In build.pwm:%d\n", pc+1);
            std::printf("%d | %s\n", pc+1, text.c_str());
            std::exit(1);
        }
    }
}