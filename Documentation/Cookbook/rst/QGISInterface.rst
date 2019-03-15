QGIS interface
==============

The QGIS-OTB plugin (requires QGIS > 3.0)
-----------------------------------------

With QGIS 3.0.2 or later, you will need to manually install the plugin.
Clone qgis-otb-plugin repository and set ``QGIS_PLUGINPATH``:

For Linux/Unix/MacOSX
^^^^^^^^^^^^^^^^^^^^^

::

    mkdir $HOME/projects; cd $HOME/projects
    git clone https://gitlab.orfeo-toolbox.org/orfeotoolbox/qgis-otb-plugin
    export QGIS_PLUGINPATH=$HOME/projects/qgis-otb-plugin

For Windows
^^^^^^^^^^^

Clone qgis-otb-plugin repository to ``C:\qgis-plugins\qgis-otb-plugin``

::

    git clone https://gitlab.orfeo-toolbox.org/orfeotoolbox/qgis-otb-plugin

Then set the ``QGIS_PLUGINPATH`` variable:

* System properties (``Windows Key + R -> sysdm.cpl`` ) 
* Select Advanced Tab -> Environment variables. 
* Under "user variables for " 
* Add or Edit variable ``QGIS_PLUGINPATH`` and set value to ``C:\qgis-plugins\qgis-otb-plugin``

Download and Install OTB
^^^^^^^^^^^^^^^^^^^^^^^^

OTB is not distributed with qgis-otb-plugin. It is a seperate project and has its own git repository.
Download latest OTB version: https://www.orfeo-toolbox.org/download/.

Configure plugin in QGIS
^^^^^^^^^^^^^^^^^^^^^^^^

Restart QGIS, then install the OTB plugin:
`Plugins -> Manage and Install Plugins`.

Click on `Installed` tab on left and make sure box next to `OrfeoToolBox (OTB)` is checked.

Open processing settings
^^^^^^^^^^^^^^^^^^^^^^^^

`Settings -> Options -> Processing (left panel)`

You can see OTB under "Providers":

* Expand OTB tab
* Tick Activate option
* Set OTB folder. This is location of your OTB installation.
* Set OTB application folder. This is location of your OTB applications. ``<OTB_FOLDER>/lib/otb/applications``
* Click "ok" to save settings and close dialog. If settings are correct, you will have OTB algorithms loaded  in Processing toolbox

Using the processing toolbox (for QGIS < 3.0)
---------------------------------------------

In older QGIS version (3.0 or before), OTB applications are available from QGIS.
Use them from the processing toolbox, which is accessible under `Processing
-> ToolBox`. Switch to “advanced interface” in the bottom of the
application widget and OTB applications will be there.

.. figure:: Art/QtImages/qgis-otb.png

Using a custom OTB
^^^^^^^^^^^^^^^^^^

If QGIS cannot find OTB, the “applications folder” and “binaries folder”
can be set from the settings found under Processing :math:`\rightarrow`
Settings :math:`\rightarrow` “service provider”.

.. figure:: Art/QtImages/qgis-otb-settings.png

On some versions of QGIS, if an existing OTB installation is found, the
textfield settings will not be shown. To use a custom OTB instead of the
existing one, you will need to replace the otbcli, otbgui and library
files in QGIS installation directly.

