rstdocs
=======

RST docs for Orfeo Toolbox CookBook

This file is renamed to README.md even though the syntax is rst and not markdown. The motive is to prevent it from including in the Cookbook

Introduction
============

This is an alternative/replacement of current OTB Cookbook which is written in Latex. This version is completely deviate from existing Latex format to reStructured format (rst).
Home page of rst says, reStructuredText is an easy-to-read, what-you-see-is-what-you-get plaintext markup syntax and parser system. Indeed, every bit is true from our experience.
You can find more about rst, and its syntax from here http://docutils.sourceforge.net/rst.html. Using sphinx build tools, rst can be converted to formats including but not limited to html, pdf, latex!.

Converting existing latex to rst is not that straightforward. All rst files for OTB applications are generated using python script otbGenerateWrappersRstDoc.py.
For others in recipes, we used a tool called pandoc to get an inital rst and then edited out errors manually. You do not have to generate them again.


HowTo generate OTB CookBook in RST
----------------------------------
i) clone OTB-Documents repository

cd $HOME/sources

git clone https://github.com/CS-SI/OTB-Documents

ii). Run cmake to configure cookbook build

mkdir ~/build-cookbook

cd ~/build-cookbook

cmake ~/sources/OTB-Documents/CookBook -DOTB_DIR=/path/where/you/installed/otb/lib/cmake/OTB-5.0

iii). Build Cookbook in RST

make

iv). View results

open the cookbook documentation in a firefox tab!

firefox ~/build-cookbook/rst/_build/html/index.html


TODO: include Applications/app_* file if they exists