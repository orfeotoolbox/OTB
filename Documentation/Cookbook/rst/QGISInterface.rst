QGIS interface
==============

OTB Applications are fully integrated in QGIS since QGIS 3.8. 
You can configure OTB for QGIS according to the: 

- `QGIS Provider documentation for latest LTR <https://docs.qgis.org/latest/en/docs/user_manual/processing/3rdParty.html>`_.
- `QGIS Provider documentation <https://docs.qgis.org/testing/en/docs/user_manual/processing/3rdParty.html>`_.

Compatibility between QGIS and OTB versions
-------------------------------------------

There are compatibility issues between the different versions of OTB and QGIS. Any version
of OTB compiled with GDAL 3.X is not compatible with QGIS 3.12 or below. This is the case
for the binary packages of OTB 7.1 and above. In this case QGIS 3.14 or more should be used.

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

Use OTB in old QGIS version (3.6 and less)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. warning:: Please consider upgrading your QGIS version.

Since QGIS 3.8 and with 3.10 LTR version, the configuration of OTB Application is 
directly done in QGIS Provider (see above).

If you use older version of QGIS (from 3.2 to 3.6), you need to install an 
additional plugin in order to use OTB applications. This plugin is available 
`here <https://gitlab.orfeo-toolbox.org/orfeotoolbox/qgis-otb-plugin>`_ 
with specific installation instructions.

Troubleshooting
^^^^^^^^^^^^^^^
As of QGIS 3.8 the OTB plugin is in the core. It might get messy if you 
have a previously installed plugin. Try to remove the old plugin before 
launching QGIS.


Issue tracking
^^^^^^^^^^^^^^
If you encounter issues with integration of OTB application into QGIS, 
please `fill an issue <https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/-/issues/new?issue[assignee_id]=&issue[milestone_id]=>`_ into the OTB issue tracker with `qgis` label.
