RST docs for Orfeo Toolbox CookBook

Introduction
============

This is a replacement of the old OTB Cookbook which was written in LaTeX. This version has deviated completely from the existing LaTeX format to reStructured format (rst).

Converting existing LaTeX to rst is not that straightforward. All rst files for OTB applications are generated using the Python script otbGenerateWrappersRstDoc.py.
For others files in the Recipes, we used a tool called pandoc to get an initial rst and then manually edit and remove the errors. You do not have to generate them again.
The old Cookbook in otb-documents is now deprecated.

Requirements
============
Install OTB, sphinx, sphinx-rtd-theme:

aptitude install python-sphinx python-sphinx-rtd-theme

How to build the Cookbook:
==========================

You will need a build of OTB with python wrapping enabled (SuperBuild is fine). Then:

    mkdir ~/build-cookbook
    cd ~/build-cookbook
    cmake ~/sources/otb/Documentation/CookBook -DOTB_DIR=/path/where/you/installed/otb/lib/cmake/OTB-X.Y
    make

View results
============

Open the cookbook documentation in a browser tab:

    firefox ~/build-cookbook/rst/_build/html/index.html

To upload it somewhere, copy the _build/html directory.
