Frequently Asked Questions
==========================

Introduction
------------

What's in OTB?
~~~~~~~~~~~~~~

-  Image access: optimized read/write access for most of remote sensing
   image formats, meta-data access, simple visualization;

-  Sensor geometry: sensor models, cartographic projections;

-  Radiometry: atmospheric corrections, vegetation indices;

-  Filtering: blurring, denoising, enhancement;

-  Fusion: image pansharpening;

-  Feature extraction: interest points, alignments, lines;

-  Image segmentation: region growing, watershed, level sets;

-  Classification: K-means, SVM, Markov random fields;

-  Change detection.

-  Object based image analysis.

-  Geospatial analysis.

For a full list of applications see the chapter :ref:`apprefdoc`.
For an introduction to the C++ API see the
`Software Guide <https://www.orfeo-toolbox.org/SoftwareGuide/>`_.
And for an exhaustive description of the C++ API see the
`Doxygen <https://www.orfeo-toolbox.org/doxygen/>`_.

What is ORFEO?
~~~~~~~~~~~~~~

ORFEO stands for Optical and Radar Federated Earth Observation. In 2001
a cooperation program was set between France and Italy to develop ORFEO,
an Earth observation dual system with metric resolution: Italy is in
charge of COSMO-Skymed the radar component development, and France of
PLEIADES the optic component.

The PLEIADES optic component is composed of two “small satellites” (mass
of one ton) offering a spatial resolution at nadir of 0.7 m and a field
of view of 20 km. Their great agility enables a daily access all over
the world, essentially for defense and civil security applications, and
a coverage capacity necessary for the cartography kind of applications
at scales better than those accessible to SPOT family satellites.
Moreover, PLEIADES have stereoscopic acquisition capacity to meet the
fine cartography needs, notably in urban regions, and to bring more
information when used with aerial photography.

The ORFEO “targeted” acquisition capacities made it a system
particularly adapted to defense or civil security missions, as well as
critical geophysical phenomena survey such as volcanic eruptions, which
require a priority use of the system resources.

With respect to the constraints of the Franco-Italian agreement,
cooperation have been set up for the PLEIADES optical component with
Sweden, Belgium, Spain and Austria.

Where can I get more information about ORFEO?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

At the PLEIADES HR web site: http://smsc.cnes.fr/PLEIADES/.

What is the ORFEO Accompaniment Program?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Beside the Pleiades (PHR) and Cosmo-Skymed (CSK) systems developments
forming ORFEO, the dual and bilateral system (France - Italy) for Earth
Observation, the ORFEO Accompaniment Program was set up, to prepare,
accompany and promote the use and the exploitation of the images derived
from these sensors.

The creation of a preparatory program is needed because of:

-  the new capabilities and performances of the ORFEO systems (optical
   and radar high resolution, access capability, data quality,
   possibility to acquire simultaneously in optic and radar),

-  the implied need of new methodological developments: new processing
   methods, or adaptation of existing methods,

-  the need to realize those new developments in very close cooperation
   with the final users, the integration of new products in their
   systems.

This program was initiated by CNES mid-2003 and will last until mid
2013. It consists in two parts, between which it is necessary to keep a
strong interaction:

-  A Methodological part,

-  A Thematic part.

This Accompaniment Program uses simulated data (acquired during airborne
campaigns) and satellite images quite similar to Pleiades (as QuickBird
and Ikonos), used in a communal way on a set of special sites. The
validation of specified products and services will be realized with
Pleiades data

Apart from the initial cooperation with Italy, the ORFEO Accompaniment
Program enlarged to Belgium, with integration of Belgian experts in the
different WG as well as a participation to the methodological part.

Where can I get more information about the ORFEO Accompaniment Program?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Go to the following web site:
http://smsc.cnes.fr/PLEIADES/A_prog_accomp.htm.

Who is responsible for OTB's development?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The French Centre National d’Études Spatiales, CNES, initiated the ORFEO
Toolbox and is responsible for the specification of the library. CNES
funds the industrial development contracts and research contracts needed
for the evolution of OTB.

License
-------

What is OTB's license?
~~~~~~~~~~~~~~~~~~~~~~

OTB is distributed under the permissive open source license Apache
v2.0 - aka Apache Software License (ASL) v2.0: http://www.apache.org/licenses/LICENSE-2.0

Am I forced to distribute my code based on OTB?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

No. The license gives you the option to distribute your application if
you want to. You do not have to exercise this option in the license.

Am I forced to contribute my code based on OTB into the official repo?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

No.

If I wanted to distribute an application using OTB what license would I need to use?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The license of your choice. The OTB license only requires you to include
a copy of the Apache license and to provide a clear attribution to the
OTB project in any distribution including a piece of OTB software.

I am a commercial user. Is there any restriction on the use of OTB?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

No. The OTB license only requires you to include a copy of the Apache
license and to provide a clear attribution to the OTB project in any
distribution including a piece of OTB software.

