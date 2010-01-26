AM_CPPFLAGS = -I$(top_srcdir)/src \
	      -I$(top_srcdir)/third_party/boost_1_34_1

if GCC
AM_CXXFLAGS = -Wall -Werror -ansi -pedantic -fno-rtti
endif

noinst_PROGRAMS = balloonwalker clone kmlfile kmzchecklinks printstyle

balloonwalker_SOURCES = balloonwalker.cc
balloonwalker_LDADD = \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

clone_SOURCES = clone.cc
clone_LDADD = \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

kmlfile_SOURCES = kmlfile.cc
kmlfile_LDADD = \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

kmzchecklinks_SOURCES = kmzchecklinks.cc
kmzchecklinks_LDADD = \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

printstyle_SOURCES = printstyle.cc
printstyle_LDADD = \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

EXTRA_DIST = \
	printstyle.vcproj
