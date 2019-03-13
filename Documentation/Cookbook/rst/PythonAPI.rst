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

Basics
------

In the ``otbApplication`` module, two main classes can be manipulated :

-  ``Registry``, which provides access to the list of available
   applications, and can create applications.

-  ``Application``, the base class for all applications. This allows one to
   interact with an application instance created by the ``Registry``.

Here is one example of how to use Python to run the ``Smoothing``
application, changing the algorithm at each iteration.

.. code-block:: python

    #  Example on the use of the Smoothing application
    #

    # We will use sys.argv to retrieve arguments from the command line.
    # Here, the script will accept an image file as first argument,
    # and the basename of the output files, without extension.
    from sys import argv

    # The python module providing access to OTB applications is otbApplication
    import otbApplication

    # otbApplication.Registry can tell you what application are available
    print('Available applications: ')
    print (str( otbApplication.Registry.GetAvailableApplications()))

    # Let's create the application  "Smoothing"
    app = otbApplication.Registry.CreateApplication("Smoothing")

    # We print the keys of all its parameters
    print (app.GetParametersKeys())

    # First, we set the input image filename
    app.SetParameterString("in", argv[1])

    # The smoothing algorithm can be set with the "type" parameter key
    # and can take 3 values: 'mean', 'gaussian', 'anidif'
    for type in ['mean', 'gaussian', 'anidif']:

      print('Running with ' + type + ' smoothing type')

      # Now we configure the smoothing algorithm
      app.SetParameterString("type", type)

      # Set the output filename, using the algorithm type to differentiate the outputs
      app.SetParameterString("out", argv[2] + type + ".tif")

      # This will execute the application and save the output to argv[2]
      app.ExecuteAndWriteOutput()

If you want to handle the parameters from a Python dictionary, you can use the
functions *SetParameters()* and *GetParameters()*.

.. code-block:: python

    params = {"in":"myInput.tif", "type.mean.radius":4}
    app.SetParameters(params)
    params2 = app.GetParameters()

Numpy array processing
----------------------

Input and output images to any OTB application in the form of NumPy array is now possible in OTB Python wrapping.
The Python wrapping only exposes OTB Application engine module (called *ApplicationEngine*) which allows one to access existing C++ applications.
Due to blissful nature of ApplicationEngine's loading mechanism no specific wrapping is required for each application.

NumPy extension to Python wrapping allows data exchange to application as an array rather than a disk file.
Of course, it is possible to load an image from file and then convert it to NumPy
array or just provide a file as explained in the previous section via
Application.SetParameterString(...).

The bridge between NumPy and OTB makes it easy to plug OTB into any image processing chain via Python code that uses
GIS/Image processing tools such as GDAL, GRASS GIS, OSSIM that can deal with NumPy.

Below code reads an input image using Python Pillow library (fork of PIL) and convert it to
NumPy array. The NumPy array is used as an input to the application via
*SetImageFromNumpyArray(...)* method.  The application used in this example is
ExtractROI. After extracting a small area the output image is taken as NumPy
array with *GetImageFromNumpyArray(...)* method thus avoid writing output to a
temporary file.

::

   import sys
   import os
   import numpy as np
   import otbApplication
   from PIL import Image as PILImage

   pilimage = PILImage.open('poupees.jpg')
   npimage = np.asarray(pilimage)
   inshow(pilimage)

   ExtractROI = otbApplication.Registry.CreateApplication('ExtractROI')
   ExtractROI.SetImageFromNumpyArray('in', npimage)
   ExtractROI.SetParameterInt('startx', 140)
   ExtractROI.SetParameterInt('starty', 120)
   ExtractROI.SetParameterInt('sizex', 150)
   ExtractROI.SetParameterInt('sizey', 150)
   ExtractROI.Execute()

   ExtractOutput = ExtractROI.GetImageAsNumpyArray('out')
   output_pil_image = PILImage.fromarray(np.uint8(ExtractOutput))
   imshow(output_pil_image)

In-memory connection
--------------------

Applications are often used as part of larger processing
workflows. Chaining applications currently requires to write/read back
images between applications, resulting in heavy I/O operations and a
significant amount of time dedicated to writing temporary files.

Since OTB 5.8, it is possible to connect an output image parameter
from one application to the input image parameter of the next
parameter. This results in the wiring of the internal ITK/OTB
pipelines together, permitting image streaming between the
applications. Consequently, this removes the need of writing temporary
images and improves performance. Only the last application of the processing chain is responsible
for writing the final result images.

In-memory connection between applications is available both at the C++
API level and using the Python bindings.

Here is a Python code sample which connects several applications together:

