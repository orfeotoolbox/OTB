AM_CPPFLAGS = -I$(top_srcdir)/src \
	      -I$(top_srcdir)/third_party/boost_1_34_1 \
              -I$(top_srcdir)/third_party/googletest-r108/include

if GCC
AM_CXXFLAGS = -Wall -Wextra -Wno-unused-parameter -Werror -ansi -pedantic -fno-rtti
AM_TEST_CXXFLAGS = -Wall -Wextra -Wno-unused-parameter -Werror -ansi -fno-rtti -DGTEST_HAS_RTTI=0
endif

# These header files are added to the distribution such that it can be built,
# but these header files should not be used in application code.
EXTRA_DIST = \
	http_client_test_util.h

lib_LTLIBRARIES = libkmlconvenience.la
libkmlconvenience_la_SOURCES = \
	atom_util.cc \
	convenience.cc \
	csv_file.cc \
	csv_parser.cc \
	feature_list.cc \
	google_doc_list.cc \
	google_maps_data.cc \
	google_picasa_web.cc \
	google_spreadsheets.cc \
	http_client.cc \
	kmz_check_links.cc

# These header files will be installed in $(includedir)/kml/convenience
libkmlconvenienceincludedir = $(includedir)/kml/convenience
libkmlconvenienceinclude_HEADERS = \
	atom_util.h \
	convenience.h \
	csv_file.h \
	csv_parser.h \
	element_counter.h \
	feature_list.h \
	google_doc_list.h \
	google_maps_data.h \
	google_picasa_web.h \
	google_spreadsheets.h \
	gpx_trk_pt_handler.h \
	http_client.h \
	kmz_check_links.h

DATA_DIR = $(top_srcdir)/testdata
TESTS = atom_util_test \
        csv_file_test \
        csv_parser_test \
	convenience_test \
	element_counter_test \
	feature_list_test \
	google_doc_list_test \
	google_maps_data_test \
	google_picasa_web_test \
	google_spreadsheets_test \
	gpx_trk_pt_handler_test \
	http_client_test \
	kmz_check_links_test

check_PROGRAMS = $(TESTS)

atom_util_test_SOURCES = atom_util_test.cc
atom_util_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
atom_util_test_LDADD = libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

convenience_test_SOURCES = convenience_test.cc
convenience_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
convenience_test_LDADD = libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

csv_file_test_SOURCES = csv_file_test.cc
csv_file_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
csv_file_test_LDADD = libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

csv_parser_test_SOURCES = csv_parser_test.cc
csv_parser_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
csv_parser_test_LDADD = libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

element_counter_test_SOURCES = element_counter_test.cc
element_counter_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
element_counter_test_LDADD = libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

feature_list_test_SOURCES = feature_list_test.cc
feature_list_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
feature_list_test_LDADD = libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

google_doc_list_test_SOURCES = google_doc_list_test.cc
google_doc_list_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
google_doc_list_test_LDADD = libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

google_maps_data_test_SOURCES = google_maps_data_test.cc
google_maps_data_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
google_maps_data_test_LDADD = libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

google_picasa_web_test_SOURCES = google_picasa_web_test.cc
google_picasa_web_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
google_picasa_web_test_LDADD = libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

google_spreadsheets_test_SOURCES = google_spreadsheets_test.cc
google_spreadsheets_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
google_spreadsheets_test_LDADD = libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

gpx_trk_pt_handler_test_SOURCES = gpx_trk_pt_handler_test.cc
gpx_trk_pt_handler_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
gpx_trk_pt_handler_test_LDADD = libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/third_party/libgtest.la

http_client_test_SOURCES = http_client_test.cc
http_client_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
http_client_test_LDADD = http_client.o \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

kmz_check_links_test_SOURCES = kmz_check_links_test.cc
kmz_check_links_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
kmz_check_links_test_LDADD = libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

CLEANFILES = check_PROGRAMS