Getting OTB
-----------

Who can download OTB?
~~~~~~~~~~~~~~~~~~~~~

Anybody can download OTB at no cost.

Where can I download OTB?
~~~~~~~~~~~~~~~~~~~~~~~~~

Go to http://www.orfeo-toolbox.org and follow the “download OTB” link.
You will have access to the OTB source code, to the Software User’s
Guide and to the Cookbook of the last release. Binary packages are also
provided for the current version. OTB and Monteverdi are also integrated
in OSGeo-Live since version 4.5. You can find more information about the
project at http://live.osgeo.org/. Moreover you can found the last
release of Monteverdi and OTB applications through the OSGeo4W
installer.

How to get the latest bleeding-edge version?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

You can get the current development version, as our repository is
public, using Git (available at http://git-scm.com). Be aware that, even
if the golden rule is *what is committed will compile*, this is not
always the case. Changes are usually more than ten per day.

The first time, you can get the source code using:

::

          git clone https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb.git

Then you can build OTB as usual using this directory as the source
(refer to build instructions). Later if you want to update your source,
from OTB's source directory, just do:

::

          git pull

A simple ``make`` in your OTB binary directory will be enough to update
the library (recompiling only the necessary files).

Special issues about compiling OTB from source
----------------------------------------------

All information about OTB compilation can be found in the Software Guide.
We present here only the special issues which can be encountered.

Debian Linux / Ubuntu
~~~~~~~~~~~~~~~~~~~~~

On some Debian and Ubuntu versions, the system GDAL library and its tiff
internal symbol might conflict with the system Tiff library
(bugs.debian.org/558733). This is most likely the case if you get odd
segmentation fault whenever trying to open a tiff image. This symbol
clash happens when using OTB. A workaround to the issue has been
provided in GDAL sources, but is available in the 1.9.0 release.

The recommended procedure is to get this recent source and build GDAL
from sources, with the following configure command:

::

          ./configure --prefix=INSTALL_DIR --with-libtiff=internal
                      --with-geotiff=internal
                      --with-rename-internal-libtiff-symbols=yes
                      --with-rename-internal-libgeotiff-symbol=yes


Errors when compiling internal libkml
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The internal version of libkml cannot be compiled when using an external
build of ITK. See http://bugs.orfeo-toolbox.org/view.php?id=879 for more
details.

To workaround the problem, either use an external build of libkml (it is
provided on most systems), or use an internal build of ITK by setting to
OFF the CMake variable OTB\_USE\_EXTERNAL\_ITK.

OTB compilation and Windows platform
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To build OTB on Windows, you should prepare an environment with the following
tools:

* Visual Studio 2015 or later
* CMake 3.1 or later
* OTB XDK : download a Windows binary package of OTB and use the supplied
  uninstall script to remove OTB binaries and headers. Now, this package only
  contains the dependencies needed to build OTB.

Then, you can download OTB sources (preferably, a version compatible with your
XDK), and compile them as a standard CMake project. More details are available
in the SoftwareGuide.

There is an other solution, using OSGeo4W distribution. However, the
dependencies may be outdated.

Using OTB
---------

What is the image size limitation of OTB ?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The maximum physical space a user can allocate depends on her platform.
Therefore, image allocation in OTB is restricted by image dimension,
size, pixel type and number of bands.

Fortunately, thanks to the streaming mechanism implemented within OTB’s
pipeline (actually ITK’s), this limitation can be bypassed. The use of
the at the end of the pipeline, will seamlessly break the large,
problematic data into small pieces whose allocation is possible. These
pieces are processed one after the other, so that there is not
allocation problem anymore. We are often working with images of
:math:`25000 \times 25000` pixels.

For the streaming to work, all the filters in the pipeline must be
streaming capable (this is the case for most of the filters in OTB). The
output image format also need to be streamable (not PNG or JPEG, but
TIFF or ENVI formats, for instance).

The class manage the steaming process following two strategies: by tile
or by strip. Different size configuration for these two strategies are
available into the interface. The default mode use the information about
how the file is streamed on the disk and will try to minimize the memory
consumption along the pipeline. More information can be found into the
documentation of the class.

Problems using OTB python wrapping along with other software
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If you use OTB standalone binaries, there should not be any dependency conflict
with other libraries installed on your system. OTB will always try to grab
supplied libraries in the standalone package.

However, when using Python wrappings, there can be conflicts if you import
*otbApplications* along with other software that share common dependencies with
OTB. For instance, if you want to use OTB Applications and Fiona in a Python
script, they both rely on GDAL library. As the libraries loaded by Python must
be unique, the first library *SomeLib* loaded will be used by any other binary
depending on it. Thus, the order of the imports has an effect. In some cases,
symbol problems have been observed in libcrypto, and the solution was to import
OTB Applications before importing Fiona.

Getting help
------------

Is there any mailing list?
~~~~~~~~~~~~~~~~~~~~~~~~~~

Yes. There is a discussion group at
http://groups.google.com/group/otb-users/ where you can get help on the
set up and the use of OTB.

Which is the main source of documentation?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The main source of documentation is the CookBook located at
https://www.orfeo-toolbox.org/CookBook/.

Secondly there is the OTB Software Guide which can be
found at https://www.orfeo-toolbox.org/SoftwareGuide/
It contains many examples and a tutorial which should be a good
starting point for any new OTB user. The code source for these examples
is distributed with the toolbox. Another information source is the
on-line API documentation which is available at
http://www.orfeo-toolbox.org/doxygen.

You can also find some information about how to use Monteverdi and the
OTB-Applications into the Cookbook at
http://www.orfeo-toolbox.org/CookBook/.

Contributing to OTB
-------------------

I want to contribute to OTB, where to begin?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

There are many ways to join us in the OTB adventure. The more people
contribute, the better the library is for everybody!

First, you can send an email to the user mailing list
(otb-users@googlegroups.com) to let us know what functionality you would
like to introduce in OTB. If the functionality seems important for
OTB users, we will then discuss on how to retrieve your code, make the
necessary adaptions, check with you that the results are correct and
finally include it in the next release.

You can also run the nightly tests so we have a wider range of platforms
to detect bugs earlier.

You can also find more information about how to contribute at
https://www.orfeo-toolbox.org/community

What are the benefits of contributing to OTB?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Besides the satisfaction of contributing to an open source project, we
will include the references to relevant papers in the software guide.
Having algorithms published in the form of reproducible research helps
science move faster and encourages people who needs your algorithms to
use them.

You will also benefit from the strengths of OTB: multi-platform,
streaming and threading, etc.

What functionality can I contribute?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

All functionalities which are useful for remote sensing data are of
interest. As OTB is a library, it should be generic algorithms: change,
detection, fusion, object detection, segmentation, interpolation, etc.

More specific applications can be contributed using the framework
directly in the Applications directory of OTB.

Running the tests
-----------------

What are the tests?
~~~~~~~~~~~~~~~~~~~

OTB is an ever changing library, it is quite active and have scores of
changes per day from different people. It would be a headache to make
sure that the brand new improvement that you introduced didn’t break
anything, if we didn’t have automated tests. You also have to take into
account differences in OS, compilers, options, versions of external
libraries, etc. By running the tests and submitting it to the dashboard,
you will help us detect problems and fix them early.

For each class, at minimum there is a test which tries to instantiate it
and another one which uses the class. The output of each test (image,
text file, binary file) is controlled against a baseline to make sure
that the result hasn’t changed.

All OTB tests source code are available in the directory ``Testing`` and
are also good examples on how to use the different classes.

How to run the tests?
~~~~~~~~~~~~~~~~~~~~~

There is more than 2500 tests for OTB and it takes from 20 minutes to 3
hours to run all the test, mainly depending on your compilation options
(Release mode does make a difference) and of course your hardware.

To run the tests, you first have to make sure that you set the option
``BUILD_TESTING`` to ``ON`` before building the library. If you want to
modify it, just rerun ccmake, change the option, then make.

For some of the tests, you also need the test data and the baselines
(see [sec:FAQTestData]).

Once OTB is built with the tests, you just have to go to the binary
directory where you built OTB and run ``ctest -N`` to have a list of all
the tests. Just using ``ctest`` will run all the tests. To select a
subset, you can do ``ctest -R Kml`` to run all tests related to kml
files or ``ctest -I 1,10`` to run tests from 1 to 10.

How to get the test data?
~~~~~~~~~~~~~~~~~~~~~~~~~

Data used for the tests are also versioned using Git (see [sec:FAQGit]).

You can get the base doing:

::

          git clone https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb-data.git

This is about 1 GB of data, so it will take a while, but you have to do
it only once, as after, a simple

::

          git pull

will update you to the latest version of the repository.

You can also easily synchronize the directory you retrieve between
different computers on your network, so you don’t have to get it several
times from the main server. Check out Git capabilities.

How to submit the results?
~~~~~~~~~~~~~~~~~~~~~~~~~~

Once you know how to run the tests, you can also help us to detect the
bugs or configuration problems specific to your configuration. As
mentioned before, the possible combinations between OS, compiler,
options, external libraries version is too big to be tested completely,
but the more the better.

You just have to launch ctest with the ``-D Experimental`` switch.
Hence:

::

          ctest -D Experimental -A CMakeCache.txt

And you will be able to see the result at

http://dash.orfeo-toolbox.org/Dashboard/index.php?project=OTB.

If you are interested in setting up a nightly test (automatically
launched every night), please contact us and we will give you the
details.

What features will the OTB include and when?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

There is no detailed plan about the availability of OTB new features,
since OTB’s content depends on ongoing research work and on feedback
from thematic users of the ORFEO Accompaniment Program. You can find ideas and
plans for the future on the Wishlist at https://wiki.orfeo-toolbox.org/index.php/Wishlist.

