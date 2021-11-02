Supported file formats
======================

OTB relies on `GDAL <https://gdal.org/>`_ for reading and writing data
(either raster or vector images). This means that all formats
supported by GDAL are theoretically also supported by OTB. That being
said, there are some limitations.

Raster images
~~~~~~~~~~~~~

GDAL provides a very large panel of drivers to access metadata in
image files. OTB stores the metadata in a special dictionary called
otbImageMetadata. There is a metadata framework that uses interfaces
(ImageMetadataInterface) to link data read by GDAL to the
otbImageMetadata. Thus, to access metadata for a specific sensor, the
corresponding interface is required. As this time, available
interfaces are:

+------------------------+-------------------------+-------------------+
| Sensor                 | Format                  | Notes             |
+========================+=========================+===================+
| CosmoSkyMed            | HDF5 / TIFF             | N/A               |
+------------------------+-------------------------+-------------------+
| Formosat               | DIMAP / TIFF            | N/A               |
+------------------------+-------------------------+-------------------+
| Ikonos                 | TIFF                    | N/A               |
+------------------------+-------------------------+-------------------+
| Pleiades               | JPEG2000 / TIFF         | N/A               |
+------------------------+-------------------------+-------------------+
| QuickBird              | TIFF                    | N/A               |
+------------------------+-------------------------+-------------------+
| Radarsat 2             | TIFF                    | N/A               |
+------------------------+-------------------------+-------------------+
| Sentinel 1             | TIFF                    | N/A               |
+------------------------+-------------------------+-------------------+
| Spot 5                 | DIMAP / TIFF            | RPC not available |
+------------------------+-------------------------+-------------------+
| Spot 6                 | DIMAP / JPEG2000 / TIFF | N/A               |
+------------------------+-------------------------+-------------------+
| TerraSarX              | COS                     | N/A               |
+------------------------+-------------------------+-------------------+
| WorldView 2            | TIFF                    | N/A               |
+------------------------+-------------------------+-------------------+

Sensors not in this list can still be used with OTB, but the metadata
won't be accessible.

Full products generally contain multiple files (image files, manifest,
metadata files, etc). For SAR products, OTB takes necessarily an image
file. For optical sensor, OTB can also take the manifest file as
input. In that case, OTB will consider all the bands of the product.

Vector images
~~~~~~~~~~~~~

OTB can read all vorctor format supported by ORG. But the writting
process is a little tricky. OTB implements two ways of dealing with
writting vector data. The first one uses OGR::Datasource. The second
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

