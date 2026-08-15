![License](https://img.shields.io/badge/License-GPL3.0-purple)
![C++](https://img.shields.io/badge/C++-23-blue)
![Version](https://img.shields.io/badge/Version-0.1.0-green)
# PWMake
PWMake is a simple C/C++ program build system. Basic by Ninja.
*No spend fuck time to make program.* Do you hate Cmake?
If you are hate too, use it is a good idea. It's lite and easy.

## How to get the binary file

### Linux
1. Install new clang or gcc and ninja. Make sure you compiler support C++23
2. Open the bootstrap folder and find build.ninja.linux. Remember modify your compiler paths.
and copy it in root of program folder. Remember to rename it as `build.ninja`.
3. Run `ninja` to bootstrap build PWMake.
4. Open the build folder. You will find the PWMake execute file.

### Macos
1. Install new clang and ninja. Use homebrew is good idea. The system clang is not support C++23.
2. Open the bootstrap folder and find build.ninja.macos. Remember modify your compiler paths.
and copy it in root of program folder. Remember to rename it as `build.ninja`.
3. Run `ninja` to bootstrap build PWMake.
4. Open the build folder. You will find the PWMake execute file.

### Windows
1. Install MSYS2 and MinGW. Yes, MSVC is not support. PWMake hate MSVC.
2. Open the MSYS2 Terminal. Use `pacman -S mingw-w64-clang-x86_64-toolchain` to install clang.
3. Install Ninja too. Use `pacman -S ninja`.
4. Open the bootstrap folder and find build.ninja.windows. Remember modify your compiler paths.
and copy it in root of program folder. Remember to rename it as `build.ninja`.
3. Run `ninja` to bootstrap build PWMake.
4. Open the build folder. You will find the PWMake execute file.

## Example
```pwm
# PWMake 1.0
$bool($value, true)
:if(value)
  print("hello world")
:fi

@compiler(compiler_path)
  language_version("c++23")
  standard_library("libc++")
  compiler_debug()
;

@files("source")
  recursion_folder("src", ".cpp")
;

@project("pwmake", "binary")
  source_files("source")
;
```

## Support
1. Variable system. Support bool and string
2. Support branch sentence. Use if, else, fi.
3. Support foreach and recursion file in folder.
4. Use Group to manager you files.
5. Use config.pwm to avoid read build file.
