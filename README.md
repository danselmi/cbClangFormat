# cbClangFormat
Code::Blocks Plugin for ClangFormat integration.

## Configuration
![alt text](https://github.com/danselmi/cbClangFormat/raw/master/images/ConfigScreen.png "Configuration dialog")
To configure C::B to use clang-format, one has just to set the path to the executable.
This is only needed if `clang-format` is not in the `PATH`.

This plugin calls `clang-format`with the option `-style=file`. So `clang-format` will try to find the .clang-format
file located in the closest parent directory of the file to format.
Details about the configuration file can be found [here](https://clang.llvm.org/docs/ClangFormatStyleOptions.html).

## Usage
Clang format can be called through the Edit menu or through the context menu as shown here:
![alt text](https://github.com/danselmi/cbClangFormat/raw/master/images/ContextMenu.png "Context Menu")
Where "clang-format" formats the whole file, "clang-format selection" formats only the selected block of text.
"disable clang-format for selection" encloses the selected block with the two lines `// clang-format off` and `// clang-format on` - don't use it too much ;-).

With the help of the "keyboard shortcuts" plugin, one can assugn any keyboard shortcut for a call to clang format in the Edit menu.
