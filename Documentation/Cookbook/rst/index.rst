Welcome to Orfeo ToolBox!
=========================

`Orfeo ToolBox (OTB) <http://orfeo-toolbox.org>`_ is an open-source project for
state-of-the-art remote sensing. Built on the shoulders of the open-source
geospatial community, it can process high resolution optical, multispectral and
radar images at the terabyte scale. A wide variety of applications are
available: from ortho-rectification or pansharpening, all the way to
classification, SAR processing, and much more!

All of OTB's algorithms are accessible from Monteverdi, QGIS, Python, the
command line or C++. Monteverdi is an easy to use visualization tool with an
emphasis on hardware accelerated rendering for high resolution imagery (optical
and SAR).  With it, end-users can visualize huge raw imagery products and
access all of the applications in the toolbox. From resource limited laptops
to high performance MPI clusters, OTB is available on Windows, Linux and Mac.
It is community driven, extensible and heavily documented.  Orfeo ToolBox is
not a black box!

This is the CookBook documentation for users. If you are new to OTB and
Monteverdi, start here. It will go through how to install OTB on your system,
how to start using Monteverdi and OTB applications to view and process your
data, and recipes on how to accomplish typical remote sensing tasks.
Finally, there is also documentation on every application shipped with OTB.

Get started now with the :doc:`Installation` chapter.

Get help, share your experience and contribute to the Orfeo-Toolbox project by
joining `our community <https://www.orfeo-toolbox.org/community/>`_ and
`users forum <https://forum.orfeo-toolbox.org/>`_.

For other documentation, be sure to read:

- OTB's website: `www.orfeo-toolbox.org <https://www.orfeo-toolbox.org/>`_
- `Doxygen <http://orfeo-toolbox.org/doxygen/>`_, for exhaustive documentation
  of the C++ API.

Table of Contents
=================

.. toctree::
    :maxdepth: 2
    :caption: Get Started

    Installation
    Monteverdi

.. toctree::
    :maxdepth: 2
    :caption: Applications

    CliInterface
    GraphicalInterface
    PythonAPI
    QGISInterface
    Applications

.. toctree::
    :maxdepth: 2
    :caption: Recipes

    recipes/optpreproc.rst
    recipes/sarprocessing.rst
    recipes/residual_registration.rst
    recipes/improc.rst
    recipes/bandmathx.rst
    recipes/contrast_enhancement.rst
    recipes/pbclassif.rst
    recipes/featextract.rst
    recipes/stereo.rst
    recipes/hyperspectral.rst

.. toctree::
    :maxdepth: 2
    :caption: C++ API

    C++/SystemOverview.rst
    C++/Tutorial.rst
    C++/UserGuide.rst
    C++/Examples.rst

    C++/Iterators.rst
    C++/Filters.rst
    C++/StreamingAndThreading.rst
    C++/PersistentFilters.rst
    C++/WriteAnApplication.rst
    C++/AddingNewModules.rst

.. toctree::
    :maxdepth: 1
    :caption: Advanced use

    EnvironmentVariables
    ExtendedFilenames
    CompilingOTBFromSource
    FAQ
    Contributors
