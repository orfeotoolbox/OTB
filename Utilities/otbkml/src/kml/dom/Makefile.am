AM_CPPFLAGS= -I$(top_srcdir)/src \
	     -I$(top_srcdir)/third_party/boost_1_34_1 \
	     -I$(top_srcdir)/third_party/googletest-r108/include

if GCC
AM_CXXFLAGS = -Wall -Wextra -Wno-unused-parameter -Werror -ansi -pedantic -fno-rtti
AM_TEST_CXXFLAGS = -Wall -Wextra -Wno-unused-parameter -Werror -ansi -fno-rtti -DGTEST_HAS_RTTI=0
endif

lib_LTLIBRARIES = libkmldom.la
libkmldom_la_SOURCES = \
	abstractlatlonbox.cc \
	abstractview.cc \
	atom.cc \
	balloonstyle.cc \
	colorstyle.cc \
	container.cc \
	document.cc \
	element.cc \
	extendeddata.cc \
	feature.cc \
	folder.cc \
	geometry.cc \
	hotspot.cc \
	iconstyle.cc \
	kml_cast.cc \
	kml_factory.cc \
	kml.cc \
	link.cc \
	labelstyle.cc \
	linestyle.cc \
	liststyle.cc \
	model.cc \
	networklink.cc \
	networklinkcontrol.cc \
	object.cc \
	overlay.cc \
	placemark.cc \
	polystyle.cc \
	region.cc \
	schema.cc \
	snippet.cc \
	style.cc \
	stylemap.cc \
	styleselector.cc \
	substyle.cc \
	timeprimitive.cc \
	gx_timeprimitive.cc \
	gx_tour.cc \
	vec2.cc \
	kml_handler.cc \
	kml_handler_ns.cc \
	parser.cc \
	serializer.cc \
	xal.cc \
	xml_serializer.cc \
	xsd.cc \
	visitor.cc \
	visitor_driver.cc

libkmldomincludedir = $(includedir)/kml/dom
libkmldominclude_HEADERS = \
	abstractlatlonbox.h \
	abstractview.h \
	atom.h \
	balloonstyle.h \
	colorstyle.h \
	container.h \
	document.h \
	element.h \
	extendeddata.h \
	feature.h \
	folder.h \
	geometry.h \
	hotspot.h \
	iconstyle.h \
	kml.h \
	kml22.h \
	kml_cast.h \
	kml_factory.h \
	kml_funcs.h \
	kml_ptr.h \
	kmldom.h \
	labelstyle.h \
	linestyle.h \
	link.h \
	liststyle.h \
	model.h \
	networklink.h \
	networklinkcontrol.h \
	object.h \
	overlay.h \
	parser.h \
	parser_observer.h \
	placemark.h \
	polystyle.h \
	region.h \
	schema.h \
	snippet.h \
	style.h \
	stylemap.h \
	styleselector.h \
	substyle.h \
	timeprimitive.h \
	gx_timeprimitive.h \
	gx_tour.h \
	vec2.h \
	xal.h \
	visitor.h \
	visitor_driver.h

EXTRA_DIST = \
	kml_handler.h \
	kml_handler_ns.h \
	serializer.h \
	stats_serializer.h \
	xml_serializer.h \
	xsd.h \
	kml22.cc

DATA_DIR = $(top_srcdir)/testdata

TESTS = abstractlatlonbox_test \
	abstractview_test \
	atom_test \
	balloonstyle_test \
	colorstyle_test \
	container_test \
	document_test \
	element_test \
	extendeddata_test \
	feature_test \
	folder_test \
	geometry_test \
	hotspot_test \
	iconstyle_test \
	kml_cast_test \
	kml_test \
	kml_factory_test \
	kml22_test \
	labelstyle_test \
	linestyle_test \
	link_test \
	liststyle_test \
	model_test \
	networklink_test \
	networklinkcontrol_test \
	object_test \
	overlay_test \
	placemark_test \
	polystyle_test \
	region_test \
	round_trip_test \
	schema_test \
	snippet_test \
	style_test \
	stylemap_test \
	styleselector_test \
	substyle_test \
	timeprimitive_test \
	vec2_test \
	unknown_test \
	kml_handler_test \
	kml_handler_ns_test \
	parser_test \
	serializer_test \
	gx_timeprimitive_test \
	gx_tour_test \
	xal_test \
	xml_serializer_test \
	xsd_test \
	visitor_test \
	visitor_driver_test


check_PROGRAMS = $(TESTS)

# Unit tests for KMLDOM classes.
abstractlatlonbox_test_SOURCES = abstractlatlonbox_test.cc
abstractlatlonbox_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
abstractlatlonbox_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

