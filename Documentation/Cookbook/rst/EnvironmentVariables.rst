Environment variables
=====================

The following environment variables are parsed by Orfeo ToolBox. Note
that they only affect default values, and that settings in extended
filenames, applications, Monteverdi or custom C++ code might override
those values.

* ``OTB_DEM_DIRECTORY``: Default directory were DEM tiles are
  stored. It should only contain ```.hgt`` or or georeferenced
  ``.tif`` files. Empty if not set (no directory set)
* ``OTB_GEOID_FILE``: Default path to the geoid file that will be used
  to retrieve height of DEM above ellipsoid. Empty if not set (no
  geoid set)
* ``OTB_MAX_RAM_HINT``: Default maximum memory that OTB should use for
  processing, in MB. If not set, default value is 128 MB.
* ``OTB_LOGGER_LEVEL``: Default level of logging for OTB. Should be
  one of ``DEBUG``, ``INFO``, ``WARNING``, ``CRITICAL`` or ``FATAL``,
  by increasing order of priority. Only messages with a higher
  priority than the level of logging will be displayed. If not set,
  default level is ``INFO``.

In addition to OTB specific environment variables, the following
environment variables are parsed by third party libraries and also
affect how OTB works:

* ``GDAL_CACHEMAX``: GDAL has an internal cache mechanism to avoid reading or decoding again image chunks. This environment variable controls the amount of memory that GDAL can use for caching. By default, GDAL can use up to 5 percent of the system's available RAM, which may be a lot. In addition, caching is only needed if the processing chain is likely to request the same chunk several times, which is unlikely to happen for a standard pixel based OTB pipeline. Setting a lower value facilitates the allocation of more memory to OTB itself (using applications ``-ram`` parameter or ``OTB_MAX_RAM_HINT`` environment variable). If the value is small, i.e. less than 100 000, it is assumed to be in megabytes, otherwise, it is assumed to be in bytes.
* ``GDAL_NUM_THREADS``: GDAL can take advantage of multi-threading to decode some formats. This variable controls the number of threads GDAL is allowed to use.
* ``OPJ_NUM_THREADS``: OpenJpeg can take advantage of multi-threading when decoding images. This variable controls the number of threads OpenJpeg is allowed to use.
* ``ITK_GLOBAL_DEFAULT_NUMBER_OF_THREADS``: This variable controls the number of threads used by ITK for processing. 

