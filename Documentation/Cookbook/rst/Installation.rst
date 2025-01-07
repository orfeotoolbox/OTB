Installation
============

Orfeo Toolbox installation is straightforward:

1.   Download otb package for your OS. You can choose between:

   -   A full package corresponding to your operating system in `the package list <https://orfeo-toolbox.org/packages/>`__. Archives are named OTB-<version>-<OS>. **For Windows, "full package" is     the only archive**. You can find detailled explaination at `recommended installation section <#recommended-installation-one-package-containing-all-the-modules>`__.
   -   **Linux users** can choose a lighter installation. You can install only *Core* package, or specifics modules. OTB dependencies can be resolved either with system packages or our dependencies archive (OTB-<version>-Linux-Dependencies in package download list). Commands are detailled at `advanced-installation section <#advanced-installation-modular-installation>`__.

2.   Configure OTB: ``source /Path/To/OTB/otbenv.profile``
3.   Play with OTB in terminal (:doc:`CliInterface`), as QGIS module (:ref:`otbqgis` and :doc:`QGISInterface`) or in python scripts (:doc:`PythonAPI`).

If you want the latests features, you might be interested to compile from source. Detailled steps at :doc:`CompilingOTBFromSource`.

OTB 10.x is known to work on:

.. table:: OTB compat list

    +--------------------------------------------+-------------------------------+--------------------+
    | **Operating System**                       | **Minimum compiler required** | **Python version** |
    +============================================+===============================+====================+
    | Debian 12 and more & Ubuntu 22.04 and more | GCC 8                         | Python >= 3.10     |
    +--------------------------------------------+-------------------------------+--------------------+
    | RedHat 8                                   | GCC 8                         | Python >= 3.8      |
    +--------------------------------------------+-------------------------------+--------------------+
    | Windows 10                                 | MSVC 2019                     | Python 3.10        |
    +--------------------------------------------+-------------------------------+--------------------+

If you want to use another version of python you will need to recompile python bindings as explained in `"Recompiling Python Bindings" <#recompiling-python-bindings>`__.

If you plan to use OTB on other operating systems that thoses listed, you can either `use Docker images <#docker-image>`__ or compile OTB from source (see :doc:`CompilingOTBFromSource` )

Linux
-----

.. include:: Installation_Linux.rst

Windows
-------

.. include:: Installation_Windows.rst

Docker image
------------

The following docker images are availables on `DockerHub <https://hub.docker.com/r/orfeotoolbox/otb>`__.
**It is the only way to run OTB for MacOS since the Intel architecture is no more in the Mac ecosystem.**:

.. table:: Docker list for version 10.0

    +-----------------+--------------+--------------------+
    | **Tag**         | **OS base**  | **Python version** |
    +=================+==============+====================+
    | 10.0.0          | ubuntu 22.04 | Python 3.10        |
    +-----------------+--------------+--------------------+
    | 10.0.0_ubuntu24 | ubuntu 24.04 | Python 3.12        |
    +-----------------+--------------+--------------------+

It can be used on any device running Docker with the following instruction:

::
   
   # you can adapt the version you want using different tag like orfeotoolbox/otb:10.0.0
   docker pull orfeotoolbox/otb

To run CLI tools, you can launch the docker image with:

::
   
   docker run -it orfeotoolbox/otb


If you are planning on using the image on MacOS with an Apple Silicon chip (M1,M2,M3) :

::
   
   docker run -it --platform=linux/amd64 orfeotoolbox/otb:10.0.0


Distribution packages
---------------------

Warning! These packages may not be up-to-date with latest OTB
releases. In addition, some features of the library may not be available
on every platform. Some of these are not maintained by the OTB-team. If you
want to get involved in the packaging of OTB for your favourite
platform, please contact us through the developer’s mailing list:
otb-developers@googlegroups.com.

There are OTB packages for Debian since version 5.2.0. OTB
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
