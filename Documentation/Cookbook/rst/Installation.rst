Installation
============

We provide different standalone binary packages for OTB-Applications:

-  for Windows platform (Seven or higher)

-  for 64 bits Linux distribution

-  for MacOS X

Other binaries can be available as packages (OSGeo packages,
Debian/Ubuntu packages, OpenSuse packages), however be advised that they
may not be up-to-date nor delivered with full features. If you want to
build from source or if we don’t provide packages for your system,
information is available in the :ref:`compilingfromsource` section.

You can get latest binary packages from our `Download page <https://www.orfeo-toolbox.org/download>`__.

Windows
-------

.. include:: Installation_Windows.rst

Linux
-----

.. include:: Installation_Linux.rst

MacOS X
-------

.. include:: Installation_Macos.rst


Conda package
-------------

A conda package is available for Orfeo Toolbox on Linux. It can be found at https://anaconda.org/orfeotoolbox/otb 
and can be installed with the command

::

    conda install -c orfeotoolbox otb 


Other packages
--------------

Warning! These packages may not be up-to-date with latest OTB
releases. In addition, some features of the library may not be available
on every platform. Some of these are not maintained by the OTB-team. If you
want to get involved in the packaging of OTB for your favourite
platform, please contact us through the developer’s mailing list:
otb-developers@googlegroups.com.

Debian
~~~~~~

There are OTB packages for Debian (unstable) since version 5.2.0. OTB
Applications packages may be available as Debian packages through APT
repositories:

-  otb-bin for command line applications

-  otb-bin-qt for Qt applications

-  python-otb for python applications

Due to license issues, the OTB package built in Debian doesn’t contain
6S. As a consequence, the package does not contain the
OpticalCalibration application.

Ubuntu 12.04 and higher
~~~~~~~~~~~~~~~~~~~~~~~

For Ubuntu 12.04 and higher, OTB Applications packages may be available
as Debian packages through APT repositories:

-  otb-bin for command line applications

-  otb-bin-qt for Qt applications

-  python-otb for python applications

Since release 3.14.1, OTB Applications packages are available in the
`ubuntugis-unstable <https://launchpad.net/~ubuntugis/+archive/ubuntugis-unstable>`__
repository.

Since release 5.2.0, the Ubuntu packages derive from the Debian
packages.

You can add it by using these command-lines:

::

    sudo aptitude install add-apt-repository
    sudo apt-add-repository ppa:ubuntugis/ubuntugis-unstable

You will then need to run:

::

    sudo aptitude install otb-bin otb-bin-qt python-otb

If you are using *Synaptic*, you can add the repositories, update and
install the packages through the graphical user interface.

For further information about Ubuntu packages go to
`ubuntugis-unstable <https://launchpad.net/~ubuntugis/+archive/ubuntugis-unstable>`__
launchpad page and click on Read about installing.

apt-add-repository will try to retrieve the GPG keys of the
repositories to certify the origin of the packages. If you are behind a
http proxy, this step won’t work and apt-add-repository will stall and
eventually quit. You can temporarily ignore this error and proceed with
the update step. Following this, aptitude update will issue a warning
about a signature problem. This warning won’t prevent you from
installing the packages.

OpenSuse 12.X and higher
~~~~~~~~~~~~~~~~~~~~~~~~

For OpenSuse 12.X and higher, OTB Applications packages are available
through *zypper*.

First, you need to add the appropriate repositories with the following
commands (please replace :math:`11.4` with your version of OpenSuse):

::

    sudo zypper ar
    http://download.opensuse.org/repositories/games/openSUSE_11.4/ Games
    sudo zypper ar
    http://download.opensuse.org/repositories/Application:/Geo/openSUSE_11.4/ GEO
    sudo zypper ar
    http://download.opensuse.org/repositories/home:/tzotsos/openSUSE_11.4/ tzotsos

You should then run:

::

    sudo zypper refresh
    sudo zypper install OrfeoToolbox
    sudo zypper install OrfeoToolbox-python

Alternatively you can use the One-Click Installer from the `openSUSE
Download
page <http://software.opensuse.org/search?q=Orfeo&baseproject=openSUSE%3A11.4&lang=en&include_home=true&exclude_debug=true>`__
or add the above repositories and install through Yast Package
Management.

There is also support for the recently introduced ’rolling’ openSUSE
distribution named ’Tumbleweed’. For Tumbleweed you need to add the
following repositories with these command-lines:

::

    sudo zypper ar
    http://download.opensuse.org/repositories/games/openSUSE_Tumbleweed/ Games
    sudo zypper ar
    http://download.opensuse.org/repositories/Application:/Geo/openSUSE_Tumbleweed/ GEO
    sudo zypper ar
    http://download.opensuse.org/repositories/home:/tzotsos/openSUSE_Tumbleweed/ tzotsos

and then add the OTB packages as shown above.


Archlinux
~~~~~~~~~~

Package is in AUR (Arch User Repository).
You will then need to run (if you use yaourt, you could use trizen instead):

::

    yaourt -S orfeo-toolbox

