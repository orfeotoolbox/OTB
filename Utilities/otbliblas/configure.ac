dnl $Id$
dnl
dnl This is main autoconf bootstrap script of libLAS project.
dnl
m4_define([liblas_version_major], [1])
m4_define([liblas_version_minor], [2])
m4_define([liblas_version_micro], [1])
m4_define([liblas_version],
          [liblas_version_major.liblas_version_minor.liblas_version_micro])

AC_PREREQ([2.59])
AC_INIT([las], [liblas_version], [hobu.inc@gmail.com],[liblas-src])
AC_CANONICAL_BUILD

RELEASE_VERSION=liblas_version
AC_SUBST([RELEASE_VERSION])

dnl #########################################################################
dnl Default compilation flags
dnl #########################################################################

m4_define([debug_default],[no])

CFLAGS="-Wall -Wno-long-long -pedantic $CFLAGS"
CXXFLAGS="-Wall -Wno-long-long -pedantic -std=c++98 $CXXFLAGS"

dnl #########################################################################
dnl Checks for programs.
dnl #########################################################################

AM_INIT_AUTOMAKE([dist-bzip2])
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

dnl #########################################################################
dnl Check platform endianness
dnl #########################################################################

AC_C_BIGENDIAN

dnl #########################################################################
dnl Checks for header files.
dnl #########################################################################

AC_CHECK_HEADERS([string.h],, [AC_MSG_ERROR([cannot find string.h, bailing out])])
AC_CHECK_HEADERS([stdio.h],, [AC_MSG_ERROR([cannot find stdio.h, bailing out])])
AC_CHECK_HEADERS([stdlib.h],, [AC_MSG_ERROR([cannot find stdlib.h, bailing out])])


LIBS="${LIBS}"


dnl #########################################################################
dnl Build mode configuration (debug/optimized)
dnl #########################################################################

AC_ARG_ENABLE([debug],
     AC_HELP_STRING([--enable-debug=ARG], [Enable debug compilation mode @<:@yes|no@:>@, default=debug_default]),,)
     
AC_MSG_CHECKING([for debug enabled])

if test "x$enable_debug" = "xyes"; then
    CFLAGS="$CFLAGS -g -DDEBUG"
    CXXFLAGS="$CXXFLAGS -g -DDEBUG"
    AC_MSG_RESULT(yes)
else
    CFLAGS="$CFLAGS -O3 -DNDEBUG"
    CXXFLAGS="$CXXFLAGS -O3 -DNDEBUG"
    AC_MSG_RESULT(no)
fi

dnl #########################################################################
dnl Definition of custom Autoconf macros
dnl #########################################################################

AC_DEFUN([LOC_MSG],[
echo "$1"
])

AC_DEFUN([AC_HAVE_LONG_LONG],
[
    AC_MSG_CHECKING([for 64bit integer type])

    echo 'int main() { long long off=0; }' >> conftest.c
    if test -z "`${CC} -o conftest conftest.c 2>&1`" ; then
        AC_DEFINE(HAVE_LONG_LONG, 1, [Define to 1, if your compiler supports long long data type])
        AC_MSG_RESULT([long long])
    else
        AC_MSG_RESULT([no])
    fi
    rm -f conftest*
])



HAVE_GDAL="no"
GDAL_CONFIG="no"
HAVE_GEOTIFF="no"
GEOTIFF_CONFIG="no"

USE_GDAL_SOURCE_TREE="no"

dnl #########################################################################
dnl Determine GDAL Support
dnl #########################################################################

AC_ARG_WITH([gdal],
    AC_HELP_STRING([--with-gdal=ARG], [Path to gdal-config]),,)
AC_MSG_CHECKING([for GDAL])

if test "$with_gdal" = "no" -o "$with_gdal" = "" ; then
    AC_MSG_RESULT([no])
