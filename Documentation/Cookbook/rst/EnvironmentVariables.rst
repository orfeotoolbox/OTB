Environment variables
=====================

The following environment variables are parsed by Orfeo ToolBox. Note
that they only affect default values, and that settings in extended
filenames, applications, or custom C++ code might override those
values.

* :envvar:`OTB_DEM_DIRECTORY`: Default directory were DEM tiles are
  stored. It should only contain ``.hgt`` or georeferenced
  ``.tif`` files. Empty if not set (no directory set)
* :envvar:`OTB_GEOID_FILE`: Default path to the geoid file that will be used
  to retrieve height of DEM above ellipsoid. Empty if not set (no
  geoid set)
* :envvar:`OTB_MAX_RAM_HINT`: Default maximum memory that OTB should use for
  processing, in MB. If not set, default value is 128 MB.
* :envvar:`OTB_LOGGER_LEVEL`: Default level of logging for OTB. Should be
  one of ``DEBUG``, ``INFO``, ``WARNING``, ``CRITICAL`` or ``FATAL``,
  by increasing order of priority. Only messages with a higher
  priority than the level of logging will be displayed. If not set,
  default level is ``INFO``.
* :envvar:`OTB_MAX_NUMBER_OF_ROWS_READ_AT_ONCE`: In some scenarios where
  multi-bands input images need to be reorganized before they could be
  processed, OTB may momentarily double the memory it uses,
  and go twice over :envvar:`OTB_MAX_RAM_HINT` value.
  For those cases, this option controls the maximum number of lines
  directly loaded in memory before they are reorganized for processing.
  A value of 0 tells to read as many lines as there are, even if there
  isn't enough RAM available to accommodate the temporary buffer used.
  By default, this value is assumed to be 0.

In addition to OTB specific environment variables, the following
environment variables are parsed by third party libraries and also
affect how OTB works:

* :envvar:`GDAL_CACHEMAX`: GDAL has an internal cache mechanism to avoid reading or decoding again image chunks. This environment variable controls the amount of memory that GDAL can use for caching. By default, GDAL can use up to 5 percent of the system's available RAM, which may be a lot. In addition, caching is only needed if the processing chain is likely to request the same chunk several times, which is unlikely to happen for a standard pixel based OTB pipeline. Setting a lower value facilitates the allocation of more memory to OTB itself (using applications ``-ram`` parameter or :envvar:`OTB_MAX_RAM_HINT` environment variable). If the value is small, i.e. less than 100 000, it is assumed to be in megabytes, otherwise, it is assumed to be in bytes.
* :envvar:`GDAL_NUM_THREADS`: GDAL can take advantage of multi-threading to decode some formats. This variable controls the number of threads GDAL is allowed to use.
* :envvar:`OPJ_NUM_THREADS`: OpenJpeg can take advantage of multi-threading when decoding images. This variable controls the number of threads OpenJpeg is allowed to use.
* :envvar:`ITK_GLOBAL_DEFAULT_NUMBER_OF_THREADS`: This variable controls the number of threads used by ITK for processing.

