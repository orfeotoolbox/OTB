SUBDIRS = src apps include test

if GDAL_IS_CONFIG
GDAL_CPPFLAGS = @GDAL_INC@ -DHAVE_GDAL=1
endif

if GEOTIFF_IS_CONFIG
GEOTIFF_CPPFLAGS = @GEOTIFF_INC@ -DHAVE_LIBGEOTIFF=1
endif

AM_CPPFLAGS = $(GEOTIFF_CPPFLAGS) $(GDAL_CPPFLAGS) -I../include -I../include/detail


EXTRA_DIST = \
            nmake.opt \
            makefile.vc \
            src/makefile.vc \
            src/Version.rc