RST docs for Orfeo Toolbox CookBook

Introduction
============

This is a replacement of old OTB Cookbook which was written in Latex. This version is completely deviate from existing Latex format to reStructured format (rst).

Converting existing latex to rst is not that straightforward. All rst files for OTB applications are generated using python script otbGenerateWrappersRstDoc.py.
For others in recipes, we used a tool called pandoc to get an inital rst and then edited out errors manually. You do not have to generate them again.
The old Cookbook in otb-documents is deprecated.

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
