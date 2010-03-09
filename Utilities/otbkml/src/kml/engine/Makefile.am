AM_CPPFLAGS= -I$(top_srcdir)/src \
	     -I$(top_srcdir)/third_party/boost_1_34_1 \
	     -I$(top_srcdir)/third_party/googletest-r108/include

if GCC
AM_CXXFLAGS = -Wall -Wextra -Wno-unused-parameter -Werror -ansi -pedantic -fno-rtti
AM_TEST_CXXFLAGS = -Wall -Wextra -Wno-unused-parameter -Werror -ansi -fno-rtti -DGTEST_HAS_RTTI=0
endif

lib_LTLIBRARIES = libkmlengine.la
libkmlengine_la_SOURCES = \
	clone.cc \
	entity_mapper.cc \
	feature_balloon.cc \
	feature_view.cc \
	feature_visitor.cc \
	find.cc \
	find_xml_namespaces.cc \
	get_link_parents.cc \
	get_links.cc \
	href.cc \
	id_mapper.cc \
	kml_cache.cc \
	kml_file.cc \
	kml_stream.cc \
	kml_uri.cc \
	kmz_cache.cc \
	kmz_file.cc \
	link_util.cc \
	location_util.cc \
	merge.cc \
	parse_old_schema.cc \
	style_inliner.cc \
	style_merger.cc \
	style_resolver.cc \
	style_splitter.cc \
	update_processor.cc \
	update.cc

libkmlengine_la_LIBADD = \
	$(top_builddir)/src/kml/base/libkmlbase.la

# These header files will be installed in $(includedir)/kml/engine for use in
# application code.
libkmlengineincludedir = $(includedir)/kml/engine
libkmlengineinclude_HEADERS = \
	bbox.h \
	clone.h \
	engine_types.h \
	entity_mapper.h \
	feature_balloon.h \
	feature_view.h \
	feature_visitor.h \
	find.h \
	find_xml_namespaces.h \
	get_link_parents.h \
	get_links.h \
	href.h \
	id_mapper.h \
	kml_cache.h \
	kml_file.h \
	kml_stream.h \
	kml_uri.h \
	kmz_cache.h \
	kmz_file.h \
	link_util.h \
	location_util.h \
	merge.h \
	object_id_parser_observer.h \
	old_schema_parser_observer.h \
	parse_old_schema.h \
	schema_parser_observer.h \
	shared_style_parser_observer.h \
	style_inliner.h \
	style_merger.h \
	style_resolver.h \
	style_splitter.h \
	update.h

# These header files are added to the distribution such that it can be built,
# but these header files should not be used in application code.
EXTRA_DIST = \
	engine_constants.h \
	id_mapper_internal.h \
	kml_uri_internal.h \
	style_inliner_internal.h \
	style_splitter_internal.h \
	update_processor.h

DATA_DIR = $(top_srcdir)/testdata
TESTS = bbox_test \
	clone_test \
	entity_mapper_test \
	feature_balloon_test \
	feature_visitor_test \
	feature_view_test\
	find_test \
	find_xml_namespaces_test \
	get_link_parents_test \
	get_links_test \
	href_test \
	id_mapper_test \
	kmz_cache_test \
	kml_cache_test \
	kml_file_test \
	kml_stream_test \
	kml_uri_test \
	kmz_file_test \
	link_util_test \
	location_util_test \
	merge_test \
	object_id_parser_observer_test \
	old_schema_parser_observer_test \
	parse_old_schema_test \
	schema_parser_observer_test \
	shared_style_parser_observer_test \
	style_inliner_test \
	style_merger_test \
	style_resolver_test \
	style_splitter_test \
	update_processor_test \
	update_test

check_PROGRAMS = $(TESTS)

# Unit tests for KML Engine
bbox_test_SOURCES = bbox_test.cc
bbox_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
bbox_test_LDADD = libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

clone_test_SOURCES = clone_test.cc
clone_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
clone_test_LDADD = libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

