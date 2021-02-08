Metadata
========

This chapter explains how OTB deals with the metadata.

otbImageMetadata
----------------

OTB uses the classes ``otbImageMetadata`` and ``otbImageMetadataBase``
to store the metadata.

.. image:: /Art/C++/UmlImageMetadata.png

``otbImageMetadataBase`` encapsulates seven ``std::map`` to store
seven different kind of metadata:

- Numeric metadata for the metadata that can be stored as a double

- String metadata for the metadata that can be stored as a std::string

- LUT 1D metadata the metadata that can be stored as a one dimension table

- LUT 2D metadata for the metadata that can be stored as two dimensions table

- Time metadata for the metadata that can be stored as a time object

- Geometry metadata for the metadata that represent a model

- Extra metadata for non generic metadata stored as std::string

The keys of the maps are described in the file
``otbMetaDataKey.h``. This file also defines the time object.

For each map, the class provides 4 methods:

- the ``[] operator`` for a read-only access the metadata from the key

- the ``Add`` method to set a metadata value

- the ``Remove`` method to delete a metadata va1ue

- the ``Has`` method to test if a key has a value

An instance of ``otbImageMetadata`` is used to store the metadata. It
inherits from ``otbImageMetadataBase``, therefore it encapsulates
seven maps used to store the metadata. It also contains a vector of
``otbImageMetadataBase`` to store band specific metadata
(``m_Bands``).

Metadata workflow
-----------------

OTB reads and writes metadata from and to products through the
workflow described by this figure:

.. image:: /Art/C++/MetadataWorkflow.png

- The GDAL input/output capabilities are encapsulated in the
  ``otbGDALImageIO`` class, which derivates from ``otbImageIO``. This
  class is in charge of fetching the metadata from the product
  (supplier capabilities inherited from the class
  ``otbMetadataSupplierInterface``) and storing them in memory as a
  keywordlist. It is also in charge of writing the metadata to the
  product (storage capabilities inherited from the class
  ``otbMetadataStorageInterface``).

- An ImageMetadataInterface (IMI) is then called to parse the
  metadata. There is one IMI per sensor. We use a classical Factory to
  find which one can parse the metadata of a product. The IMI's parse
  method will pick the metadata from the ImageIO and fill an
  ImageMetadata object.

- Some metadata are not read by GDAL. To parse those metadata, the IMI
  can call other suppliers, depending on the file format:

    * to parse XML files, ``otbXMLMetadataSupplier`` uses GDAL's XML
      parsing mechanism ("ReadXMLToList" method from the
      "GDALMDReaderBase" class) to convert the XML file into a GDAL
      ListString, which is a succession of 'key=value' pairs.

    * to parse text files, TextMetadataSupplier tries to parse
      'key=value' pairs.

  Other suppliers can be added if needed. Those classes (including
  ``otbGDALImageIO``) all implement the method ``GetMetadataValue``
  which returns the value of the metadata from a given key. The base
  class also implements the methods ``GetAs`` and ``GetAsVector``
  which are used by the IMI.

- The IMI finds the relevant metadata in the different Metadata
  Suppliers and use the ``Add`` method of the ImageMetadata object to
  store the metadata. If the parsing returns successfully, the
  generated ImageMetadata is given to the ImageCommon that propagate
  through the pipeline.
