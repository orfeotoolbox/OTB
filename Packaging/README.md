OTB recipes to build standalone binary packages for Windows/Linux/Mac OS X.



##### Create a binary package

After compiling you can generate a standalone binary package for OTB-Applications using this command in the Packaging folder:

```
cmake -DSUPERBUILD_BINARY_DIR=<PathToSuperbuildBinaryDir> \
      -DSUPERBUILD_INSTALL_DIR=<PathToSuperbuildInstallDir> \
      -DCMAKE_INSTALL_PREFIX=`pwd`/../../install

make 
make install

```

This will create the package OTB-*.run inside the *pwd`/../../install* folder.


