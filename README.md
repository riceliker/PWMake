# PWMake
PWMake is a simple C/C++ program build system. Basic by Ninja.
*No spend fuck time to make program.* Do you hate Cmake?
If you are hate too, use it is a good idea. It's lite and easy.

## How to use
1. Firstly, you should have a ninja environment.
2. Use ninja to build PWMake.
3. Write build.pwm to create you build task.
4. Use PWMAke to build your task.

## Example
```pwm
@compiler("/opt/homebrew/opt/llvm@19/bin/clang++")
  language_version("c++23")
  standard_library("libc++")
;

@files("Source")
  foreach_folder("src", ".cpp")
;

@project("PWMake", "binary")
  source_files("Source")
;
```
