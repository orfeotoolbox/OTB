How to write an application
===========================

This chapter presents the different steps to write your own application,
and the framework surrounding it.

Application design
------------------

The first logical step is to define the role of your application:

-  What is the function of your application? Try to draw a box diagram
   to describe the design of your application. Note that you don’t have
   to worry about opening and saving images (or vector data) files, this
   is handled by the framework.

-  What variables (or data objects) must be exposed outside the
   application? Try to make a list of the inputs, outputs and
   parameters of your application.

Then you should have a good vision of your application pipeline.
Depending on the different filters used, the application can be streamed
and threaded. The threading capabilities can be different between the
filters so there is no overall threading parameter (by default, each
filter has its own threading settings).

It is a different story for streaming. Since the image writers are
handled within the framework and outside the reach of the developer, the
default behaviour is to use streaming. If one of the filters doesn’t
support streaming, it will enlarge the requested output region to the
largest possible region and the entire image will be processed at once.
As a result, the developer doesn’t have to handle streaming nor
threading. However, there is a way to choose the number of streaming
divisions (see section :ref:`appParam`).

Architecture of the class
-------------------------

Every application derives from the class `otb::Wrapper::Application <https://www.orfeo-toolbox.org/doxygen/classotb_1_1Wrapper_1_1Application.html>`_. An application can’t be
templated. It must contain the standard class typedefs and a call to the
``OTB_APPLICATION_EXPORT`` macro.

You need also to define standard macros ``itk::NewMacro`` and
``itk::TypeMacro``.

It is also mandatory to implement three methods in a new application:

-  ``DoInit()``

-  ``DoUpdateParameters()``

-  ``DoExecute()``

DoInit()
~~~~~~~~

This method is called once, when the application is instantiated. It
should contain the following actions:

-  Set the name and the description of the application

-  Fill the documentation and give an example

-  Declare all the parameters

-  Define the documentation link: using for contrib application use ``SetDocLink("docLink")``, for official application use ``SetOfficialDocLink()``.

DoUpdateParameters()
~~~~~~~~~~~~~~~~~~~~

This method is called after every modification of a parameter value.
With the command line launcher, it is called each time a parameter is
loaded. With the Qt launcher, it is called each time a parameter field
is modified. It can be used to maintain consistency and relationship
between parameters (e.g. in ExtractROI: when changing the input image,
maybe the ROI size has to be updated).

DoExecute()
~~~~~~~~~~~

This method contains the real action of the application. This is where
the pipeline must be set up. The application framework provides
different methods to get a value or an object associated to a parameter:

-  ``GetParameterInt(key)`` : get the integer value of a parameter

-  ``GetParameterFloat(key)`` : get the float value of a parameter

-  ``GetParameterString(key)`` : get the string value of a parameter

-  ``GetParameterImage(key)`` : get a pointer to an image object, read
   from the file name given in input

-  …

where ``key`` refers to parameter key, defined using ``AddParameter()``
method in ``DoInit()`` method.

Similar methods exist for binding a data object to an output parameter:

-  ``SetParameterOutputImage(key,data)`` : link the image object to the
   given output parameter

-  ``SetParameterComplexOutputImage(key,data)`` : link the complex image
   object to the given output parameter

-  ``SetParameterOutputVectorData(key,data)`` : link the vector data
   object to the given output parameter

If possible, no filter update should be called inside this function. The
update will be automatically called afterwards : for every image or
vector data output, a writer is created and updated.

.. _appParam:

Parameters selection
~~~~~~~~~~~~~~~~~~~~

In the new application framework, every input, output or parameter
derive from `otb::Wrapper::Parameter <https://www.orfeo-toolbox.org/doxygen/classotb_1_1Wrapper_1_1Parameter.html>`_. The application engine supplies the following types of
parameters:

-  ``ParameterType_Bool`` : parameter storing a boolean.

-  ``ParameterType_Int`` : parameter storing an integer.

-  ``ParameterType_Radius`` : parameter storing a radius.

-  ``ParameterType_Float`` : parameter storing a float.

-  ``ParameterType_String`` : parameter storing character string.

-  ``ParameterType_StringList`` : parameter storing a list of character
   string.

