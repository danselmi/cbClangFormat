# cbClangFormat
Code::Blocks Plugin for ClangFormat integration.

## Configuration
![alt text](https://github.com/danselmi/cbClangFormat/images/ConfigScreen.png "Configuration dialog")
To configure C::B to use clang-format, one has just to set the path to the executable. 
This is only needed if `clang-format` is not in the `PATH`.

This plugin calls `clang-format`with the option `-style=file`. So `clang-format` will try to find the .clang-format
file located in the closest parent directory of the file to format. 
Details about the configuration file can be found[here](https://clang.llvm.org/docs/ClangFormatStyleOptions.html).

## Usage
Context Menu
![alt text](https://github.com/danselmi/cbClangFormat/images/ContextMenu.png "Context Menu")
