Installation
============

Orfeo Toolbox installation is straightforward:

1.   Download otb package for your OS. You can choose between:

   -   A full package corresponding to your operating system in `the package list <https://orfeo-toolbox.org/packages/>`__. Archives are named OTB-<version>-<OS>. **For Windows, "full package" is     the only archive**. You can find detailled explaination at `recommended installation section <#recommended-installation-one-package-containing-all-the-modules>`__.
   -   **Linux users** can choose a lighter installation. You can install only *Core* package, or specifics modules. OTB dependencies can be resolved either with system packages or our dependencies archive (OTB-<version>-Linux-Dependencies in package download list). Commands are detailled at `advanced-installation section <#advanced-installation-modular-installation>`__.

2.   Configure OTB: ``source /Path/To/OTB/otbenv.profile``
3.   Play with OTB in terminal (:doc:`CliInterface`), as QGIS module (:ref:`otbqgis` and :doc:`QGISInterface`) or in python scripts (:doc:`PythonAPI`).

If you want the latests features, you might be interested to compile from source. Detailled steps at :ref:`compilingfromsource`.

Linux
-----

.. include:: Installation_Linux.rst

Windows
-------

.. include:: Installation_Windows.rst

Docker image
------------

A Docker image is available on `DockerHub <https://hub.docker.com/r/orfeotoolbox/otb>`__. It
can be used on any device running Docker, and provides python 3.8. 

**It is the only way to run OTB for MacOS since the Intel architecture is no more in the Mac ecosystem.**

::

    docker pull orfeotoolbox/otb

To run CLI tools, you can launch the docker image with:

::
   
    docker run -it orfeotoolbox/otb


If you are planning on using the image on MacOS with an Apple Silicon chip (M1,M2,M3) :

::
   
   docker run -it --platform=linux/amd64 orfeotoolbox/otb:9.0.0


Distribution packages
---------------------

Warning! These packages may not be up-to-date with latest OTB
releases. In addition, some features of the library may not be available
on every platform. Some of these are not maintained by the OTB-team. If you
want to get involved in the packaging of OTB for your favourite
platform, please contact us through the developer’s mailing list:
otb-developers@googlegroups.com.

There are OTB packages for Debian (unstable) since version 5.2.0. OTB
Applications packages may be available as Debian packages through APT
repositories:

-  otb-bin for command line applications

-  python-otb for python applications

::

    apt install otb-bin python-otb

Due to license issues, the OTB package built in Debian doesn’t contain
6S. As a consequence, the package does not contain the
OpticalCalibration application.

**This is why it is recommended to use the official binary package.**
