Installation
============

We provide different standalone binary packages for OTB-Applications:

-  for Windows platform (7 or higher)

-  for 64bit Linux distribution

-  for MacOS X

Other binaries can be available as packages (OSGeo packages,
Debian/Ubuntu packages, OpenSuse packages), however be advised that they
may not be up-to-date nor delivered with full features. If you want to
build from source or if we don’t provide packages for your system,
information is available in the `Software Guide <http://orfeo-toolbox.org/SoftwareGuide/>`_,
in the section "Building from Source".

Windows
-------

We provide for Windows Seven and later through standalone packages. They
are cross-compiled with MinGW, for 32 bits and 64 bits platforms. They
contain all applications and their launchers (both command line and graphical
launchers are provided).
Check the `Download page <https://www.orfeo-toolbox.org/download>`__

There is a 32 bits and a 64 bits version. They contain the same directory
structure:

-  ``monteverdi.bat`` : A launcher script for Monteverdi

-  ``mapla.bat`` : A launcher script for Mapla

-  ``otbenv.bat`` : A script to initialize the environment for OTB
   executables

-  ``bin`` : A folder containing application launchers (otbcli.bat,
   otbgui.bat) and the DLLs.

-  ``lib`` : A folder containing application DLLs.

The applications can be launched from the Mapla launcher. If you want to
use the otbcli and otbgui launchers, you can initialize a command prompt
with ``otbenv.bat``.

Linux 64bit
-----------

We provide for Linux 64 bits OS through standalone packages. They contain
all OTB Applications and their launchers (both command line and
graphical launchers are provided). Check the `Download page <https://www.orfeo-toolbox.org/download>`__

This package is a self-extractible archive. You may uncompress it with a
double-click on the file, or with the command line :

.. parsed-literal::

   chmod +x OTB-|version|-Linux64.run
   ./OTB-|version|-Linux64.run

Please note that the resulting installation is not meant to be moved,
you should uncompress the archive in its final location. Once the
archive is extracted, the directory structure is made of:

-  ``monteverdi.sh`` : A launcher script for Monteverdi

-  ``mapla.sh`` : A launcher script for Mapla

-  ``otbenv.profile`` : A script to initialize the environment for OTB
   executables

-  ``bin`` : A folder containing application launchers (otbcli.sh,
   otbgui.sh), Monteverdi and Mapla.

-  ``lib`` : A folder containing all shared libraries and OTB
   applications.

-  ``share`` : A folder containing common resources and copyright
   mentions.

In order to run the command line launchers, this package doesn’t require
any special library that is not present in most modern Linux
distributions. The graphical executable (otbgui launchers, Monteverdi
and Mapla) use the X11 libraries, which are widely used in a lot of
distributions :

::

    libx11-6 libxext6 libxau6 libxxf86vm1 libxdmcp6 libdrm2

Monteverdi also requires the standard graphics libraries libgl1 and
libglu1. Make sure you have at least one version of them installed
in your system.

The applications can be launched from the Mapla launcher. If you want to
use the otbcli and otbgui launchers, you can initialize your environment
with ``source otbenv.profile``.

Notes:

- You must use monteverdi and mapla through ``mapla.sh`` and ``monteverdi.sh`` helper scripts in extracted directory.

- The helper scripts for monteverdi and mapla set required environment variables

- You might be tempted to move "OTB-|version|-Linux64" into another location say /usr/local/ after extraction. But avoid this action!

- To have "OTB-|version|-Linux64" installed in /usr/local or /opt execute "OTB-|version|-Linux64.run" in that directory.

- Multiple installation of OTB can exists in same system without one conflicting the other!

MacOS X
-------

We provide for MacOS X through a standalone package. This package is a
self-extractible archive, quite similar to the Linux one. You may
uncompress it with the command line :

.. parsed-literal::

    chmod +x OTB-|version|-Linux64.run
    ./OTB-|version|-Linux64.run

Once the archive is extracted, you can see OTB-|version|-Darwin64 directory in
the same direcrtory along with OTB-|version|-Darwin64.run

Contents of OTB-|version|-Darwin64 is briefly listed below:

-  ``Monteverdi.app`` : A Mac OSX .app for Monteverdi

-  ``Mapla.app`` : A Mac OSX .app for Mapla.

-  ``bin`` : A folder containing application launchers (otbcli.sh,
   otbgui.sh), monteverdi and mapla binaries.

-  ``lib`` : A folder containing all shared libraries and OTB
   applications.

-  ``share`` : A folder containing common resources and copyright
   mentions.

Notes:

-  If you want to use the otbcli and otbgui launchers, you must access
   them via a terminal prompt.

-  The OSX .app are provided for monteverdi (viewer) and mapla
   (application browser).

-  You must use monteverdi and mapla through their .app files only.

-  You are allowed to move these .app files and refrain from moving or
   deleting OTB-|version|-Darwin64 after extraction. In case you need to have OTB
   installed in some other directory. Extract the .run file there.

Other packages
--------------

Warning! These packages may not be up-to-date with latest OTB
releases. In addition, some features of the library may not be available
on every platform. Some of these are not maintained by OTB-team. If you
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

After you can run:

::

    sudo aptitude install otb-bin otb-bin-qt python-otb

If you are using *Synaptic*, you can add the repositories, update and
install the packages through the graphical interface.

For further informations about Ubuntu packages go to
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

First, you need to add the appropriate repositories with these
command-lines (please replace :math:`11.4` by your OpenSuse version):

::

    sudo zypper ar
    http://download.opensuse.org/repositories/games/openSUSE_11.4/ Games
    sudo zypper ar
    http://download.opensuse.org/repositories/Application:/Geo/openSUSE_11.4/ GEO
    sudo zypper ar
    http://download.opensuse.org/repositories/home:/tzotsos/openSUSE_11.4/ tzotsos

Now run:

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

MacPort
~~~~~~~

OTB Applications are now available on
`MacPorts <http://http://www.macports.org/>`__. The port name is called
’orfeotoolbox’. You can follow the `MacPorts
documentation < http://guide.macports.org/>`__ to install MacPorts
first, then install the ’orfeotoolbox’ port. After the installation, you
can used directly on your system, the OTB applications.
