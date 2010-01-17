if WITH_LIBCURL
AM_CPPFLAGS = -I$(top_srcdir)/src \
	      -I$(top_srcdir)/examples/hellonet \
	      -I$(top_srcdir)/third_party/boost_1_34_1

if GCC
AM_CXXFLAGS = -Wall -Werror -ansi -pedantic -fno-rtti
endif

noinst_PROGRAMS = kmlwalk kmzfetch resolvestyle

noinst_LTLIBRARIES = libcurlfetch.la

libcurlfetch_la_SOURCES = curlfetch.cc
libcurlfetch_la_LIBADD = -lcurl

kmlwalk_SOURCES = kmlwalk.cc
kmlwalk_LDADD = \
        libcurlfetch.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

kmzfetch_SOURCES = kmzfetch.cc
kmzfetch_LDADD = \
        libcurlfetch.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

resolvestyle_SOURCES = resolvestyle.cc
resolvestyle_LDADD = \
        libcurlfetch.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

EXTRA_DIST = README curlfetch.h
endif