-  ``ParameterType_InputFilename`` : parameter storing an input file
   name.

-  ``ParameterType_InputFilenameList`` : parameter storing a list of
   input file names.

-  ``ParameterType_Directory`` : parameter storing a folder name.

-  ``ParameterType_Group`` : parameter storing children parameters.

-  ``ParameterType_Choice`` : parameter storing a list of choices
   (doesn’t support multi-choice). It also allows to create specific
   sub-parameters for each available choice.

-  ``ParameterType_ListView`` : parameter storing a list of choices
   (support multi-choice and single-choice).

-  ``ParameterType_InputImage`` : parameter storing an input image.

-  ``ParameterType_InputImageList`` : parameter storing a list of input
   image.

-  ``ParameterType_ComplexInputImage`` : parameter storing a complex
   input image.

-  ``ParameterType_InputVectorData`` : parameter storing input vector
   data.

-  ``ParameterType_InputVectorDataList`` : parameter storing a list of
   input vector data.

-  ``ParameterType_InputProcessXML`` : parameter storing an input XML
   file name.

-  ``ParameterType_OutputFilename`` : parameter storing an output file
   name.

-  ``ParameterType_OutputImage`` : parameter storing an output image.

-  ``ParameterType_ComplexOutputImage`` : parameter storing a complex
   output image.

-  ``ParameterType_OutputVectorData`` : parameter storing an output
   vector data.

-  ``ParameterType_OutputProcessXML`` : parameter storing an output XML
   file name.

-  ``ParameterType_RAM`` : parameter storing the maximum amount of RAM
   to be used.

Parameters description
~~~~~~~~~~~~~~~~~~~~~~

Each created parameter has a unique key and several boolean flags to
represent its state. These flags can be used to set a parameter optional
or test if the user has modified the parameter value. The parameters are
created in the ``DoInit()`` method, then the framework will set their
value (either by parsing the command line or reading the graphical user
interface). The ``DoExecute()`` method is called when all mandatory
parameters have been given a value, which can be obtained with “Get”
methods defined in `otb::Wrapper::Application <https://www.orfeo-toolbox.org/doxygen/classotb_1_1Wrapper_1_1Application.html>`_. Parameters are set mandatory (or not) using
``MandatoryOn(key)`` method (``MandatoryOff(key)``).

Some functions are specific to numeric parameters, such as
``SetMinimumParameterIntValue(key,value)`` or
``SetMaximumParameterFloatValue(key,value)``. By default, numeric
parameters are treated as inputs. If your application outputs a number,
you can use a numeric parameter and change its role by calling
``SetParameterRole(key,Role_Output)``.

The input types ``InputImage``, ``InputImageList``,
``ComplexInputImage``, ``InputVectorData`` and ``InputVectorDataList``
store the name of the files to load, but they also encapsulate the
readers needed to produce the input data.

The output types ``OutputImage``, ``ComplexOutputImage`` and
``OutputVectorData`` store the name of the files to write, but they also
encapsulate the corresponding writers.

Composite application
---------------------

The application framework has been extended to allow the implementation
of composite applications : **applications that use other
applications**. The concept is simple : you have two applications A and
B that you want to chain in order to build a third application C. Rather
than writing C by copying the code of A and B, you would like to re-use
applications A and B. This plain example will be re-used in this section
for explanations.

A dedicated class `otb::Wrapper::CompositeApplication <https://www.orfeo-toolbox.org/doxygen/classotb_1_1Wrapper_1_1CompositeApplication.html>`_ exists to create such applications. If you
derive this class to implement application C, you will be able to create
a composite application.

Creating internal applications
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Like with standard applications, you have to write a ``DoInit()``
function. In this function, you should first clean any internal
application with the function ``ClearApplications()`` (the ``DoInit()``
function is called twice in some cases). Then you can instantiate the
internal applications that you want to use (for instance A and B). The
function ``AddApplication()`` will do that, based on :

-  The application type (i.e. its official name, such as ExtractROI,
   BandMath, …)

-  An identifier : like with parameter keys, you have to specify an
   identifier to refer to this internal application. Use the same naming
   conventions as parameters.

-  A description : give a small description of the role of this internal
   application.