abstractview_test_SOURCES = abstractview_test.cc
abstractview_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
abstractview_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

atom_test_SOURCES = atom_test.cc
atom_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
atom_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

balloonstyle_test_SOURCES = balloonstyle_test.cc
balloonstyle_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
balloonstyle_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

colorstyle_test_SOURCES = colorstyle_test.cc
colorstyle_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
colorstyle_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

container_test_SOURCES = container_test.cc
container_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
container_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

document_test_SOURCES = document_test.cc
document_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
document_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

element_test_SOURCES = element_test.cc
element_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
element_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

extendeddata_test_SOURCES = extendeddata_test.cc
extendeddata_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
extendeddata_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

feature_test_SOURCES = feature_test.cc
feature_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
feature_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

folder_test_SOURCES = folder_test.cc
folder_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
folder_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

geometry_test_SOURCES = geometry_test.cc
geometry_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
geometry_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

hotspot_test_SOURCES = hotspot_test.cc
hotspot_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
hotspot_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

iconstyle_test_SOURCES = iconstyle_test.cc
iconstyle_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
iconstyle_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

kml_cast_test_SOURCES = kml_cast_test.cc
kml_cast_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
kml_cast_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

kml_test_SOURCES = kml_test.cc
kml_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
kml_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

kml_factory_test_SOURCES = kml_factory_test.cc
kml_factory_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
kml_factory_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

kml22_test_SOURCES = kml22_test.cc
kml22_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
kml22_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

labelstyle_test_SOURCES = labelstyle_test.cc
labelstyle_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
labelstyle_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

linestyle_test_SOURCES = linestyle_test.cc
linestyle_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
linestyle_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

link_test_SOURCES = link_test.cc
link_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
link_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

liststyle_test_SOURCES = liststyle_test.cc
liststyle_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
liststyle_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

model_test_SOURCES = model_test.cc
model_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
model_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

networklink_test_SOURCES = networklink_test.cc
networklink_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
networklink_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

networklinkcontrol_test_SOURCES = networklinkcontrol_test.cc
networklinkcontrol_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
networklinkcontrol_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

object_test_SOURCES = object_test.cc
object_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
object_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

overlay_test_SOURCES = overlay_test.cc
overlay_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
overlay_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

placemark_test_SOURCES = placemark_test.cc
placemark_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
placemark_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

polystyle_test_SOURCES = polystyle_test.cc
polystyle_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
polystyle_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

region_test_SOURCES = region_test.cc
region_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
region_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

round_trip_test_SOURCES = round_trip_test.cc
round_trip_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
round_trip_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

schema_test_SOURCES = schema_test.cc
schema_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
schema_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

snippet_test_SOURCES = snippet_test.cc
snippet_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
snippet_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

style_test_SOURCES = style_test.cc
style_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
style_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

stylemap_test_SOURCES = stylemap_test.cc
stylemap_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
stylemap_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

styleselector_test_SOURCES = styleselector_test.cc
styleselector_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
styleselector_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

substyle_test_SOURCES = substyle_test.cc
substyle_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
substyle_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

timeprimitive_test_SOURCES = timeprimitive_test.cc
timeprimitive_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
timeprimitive_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

gx_timeprimitive_test_SOURCES = gx_timeprimitive_test.cc
gx_timeprimitive_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
gx_timeprimitive_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

gx_tour_test_SOURCES = gx_tour_test.cc
gx_tour_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
gx_tour_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

vec2_test_SOURCES = vec2_test.cc
vec2_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
vec2_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

unknown_test_SOURCES = unknown_test.cc
unknown_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
unknown_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

kml_handler_test_SOURCES = kml_handler_test.cc
kml_handler_test_CXXFLAGS =  -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
kml_handler_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

kml_handler_ns_test_SOURCES = kml_handler_ns_test.cc
kml_handler_ns_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
kml_handler_ns_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

parser_test_SOURCES = parser_test.cc
parser_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
parser_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

serializer_test_SOURCES = serializer_test.cc
serializer_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
serializer_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

xal_test_SOURCES = xal_test.cc
xal_test_CXXFLAGS =  -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
xal_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

xml_serializer_test_SOURCES = xml_serializer_test.cc
xml_serializer_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
xml_serializer_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

xsd_test_SOURCES = xsd_test.cc
xsd_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
xsd_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

visitor_test_SOURCES = visitor_test.cc
visitor_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
visitor_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

visitor_driver_test_SOURCES = visitor_driver_test.cc
visitor_driver_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
visitor_driver_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/third_party/libgtest.la

CLEANFILES = check_PROGRAMS