entity_mapper_test_SOURCES = entity_mapper_test.cc
entity_mapper_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
entity_mapper_test_LDADD= libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

feature_balloon_test_SOURCES = feature_balloon_test.cc
feature_balloon_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
feature_balloon_test_LDADD= libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

feature_view_test_SOURCES = feature_view_test.cc
feature_view_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
feature_view_test_LDADD= libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

feature_visitor_test_SOURCES = feature_visitor_test.cc
feature_visitor_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
feature_visitor_test_LDADD= libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

find_test_SOURCES = find_test.cc
find_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
find_test_LDADD= libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

get_link_parents_test_SOURCES = get_link_parents_test.cc
get_link_parents_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
get_link_parents_test_LDADD= libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

get_links_test_SOURCES = get_links_test.cc
get_links_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
get_links_test_LDADD= libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

href_test_SOURCES = href_test.cc
href_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
href_test_LDADD= libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

id_mapper_test_SOURCES = id_mapper_test.cc
id_mapper_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
id_mapper_test_LDADD= libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

kml_cache_test_SOURCES = kml_cache_test.cc
kml_cache_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
kml_cache_test_LDADD= libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

kml_file_test_SOURCES = kml_file_test.cc
kml_file_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
kml_file_test_LDADD= libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

kml_stream_test_SOURCES = kml_stream_test.cc
kml_stream_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
kml_stream_test_LDADD= libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

kml_uri_test_SOURCES = kml_uri_test.cc
kml_uri_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
kml_uri_test_LDADD= libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

kmz_cache_test_SOURCES = kmz_cache_test.cc
kmz_cache_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
kmz_cache_test_LDADD= libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

kmz_file_test_SOURCES = kmz_file_test.cc
kmz_file_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
kmz_file_test_LDADD= libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

link_util_test_SOURCES = link_util_test.cc
link_util_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
link_util_test_LDADD = libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

location_util_test_SOURCES = location_util_test.cc
location_util_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
location_util_test_LDADD = libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

merge_test_SOURCES = merge_test.cc
merge_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
merge_test_LDADD= libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

object_id_parser_observer_test_SOURCES = object_id_parser_observer_test.cc
object_id_parser_observer_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
object_id_parser_observer_test_LDADD= libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

old_schema_parser_observer_test_SOURCES = old_schema_parser_observer_test.cc
old_schema_parser_observer_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
old_schema_parser_observer_test_LDADD= libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

parse_old_schema_test_SOURCES = parse_old_schema_test.cc
parse_old_schema_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
parse_old_schema_test_LDADD= libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

schema_parser_observer_test_SOURCES = schema_parser_observer_test.cc
schema_parser_observer_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
schema_parser_observer_test_LDADD= \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

shared_style_parser_observer_test_SOURCES = shared_style_parser_observer_test.cc
shared_style_parser_observer_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
shared_style_parser_observer_test_LDADD= \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

style_inliner_test_SOURCES = style_inliner_test.cc
style_inliner_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
style_inliner_test_LDADD= libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/liburiparser.la \
	$(top_builddir)/third_party/libgtest.la

style_merger_test_SOURCES = style_merger_test.cc
style_merger_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
style_merger_test_LDADD= libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

style_resolver_test_SOURCES = style_resolver_test.cc
style_resolver_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
style_resolver_test_LDADD= libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

style_splitter_test_SOURCES = style_splitter_test.cc
style_splitter_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
style_splitter_test_LDADD= libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

update_processor_test_SOURCES = update_processor_test.cc
update_processor_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
update_processor_test_LDADD= libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

update_test_SOURCES = update_test.cc
update_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
update_test_LDADD= libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

find_xml_namespaces_test_SOURCES = find_xml_namespaces_test.cc
find_xml_namespaces_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
find_xml_namespaces_test_LDADD= libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

CLEANFILES = check_PROGRAMS
