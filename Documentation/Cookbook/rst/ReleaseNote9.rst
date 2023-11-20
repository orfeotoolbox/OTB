What is new with OTB 9.0
========================

.. note:: To see the complete list of updates, see the `RELEASE NOTES
          <https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/-/blob/develop/RELEASE_NOTES.txt>`_.

No more Qt GUI
--------------

Qt was the framework used to develop the GUI programs of OTB. It brings some dependencies with it
which adds a 30% compilation time. One of the main problem of OTB is its complexity induced by the number of dependencies first.
The GUI and CLI are redundant (using the exact same app.so) so a choice was made to remove the GUI from OTB in favor of the QGIS alternative
Monteverdi was the default viewer for OTB, which is replaced by QGIS largely used in the Spatial community.
Another argument for this removal is that Qt5 is not getting any new update. It gets security patches via the commercial licence only, so it will be
deprecated in the next years. Going from Qt5 to Qt6 would be quite a big effort that the team cannot handle because there are more important priorities
for the toolbox.

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


Core and Dependencies
`````````````````````
This is the only mandatory module for OTB, it contains the base modules for the toolbox such as IO, DEM, ImageBase, Streaming, Wrappers...
With modularity in mind, we made the possibility to choose the dependencies you want to install with the Core package.

- **OTB-All-Dependencies** package contains the dependencies for the core **and** optional modules. If you install the Core and decide
 to use an optional module afterwards, you just need to install this optional module over the current OTB install. This is the **package 
 that you should install by default**

- **OTB-Core-Dependencies** suits to users who only need the core fonctionnalities of OTB. If you plan to install optional module afterwards,
 you need to download and install le All-Dependencies package before installing the module.

The application list for each package is described in the cookbook

ImageProcessing
```````````````
This Optional package contains specific image processing applications that uses MuParser such as BandMath, TextureExtraction, EdgeExtraction, Smoothing.
it can be installed along with the Core package with a simple tar extract command and directly available after sourcing the otbenv.profile

Learning
````````
This Optional package depends on LibSVM, OpenCV and Shark to provide Machine Learning applications such as ImageClassifier, Regression, SampleExtraction...
see the cookbook for the full list.
it can be installed along with the Core package with a simple tar extract command and directly available after sourcing the otbenv.profile

SAR
```

StereoProcessing
````````````````

Hyperspectral
`````````````