.. code-block:: python

    import otbApplication as otb

    app1 = otb.Registry.CreateApplication("Smoothing")
    app2 = otb.Registry.CreateApplication("Smoothing")
    app3 = otb.Registry.CreateApplication("Smoothing")
    app4 = otb.Registry.CreateApplication("ConcatenateImages")

    app1.IN = argv[1]
    app1.Execute()

    # Connection between app1.out and app2.in
    app2.SetParameterInputImage("in",app1.GetParameterOutputImage("out"))

    # Execute call is mandatory to wire the pipeline and expose the
    # application output. It does not write image
    app2.Execute()

    app3.IN = argv[1]

    # Execute call is mandatory to wire the pipeline and expose the
    # application output. It does not write image
    app3.Execute()

    # Connection between app2.out, app3.out and app4.il using images list
    app4.AddImageToParameterInputImageList("il",app2.GetParameterOutputImage("out"));
    app4.AddImageToParameterInputImageList("il",app3.GetParameterOutputImage("out"));

    app4.OUT = argv[2]

    # Call to ExecuteAndWriteOutput() both wires the pipeline and
    # actually writes the output, only necessary for last application of
    # the chain.
    app4.ExecuteAndWriteOutput()

**Note:** Streaming will only work properly if the application internal
implementation does not break it, for instance by using an internal
writer to write intermediate data. In this case, execution should
still be correct, but some intermediate data will be read or written.

Interactions with OTB pipeline
------------------------------

[Since OTB 6.6]

The application framework has been extended in order to provide ways to
interact with the pipelines inside each application. It applies only to
applications that use input or output images. Let's check which  
functions are available in the ``Application`` class. There are lots of getter 
functions:

+---------------------------------+---------------------------------------+
| Function name                   | return value                          |
+=================================+=======================================+
| ``GetImageOrigin(...)``         | origin of the image (physical position|
|                                 | of the first pixel center)            |
+---------------------------------+---------------------------------------+
| ``GetImageSpacing(...)``        | signed spacing of the image           |
+---------------------------------+---------------------------------------+
| ``GetImageSize(...)``           | size of the LargestPossibleRegion     |
+---------------------------------+---------------------------------------+
| ``GetImageNbBands(...)``        | number of components per pixel        |
+---------------------------------+---------------------------------------+
| ``GetImageProjection(...)``     | Projection WKT string                 |
+---------------------------------+---------------------------------------+
| ``GetImageKeywordlist(...)``    | Ossim keywordlist (sensor model)      |
+---------------------------------+---------------------------------------+
| ``GetImageMetaData(...)``       | the entire MetaDataDictionary         |
+---------------------------------+---------------------------------------+
| ``GetImageRequestedRegion(...)``| requested region                      |
+---------------------------------+---------------------------------------+
| ``GetImageBasePixelType(...)``  | pixel type of the underlying          |
|                                 | Image/VectorImage.                    |
+---------------------------------+---------------------------------------+

All these getters functions use the following arguments:

* ``key``: a string containing the key of the image parameter
* ``idx``: an optional index (default is 0) that can be used to access ImageList
  parameters transparently

There is also a function to send orders to the pipeline:

  ``PropagateRequestedRegion(key, region, idx=0)``: sets a given RequestedRegion
  on the image and propagate it, returns the memory print estimation. This function
  can be used to measure the requested portion of input images necessary to produce
  an extract of the full output.

Note: a requested region (like other regions in the C++ API of otb::Image) consists 
of an image index and a size, which defines a rectangular extract of
the full image.

This set of functions has been used to enhance the bridge between OTB images
and Numpy arrays. There are now import and export functions available in
Python that preserve the metadata of the image during conversions to Numpy
arrays:

* ``ExportImage(self, key)``: exports an output image parameter into a Python
  dictionary.
* ``ImportImage(self, key, dict, index=0)``: imports the image from a Python
  dictionary into an image parameter (as a monoband image).
* ``ImportVectorImage(self, key, dict, index=0)``: imports the image from a
  Python dictionary into an image parameter (as a multiband image).

The Python dictionary used has the following entries:

  * ``'array'``: the Numpy array containing the pixel buffer
  * ``'origin'``: origin of the image
  * ``'spacing'``: signed spacing of the image
  * ``'size'``: full size of the image
  * ``'region'``: region of the image present in the buffer
  * ``'metadata'``: metadata dictionary (contains projection, sensor model,...)

