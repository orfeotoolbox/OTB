
SUBDIRS = .

if GEOTIFF_IS_CONFIG
GEOTIFF_CPPFLAGS = @GEOTIFF_INC@ -DHAVE_LIBGEOTIFF=1
endif

AM_CPPFLAGS = $(GEOTIFF_CPPFLAGS) -I../include -I../include/detail

lib_LTLIBRARIES = liblas.la

liblas_la_SOURCES = \
	laserror.cpp \
	laspoint.cpp \
	lasheader.cpp \
	lasrecordheader.cpp \
	lasreader.cpp \
	laswriter.cpp \
	lasfile.cpp \
	las_c_api.cpp \
    detail/reader.cpp \
    detail/reader10.cpp \
    detail/reader11.cpp \
    detail/writer.cpp \
    detail/writer10.cpp \
    detail/writer11.cpp \
    detail/file.cpp

#liblas_la_LIBADD = 	detail/liblasdetail.la

liblas_la_LDFLAGS = -version-info 1:0:0
