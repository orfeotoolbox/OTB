.. _SystemOverview:

System Overview
===============

The purpose of this chapter is to provide you with an overview of the
*ORFEO Toolbox* system. We recommend that you read this chapter to gain
an appreciation for the breadth and area of application of OTB. In this
chapter, we will make reference either to *OTB features* or *ITK
features* without distinction. Bear in mind that OTB uses ITK as its
core element, so all the fundamental elements of OTB come from ITK. OTB
extends the functionalities of ITK for the remote sensing image
processing community. We benefit from the Open Source development
approach chosen for ITK, which allows us to provide an impressive set of
functionalities with much less effort than would have been the case in a
closed source universe!

System Organization
-------------------

The Orfeo Toolbox consists of several subsystems:

Essential System Concepts
    Like any software system, OTB is built around some core design
    concepts. OTB uses those of ITK. Some of the more important concepts
    include generic programming, smart pointers for memory management,
    object factories for adaptable object instantiation, event
    management using the command/observer design paradigm, and
    multithreading support.

Numerics
    OTB, as ITK uses VXL’s VNL numerics libraries. These are easy-to-use
    C++ wrappers around the Netlib Fortran numerical analysis routines
    (http://www.netlib.org).

Data Representation and Access
    Two principal classes are used to represent data: the
    :doxygen:`Image` and :doxygen-itk:`Mesh` classes. In addition,
    various types of iterators and containers are used in ITK to hold
    and traverse the data. Other important but less popular classes are
    also used to represent data such as histograms.

ITK’s Data Processing Pipeline
    The data representation classes (known as *data objects*) are
    operated on by *filters* that in turn may be organized into data
    flow *pipelines*. These pipelines maintain state and therefore
    execute only when necessary. They also support multi-threading, and
    are streaming capable (i.e., can operate on pieces of data to
    minimize the memory footprint).

IO Framework
    Associated with the data processing pipeline are *sources*, filters
    that initiate the pipeline, and *mappers*, filters that terminate
    the pipeline. The standard examples of sources and mappers are
    *readers* and *writers* respectively. Readers input data (typically
    from a file), and writers output data from the pipeline. *Viewers*
    are another example of mappers.

Spatial Objects
    Geometric shapes are represented in OTB using the ITK spatial object
    hierarchy. These classes are intended to support modeling of
    anatomical structures in ITK. OTB uses them in order to model
    cartographic elements. Using a common basic interface, the spatial
    objects are capable of representing regions of space in a variety of
    different ways. For example: mesh structures, image masks, and
    implicit equations may be used as the underlying representation
    scheme. Spatial objects are a natural data structure for
    communicating the results of segmentation methods and for
    introducing geometrical priors in both segmentation and registration
    methods.

ITK’s Registration Framework
    A flexible framework for registration supports four different types
    of registration: image registration, multiresolution registration,
    PDE-based registration, and FEM (finite element method)
    registration.

FEM Framework
    ITK includes a subsystem for solving general FEM problems, in
    particular non-rigid registration. The FEM package includes mesh
    definition (nodes and elements), loads, and boundary conditions.

Level Set Framework
    The level set framework is a set of classes for creating filters to
    solve partial differential equations on images using an iterative,
    finite difference update scheme. The level set framework consists of
    finite difference solvers including a sparse level set solver, a
    generic level set segmentation filter, and several specific
    subclasses including threshold, Canny, and Laplacian based methods.

Wrapping
    ITK uses a unique, powerful system for producing interfaces (i.e.,
    “wrappers”) to interpreted languages such as Tcl and Python. The
    GCC\_XML tool is used to produce an XML description of arbitrarily
    complex C++ code; CSWIG is then used to transform the XML
    description into wrappers using the `SWIG <http://www.swig.org/>`__
    package. OTB does not use this system at present.

Essential System Concepts
-------------------------

This section describes some of the core concepts and implementation
features found in ITK and therefore also in OTB.

Generic Programming
~~~~~~~~~~~~~~~~~~~

Generic programming is a method of organizing libraries consisting of
generic—or reusable—software components. The idea is to make software
that is capable of “plugging together” in an efficient, adaptable
manner. The essential ideas of generic programming are *containers* to
hold data, *iterators* to access the data, and *generic algorithms* that
use containers and iterators to create efficient, fundamental algorithms
such as sorting. Generic programming is implemented in C++ with the
*template* programming mechanism and the use of the STL Standard
Template Library.

C++ templating is a programming technique allowing users to write
software in terms of one or more unknown types ``T``. To create
executable code, the user of the software must specify all types ``T``
(known as *template instantiation*) and successfully process the code
with the compiler. The ``T`` may be a native type such as ``float`` or
``int``, or ``T`` may be a user-defined type (e.g., ``class``). At
compile-time, the compiler makes sure that the templated types are
compatible with the instantiated code and that the types are supported
by the necessary methods and operators.

ITK uses the techniques of generic programming in its implementation.
The advantage of this approach is that an almost unlimited variety of
data types are supported simply by defining the appropriate template
types. For example, in OTB it is possible to create images consisting of
almost any type of pixel. In addition, the type resolution is performed
at compile-time, so the compiler can optimize the code to deliver
maximal performance. The disadvantage of generic programming is that
many compilers still do not support these advanced concepts and cannot
compile OTB. And even if they do, they may produce completely
undecipherable error messages due to even the simplest syntax errors.

Include Files and Class Definitions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In ITK and OTB classes are defined by a maximum of two files: a header
``.h`` file and an implementation file—\ ``.cxx`` if a non-templated
class, and a ``.hxx`` if a templated class. The header files contain
class declarations and formatted comments that are used by the Doxygen
documentation system to automatically produce HTML manual pages.

In addition to class headers, there are a few other important ITK header
files.

``itkMacro.h``
    defines standard system-wide macros (such as ``Set/Get``, constants,
    and other parameters).

``itkNumericTraits.h``
    defines numeric characteristics for native types such as its maximum
    and minimum possible values.

``itkWin32Header.h``
    is used to define operating system parameters to control the
    compilation process.

Object Factories
~~~~~~~~~~~~~~~~

Most classes in OTB are instantiated through an *object factory*
mechanism. That is, rather than using the standard C++ class constructor
and destructor, instances of an OTB class are created with the static
class ``New()`` method. In fact, the constructor and destructor are
``protected:`` so it is generally not possible to construct an OTB
instance on the heap. (Note: this behavior pertains to classes that are
derived from :doxygen-itk:`LightObject`. In some cases the need for
speed or reduced memory footprint dictates that a class not be derived
from LightObject and in this case instances may be created on the heap.
An example of such a class is :doxygen-itk:`EventObject`.)

The object factory enables users to control run-time instantiation of
classes by registering one or more factories with
:doxygen-itk:`ObjectFactoryBase`. These registered factories support the
method ``CreateInstance(classname)`` which takes as input the name of a
class to create. The factory can choose to create the class based on a
number of factors including the computer system configuration and
environment variables. For example, in a particular application an OTB
user may wish to deploy their own class implemented using specialized
image processing hardware (i.e., to realize a performance gain). By
using the object factory mechanism, it is possible at run-time to
replace the creation of a particular OTB filter with such a custom
class. (Of course, the class must provide the exact same API as the one
it is replacing.) To do this, the user compiles their class (using the
same compiler, build options, etc.) and inserts the object code into a
shared library or DLL. The library is then placed in a directory
referred to by the ``OTB_AUTOLOAD_PATH`` environment variable. On
instantiation, the object factory will locate the library, determine
that it can create a class of a particular name with the factory, and
use the factory to create the instance. (Note: if the
``CreateInstance()`` method cannot find a factory that can create the
named class, then the instantiation of the class falls back to the usual
constructor.)

In practice object factories are used mainly (and generally
transparently) by the OTB input/output (IO) classes. For most users the
greatest impact is on the use of the ``New()`` method to create a class.
Generally the ``New()`` method is declared and implemented via the macro
``itkNewMacro()`` found in ``Modules/Core/Common/include/itkMacro.h``.

Smart Pointers and Memory Management
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

By their nature object-oriented systems represent and operate on data
through a variety of object types, or classes. When a particular class
is instantiated to produce an instance of that class, memory allocation
occurs so that the instance can store data attribute values and method
pointers (i.e., the vtable). This object may then be referenced by other
classes or data structures during normal operation of the program.
Typically during program execution all references to the instance may
disappear at which point the instance must be deleted to recover memory
resources. Knowing when to delete an instance, however, is difficult.
Deleting the instance too soon results in program crashes; deleting it
too late and memory leaks (or excessive memory consumption) will occur.
This process of allocating and releasing memory is known as memory
management.

In ITK, memory management is implemented through reference counting.
This compares to another popular approach—garbage collection—used by
many systems including Java. In reference counting, a count of the
number of references to each instance is kept. When the reference goes
to zero, the object destroys itself. In garbage collection, a background
process sweeps the system identifying instances no longer referenced in
the system and deletes them. The problem with garbage collection is that
the actual point in time at which memory is deleted is variable. This is
unacceptable when an object size may be gigantic (think of a large 3D
volume gigabytes in size). Reference counting deletes memory immediately
(once all references to an object disappear).

Reference counting is implemented through a ``Register()``/``Delete()``
member function interface. All instances of an OTB object have a
``Register()`` method invoked on them by any other object that
references an them. The ``Register()`` method increments the instances’
reference count. When the reference to the instance disappears, a
``Delete()`` method is invoked on the instance that decrements the
reference count—this is equivalent to an ``UnRegister()`` method. When
the reference count returns to zero, the instance is destroyed.

This protocol is greatly simplified by using a helper class called a
:doxygen-itk:`SmartPointer`. The smart pointer acts like a regular
pointer (e.g. supports operators ``->`` and ``*``) but automagically
performs a ``Register()`` when referring to an instance, and an
``UnRegister()`` when it no longer points to the instance. Unlike most
other instances in OTB, SmartPointers can be allocated on the program
stack, and are automatically deleted when the scope that the
SmartPointer was created is closed. As a result, you should *rarely if
ever call Register() or Delete()* in OTB. For example:

.. code-block:: cpp

    void MyRegistrationFunction()
    { // Start of scope
      // here an interpolator is created and associated to the
      // SmartPointer "interp".
      InterpolatorType::Pointer interp = InterpolatorType::New();
    } // End of scope

In this example, reference counted objects are created (with the
``New()`` method) with a reference count of one. Assignment to the
SmartPointer ``interp`` does not change the reference count. At the end
of scope, ``interp`` is destroyed, the reference count of the actual
interpolator object (referred to by ``interp``) is decremented, and if
it reaches zero, then the interpolator is also destroyed.

Note that in ITK SmartPointers are always used to refer to instances of
classes derived from :doxygen-itk:`LightObject`. Method invocations and
function calls often return “real” pointers to instances, but they are
immediately assigned to a SmartPointer. Raw pointers are used for
non-LightObject classes when the need for speed and/or memory demands a
smaller, faster class.

Data Representation
-------------------

:doxygen:`Image` represents an *n*-dimensional, regular sampling of
data. The sampling direction is parallel to each of the coordinate axes,
and the origin of the sampling, inter-pixel spacing, and the number of
samples in each direction (i.e., image dimension) can be specified. The
sample, or pixel, type in OTB is arbitrary—a template parameter
``TPixel`` specifies the type upon template instantiation. (The
dimensionality of the image must also be specified when the image class
is instantiated.) The key is that the pixel type must support certain
operations (for example, addition or difference) if the code is to
compile in all cases (for example, to be processed by a particular
filter that uses these operations). In practice the OTB user will use a
C++ simple type (e.g., ``int``, ``float``) or a pre-defined pixel type
and will rarely create a new type of pixel class.

One of the important ITK concepts regarding images is that rectangular,
continuous pieces of the image are known as *regions*. Regions are used
to specify which part of an image to process, for example in
multithreading, or which part to hold in memory. In ITK there are three
common types of regions:

#. ``LargestPossibleRegion`` —the image in its entirety.

#. ``BufferedRegion`` —the portion of the image retained in memory.

#. ``RequestedRegion`` —the portion of the region requested by a filter
   or other class when operating on the image.

The :doxygen:`Image` class extends the functionalities of the
:doxygen-itk:`Image` in order to take into account particular remote
sensing features as geographical projections, etc.

Data Processing Pipeline
------------------------

While data objects (e.g., images) are used to represent data, *process
objects* are classes that operate on data objects and may produce new
data objects. Process objects are classed as *sources*, *filter
objects*, or *mappers*. Sources (such as readers) produce data, filter
objects take in data and process it to produce new data, and mappers
accept data for output either to a file or some other system. Sometimes
the term *filter* is used broadly to refer to all three types.

The data processing pipeline ties together data objects (e.g., images)
and process objects. The pipeline supports an automatic updating
mechanism that causes a filter to execute if and only if its input or
its internal state changes. Further, the data pipeline supports
*streaming*, the ability to automatically break data into smaller
pieces, process the pieces one by one, and reassemble the processed data
into a final result.

Typically data objects and process objects are connected together using
the ``SetInput()`` and ``GetOutput()`` methods as follows:

.. code-block:: cpp

    typedef otb::Image<float,2> FloatImage2DType;

    itk::RandomImageSource<FloatImage2DType>::Pointer random;
    random = itk::RandomImageSource<FloatImage2DType>::New();
    random->SetMin(0.0);
    random->SetMax(1.0);

    itk::ShrinkImageFilter<FloatImage2DType,FloatImage2DType>::Pointer shrink;
    shrink = itk::ShrinkImageFilter<FloatImage2DType,FloatImage2DType>::New();
    shrink->SetInput(random->GetOutput());
    shrink->SetShrinkFactors(2);

    otb::ImageFileWriter::Pointer<FloatImage2DType> writer;
    writer = otb::ImageFileWriter::Pointer<FloatImage2DType>::New();
    writer->SetInput (shrink->GetOutput());
    writer->SetFileName("test.raw");
    writer->Update();

In this example the source object :doxygen-itk:`RandomImageSource` is
connected to the :doxygen-itk:`ShrinkImageFilter`, and the shrink filter
is connected to the mapper :doxygen:`ImageFileWriter`. When the
``Update()`` method is invoked on the writer, the data processing
pipeline causes each of these filters in order, culminating in writing
the final data to a file on disk.
