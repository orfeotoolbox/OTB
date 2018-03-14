OTB processing in Python
========================

Basics
------

In the ``otbApplication`` module, two main classes can be manipulated :

-  ``Registry``, which provides access to the list of available
   applications, and can create applications.

-  ``Application``, the base class for all applications. This allows to
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
    print "Available applications: "
    print str( otbApplication.Registry.GetAvailableApplications() )

    # Let's create the application  "Smoothing"
    app = otbApplication.Registry.CreateApplication("Smoothing")

    # We print the keys of all its parameters
    print app.GetParametersKeys()

    # First, we set the input image filename
    app.SetParameterString("in", argv[1])

    # The smoothing algorithm can be set with the "type" parameter key
    # and can take 3 values: 'mean', 'gaussian', 'anidif'
    for type in ['mean', 'gaussian', 'anidif']:

      print 'Running with ' + type + ' smoothing type'

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
The Python wrapping only exposes OTB Application engine module (called *ApplicationEngine*) which allows to access existing C++ applications.
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

Applications are often use as parts of larger processing
workflow. Chaining applications currently requires to write/read back
images between applications, resulting in heavy I/O operations and a
significant amount of time dedicated to writing temporary files.

Since OTB 5.8, it is possible to connect an output image parameter
from one application to the input image parameter of the next
parameter. This results in the wiring of the internal ITK/OTB
pipelines together, allowing to perform image streaming between the
applications. There is therefore no more writing of temporary
images. The last application of the processing chain is responsible
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

[since OTB 6.6]

The application framework has been extended in order to provide ways to
interact with the pipelines inside each application. It applies only to
applications that use input or output images. Let's check what are the 
functions added to the ``Application`` class:

* ``GetImageOrigin(key, idx=0)``: returns the origin of the image (physical position of the first pixel center)
* ``GetImageSpacing(key, idx=0)``: returns the signed spacing of the image
* ``GetImageSize(key, idx=0)``: returns the size of the LargestPossibleRegion
* ``GetImageNbBands(key, idx=0)``: returns the number of components per pixel
* ``GetImageProjection(key, idx=0)``: returns the ProjRef
* ``GetImageKeywordlist(key, idx=0)``: returns the Ossim keywordlist
* ``GetImageMetaData(key, idx=0)``: returns the itk::MetaDataDictionary
* ``GetImageRequestedRegion(key, idx=0)``
* ``PropagateRequestedRegion(key, region, idx=0)``: set a given RequestedRegion on the image and propagate it, returns the memory print estimation
* ``GetImageBasePixelType(key, idx=0)``: helper function to guess the pixel type of the underlying Image/VectorImage.
* ``GetParameterImageBase(key, idx=0)``: helper, returns the image as a ImageBaseType (works on all image parameters)
* ``SetParameterImageBase(key, idx=0)``: helper, sets the parameter InputImage/InputImageList/ComplexInputImage with an ImageBaseType

Note: some of these function use an optional index to refer to an image parameter. By default this index is 0. It allows to access InputImageLists transparently.

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
geographic projection. It means that if you want to import back a NumPy array into OTB,
the image won't have any of these metadata. It can be a problem for applications
doing geometry, projections, and also calibration.

Future developments will probably offer a more adapted structure to import and
export images between OTB and the Python world.

Setting of EmptyParameter
^^^^^^^^^^^^^^^^^^^^^^^^^

Most of the parameters are set using functions ``SetParameterXXX()``, except for
one type of parameter: the ``EmptyParameter``. This class was the first
implementation of a boolean. It is now **deprecated**, you should use ``BoolParameter``
instead.

Let's take an example with the application ``ReadImageInfo`` when it was still
using an ``EmptyParameter`` for parameter ``keywordlist``:

.. code-block:: python

    import otbApplication as otb
    app = otb.Registry.CreateApplication("ReadImageInfo")

If you want the get the state of parameter ``keywordlist``, a boolean, use:

.. code-block:: python

    app.IsParameterEnabled("keywordlist")

To set this parameter ON/OFF, use the functions:

.. code-block:: python

    app.EnableParameter("keywordlist")
    app.DisableParameter("keywordlist")

Don't try to use other functions to set the state of a boolean. For instance,
try the following commands:

.. code-block:: python

    app.SetParameterInt("keywordlist", 0)
    app.IsParameterEnabled("keywordlist")

You will get a state ``True`` even if you asked the opposite.
