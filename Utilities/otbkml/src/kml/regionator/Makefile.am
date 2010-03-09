AM_CPPFLAGS= -I$(top_srcdir)/src \
             -I$(top_srcdir)/third_party/boost_1_34_1 \
	     -I$(top_srcdir)/third_party/googletest-r108/include


if GCC
AM_CXXFLAGS = -Wall -Wextra -Wno-unused-parameter -Werror -ansi -pedantic -fno-rtti
AM_TEST_CXXFLAGS = -Wall -Wextra -Wno-unused-parameter -Werror -ansi -fno-rtti -DGTEST_HAS_RTTI=0
endif

lib_LTLIBRARIES = libkmlregionator.la
libkmlregionator_la_SOURCES = \
	feature_list_region_handler.cc \
	regionator.cc \
	regionator_util.cc

# These header files will be installed in $(includedir)/kml/regionator
libkmlregionatorincludedir = $(includedir)/kml/regionator
libkmlregionatorinclude_HEADERS = \
	feature_list_regionator.h \
	feature_list_region_handler.h \
	region_handler.h \
	regionator.h \
	regionator_qid.h \
	regionator_util.h

TESTS = \
	feature_list_region_handler_test \
	regionator_test \
	regionator_qid_test \
	regionator_util_test
check_PROGRAMS = $(TESTS)

feature_list_region_handler_test_SOURCES = feature_list_region_handler_test.cc
feature_list_region_handler_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
feature_list_region_handler_test_LDADD = libkmlregionator.la \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

regionator_test_SOURCES = regionator_test.cc
regionator_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
regionator_test_LDADD = libkmlregionator.la \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

regionator_qid_test_SOURCES = regionator_qid_test.cc
regionator_qid_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
regionator_qid_test_LDADD = libkmlregionator.la \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

regionator_util_test_SOURCES = regionator_util_test.cc
regionator_util_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
regionator_util_test_LDADD = libkmlregionator.la \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

CLEANFILES = check_PROGRAMS
