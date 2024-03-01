QGIS interface
==============

OTB Applications are fully integrated in QGIS since QGIS 3.8.
You can configure OTB for QGIS according to the:

- `QGIS Provider documentation for latest LTR <https://docs.qgis.org/latest/en/docs/user_manual/processing/3rdParty.html>`_.
- `QGIS Provider documentation <https://docs.qgis.org/testing/en/docs/user_manual/processing/3rdParty.html>`_.


.. warning:: Since QGIS 3.22: the plugin is not activated by default. It should be activated in the plugins settings (``Plugins/Manage and Install Plugins...`` toolbar). The plugin should then be configured as detailed in the QGIS documentation (see the links provided above).

.. image:: Art/plugins.png

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
please `fill an issue <https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/-/issues/new?issue[assignee_id]=&issue[milestone_id]=>`_ into the OTB issue tracker with `qgis` label.
