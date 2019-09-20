QGIS interface
==============

The QGIS-OTB plugin (requires QGIS > 3.2)
-----------------------------------------

With QGIS < 3.8 you will need to manually install the plugin.
You can follow the instruction here: https://gitlab.orfeo-toolbox.org/orfeotoolbox/qgis-otb-plugin#otb-provider-for-qgis-processing


With QGIS > 3.8, the plugin is in the QGIS core. So you just need install OTB and set the plugin up.

Download and Install OTB
^^^^^^^^^^^^^^^^^^^^^^^^

OTB is not distributed with qgis-otb-plugin. It is a separate project and has its own git repository.
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

Troubleshoot
------------
As of QGIS 3.8 the otb plugin is in the core. It might get messy if you have a previously installed plugin. Try to remove the old plugin before launching QGIS.
