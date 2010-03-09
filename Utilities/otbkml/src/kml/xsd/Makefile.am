AM_CPPFLAGS = -I$(top_srcdir)/src \
              -I$(top_srcdir)/third_party/boost_1_34_1 \
	      -I$(top_srcdir)/third_party/googletest-r108/include

if GCC
AM_CXXFLAGS = -Wall -Wextra -Wno-unused-parameter -Werror -ansi -pedantic -fno-rtti
AM_TEST_CXXFLAGS = -Wall -Wextra -Wno-unused-parameter -Werror -ansi -fno-rtti -DGTEST_HAS_RTTI=0
endif

lib_LTLIBRARIES = libkmlxsd.la
libkmlxsd_la_SOURCES = \
	xsd_complex_type.cc \
	xsd_element.cc \
	xsd_file.cc \
	xsd_handler.cc \
	xsd_primitive_type.cc \
	xsd_util.cc \
	xst_parser.cc

# These header files will be installed in $(includedir)/kml/xsd
libkmlxsdincludedir = $(includedir)/kml/xsd
libkmlxsdinclude_HEADERS = \
	xsd_complex_type.h \
	xsd_element.h \
	xsd_file.h \
	xsd_handler.h \
	xsd_primitive_type.h \
	xsd_simple_type.h \
	xsd_schema.h \
	xsd_type.h \
	xsd_util.h \
	xst_parser.h

DATA_DIR = $(top_srcdir)/testdata
TESTS = \
	xsd_complex_type_test \
	xsd_file_test \
	xsd_element_test \
	xsd_handler_test \
	xsd_primitive_type_test \
	xsd_simple_type_test \
	xsd_schema_test \
	xsd_util_test \
	xst_parser_test

check_PROGRAMS = $(TESTS)

xsd_complex_type_test_SOURCES = xsd_complex_type_test.cc
xsd_complex_type_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
xsd_complex_type_test_LDADD = libkmlxsd.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

xsd_file_test_SOURCES = xsd_file_test.cc
xsd_file_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
xsd_file_test_LDADD = libkmlxsd.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

xsd_element_test_SOURCES = xsd_element_test.cc
xsd_element_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
xsd_element_test_LDADD = libkmlxsd.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

xsd_handler_test_SOURCES = xsd_handler_test.cc
xsd_handler_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
xsd_handler_test_LDADD = libkmlxsd.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

xsd_primitive_type_test_SOURCES = xsd_primitive_type_test.cc
xsd_primitive_type_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
xsd_primitive_type_test_LDADD = libkmlxsd.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

xsd_schema_test_SOURCES = xsd_schema_test.cc
xsd_schema_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
xsd_schema_test_LDADD = libkmlxsd.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

xsd_simple_type_test_SOURCES = xsd_simple_type_test.cc
xsd_simple_type_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
xsd_simple_type_test_LDADD = libkmlxsd.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

xsd_util_test_SOURCES = xsd_util_test.cc
xsd_util_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
xsd_util_test_LDADD = libkmlxsd.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

xst_parser_test_SOURCES = xst_parser_test.cc
xst_parser_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
xst_parser_test_LDADD = libkmlxsd.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

CLEANFILES = check_PROGRAMS