Using the function ``GetInternalApplication()``, you can get a pointer
to the internal application corresponding to a given identifier.

In the example given in introduction, we assume that :

-  An internal application of type A has been added with identifier
   ``a``

-  An internal application of type B has been added with identifier
   ``b``

Connecting parameters
~~~~~~~~~~~~~~~~~~~~~

Once you have internal applications, you may want to setup their
parameters. There are typically 3 cases.

You may want to expose a parameter of an internal application as a
parameter of your composite application. Let say you want to expose
parameter ``io.in`` from application ``a`` into your composite
application C with the key ``input``. You can call the function :

``ShareParameter(input,a.io.in)``

As a result, the parameters ``input`` in application C and ``io.in`` in
application ``a`` will point to the same object. Under the two parameter
keys, there is a unique value. These two parameters can be considered as
synchronized.

This leads to the second case : you may want to synchronize two
parameters from internal applications. Let say you want to synchronize
parameter ``field`` from application ``a`` with parameter ``fname`` from
application ``b``. You can call the function :

``Connect(a.field,b.fname)``

Note that the functions ``ShareParameter()`` and ``Connect()`` :

-  Use the same syntax to access internal parameters (“application
   identifier” dot “parameter key”).

-  Shall be used in the DoInit() function, after the internal
   applications have been added.

In this synchronization, the two parameters should have the same type,
or have a similar interface, such as input and output filenames that are
both accessed using ``GetParameterString()`` and
``SetParameterString()``.

This type of connection is a transition to the third case : you may want
to connect the output of an internal application to the input of an
other internal application. Here the difficulty is that the two
parameters to connect probably have different types. Let say you want to
connect parameter ``a.out`` to parameter ``b.in``. The “Connect()”
function may work in favorable cases (see previous paragraph), but for
images, you have two options :

-  Explicitly copy the image pointer from the output image parameter in
   the input image parameter (with functions
   ``SetParameterInputImage()`` and ``GetParameterOutputImage()``). It
   will connect the pipelines in applications A and B, to form an
   “in-memory” connexion. This has to be done between the calls to
   ``DoExecute()`` of application A and B.

-  Use a temporary filename to store the output image ``a.out`` and read
   it with ``b.in``. In this case, you have to manually call the writers
   of parameter ``a.out``.

At the moment, the in-memory connexion of vector data parameters is not
supported.

Orchestration
~~~~~~~~~~~~~

In the ``DoUpdateParameters()`` of your composite application, you can
call the same function on an internal application with the function
``UpdateInternalParameters()``. This is needed only if your internal
applications have a specific behaviour during parameter update.

In the ``DoExecute()`` of your composite application, you have to call
``ExecuteInternal()`` in order to launch each internal application. The
order should be compatible with image parameter connexions. If you want
to do “in-memory” connexions, you can do it between two calls to
``ExecuteInternal()``, for instance :

.. code-block:: cpp

    ExecuteInternal("a");
    GetInternalApplication("b")->SetParameterInputImage("in", GetInternalApplication("a")->GetParameterOutputImage("out"));
    ExecuteInternal("b");

The application :ref:`BundleToPerfectSensor` is a simple example of composite
applications. For a more complex example, you can check the application
TrainImagesClassifier.

Compile your application
------------------------

In order to compile your application you must call the macro
``OTB_CREATE_APPLICATION`` in the *CMakelists.txt* file. This macro
generates the lib *otbapp\_XXX.so*, in
(OTB\_BINARY\_DIR/lib/otb/applications), where *XXX* refers to the class
name.

Execute your application
------------------------

There are different ways to launch applicatons :

CommandLine :
    The command line option is invoked using
    *otbApplicationLauncherCommandLine* executable followed by the
    classname, the application dir and the application parameters.

QT :
    Application can be encapsuled in Qt framework using
    *otbApplicationLauncherQt* executable followed by the classname and
    the application dir.

Python :
    A Python wrapper is also available.

Testing your application
------------------------

It is possible to write application tests. They are quite similar to
filters tests. The macro ``OTB_TEST_APPLICATION`` makes it easy to
define a new test.

Application Example
-------------------

See example :ref:`ApplicationExample.cxx`
