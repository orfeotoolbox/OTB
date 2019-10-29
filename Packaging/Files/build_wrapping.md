# How to recompile OTB Python bindings with your Python version?

The OTB Python bindings are made of:

* a Python module `otbApplication.py`
* a Python C++ extension `_otbApplication.so` (or `_otbApplication.pyd` on Windows)

The Python module should be independent from the Python version you are using,
but the compiled C++ extension is linked to a specific Python library. If this
library is not present on your system, you can recompile the extension to use
your Python version.

The procedure is simple, once you have the following prerequisite:

* A C++ compiler:
    * On Linux: GCC >= 5.0
    * On macos: XCode
    * On Windows: either Visual Studio >= 2015 with the desktop C++ workload,
      or just the MSVC Build Tools.
* CMake >= 3.10
* Python library and headers for your Python version
    * On macos and Windows: they are usually supplied with installers from
      official website
    * On Linux packages: you should look for a `python-dev` package matching
      your Python interpreter version.
* Numpy module for your Python version

The compilation and installation of the C++ extension can be launched from the
command line (on Windows you can use `cmd.exe`):

```
ctest -S build_wrapping.cmake
```

If you want to use a specific Python interpreter:

```
ctest -DPYTHON_EXECUTABLE=/my/own/python -S build_wrapping.cmake
```

Note: on Windows, you have to setup MSVC compiler before calling `ctest`. Use
the `vcvarsall.bat` script with the right architecture (x32 or x64). For
instance:

```
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64
```