else

    dnl Check if full path to gdal-config provided
    if test "`basename xx/$with_gdal`" = "gdal-config" ; then
        AC_MSG_RESULT([GDAL enabled with provided gdal-config])
        GDAL_CONFIG="$with_gdal"
    elif test -f ${with_gdal}/frmts/gtiff/libgeotiff/geotiff.h; then
        GDAL_INC="-I$with_gdal/port -I$with_gdal/ogr -I$with_gdal/alg -I$with_gdal/frmts"
        GDAL_PREFIX="$with_gdal"
        HAVE_GDAL="yes"
        HAVE_GEOTIFF="yes"
        GEOTIFF_CONFIG="yes"
        GDAL_CONFIG="yes"
        USE_GDAL_SOURCE_TREE="yes"
        LIBS="-L${with_gdal}/.libs -lgdal -L${with_gdal}/ -lgdal $LIBS"
        AC_MSG_RESULT([GDAL and GeoTIFF enabled with provided GDAL source tree])
    else
        dnl --with-gdal=yes given, so try to find gdal-config in PATH
        GDAL_CONFIG_TMP=`which gdal-config`
        if test "`basename xx/${GDAL_CONFIG_TMP}`" = "gdal-config" ; then
            AC_MSG_RESULT([GDAL enabled with gdal-config found in PATH])
            GDAL_CONFIG=${GDAL_CONFIG_TMP}
        else
            AC_MSG_ERROR([--with-gdal should be yes, a path to gdal-config, or a path to GDAL's source tree])
        fi
    fi
fi

if test "$GDAL_CONFIG" != "no" ; then

dnl if we are already yes, we don't need to run gdal-config
  if test "$GDAL_CONFIG" != "yes" ; then

    GDAL_VERSION=`$GDAL_CONFIG --version`
    AC_MSG_RESULT([$GDAL_CONFIG reports version $GDAL_VERSION])
    LIBS="`$GDAL_CONFIG --libs` $LIBS"
    GDAL_INC=`$GDAL_CONFIG --cflags`
    GDAL_PREFIX=`$GDAL_CONFIG --prefix`
    OGR=`$GDAL_CONFIG --ogr-enabled`
    if test "$OGR" != "yes" ; then
        AC_MSG_ERROR([$GDAL_CONFIG says OGR is not enabled])
    fi
    HAVE_GDAL="yes"
  fi
fi

AC_SUBST([GDAL_INC])
AC_SUBST([GDAL_PREFIX])
AM_CONDITIONAL([GDAL_IS_CONFIG], [test ! x$GDAL_CONFIG = xno])

dnl #########################################################################
dnl Determine GeoTIFF Support
dnl #########################################################################

AC_ARG_WITH([geotiff],
    AC_HELP_STRING([--with-geotiff=ARG], [libgeotiff library to use (no or path)]),,)

if test "$with_geotiff" != "no" -a "$with_geotiff" != "" -a "${GEOTIFF_CONFIG}" != "yes" ; then
    dnl We now require libgeotiff 1.2.5 (for ST_Create the simpletags stuff). 
    dnl first check if $with_geotiff/lib has the library:
    AC_CHECK_LIB([geotiff], [ST_Create],
                 [GEOTIFF_CONFIG=external], [GEOTIFF_CONFIG=no],
                 [-L$with_geotiff/lib])
fi

if test "${GEOTIFF_CONFIG}" != "no" ; then
    dnl libgeotiff 1.2.5, is first version with geo_simpletags.h
    AC_MSG_CHECKING([for geo_simpletags.h of libgeotiff >= 1.2.5])
    if test -f ${with_geotiff}/include/geo_simpletags.h ; then
        GEOTIFF_INC="-I$with_geotiff/include"
    AC_MSG_RESULT([yes])
    elif test -f ${with_geotiff}/include/geotiff/geo_simpletags.h ; then
        GEOTIFF_INC="-I$with_geotiff/include/geotiff"
    AC_MSG_RESULT([yes])
    elif test -f ${with_gdal}/frmts/gtiff/libgeotiff/geo_simpletags.h ; then
        GEOTIFF_INC="-I$with_gdal/frmts/gtiff/libgeotiff/ -I$with_gdal/frmts/gtiff/libtiff"
    AC_MSG_RESULT([yes, from GDAL]) 
    else
        GEOTIFF_CONFIG="no"
    AC_MSG_RESULT([no])
    fi
fi
    
AC_MSG_CHECKING([for libgeotiff])
if test "${GEOTIFF_CONFIG}" = "no" ; then
    HAVE_GEOTIFF="no"
    AC_MSG_RESULT([no])
else
    if test "${USE_GDAL_SOURCE_TREE}" != "yes" ; then
        LIBS="-L$with_geotiff/lib -lgeotiff $LIBS"
        HAVE_GEOTIFF="yes"
        AC_MSG_RESULT([yes])
    else
        AC_MSG_RESULT([use GDAL source])
    fi
fi

AC_SUBST([GEOTIFF_INC])
AC_SUBST([GEOTIFF_PREFIX])
AM_CONDITIONAL([GEOTIFF_IS_CONFIG], [test ! x$GEOTIFF_CONFIG = xno])

dnl #########################################################################
dnl If GDAL was requested, but geotiff is not configured, complain
dnl #########################################################################

if test "${HAVE_GDAL}" != "no" ; then
	if test "${GEOTIFF_CONFIG}" = "no" -a "${USE_GDAL_SOURCE_TREE}" != "yes"; then
		AC_MSG_ERROR([You must configure a libgeotiff if you are using GDAL binaries.  You can configure using a GDAL source tree without libgeotiff, but not from binaries.])
	fi
fi

dnl #########################################################################
dnl Determine other features of compiler
dnl #########################################################################

AC_HAVE_LONG_LONG

dnl #########################################################################
dnl Checks for library functions.
dnl #########################################################################

AC_CHECK_FUNCS([gettimeofday bzero memset memcpy bcopy])

dnl #########################################################################
dnl Generate makefiles
dnl #########################################################################

AC_CONFIG_FILES([
    Makefile
    include/Makefile
    src/Makefile
    apps/Makefile
    apps/liblas-config
    test/Makefile
    test/unit/Makefile
])

AC_OUTPUT

dnl #########################################################################
dnl Print configuration summary
dnl #########################################################################

LOC_MSG()
LOC_MSG([libLAS configuration summary:])
LOC_MSG()
LOC_MSG([  Version..................: ${RELEASE_VERSION}])
LOC_MSG([  Installation directory...: ${prefix}])
LOC_MSG([  C compiler...............: ${CC} ${CFLAGS}])
LOC_MSG([  C++ compiler.............: ${CXX} ${CXXFLAGS}])
LOC_MSG([  Debugging support........: ${enable_debug}])
LOC_MSG([  GDAL support.............: ${HAVE_GDAL}])
if test "${HAVE_GDAL}" != "no" ; then
    LOC_MSG([   - gdal-config...........: ${GDAL_CONFIG}])
    if test -n "${GDAL_INC}" ; then
        LOC_MSG([   - INCLUDE directories...: ${GDAL_INC}])
    fi
fi
LOC_MSG([  GeoTIFF SRS support......: ${HAVE_GEOTIFF}])
if test "${HAVE_GEOTIFF}" != "no" ; then
    LOC_MSG([   - libgeotiff............: ${GEOTIFF_CONFIG}])
    if test -n "${GEOTIFF_INC}" ; then
        LOC_MSG([   - INCLUDE directories...: ${GEOTIFF_INC}])
    fi
fi
LOC_MSG()
LOC_MSG([  LIBS.....................: ${LIBS}])
LOC_MSG()
LOC_MSG([  libLAS - http://liblas.org])
LOC_MSG()

dnl EOF
