:: uninstall script for OTB
set CUR_DIR=%~dp0
cmake -S OUT_DIR/cmake_uninstall_otb.cmake -DMY_INSTALL_DIR=%CUR_DIR% -DCMAKE_COMMAND=cmake
del %CUR_DIR%mapla.bat
del %CUR_DIR%monteverdi.bat
echo "OTB is now uninstalled from %CUR_DIR%"
