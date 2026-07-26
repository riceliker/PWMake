# PWMake
PWMake is a simple C/C++ program build system. Basic by Ninja.
*No spend fuck time to make program.* Do you hate Cmake?
If you are hate too, use it is a good idea. It's lite and easy.

## How to use
1. Firstly, you should have a ninja environment.
2. Run the build.ninja in the root of the project.
3. Write build.pwm to create you build task.

## Example
```pwm
@compiler("clang")
  .compiler_path("/opt/homebrew/opt/llvm")
  .language_version("c++23")
  .standard_library("libc++")
;

@set("Source")
ergodic_folder("src", ".cpp")
;

@project("PWMake", "binary")
  .source_files("Source")
;
```
