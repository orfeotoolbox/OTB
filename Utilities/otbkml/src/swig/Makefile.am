SUBDIRS = java python

AM_CPPFLAGS = -I$(top_srcdir)/src \
	      -I$(top_srcdir)/third_party/boost_1_34_1

COMMONLIBS = $(top_builddir)/src/kml/engine/libkmlengine.la \
             $(top_builddir)/src/kml/dom/libkmldom.la \
	     $(top_builddir)/src/kml/base/libkmlbase.la

pkglib_LTLIBRARIES=

if WITH_SWIG
if WITH_JAVA
pkglib_LTLIBRARIES += libkmldom_swig_java.la
pkglib_LTLIBRARIES += libkmlengine_swig_java.la
endif
if WITH_PYTHON
pkglib_LTLIBRARIES += libkmldom_swig_python.la
pkglib_LTLIBRARIES += libkmlengine_swig_python.la
endif
endif

libkmldom_swig_java_la_SOURCES = kmldom_java.cc
libkmldom_swig_java_la_CPPFLAGS = $(AM_CPPFLAGS) -I$(JAVA_INCLUDE_DIR) -I$(JAVA_INCLUDE_DIR)/$(PLATFORM)
libkmldom_swig_java_la_LIBADD = $(COMMONLIBS)
kmldom_java.cc: kmldom.i
	$(SWIG) -c++ $(AM_CPPFLAGS) -java -o kmldom_java.cc \
	$(top_srcdir)/src/swig/kmldom.i

libkmldom_swig_python_la_SOURCES = kmldom_python.cc kmldom.py
libkmldom_swig_python_la_CPPFLAGS = $(AM_CPPFLAGS) -I$(PYTHON_INCLUDE_DIR)
libkmldom_swig_python_la_LIBADD = $(COMMONLIBS)
libkmldom_swig_python_la_LDFLAGS = -dynamiclib -python -lexpat
kmldom_python.cc: kmldom.i
	$(SWIG) -c++ $(AM_CPPFLAGS) -python -o kmldom_python.cc \
	$(top_srcdir)/src/swig/kmldom.i

libkmlengine_swig_java_la_SOURCES = kmlengine_java.cc
libkmlengine_swig_java_la_CPPFLAGS = $(AM_CPPFLAGS) -I$(JAVA_INCLUDE_DIR) -I$(JAVA_INCLUDE_DIR)/$(PLATFORM)
libkmlengine_swig_java_la_LIBADD = $(COMMONLIBS)
kmlengine_java.cc: kmlengine.i
	$(SWIG) -c++ $(AM_CPPFLAGS) -java -o kmlengine_java.cc \
	$(top_srcdir)/src/swig/kmlengine.i

libkmlengine_swig_python_la_SOURCES = kmlengine_python.cc kmlengine.py
libkmlengine_swig_python_la_CPPFLAGS = $(AM_CPPFLAGS) -I$(PYTHON_INCLUDE_DIR)
libkmlengine_swig_python_la_LIBADD = $(COMMONLIBS)
libkmlengine_swig_python_la_LDFLAGS = -dynamiclib -python -lexpat
kmlengine_python.cc: kmlengine.i
	$(SWIG) -c++ $(AM_CPPFLAGS) -python -o kmlengine_python.cc \
	$(top_srcdir)/src/swig/kmlengine.i

if WITH_SWIG
if WITH_PYTHON
pythonext_DATA = _kmldom.so kmldom.py _kmlengine.so kmlengine.py
_kmldom.so:
	cp $(top_builddir)/src/swig/.libs/libkmldom_swig_python.$(SHLIBSUFFIX) _kmldom.so
kmldom.py:
	cp $(top_srcdir)/src/swig/kmldom.py kmldom.py
_kmlengine.so:
	cp $(top_builddir)/src/swig/.libs/libkmlengine_swig_python.$(SHLIBSUFFIX) _kmlengine.so
kmlengine.py:
	cp $(top_srcdir)/src/swig/kmlengine.py kmlengine.py

pythonextdir = $(PYTHON_SITE_PACKAGES_DIR)

clean-local:
	-rm -f _kmldom.so _kmlengine.so
endif
endif

EXTRA_DIST = \
	intrusive_ptr.i \
	kmldom.i \
	kmldom_test.py \
	kmlelements.i \
	kmlengine.i \
	kmlengine_test.py

# The Python library on both Linux and OS X has to be called _kmldom.so. As
# above, we accomplish this with a _DATA directive that copies the file. Since
# there is an inherit dependency, we have to disable parallel builds in this
# file, otherwise 'make -j3' or similar is likely to hit the race condition.
.NOTPARALLEL: 
