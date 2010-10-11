if GDAL_IS_CONFIG
GDAL_CPPFLAGS = @GDAL_INC@ -DHAVE_GDAL=1
endif

if GEOTIFF_IS_CONFIG
GEOTIFF_CPPFLAGS = @GEOTIFF_INC@ -DHAVE_LIBGEOTIFF=1
endif

AM_CPPFLAGS = $(GEOTIFF_CPPFLAGS) $(GDAL_CPPFLAGS) -I../include -I../include/detail


INCLUDES = -I. -I../../include

TESTS = liblas_test

check_PROGRAMS = liblas_test

liblas_test_LDADD = \
	$(top_builddir)/src/liblas.la \
	$(NULL)

liblas_test_SOURCES = \
    common.cpp \
    guid_test.cpp \
    laserror_test.cpp \
    lasfile_test.cpp \
    lasheader_test.cpp \
    lasspatialreference_test.cpp \
    laspoint_test.cpp \
    lasreader_iterator_test.cpp \
    lasreader_test.cpp \
    lasvariablerecord_test.cpp \
    laswriter_test.cpp \
    liblas_test_suite.cpp

