This folder contains all the sources used to compile the SuperBuild. This project is intended for people who want to build OTB from source on platforms where they don't have an easy access to OTB dependencies. The SuperBuild can automatically download and compile the necessary dependencies for OTB.

In order to compile the SuperBuild, you shall :
- go to your build directory
- run cmake using 'path-to-OTB/SuperBuild' as the source folder
- it is advised to give a specific install prefix for the superbuild as all dependencies will be deployed there
- once the configure step is done, nothing is built yet, but the build directory is setup with sub-folders for each dependency.
- run make in order to begin the SuperBuild. For each dependency, CMake will do the following tasks :
  * download source archive or clone repository
  * uncompress sources and patch them if necessary
  * configure, build and install the library.

During the configure step, you have access to a range of parameters that control the build of OTB dependencies. For each dependency 'XXX', there is a variable USE_SYSTEM_XXX to choose if a system install of XXX should be used, or if a SuperBuild version should be compiled. 
If you want to use a system version of XXX, you have to set the corresponding variable USE_SYSTEM_XXX to ON. When your system XXX is installed in default system location, CMake should find it without help. If the location is different, you can specify it using :
- CMAKE_PREFIX_PATH : this is the easiest way to point other install prefixes so that CMake will look into them. This variable may hold a list of paths for each install prefix. For instance, CMAKE_PREFIX_PATH=/opt/local will tell CMake to look in the folders /opt/local/lib, /opt/local/include, ...
- XXX_INCLUDE_DIR, XXX_LIBRARY, XXX_DIR : you can give more specific locations using those variables

Some variables are used to configure the OTB itself. They have the same name as in OTB project. For instance, you have access to the third-party enablement options (OTB_USE_XXX).
