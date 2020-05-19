QGIS interface
==============

The OTB QGIS plugin provides a GUI for OTB applications directly into QGIS. Note that OTB is not distributed with the plugin and needs to be installed separately. Binary packages for OTB can be found on the [download page](https://www.orfeo-toolbox.org/download).

The plugin requires QGIS 3.2 or later, and is fully integrated within QGIS since version 3.8. 

This page explains how to setup the plugin with QGIS 3.8 or later. For QGIS version between 3.2 and 3.8 the instructions on this (page)[https://gitlab.orfeo-toolbox.org/orfeotoolbox/qgis-otb-plugin#otb-provider-for-qgis-processing] should be followed.


Plugin configuration
--------------------


Plugin activation
^^^^^^^^^^^^^^^^^

The plugin should be configured to find the OTB library : 

Open processing settings : 


`Settings -> Options -> Processing (left panel)`

You can see OTB under "Providers":

* Expand OTB tab
* Tick Activate option
* Set OTB folder. This is location of your OTB installation.
* Set OTB application folder. This is location of your OTB applications. ``<OTB_FOLDER>/lib/otb/applications``
* Click "ok" to save settings and close dialog. If settings are correct, you will have OTB algorithms loaded  in Processing toolbox


Documentation of OTB settings available in QGIS Processing
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* **Activate**: This is a checkbox to activate or deactivate OTB provider. Any invalid settings in OTB folder will uncheck this when saved.

* **OTB folder**: This is the directory where OTB is available. Valid values are listed below.

* **OTB application folder**: This is the location(s) of OTB applications. Multiple paths are allowed to use custom/proprietary OTB applications.

* **Logger level** (optional): Level of logger to use by OTB applications. The level of logging controls the amount of details printed during algorithm execution.
Possible values for logger level are INFO, WARNING, CRITICAL, DEBUG. You can refer to otb::Logger documentation for more on this values. This value is INFO by default. This is an advanced user configuration.

* **Maximum RAM to use** (optional): by default OTB applications use system RAM as available. You can however instruct OTB to use a specific amount of RAM from available using this option.
A value of 256 is ignored by OTB processing provider. This is an advanced user configuration.

* **Geoid file** (optional): Path to geoid file. Value of this options is set for elev.dem.geoid and elev.geoid parameters in OTB applications.
Setting this value globally help users to share it across multiple processing algorithms. This value is empty by default.

* **SRTM tiles folder** (optional): Directory where SRTM tiles are available.
SRTM data can be stored locally to avoid connecting to downloading of files during processing.
Value of this options is set for elev.dem.path and elev.dem parameters in OTB applications.
Setting this value globally help users to share it across multiple processing algorithms. This value is empty by default.

Compatibility between QGIS and OTB versions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

There are compatibility issues between the different versions of OTB and QGIS. In particular, OTB 7.0 
is not compatible with QGIS 3.12 and less. It will be compatible with QGIS 3.14.

The table below summarizes which version of OTB can be used with which version of QGIS.

+---------------+-----------------+--------------------+
|               | QGIS 3.8 - 3.12 | QGIS 3.14 and more |
+---------------+-----------------+--------------------+
| OTB 6.6.1     | Compatible      | Compatible         |
+---------------+-----------------+--------------------+
| OTB 7.0.0     | Compatible      | Compatible         |
+---------------+-----------------+--------------------+
| OTB 7.1.0     | Not Compatible  | Compatible         |
+---------------+-----------------+--------------------+

Troubleshoot
------------
As of QGIS 3.8 the otb plugin is in the core. It might get messy if you have a previously installed plugin. Try to remove the old plugin before launching QGIS.
