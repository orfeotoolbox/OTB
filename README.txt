Monteverdi was developed 4 years ago in order to provide an integrated
application for capacity building activities (teaching, simple image
manipulation, etc.). Its success went far beyond this initial scope since it
opened the OTB world to a wide range of users who needed a ready to use
graphical tool more than a library of components to write their own processing
chain. With this 4 years of lifetime, we have a lot of feedbacks regarding how
useful the tool was, but also regarding what should be improved to move toward
greater usability and operationnality. 
We therefore decided to rework the Monteverdi concept into a brand new software,
enlightened by this experience.

Here are suggested steps for the Monteverdi2 beginner.

1) Get the software.

            http://orfeo-toolbox.org/otb/download.html

2) The software is compiled using CMake http://www.cmake.org. CMake
   generates Makefiles on Unix platforms, and Windows workspaces (or
   appropriate) build files according to your compiler, operating
   system, and other system variables.

   Create a binary directory, and then run cmake. Provide to CMake
   the source directory of Monteverdi2 and the binary directory that you
   just created.

   Monteverdi2 depends on OTB. You have to fill up the OTB_DIR variable with the
   location of the compiled OTB. If you use ccmake or cmake-gui, you have an
   inteface to enter the value.


Please submit bug reports using the OTB bug tracker at:
            http://bugs.orfeo-toolbox.org

You can get support :

1. Using the OTB community site at http://orfeo-toolbox.org/
2. Joining the otb-users mailing list at http://groups.google.com/group/otb-users

Monteverdi2 source code is available at http://hg.orfeo-toolbox.org/Monteverdi2

There is many ways to join us in the OTB adventure and the more people
contribute, the better the library is for everybody! See at
http://www.orfeo-toolbox.org/otb/contribute.html
