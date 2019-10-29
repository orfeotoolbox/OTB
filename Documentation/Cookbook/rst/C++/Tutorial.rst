Building simple OTB code
========================

Well, that’s it, you’ve just downloaded and installed OTB, lured by the
promise that you will be able to do everything with it. That’s true, you
will be able to do everything but - there is always a *but* - some
effort is required.

OTB uses the very powerful systems of generic programming, many classes
are already available, some powerful tools are defined to help you with
recurrent tasks, but it is not an easy world to enter.

These tutorials are designed to help you enter this world and grasp the
logic behind OTB. Each of these tutorials should not take more than 10
minutes (typing included) and each is designed to highlight a specific
point. You may not be concerned by the latest tutorials but it is
strongly advised to go through the first few which cover the basics
you’ll use almost everywhere.

Hello world
-----------

Let’s start by the typical *Hello world* program. We are going to
compile this C++ program linking to your new OTB.

First, create a new folder to put your new programs (all the examples
from this tutorial) in and go into this folder.

Since all programs using OTB are handled using the CMake system, we need
to create a ``CMakeLists.txt`` that will be used by CMake to compile our
program. An example of this file can be found in the
``OTB/Examples/Tutorials`` directory. The ``CMakeLists.txt`` will be
very similar between your projects.

Open the ``CMakeLists.txt`` file and write in the few lines:

.. code-block:: cmake

    project(Tutorials)

    cmake_minimum_required(VERSION 3.1.0)

    find_package(OTB)
    if(OTB_FOUND)
      include(${OTB_USE_FILE})
    else(OTB_FOUND)
      message(FATAL_ERROR "Cannot build OTB project without OTB.  Please set OTB_DIR.")
    endif(OTB_FOUND)

    add_executable(HelloWorldOTB HelloWorldOTB.cxx )
    target_link_libraries(HelloWorldOTB ${OTB_LIBRARIES})

The first line defines the name of your project as it appears in Visual
Studio (it will have no effect under UNIX or Linux). The second line
loads a CMake file with a predefined strategy for finding OTB. If
the strategy for finding OTB fails, CMake will prompt you for the
directory where OTB is installed in your system. In that case you will
write this information in the ``OTB_DIR`` variable. The line
``include(${USE_OTB_FILE})`` loads the ``UseOTB.cmake`` file to set all
the configuration information from OTB.

The line ``add_executable`` defines as its first argument the name of
the executable that will be produced as result of this project. The
remaining arguments of ``add_executable`` are the names of the source
files to be compiled and linked. Finally, the ``target_link_libraries``
line specifies which OTB libraries will be linked against this project.

:ref:`HelloWorldOTB.cxx`

Once the file is written, run ``ccmake`` on the current directory (that
is ``ccmake ./`` under Linux/Unix). If OTB is on a non standard place,
you will have to tell CMake where it is. Once your done with CMake (you
shouldn’t have to do it anymore) run ``make``.

You finally have your program. When you run it, you will have the *OTB
Hello World !* printed.

Ok, well done! You’ve just compiled and executed your first OTB program.
Actually, using OTB for that is not very useful, and we doubt that you
downloaded OTB only to do that. It’s time to move on to a more advanced
level.

Pipeline basics: read and write
-------------------------------

OTB is designed to read images, process them and write them to disk or
view the result. In this tutorial, we are going to see how to read and
write images and the basics of the pipeline system.

First, let’s add the following lines at the end of the
``CMakeLists.txt`` file:

.. code-block:: cmake

    add_executable(Pipeline Pipeline.cxx )
    target_link_libraries(Pipeline ${OTB_LIBRARIES})

Now, create a ``Pipeline.cxx`` file: :ref:`Pipeline.cxx`.

Once this file is written you just have to run ``make``. The ``ccmake``
call is not required anymore.

Get one image from the ``OTB-Data/Examples`` directory from the OTB-Data
repository. You can get it either by cloning the OTB data repository
(``git clone https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb-data.git``),
but that might be quite long as this also gets the data to run the
tests. Alternatively, you can get it from
http://www.orfeo-toolbox.org/packages/OTB-Data-Examples.tgz. Take for
example get ``QB_Suburb.png``.

