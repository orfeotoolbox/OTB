What is new with OTB 9.0
========================

.. note:: To see the complete list of updates, see the `RELEASE NOTES
          <https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/-/blob/develop/RELEASE_NOTES.txt>`_.

A new modular Toolbox
---------------------

The simplification process also goes via a shrink in the number of applications available.
The idea in OTB 9 is to have a Core package, which consists of all the mandatory modules and apps
and provide separate packages for each optional module.
Each module is composed of a folder tree :

::

    * Applications : contains modules specific applications and app tests
    * FonctionalTests : contains a sequence of application called to recreate a production environment
    * SubModule1 (Common for example in Core)
    * SubModule2 (IO for example in Core)

Each submodule is architectured as follow :

::

    * src : module sources
    * include : module includes
    * test : unit tests for this module only

Ths list of modules is available in the page :doc:`Modules`

The packaging is done per module for Linux, with tar.gz archives.
For windows users, the zip remains unchanged, it contains the entire OTB package

No more Qt GUI
--------------

QT based GUI were not widely used by the community and this library (Qt5) is now outdated and replaced by Qt6 so we decided to remove GUI and move to QGIS.
QGIS is now the recommended image viewer : Monteverdi, based on Qt, is removed from OTB 9. The total package size is lowered by 40% < 100Mo.

Drop support for MacOS Intel
----------------------------

Since 2020, Mac computers are not using the Intel x86_64 architecture anymore, the team had to find a solution for ARM M1/M2 Macs users as OTB cannot compile on ARM due to some dependencies not available for this architecture
Docker permits to launch x86_64 images on the ARM architecture (thanks to the Rosetta system embedded in macOS).
After further testing this solution works well and permits to Mac users to continue using OTB in command line.

See the page :doc:`First_Step_Docker` for more information on how to use it on Mac.