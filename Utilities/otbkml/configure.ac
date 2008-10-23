dnl Process this file with autoconf to produce a configure script.

dnl Make sure we're interpreted by some minimal autoconf.
AC_PREREQ(2.57)

AC_INIT(libkml, 0.4.0, http://code.google.com/p/libkml/issues)
AC_CONFIG_SRCDIR(README)
AC_CONFIG_AUX_DIR(config)
AM_INIT_AUTOMAKE(-Wall -Werror)

dnl Where can we find cppunit headers and libs?
AM_PATH_CPPUNIT(1.12.0)
dnl If we have cppunit installed, we set a define here. The unit tests 
dnl elsewhere will build conditionally against this flag.
AM_CONDITIONAL(HAVE_CPPUNIT, test "$CPPUNIT_LIBS")

AC_PROG_CPP
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LIBTOOL

AC_CHECK_HEADERS([expat.h])
AC_CHECK_HEADERS([float.h limits.h stdlib.h string.h])
AC_CHECK_FUNCS([floor memset strstr])

AC_C_CONST
AC_C_INLINE
AC_FUNC_MALLOC
AC_FUNC_STRTOD
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T

dnl Let Makefile know if we are GCC.
AM_CONDITIONAL(GCC, test "$GCC" = yes)

dnl Which platform are we building on?
PLATFORM=
SHLIBSUFFIX=
case "`uname -s`" in
	Linux)	PLATFORM=linux
		SHLIBSUFFIX="so"
	;;
	Darwin)	PLATFORM=darwin
		SHLIBSUFFIX="dylib"
	;;
	*) 	PLATFORM=unknown
		SHLIBSUFFIX="so"
	;;
esac
AC_SUBST(PLATFORM)
AC_SUBST(SHLIBSUFFIX)

dnl Where can we find the Java headers and libs?
AC_ARG_WITH(java-include-dir,
	    [--with-java-include-dir=DIR look in DIR for Java headers],
	    [JAVA_INCLUDE_DIR=$withval],)
AC_SUBST(JAVA_INCLUDE_DIR)
AC_ARG_WITH(java-lib-dir,
	    [--with-java-lib-dir=DIR look in DIR for Java libraries],
	    [JAVA_LIB_DIR=$withval],)
AC_SUBST(JAVA_LIB_DIR)

dnl Where can we find the Python headers and libs?
AC_ARG_WITH(python-include-dir,
	    [--with-python-include-dir=DIR look in DIR for Python headers],
	    [PYTHON_INCLUDE_DIR=$withval],)
AC_SUBST(PYTHON_INCLUDE_DIR)
AC_ARG_WITH(python-lib-dir,
	    [--with-python-lib-dir=DIR look in DIR for Python libraries],
	    [PYTHON_LIB_DIR=$withval],)
AC_SUBST(PYTHON_LIB_DIR)

dnl If SWIG is available we will try to generate bindings.
AC_ARG_ENABLE(swig,
	[AS_HELP_STRING([--disable-swig],
			[do not generate any SWIG language bindings])])
if test "x$enable_swig" != xno; then
	AC_CHECK_PROG(SWIG, swig, swig)
	if test "x$SWIG" != x; then
		SWIG_VERSION=`swig -version 2>&1|grep Version|cut -d" " -f3`
		if test $SWIG_VERSION != 1.3.35; then
			AC_MSG_WARN([You have SWIG $SWIG_VERSION installed, but libkml requires SWIG 1.3.35. The bindings will not be built.])
			SWIG=
		fi
	fi
fi
AM_CONDITIONAL(WITH_SWIG, [test "x$SWIG" != x])

dnl Shall we install the python extensions in the system python extension
dnl directory? Note: to allow unprivileged installation this is off by default.
AC_ARG_ENABLE(systempython,
	[AS_HELP_STRING([--enable-systempython],
			[install the python SWIG bindings to the default system location using root privileges])])
USE_DEFAULT_PYTHON_LIBDIR=false
if test "x$enable_systempython" = xyes; then
	USE_DEFAULT_PYTHON_LIBDIR=true
fi

dnl Shall we build the Java SWIG bindings?
if test "x$SWIG" != x; then
	AC_ARG_ENABLE(java,
		      [AS_HELP_STRING([--disable-java],
				      [do not generate Java SWIG bindings])])
	if test "x$enable_java" != xno; then
		AC_CHECK_PROG(JAVA, javac, javac)
		if test "x$JAVA" != x; then
			current_CPPFLAGS="$CPPFLAGS"
			if test "$CPPFLAGS" != ""; then
				CPPFLAGS="-I$current_CPPFLAGS -I$JAVA_INCLUDE_DIR -I$JAVA_INCLUDE_DIR/$PLATFORM"
			else
				CPPFLAGS="-I$JAVA_INCLUDE_DIR -I$JAVA_INCLUDE_DIR/$PLATFORM"
			fi
			AC_CHECK_HEADER(jni.h,,
					[AC_MSG_WARN([could not find jni.h. The Java bindings will not be built.])
			JAVA=
			])
			CPPFLAGS="$current_CPPFLAGS"
		fi
	fi
