Using OTB applications
======================

Introduction
------------

The package (see [chap:otb-applications]) has brought a nice set of
applications, along with several tools to launch them (command line,
Qt,…). As for the future of these applications, it has been decided to
integrate them inside the library. This migration has been an
opportunity to enhance all the framework around the applications: not
only new features have been added, but the interface with the developer
has also been simplified. The new framework has inherited the wrappers
from , and new ones have been added. The development philosophy behind
these applications is to provide users with modular functional blocs for
remote sensing, ready to be integrated in any environment.

Because the applications are now a part of the library, their
installation doesn’t require much effort: when building the library, you
can activate the applications with the CMake boolean option .

List of applications
--------------------

The documentation of the available applications is accessible
`here <http://orfeo-toolbox.org/Applications>`__. Most of the old
applications have been migrated. They are sorted by categories.

Available wrappers
------------------

Command line
~~~~~~~~~~~~

By default, the applications can be called with the command line
launcher. This launcher is built in your directory. It needs at least
two arguments: the application name and the path to the directory. Any
additional argument will be given to the application itself. You can ask
the application to print its help message:

::

    otbApplicationLauncherCommandLine Rescale OTB_DIR/bin -help

The help message (but also the
`documentation <http://orfeo-toolbox.org/Applications>`__) will give you
the list of available parameters. Each parameter must be set by giving
the corresponding key followed by its value:

::

    otbApplicationLauncherCommandLine Rescale OTB_DIR/bin -in QB_Toulouse_Ortho_PAN.tif -out QB_Toulouse_rescaled.tif -outmin 0 -outmax 255

An other way to launch your application is to use the auto-generated
scripts located in the directory. There is one for each application,
their name is the same as the application, prefixed with :

::

    otbcli_Rescale -in QB_Toulouse_Ortho_PAN.tif -out QB_Toulouse_rescaled.tif -outmin 0 -outmax 255

If the application has sub-parameters (i.e. parameters contained in
other parameters), their keys must be prefixed by their full tree path.
For instance, the application has a paramater for bicubic interpolation
radius whose key is . This parameter should be called with the path .

Note that some types of parameters allow you to give several values
after the key (they must be separated with whitespaces).

For example an *InputImageList* key, which can be followed by one or
several filenames :

::

    otbApplicationLauncherCommandLine ConcatenateImages OTB_DIR/bin -il GomaAvant.png GomaApres.png -out otbConcatenateImages.tif

Other wrappers
~~~~~~~~~~~~~~

If you want to use the other available wrappers, you have to activate
the corresponding CMake options when building the library:

-  Enable to build a Qt application launcher. It opens a GUI which
   allows you to set the parameters and execute the application. This
   launcher only needs the same two arguments as the command line:

   ::

         otbApplicationLauncherQt Rescale OTB_DIR/bin
         

   There are also auto-generated scripts, similar to command line ones:

   ::

         otbgui_Rescale
         

   It displays a window with several tabs. **[Parameters]** is where you
   set the parameters and execute the application. **[Logs]** is where
   you see the informations given by the application during its
   execution. **[Progress]** is where you see a progress bar of the
   execution (not available for all applications). **[Documentation]**
   is where you find a summary of the application documentation.

-  : **TODO**

-  : **TODO**

-  : **TODO**