Now some basic Q&A about this interface:

    Q: What portion of the image is exported to Numpy array?
    A: By default, the whole image is exported. If you had a non-empty requested
    region (the result of calling PropagateRequestedRegion()), then this region
    is exported.
    
    Q: What is the difference between ImportImage and ImportVectorImage?
    A: The first one is here for Applications that expect a monoband otb::Image.
    In most cases, you will use the second one: ImportVectorImage.
    
    Q: What kind of objects are there in this dictionary export?
    A: The array is a numpy.ndarray. The other fields are wrapped
    objects from the OTB library but you can interact with them in a
    Python way: they support ``len()`` and ``str()`` operator, as well as 
    bracket operator ``[]``. Some of them also have a ``keys()`` function just like
    dictionaries.
    
This interface allows you to export OTB images (or extracts) to Numpy array,
process them  by other means, and re-import them with preserved metadata. Please
note that this is different from an in-memory connection.

Here is a small example of what can be done:

.. code-block:: python

  import otbApplication as otb
  
  # Create a smoothing application
  app = otb.Registry.CreateApplication("Smoothing")
  app.SetParameterString("in",argv[1])
  
  # only call Execute() to setup the pipeline, not ExecuteAndWriteOutput() which would
  # run it and write the output image
  app.Execute()

  # Setup a special requested region
  myRegion = otb.itkRegion()
  myRegion['size'][0] = 20
  myRegion['size'][1] = 25
  myRegion['index'].Fill(10)
  ram = app.PropagateRequestedRegion("out",myRegion)
  
  # Check the requested region on the input image
  print(app.GetImageRequestedRegion("in"))
  
  # Create a ReadImageInfo application
  app2 = otb.Registry.CreateApplication("ReadImageInfo")
  
  # export "out" from Smoothing and import it as "in" in ReadImageInfo
  ex = app.ExportImage("out")
  app2.ImportVectorImage("in", ex)
  app2.Execute()
  
  # Check the result of ReadImageInfo
  someKeys = ['sizex', 'sizey', 'spacingx', 'spacingy', 'sensor', 'projectionref']
  for key in someKeys:
    print(key + ' : ' + str(app2.GetParameterValue(key)))
  
  # Only a portion of "out" was exported but ReadImageInfo is still able to detect the 
  # correct full size of the image


Corner cases
------------

There are a few corner cases to be aware of when using Python wrappers. They are
often limitations, that one day may be solved in future versions. If it
happens, this documentation will report the OTB version that fixes the issue.

Calling UpdateParameters()
^^^^^^^^^^^^^^^^^^^^^^^^^^

These wrappers are made as a mirror of the C++ API, so there is a function
``UpdateParameters()``. Its role is to update parameters that depend on others.
It is called at least once at the beginning of ``Execute()``.

In command line and GUI launchers, this functions gets called each time a
parameter of the application is modified. In Python, this mechanism is not
automated: there are cases where you may have to call it yourself.

Let's take an example with the application ``PolygonClassStatictics``. In this
application, the choices available in the parameter ``field`` depend on the list
of fields actually present in the vector file ``vec``. If you try to set the
parameters ``vec`` and ``field``, you will get an error:

.. code-block:: python

    import otbApplication as otb
    app = otb.Registry.CreateApplication("PolygonClassStatistics")
    app.SetParameterString("vec","../../src/OTB-Data/Input/Classification/variousVectors.sqlite")
    app.SetParameterString("field", "label")

::

  Traceback (most recent call last):
    File "<stdin>", line 1, in <module>
    File "/home/gpasero/Projet_OTB/build/OTB/lib/otb/python/otbApplication.py", line 897, in SetParameterString
      def SetParameterString(self, *args): return _otbApplication.Application_SetParameterString(self, *args)
  RuntimeError: Exception thrown in otbApplication Application_SetParameterString: /home/gpasero/Projet_OTB/src/OTB/Modules/Wrappers/ApplicationEngine/src/otbWrapperListViewParameter.cxx:141:
  itk::ERROR: ListViewParameter(0x149da10): Cannot find label

The error says that the choice ``label`` is not recognized, because ``UpdateParameters()``
was not called after setting the vector file. The solution is to call it before
setting the ``field`` parameter:

.. code-block:: python

    app.UpdateParameters()
    app.SetParameterString("field", "label")

No metadata in NumPy arrays
^^^^^^^^^^^^^^^^^^^^^^^^^^^

With the NumPy module, it is possible to convert images between OTB and NumPy
arrays. For instance, when converting from OTB to NumPy array:

* An ``Update()`` of the underlying ``otb::VectorImage`` is requested. Be aware
  that the full image is generated.
* The pixel buffer is copied into a ``numpy.array``

As you can see, there is no export of the metadata, such as origin, spacing,
geographic projection. It means that if you want to re-import a NumPy array back into OTB,
the image won't have any of these metadata. This can pose problems for applications
that relate to geometry, projections, and also calibration.

Future developments will probably offer a more adapted structure to import and
export images between OTB and the Python world.
