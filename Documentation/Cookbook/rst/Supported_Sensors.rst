Supported file formats
======================

OTB relies on `GDAL <https://gdal.org/>`_ for reading and writing data
(either raster or vector images). This means that all formats
supported by GDAL are theoretically also supported by OTB. That being
said, there are some limitations.

Raster images
-------------

Reading a raster image
~~~~~~~~~~~~~~~~~~~~~~

GDAL provides a very large panel of drivers to access metadata in
image files. OTB stores the metadata in a special dictionary called
otbImageMetadata. There is a metadata framework that uses interfaces
(ImageMetadataInterface) to link data read by GDAL to the
otbImageMetadata. Thus, to access metadata for a specific sensor, the
corresponding interface is required. As this time, available
interfaces are:

+------------------------+-------------------------+--------------------------------------------------------------------+
| Sensor                 | Format                  | Notes                                                              |
+========================+=========================+====================================================================+
| CosmoSkyMed            | HDF5 / TIFF             | See HDF particularity below                                        |
+------------------------+-------------------------+--------------------------------------------------------------------+
| Formosat               | DIMAP / TIFF            | N/A                                                                |
+------------------------+-------------------------+--------------------------------------------------------------------+
| Ikonos                 | TIFF                    | N/A                                                                |
+------------------------+-------------------------+--------------------------------------------------------------------+
| Pleiades               | JPEG2000 / TIFF         | N/A                                                                |
+------------------------+-------------------------+--------------------------------------------------------------------+
| QuickBird              | TIFF                    | N/A                                                                |
+------------------------+-------------------------+--------------------------------------------------------------------+
| Radarsat 2             | TIFF                    | N/A                                                                |
+------------------------+-------------------------+--------------------------------------------------------------------+
| Sentinel 1             | TIFF                    | Use the tiff file, not the manifest                                |
+------------------------+-------------------------+--------------------------------------------------------------------+
| Spot 5                 | DIMAP / TIFF            | RPC not available                                                  |
+------------------------+-------------------------+--------------------------------------------------------------------+
| Spot 6                 | DIMAP / JPEG2000 / TIFF | N/A                                                                |
+------------------------+-------------------------+--------------------------------------------------------------------+
| TerraSarX              | COS                     | Use the cos file, not the main XML. MGD products are not supported |
+------------------------+-------------------------+--------------------------------------------------------------------+
| WorldView 2            | TIFF                    | N/A                                                                |
+------------------------+-------------------------+--------------------------------------------------------------------+

Sensors not in this list can still be used with OTB, but the metadata
won't be accessible.

Full products generally contain multiple files (image files, manifest,
metadata files, etc). For SAR products, OTB takes necessarily an image
file. For optical sensor, OTB can also take the manifest file as
input. In that case, OTB will consider all the bands of the product.

Writing a raster image
~~~~~~~~~~~~~~~~~~~~~~

OTB uses a correspondence table to link a specific file extension to a
GDAL driver. This means that formats not present in this table can't
be written, even if the driver exists in GDAL. The formats available in
OTB for writing a raster are:

- GTiff (.tif / .tiff)

- ENVI (.hdr)

- HFA (.img)

- NITF (.ntf)

- PNG (.png)

- JPEG (.jpg / .jpeg)

- PCIDSK (.pix)

- ISIS2 (.lbl / .pds)

- JP2OpenJPEG / JP2KAK / JP2ECW (.j2k / .jp2 / .jpx)

A particularity of HDF datasets
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When reading a HDF dataset, one needs to select the right subdataset
using the :ref:`Extended Filename <extended-filename-sdataidx>`
``&sdataidx=<(int)idx>``.  For example, in some CosmoSkyMed products,
the first subDataset is a quicklook, and the actual product is the
second subdataset.


Vector data
-------------

OTB can read all vector format supported by ORG. But the writing
process is a little tricky. OTB implements two ways of dealing with
writing vector data. The first one uses OGR::Datasource. The second
one uses otb::VectorData. Depending on which class is used by the
application, different formats are available. This is confusing, and
we plan on fixing this. For now, formats fully supported
(IE. supported by all applications) are:

- ESRI Shapefile (.shp)

- MapInfo File (.tab)

- Geographical Markup Language (.gml)

- GPS Exchange Format (.gpx)

- SQLite (.sqlite)

- Keyhole Markup Language (.kml)

- GeoPackage (.gpkg)

