AM_CPPFLAGS = -I$(top_srcdir)/src \
              -I$(top_srcdir)/third_party/boost_1_34_1 \
              -I$(top_srcdir)/third_party/uriparser-0.7.1/include

if GCC
AM_CXXFLAGS = -Wall -Werror -ansi -pedantic -fno-rtti
endif

lib_LTLIBRARIES = libkmlbase.la
libkmlbase_la_SOURCES = \
	file.cc \
	file_posix.cc \
	referent.cc \
	string_util.cc \
	time_util.cc

# These header files will be installed in $(includedir)/kml/base
libkmlbaseincludedir = $(includedir)/kml/base
libkmlbaseinclude_HEADERS = \
	file.h \
	memory_file.h \
	net_cache.h \
	referent.h \
	string_util.h \
	tempfile.h \
	time_util.h \
	unit_test.h \
	uri_parser.h \
	util.h

EXTRA_DIST = \
	file_win32.cc \
	net_cache_test_util.h

if HAVE_CPPUNIT 
DATA_DIR = $(top_srcdir)/testdata
TESTS = \
	file_test \
	net_cache_test \
	referent_test \
	string_util_test \
	tempfile_test \
	time_util_test \
	uri_parser_test

check_PROGRAMS = $(TESTS)

file_test_SOURCES = file_test.cc
file_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(CPPUNIT_CFLAGS)
file_test_LDFLAGS = $(CPPUNIT_LIBS)
file_test_LDADD = libkmlbase.la

net_cache_test_SOURCES = net_cache_test.cc
net_cache_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(CPPUNIT_CFLAGS)
net_cache_test_LDFLAGS = $(CPPUNIT_LIBS)
net_cache_test_LDADD = libkmlbase.la \
        $(top_builddir)/third_party/liburiparser.la

referent_test_SOURCES = referent_test.cc
referent_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
referent_test_LDFLAGS = $(CPPUNIT_LIBS)
referent_test_LDADD= libkmlbase.la

tempfile_test_SOURCES = tempfile_test.cc
tempfile_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(CPPUNIT_CFLAGS)
tempfile_test_LDFLAGS = $(CPPUNIT_LIBS)
tempfile_test_LDADD = libkmlbase.la \
        $(top_builddir)/src/kml/dom/libkmldom.la

string_util_test_SOURCES = string_util_test.cc
string_util_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
string_util_test_LDFLAGS = $(CPPUNIT_LIBS)
string_util_test_LDADD= libkmlbase.la

time_util_test_SOURCES = time_util_test.cc
time_util_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
time_util_test_LDFLAGS = $(CPPUNIT_LIBS)
time_util_test_LDADD= libkmlbase.la

uri_parser_test_SOURCES = uri_parser_test.cc
uri_parser_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
uri_parser_test_LDFLAGS = $(CPPUNIT_LIBS)
uri_parser_test_LDADD= libkmlbase.la \
        $(top_builddir)/third_party/liburiparser.la

CLEANFILES = check_PROGRAMS
endif
