AM_CPPFLAGS = -I$(top_srcdir)/src \
	      -I$(top_srcdir)/third_party/boost_1_34_1


if GCC
AM_CXXFLAGS = -Wall -Werror -ansi -pedantic -fno-rtti
endif

lib_LTLIBRARIES = libkmlconvenience.la
libkmlconvenience_la_SOURCES = \
	csv_file.cc \
	convenience.cc \
	feature_list.cc \
	kmz_check_links.cc

# These header files will be installed in $(includedir)/kml/convenience
libkmlconvenienceincludedir = $(includedir)/kml/convenience
libkmlconvenienceinclude_HEADERS = \
	csv_file.h \
	convenience.h \
	feature_list.h \
	kmz_check_links.h

if HAVE_CPPUNIT 
DATA_DIR = $(top_srcdir)/testdata
TESTS = csv_file_test convenience_test kmz_check_links_test feature_list_test
check_PROGRAMS = $(TESTS)

convenience_test_SOURCES = convenience_test.cc
convenience_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
convenience_test_LDFLAGS = $(CPPUNIT_LIBS)
convenience_test_LDADD = libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

csv_file_test_SOURCES = csv_file_test.cc
csv_file_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(CPPUNIT_CFLAGS)
csv_file_test_LDFLAGS = $(CPPUNIT_LIBS)
csv_file_test_LDADD = libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

feature_list_test_SOURCES = feature_list_test.cc
feature_list_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
feature_list_test_LDFLAGS = $(CPPUNIT_LIBS)
feature_list_test_LDADD = libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

kmz_check_links_test_SOURCES = kmz_check_links_test.cc
kmz_check_links_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(CPPUNIT_CFLAGS)
kmz_check_links_test_LDFLAGS = $(CPPUNIT_LIBS)
kmz_check_links_test_LDADD = libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

CLEANFILES = check_PROGRAMS
endif
