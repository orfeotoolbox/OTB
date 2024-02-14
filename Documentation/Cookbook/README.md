RST docs for Orfeo Toolbox CookBook

Introduction
============

This is a replacement of the old OTB Cookbook which was written in LaTeX. This version has deviated completely from the existing LaTeX format to reStructured format (rst).

Converting existing LaTeX to rst is not that straightforward. All rst files for OTB applications are generated using the Python script otbGenerateWrappersRstDoc.py.
For others files in the Recipes, we used a tool called pandoc to get an initial rst and then manually edit and remove the errors. You do not have to generate them again.
The old Cookbook in otb-documents is now deprecated.

Requirements
============
Install OTB, sphinx, sphinx-rtd-theme and sphinx-rtd-theme-common. You may need to adapt packages names depending on your Linux OS:

```bash
apt-get install python3-sphinx python3-sphinx-rtd-theme sphinx-rtd-theme-common
```

How to build the Cookbook:
==========================

You will need a build of OTB with python wrapping enabled (SuperBuild is fine). Two options:

* You already build otb with superbuild: ```cd /your/otb/build/dir; ccmake .```, enable the **BUILD_COOKBOOK** option.
* You build otb from scratch with superbuild and you can use the ```-DBUILD_COOKBOOK=ON``` option.

View results
============

Open the cookbook documentation in a browser tab:

    firefox ~/build-cookbook/rst/_build/html/index.html

To upload it somewhere, copy the _build/html directory.
