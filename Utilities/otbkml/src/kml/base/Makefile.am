AM_CPPFLAGS = -I$(top_srcdir)/src \
              -I$(top_srcdir)/third_party/boost_1_34_1 \
              -I$(top_srcdir)/third_party/googletest-r108/include \
              -I$(top_srcdir)/third_party/uriparser-0.7.5/include \
              -I$(top_srcdir)/third_party/zlib-1.2.3/contrib

if GCC
AM_CXXFLAGS = -Wall -Wextra -Wno-unused-parameter -Werror -ansi -pedantic -fno-rtti
AM_TEST_CXXFLAGS = -Wall -Wextra -Wno-unused-parameter -Werror -ansi -fno-rtti -DGTEST_HAS_RTTI=0
endif

libkmlbase_la_LDFLAGS = -lexpat

lib_LTLIBRARIES = libkmlbase.la
libkmlbase_la_SOURCES = \
	attributes.cc \
	csv_splitter.cc \
	date_time.cc \
	expat_handler_ns.cc \
	expat_parser.cc \
	file.cc \
	file_posix.cc \
	math_util.cc \
	mimetypes.cc \
	referent.cc \
	string_util.cc \
	time_util.cc \
	uri_parser.cc \
	version.cc \
	xml_namespaces.cc \
	zip_file.cc

libkmlbase_la_LIBADD = \
	$(top_builddir)/third_party/libminizip.la \
	$(top_builddir)/third_party/liburiparser.la

# These header files will be installed in $(includedir)/kml/base
libkmlbaseincludedir = $(includedir)/kml/base
libkmlbaseinclude_HEADERS = \
	attributes.h \
	csv_splitter.h \
	date_time.h \
	color32.h \
	expat_handler.h \
	expat_handler_ns.h \
	expat_parser.h \
	file.h \
	math_util.h \
	memory_file.h \
	mimetypes.h \
	net_cache.h \
	referent.h \
	string_util.h \
	tempfile.h \
	time_util.h \
	util.h \
	vec3.h \
	version.h \
	xml_element.h \
	xml_file.h \
	xml_namespaces.h \
	xmlns.h \
	zip_file.h

EXTRA_DIST = \
	file_win32.cc \
	net_cache_test_util.h \
	unit_test.h \
	uri_parser.h

DATA_DIR = $(top_srcdir)/testdata
TESTS = \
	attributes_test \
	color32_test \
	csv_splitter_test \
	date_time_test \
	expat_handler_ns_test \
	expat_parser_test \
	file_test \
	math_util_test \
	net_cache_test \
	referent_test \
	string_util_test \
	tempfile_test \
	time_util_test \
	uri_parser_test \
	util_test \
	vec3_test \
	version_test \
	xml_element_test \
	xml_file_test \
	xml_namespaces_test \
	xmlns_test \
	zip_file_test

check_PROGRAMS = $(TESTS)

attributes_test_SOURCES = attributes_test.cc
attributes_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
attributes_test_LDADD = libkmlbase.la \
			$(top_builddir)/third_party/libgtest.la

color32_test_SOURCES = color32_test.cc
color32_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
color32_test_LDADD = libkmlbase.la \
		     $(top_builddir)/third_party/libgtest.la

csv_splitter_test_SOURCES = csv_splitter_test.cc
csv_splitter_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
csv_splitter_test_LDADD = libkmlbase.la \
			$(top_builddir)/third_party/libgtest.la

date_time_test_SOURCES = date_time_test.cc
date_time_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
date_time_test_LDADD = libkmlbase.la \
		       $(top_builddir)/third_party/libgtest.la

expat_handler_ns_test_SOURCES = expat_handler_ns_test.cc
expat_handler_ns_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
expat_handler_ns_test_LDADD = libkmlbase.la \
			      $(top_builddir)/third_party/libgtest.la

expat_parser_test_SOURCES = expat_parser_test.cc
expat_parser_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
expat_parser_test_LDADD = libkmlbase.la \
			      $(top_builddir)/third_party/libgtest.la

file_test_SOURCES = file_test.cc
file_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
file_test_LDADD = libkmlbase.la \
		  $(top_builddir)/third_party/libgtest.la

math_util_test_SOURCES = math_util_test.cc
math_util_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
math_util_test_LDADD = libkmlbase.la \
		  $(top_builddir)/third_party/libgtest.la

net_cache_test_SOURCES = net_cache_test.cc
net_cache_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
net_cache_test_LDADD = libkmlbase.la \
        $(top_builddir)/third_party/liburiparser.la \
	$(top_builddir)/third_party/libgtest.la

referent_test_SOURCES = referent_test.cc
referent_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
referent_test_LDADD= libkmlbase.la \
		     $(top_builddir)/third_party/libgtest.la

string_util_test_SOURCES = string_util_test.cc
string_util_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
string_util_test_LDADD= libkmlbase.la \
			$(top_builddir)/third_party/libgtest.la

tempfile_test_SOURCES = tempfile_test.cc
tempfile_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
tempfile_test_LDADD = libkmlbase.la \
		      $(top_builddir)/third_party/libgtest.la

time_util_test_SOURCES = time_util_test.cc
time_util_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
time_util_test_LDADD= libkmlbase.la \
		      $(top_builddir)/third_party/libgtest.la

uri_parser_test_SOURCES = uri_parser_test.cc
uri_parser_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
uri_parser_test_LDADD= libkmlbase.la \
        $(top_builddir)/third_party/liburiparser.la \
	$(top_builddir)/third_party/libgtest.la

util_test_SOURCES = util_test.cc
util_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
util_test_LDADD= libkmlbase.la \
		 $(top_builddir)/third_party/libgtest.la

vec3_test_SOURCES = vec3_test.cc
vec3_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
vec3_test_LDADD= libkmlbase.la \
		 $(top_builddir)/third_party/libgtest.la

version_test_SOURCES = version_test.cc
version_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
version_test_LDADD= libkmlbase.la \
		 $(top_builddir)/third_party/libgtest.la

xml_element_test_SOURCES = xml_element_test.cc
xml_element_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
xml_element_test_LDADD= libkmlbase.la \
		 $(top_builddir)/third_party/libgtest.la

xml_file_test_SOURCES = xml_file_test.cc
xml_file_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
xml_file_test_LDADD= libkmlbase.la \
		 $(top_builddir)/third_party/libgtest.la

xml_namespaces_test_SOURCES = xml_namespaces_test.cc
xml_namespaces_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
xml_namespaces_test_LDADD= libkmlbase.la \
		 $(top_builddir)/third_party/libgtest.la

xmlns_test_SOURCES = xmlns_test.cc
xmlns_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
xmlns_test_LDADD= libkmlbase.la \
		  $(top_builddir)/third_party/libgtest.la

zip_file_test_SOURCES = zip_file_test.cc
zip_file_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
zip_file_test_LDADD= libkmlbase.la \
		 $(top_builddir)/third_party/libminizip.la \
		 $(top_builddir)/third_party/libgtest.la

CLEANFILES = check_PROGRAMS

