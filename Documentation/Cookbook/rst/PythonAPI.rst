Python API
==========

The applications can also be accessed from Python, through a module
named ``otbApplication``. However, there are technical requirements to use it.
If you use OTB through standalone packages, you should use the supplied
environment script ``otbenv`` to properly setup variables such as
``PYTHONPATH`` and ``OTB_APPLICATION_PATH`` (on Unix systems, don't forget to
source the script). In other cases, you should set these variables depending on
your configuration.

On Unix systems, it is typically available in the ``/usr/lib/otb/python``
directory. Depending on how you installed OTB, you may need to configure the
environment variable ``PYTHONPATH`` to include this directory so that the module
becomes available from Python.

On Windows, you can install the ``otb-python`` package, and the module
will be automatically available from an OSGeo4W shell.

As for the command line and GUI launchers, the path to the application
modules needs to be properly set with the ``OTB_APPLICATION_PATH``
environment variable. The standard location on Unix systems is
``/usr/lib/otb/applications``. On Windows, the applications are
available in the ``otb-bin`` OSGeo4W package, and the environment is
configured automatically so ``OTB_APPLICATION_PATH`` doesn't need to be modified
``OTB_APPLICATION_PATH``.

Once your environment is set, you can use OTB applications from Python, just
like this small example:

.. code-block:: python

    #  Example on the use of the Smoothing application

    # The python module providing access to OTB applications is otbApplication
    import otbApplication as otb

    # Let's create the application with codename "Smoothing"
    app = otb.Registry.CreateApplication("Smoothing")

    # We set its parameters
    app.SetParameterString("in", "my_input_image.tif")
    app.SetParameterString("type", "mean")
    app.SetParameterString("out", "my_output_image.tif")

    # This will execute the application and save the output file
    app.ExecuteAndWriteOutput()

For more information about this Python interface, check the recipe section.