Now, run your new program as ``Pipeline QB_Suburb.png output.png``. You
obtain the file ``output.png`` which is the same image as
``QB_Suburb.png``. When you triggered the ``Update()`` method, OTB
opened the original image and wrote it back under another name.

Well…that’s nice but a bit complicated for a copy program!

Wait a minute! We didn’t specify the file format anywhere! Let’s try
``Pipeline QB_Suburb.png output.jpg``. And voila! The output image is a
jpeg file.

That’s starting to be a bit more interesting: this is not just a program
to copy image files, but also to convert between image formats.

You have just experienced the pipeline structure which executes the
filters only when needed and the automatic image format detection.

Now it’s time to do some processing in between.

Filtering pipeline
------------------

We are now going to insert a simple filter to do some processing between
the reader and the writer.

Let’s first add the 2 following lines to the ``CMakeLists.txt`` file:

.. code-block:: cmake

    add_executable(FilteringPipeline FilteringPipeline.cxx )
    target_link_libraries(FilteringPipeline ${OTB_LIBRARIES})

See example :ref:`FilteringPipeline.cxx`

Compile with ``make`` and execute as
``FilteringPipeline QB_Suburb.png output.png``.

You have the filtered version of your image in the ``output.png`` file.

Now, you can practice a bit and try to replace the filter by one of the
150+ filters which inherit from the :doxygen-itk:`ImageToImageFilter`
class. You will definitely find some useful filters here!

Handling types: scaling output
------------------------------

If you tried some other filter in the previous example, you may have
noticed that in some cases, it does not make sense to save the output
directly as an integer. This is the case if you tried the
:doxygen-itk:`CannyEdgeDetectionImageFilter`. If you tried to use it
directly in the previous example, you will have some warning about
converting to unsigned char from double.

The output of the Canny edge detection is a floating point number. A
simple solution would be to used double as the pixel type.
Unfortunately, most image formats use integer typed and you should
convert the result to an integer image if you still want to visualize
your images with your usual viewer (we will see in a tutorial later how
you can avoid that using the built-in viewer).

To realize this conversion, we will use the
:doxygen-itk:`RescaleIntensityImageFilter`.

Add the two lines to the ``CMakeLists.txt`` file:

.. code-block:: cmake

    add_executable(ScalingPipeline ScalingPipeline.cxx )
    target_link_libraries(ScalingPipeline ${OTB_LIBRARIES})

See example :ref:`ScalingPipeline.cxx`

As you should be getting used to it by now, compile with ``make`` and
execute as ``ScalingPipeline QB_Suburb.png output.png``.

You have the filtered version of your image in the ``output.png`` file.

Working with multispectral or color images
------------------------------------------

So far, as you may have noticed, we have been working with grey level
images, i.e. with only one spectral band. If you tried to process a
color image with some of the previous examples you have probably
obtained a deceiving grey result.

Often, satellite images combine several spectral band to help the
identification of materials: this is called multispectral imagery. In
this tutorial, we are going to explore some of the mechanisms used by
OTB to process multispectral images.

Add the following lines in the ``CMakeLists.txt`` file:

.. code-block:: cmake

    add_executable(Multispectral Multispectral.cxx )
    target_link_libraries(Multispectral ${OTB_LIBRARIES})

See example :ref:`Multispectral.cxx`

Compile with ``make`` and execute as
``./Multispectral qb_RoadExtract.tif qb_blue.tif qb_shiftscale.tif``.

Going from raw satellite images to useful products
--------------------------------------------------

Quite often, when you buy satellite images, you end up with several
images. In the case of optical satellite, you often have a panchromatic
spectral band with the highest spatial resolution and a multispectral
product of the same area with a lower resolution. The resolution ratio
is likely to be around 4.

To get the best of the image processing algorithms, you want to combine
these data to produce a new image with the highest spatial resolution
and several spectral band. This step is called fusion and you can find
more details about it in [sec:Fusion]. However, the fusion suppose that
your two images represents exactly the same area. There are different
solutions to process your data to reach this situation. Here we are
going to use the metadata available with the images to produce an
orthorectification as detailed in [sec:Ortho].

First you need to add the following lines in the ``CMakeLists.txt``
file:

.. code-block:: cmake

    add_executable(OrthoFusion  OrthoFusion.cxx)
    target_link_libraries(OrthoFusion ${OTB_LIBRARIES})

See example :ref:`OrthoFusion.cxx`