fi
AM_CONDITIONAL(WITH_JAVA, [test "x$JAVA" != x])

dnl Shall we build the Python SWIG bindings?
if test "x$SWIG" != x; then
	AC_ARG_ENABLE(python,
		      [AS_HELP_STRING([--disable-python],
				      [do not generate Python SWIG bindings])])
	if test "x$enable_python" != xno; then
		AC_CHECK_PROG(PYTHON, python, python)
		if test "x$PYTHON" != x; then
			PYTHON_VERSION=`$PYTHON -c "import sys; print sys.version[[:3]]"`
			if test "$USE_DEFAULT_PYTHON_LIBDIR" = true; then
				PYTHON_SITE_PACKAGES_DIR=`$PYTHON -c 'import distutils.sysconfig; print distutils.sysconfig.get_python_lib(1,0)'`
			else
				PYTHON_SITE_PACKAGES_DIR=`$PYTHON -c 'import distutils.sysconfig; print distutils.sysconfig.get_python_lib(1,0,"${exec_prefix}")'`
			fi
			AC_SUBST(PYTHON_SITE_PACKAGES_DIR)
			if test "$PYTHON_INCLUDE_DIR" = ""; then
				PYTHON_INCLUDE_DIR=`$PYTHON -c "import distutils.sysconfig; print distutils.sysconfig.get_python_inc()"`
			fi
			if test "$PYTHON_LIB_DIR" = ""; then
				PYTHON_LIB_DIR=`$PYTHON -c "import distutils.sysconfig; print distutils.sysconfig.get_config_var('LIBPL')"`
			fi
			current_CPPFLAGS="$CPPFLAGS"
			CPPFLAGS="-I$PYTHON_INCLUDE_DIR"
			AC_CHECK_HEADER(Python.h,,
					[AC_MSG_WARN([could not find Python.h. The Python bindings will not be built.])
			PYTHON=
			])
			PYTHON_LIBNAME="python"
			if test "$PLATFORM" = "linux"; then
				PYTHON_LIBNAME="python$PYTHON_VERSION"
			fi
			AC_CHECK_LIB($PYTHON_LIBNAME, Py_Initialize,,
					[AC_MSG_WARN([could not find the Python library. The Python bindings will not be built.])
			PYTHON=
			])
			LIBS=''  # don't append -lpython here.
			CPPFLAGS="$current_CPPFLAGS"
		fi
	fi
fi
AM_CONDITIONAL(WITH_PYTHON, [test "x$PYTHON" != x])

dnl If libcurl is available we will build networking examples.
dnl TODO: permit a --with-curl-prefix, etc
AC_CHECK_HEADER([curl/curl.h],
                [AC_SUBST([HAVE_CURL_H], [1])]
                [AC_MSG_WARN([no curl.h])])
AC_CHECK_LIB([curl],
             [curl_easy_init],
             [AC_SUBST([HAVE_LIBCURL], [1])],
             [AC_MSG_WARN([no libcurl])],
             [])
AM_CONDITIONAL(WITH_LIBCURL,
               [test "x$HAVE_CURL_H" != x -a "x$HAVE_LIBCURL" != x])

dnl automake will add -DHAVE_CONFIG_H to compiler flags.
AM_CONFIG_HEADER([config.h]) 

AC_CONFIG_FILES([
	Makefile
	examples/Makefile
	examples/domviewer/Makefile
	examples/engine/Makefile
	examples/hellonet/Makefile
	examples/helloworld/Makefile
	examples/java/Makefile
	examples/kml/Makefile
	examples/python/Makefile
	examples/regionator/Makefile
	src/Makefile
	src/kml/Makefile
	src/kml/convenience/Makefile
	src/kml/dom/Makefile
	src/kml/engine/Makefile
	src/kml/regionator/Makefile
	src/kml/base/Makefile
	src/swig/Makefile
	src/swig/java/Makefile
	src/swig/python/Makefile
	testdata/Makefile
	testdata/balloon/Makefile
	testdata/csv/Makefile
	testdata/kmz/Makefile
	testdata/links/Makefile
	testdata/style/Makefile
	testdata/style/weather/Makefile
	third_party/Makefile
])

AC_OUTPUT

