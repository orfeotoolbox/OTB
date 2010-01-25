AM_CPPFLAGS = -I$(top_srcdir)/src \
	      -I$(top_srcdir)/examples/helloworld \
	      -I$(top_srcdir)/third_party/boost_1_34_1

if GCC
AM_CXXFLAGS = -Wall -Werror -ansi -pedantic -fno-rtti
endif

noinst_LTLIBRARIES = libhelloutil.la

libhelloutil_la_SOURCES = print.cc
libhelloutil_la_LIBADD = $(top_builddir)/third_party/libminizip.la

noinst_PROGRAMS = countkml createkml checklinks helloattrs helloenum \
                  hellofeatures hellofolder hellogeometry hellohref hellokmz \
                  helloregion helloworld parsekml prettykml printgeometry \
                  sortplacemarks

createkml_SOURCES = createkml.cc
createkml_LDADD = \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

checklinks_SOURCES = checklinks.cc
checklinks_LDADD = \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

countkml_SOURCES = countkml.cc
countkml_LDADD = \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

helloattrs_SOURCES = helloattrs.cc
helloattrs_LDADD = \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

helloenum_SOURCES = helloenum.cc
helloenum_LDADD = \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

hellofeatures_SOURCES = hellofeatures.cc
hellofeatures_LDADD = \
	libhelloutil.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

hellofolder_SOURCES = hellofolder.cc
hellofolder_LDADD = \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

hellogeometry_SOURCES = hellogeometry.cc
hellogeometry_LDADD = \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

hellohref_SOURCES = hellohref.cc
hellohref_LDADD = \
	libhelloutil.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

hellokmz_SOURCES = hellokmz.cc
hellokmz_LDADD = \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

helloregion_SOURCES = helloregion.cc
helloregion_LDADD = \
	libhelloutil.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

helloworld_SOURCES = helloworld.cc
helloworld_LDADD = \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

parsekml_SOURCES = parsekml.cc
parsekml_LDADD = \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

prettykml_SOURCES = prettykml.cc
prettykml_LDADD = \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

printgeometry_SOURCES = printgeometry.cc
printgeometry_LDADD = \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

sortplacemarks_SOURCES = sortplacemarks.cc
sortplacemarks_LDADD = \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

EXTRA_DIST = \
	print.h \
	createkml.vcproj \
	parsekml.vcproj \
	prettykml.vcproj \
	sortplacemarks.vcproj
