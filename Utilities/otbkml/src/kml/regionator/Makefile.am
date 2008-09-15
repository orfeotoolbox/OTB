AM_CPPFLAGS= -I$(top_srcdir)/src \
             -I$(top_srcdir)/third_party/boost_1_34_1

if GCC
AM_CXXFLAGS = -Wall -Werror -ansi -pedantic -fno-rtti
endif

lib_LTLIBRARIES = libkmlregionator.la
libkmlregionator_la_SOURCES = \
	feature_list_region_handler.cc \
	regionator.cc \
	regionator_util.cc

# These header files will be installed in $(includedir)/kml/regionator
libkmlregionatorincludedir = $(includedir)/kml/regionator
libkmlregionatorinclude_HEADERS = \
	feature_list_region_handler.h \
	region_handler.h \
	regionator.h \
	regionator_qid.h \
	regionator_util.h

if HAVE_CPPUNIT 
TESTS = \
	feature_list_region_handler_test \
	regionator_test \
	regionator_qid_test \
	regionator_util_test
check_PROGRAMS = $(TESTS)

feature_list_region_handler_test_SOURCES = feature_list_region_handler_test.cc
feature_list_region_handler_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
feature_list_region_handler_test_LDFLAGS = $(CPPUNIT_LIBS)
feature_list_region_handler_test_LDADD= libkmlregionator.la \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

regionator_test_SOURCES = regionator_test.cc
regionator_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
regionator_test_LDFLAGS = $(CPPUNIT_LIBS)
regionator_test_LDADD= libkmlregionator.la \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

regionator_qid_test_SOURCES = regionator_qid_test.cc
regionator_qid_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
regionator_qid_test_LDFLAGS = $(CPPUNIT_LIBS)

regionator_util_test_SOURCES = regionator_util_test.cc
regionator_util_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
regionator_util_test_LDFLAGS = $(CPPUNIT_LIBS)
regionator_util_test_LDADD= libkmlregionator.la \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

CLEANFILES = check_PROGRAMS
endif
