QGIS interface
==============

OTB Applications can be used with QGis:

- Since QGis 3.8 and until 3.34, OTB provider is fully integrated
- Since QGis 3.36, OTB provider is available in QGis plugin's catalog

.. warning:: Since QGIS 3.22 and until 3.34: the plugin is not activated by default. It should be activated in the plugins settings (``Plugins/Manage and Install Plugins...`` toolbar). The plugin should then be configured as detailed in the QGIS documentation (see the links provided above).

Plugin installation
-------------------

.. note:: Instructions only valid since QGis 3.36

The easiest method is to install OTB provider plugin from QGis catalog:

- :menuselection:`Plugin --> Manage and Install Plugins...`
- Go to :guilabel:`Not installed` list and search for otb
- Install plugin, it should be available in :guilabel:`Installed` list

If you need the plugin on a terminal disconnected from public network, use the following instructions. You need at least one terminal connected to network and a way to transfer plugin from one terminal to another:

- Download plugin from https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb-qgis-plugin ( :guilabel:`Code` button on right then :guilabel:`zip` in download source code section)
- Transfer you files from one terminal to another and unzip it somewhere
- copy only ``orfeoToolbox_provider`` folder to ~/.local/share/QGIS/QGIS3/profiles/default/python/plugins
- Plugin should be available

.. image:: Art/plugins.png

Plugin configuration/parameters
-------------------------------

When installed, to use OTB in QGIS you need to setup the provider plugin:

- Open the processing settings: :menuselection:`Settings --> Options --> Processing`
- You can see ``OTB`` under :menuselection:`Providers` menu (see following picture), expand the :guilabel:`OTB` entry.

The following parameters can be personnalized:

- **Mandatory parameter** :guilabel:`OTB folder`, i.e. the location of the root of your OTB installation.
- **Mandatory parameter** :guilabel:`OTB application folder`. The location of your OTB applications (e.g. :file:`<PATH_TO_OTB_INSTALLATION>/lib/otb/applications`). Multiple paths are allowed.
- :guilabel:`Logger level`, an optional parameter indicates level of logger used by OTB applications.
  The level of logging controls the amount of detail printed during
  algorithm execution. Possible values from less to most verbose are:
  ``CRITICAL``, ``WARNING``, ``INFO`` and ``DEBUG``. Default to ``INFO``.
- :guilabel:`Maximum RAM to use`, an optional and advanced parameter. By default, OTB applications use all available system RAM.
  You can, however, instruct OTB to use a specific amount of RAM (in megabytes). A value of 256 is ignored by the OTB processing provider.
- :guilabel:`Geoid file`, optional and advanced parameter. Path to the geoid file.
  This option sets the value of the *elev.dem.geoid* and *elev.geoid*
  parameters in OTB applications.
  Setting this value globally enables users to share it across multiple
  processing algorithms. Empty by default.
- :guilabel:`SRTM tiles folder`, an optional and advanced parameter. The directory where SRTM tiles are available. SRTM data can be stored locally to avoid downloading of files during processing. This option sets the value of *elev.dem.path*
  and *elev.dem* parameters in OTB applications. Setting this value
  globally enables users to share it across multiple processing algorithms.
  Empty by default.

.. image:: Art/plugins_config.png


Compatibility between QGIS and OTB versions
-------------------------------------------

There are compatibility issues between the different versions of OTB and QGIS. Any version
of OTB compiled with GDAL 3.X is not compatible with QGIS 3.12 or below. This is the case
for the binary packages of OTB 7.1 and above. In this case QGIS 3.14 or more should be used.

The table below summarizes which version of OTB can be used with which version of QGIS.

+---------------+-----------------+-----------------+-----------------+-----------------+
|               | QGIS 3.8 - 3.12 | QGIS 3.14       | QGIS 3.16       | QGIS >= 3.18    |
+---------------+-----------------+-----------------+-----------------+-----------------+
| OTB 6.6.1     | Compatible      | Compatible      | Compatible      | Compatible      |
+---------------+-----------------+-----------------+-----------------+-----------------+
| OTB 7.0.0     | Compatible      | Compatible      | Compatible      | Compatible      |
+---------------+-----------------+-----------------+-----------------+-----------------+
| OTB 7.1.0     | Not Compatible  | Compatible      | Compatible      | Compatible      |
+---------------+-----------------+-----------------+-----------------+-----------------+
| OTB 7.2.0     | Not Compatible  | Compatible      | Compatible      | Compatible      |
+---------------+-----------------+-----------------+-----------------+-----------------+
| OTB 8.x.0     | Not Compatible  | Not Compatible  | Not Compatible  | Compatible      |
+---------------+-----------------+-----------------+-----------------+-----------------+
| OTB 9.x.0     | Not Compatible  | Not Compatible  | Not Compatible  | Compatible      |
+---------------+-----------------+-----------------+-----------------+-----------------+

Issue tracking
--------------

If you encounter issues with integration of OTB application into QGIS,
please `fill an issue in otb-qgis-plugin board <https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb-qgis-plugin/-/issues/new?issue[assignee_id]=&issue[milestone_id]=>`_ .
