What is new with OTB 9.0
========================

.. note:: To see the complete list of updates, see the `RELEASE NOTES
          <https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/-/blob/develop/RELEASE_NOTES.txt>`_.

No more Qt GUI
--------------

QT based GUI were not widely used by the community and this library is outdated so we decided to remove GUI and move to QGIS.
QGIS is also the recommended image viewer : Monteverdi is not maintained anymore.

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