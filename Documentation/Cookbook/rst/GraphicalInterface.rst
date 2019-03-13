Graphical interface
===================

The graphical interface for the applications provides a useful
interactive user interface to set the parameters, choose files, and
monitor the execution progress.

This launcher needs the same two arguments as the command line launcher:

::

    $ otbApplicationLauncherQt module_name [MODULEPATH]

The application paths can be set with the ``OTB_APPLICATION_PATH``
environment variable, as for the command line launcher. Also, as for the
command-line application, a more simple script is generated and
installed by OTB to ease the configuration of the module path: to
launch the graphical user interface, one will start the
``otbgui_Rescale`` script.

The resulting graphical application displays a window with several tabs:

-  Parameters is where you set the parameters and execute the
   application.

-  Logs is where you see the output given by the application during its
   execution.

-  Progress is where you see a progress bar of the execution (not
   available for all applications).

-  Documentation is where you find a summary of the application
   documentation.

In this interface, every optional parameter has a check box that you
have to tick if you want to set a value and use this parameter. The
mandatory parameters cannot be unchecked.

The interface of the application is shown here as an example:

.. figure:: Art/QtImages/rescale_param.png
    :align: center
