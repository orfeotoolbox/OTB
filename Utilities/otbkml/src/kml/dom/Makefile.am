AM_CPPFLAGS= -I$(top_srcdir)/src \
	     -I$(top_srcdir)/third_party/boost_1_34_1

if GCC
AM_CXXFLAGS = -Wall -Werror -ansi -pedantic -fno-rtti
endif

lib_LTLIBRARIES = libkmldom.la
libkmldom_la_SOURCES = \
	abstractlatlonbox.cc \
	abstractview.cc \
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
	vec2.cc \
	kml_handler.cc \
	parser.cc \
	serializer.cc \
	xml_serializer.cc \
	xsd.cc

libkmldom_la_LDFLAGS = -lexpat

libkmldomincludedir = $(includedir)/kml/dom
libkmldominclude_HEADERS = \
	abstractlatlonbox.h \
	abstractview.h \
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
	vec2.h

EXTRA_DIST = \
	attributes.h \
	expat_handler.h \
	kml_handler.h \
	serializer.h \
	xml_serializer.h \
	xsd.h \
	kml22.cc

if HAVE_CPPUNIT 
TESTS = abstractlatlonbox_test \
	abstractview_test \
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
	schema_test \
	snippet_test \
	style_test \
	stylemap_test \
	styleselector_test \
	substyle_test \
	timeprimitive_test \
	vec2_test \
	unknown_test \
	attributes_test \
	kml_handler_test \
	parser_test \
	serializer_test \
	xml_serializer_test \
	xsd_test


check_PROGRAMS = $(TESTS)

# Unit tests for KMLDOM classes.
abstractlatlonbox_test_SOURCES = abstractlatlonbox_test.cc
abstractlatlonbox_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
abstractlatlonbox_test_LDFLAGS = $(CPPUNIT_LIBS)
abstractlatlonbox_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

abstractview_test_SOURCES = abstractview_test.cc
abstractview_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
abstractview_test_LDFLAGS = $(CPPUNIT_LIBS)
abstractview_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

balloonstyle_test_SOURCES = balloonstyle_test.cc
balloonstyle_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
balloonstyle_test_LDFLAGS = $(CPPUNIT_LIBS)
balloonstyle_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

colorstyle_test_SOURCES = colorstyle_test.cc
colorstyle_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
colorstyle_test_LDFLAGS = $(CPPUNIT_LIBS)
colorstyle_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

container_test_SOURCES = container_test.cc
container_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
container_test_LDFLAGS = $(CPPUNIT_LIBS)
container_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

document_test_SOURCES = document_test.cc
document_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
document_test_LDFLAGS = $(CPPUNIT_LIBS)
document_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

element_test_SOURCES = element_test.cc
element_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
element_test_LDFLAGS = $(CPPUNIT_LIBS)
element_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

extendeddata_test_SOURCES = extendeddata_test.cc
extendeddata_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
extendeddata_test_LDFLAGS = $(CPPUNIT_LIBS)
extendeddata_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

feature_test_SOURCES = feature_test.cc
feature_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
feature_test_LDFLAGS = $(CPPUNIT_LIBS)
feature_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

folder_test_SOURCES = folder_test.cc
folder_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
folder_test_LDFLAGS = $(CPPUNIT_LIBS)
folder_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

geometry_test_SOURCES = geometry_test.cc
geometry_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
geometry_test_LDFLAGS = $(CPPUNIT_LIBS)
geometry_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

hotspot_test_SOURCES = hotspot_test.cc
hotspot_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
hotspot_test_LDFLAGS = $(CPPUNIT_LIBS)
hotspot_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

iconstyle_test_SOURCES = iconstyle_test.cc
iconstyle_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
iconstyle_test_LDFLAGS = $(CPPUNIT_LIBS)
iconstyle_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

kml_cast_test_SOURCES = kml_cast_test.cc
kml_cast_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
kml_cast_test_LDFLAGS = $(CPPUNIT_LIBS)
kml_cast_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

kml_test_SOURCES = kml_test.cc
kml_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
kml_test_LDFLAGS = $(CPPUNIT_LIBS)
kml_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

kml_factory_test_SOURCES = kml_factory_test.cc
kml_factory_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
kml_factory_test_LDFLAGS = $(CPPUNIT_LIBS)
kml_factory_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

labelstyle_test_SOURCES = labelstyle_test.cc
labelstyle_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
labelstyle_test_LDFLAGS = $(CPPUNIT_LIBS)
labelstyle_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

linestyle_test_SOURCES = linestyle_test.cc
linestyle_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
linestyle_test_LDFLAGS = $(CPPUNIT_LIBS)
linestyle_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

link_test_SOURCES = link_test.cc
link_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
link_test_LDFLAGS = $(CPPUNIT_LIBS)
link_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

liststyle_test_SOURCES = liststyle_test.cc
liststyle_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
liststyle_test_LDFLAGS = $(CPPUNIT_LIBS)
liststyle_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

model_test_SOURCES = model_test.cc
model_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
model_test_LDFLAGS = $(CPPUNIT_LIBS)
model_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

networklink_test_SOURCES = networklink_test.cc
networklink_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
networklink_test_LDFLAGS = $(CPPUNIT_LIBS)
networklink_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

networklinkcontrol_test_SOURCES = networklinkcontrol_test.cc
networklinkcontrol_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
networklinkcontrol_test_LDFLAGS = $(CPPUNIT_LIBS)
networklinkcontrol_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

object_test_SOURCES = object_test.cc
object_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
object_test_LDFLAGS = $(CPPUNIT_LIBS)
object_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

overlay_test_SOURCES = overlay_test.cc
overlay_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
overlay_test_LDFLAGS = $(CPPUNIT_LIBS)
overlay_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

placemark_test_SOURCES = placemark_test.cc
placemark_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
placemark_test_LDFLAGS = $(CPPUNIT_LIBS)
placemark_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

polystyle_test_SOURCES = polystyle_test.cc
polystyle_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
polystyle_test_LDFLAGS = $(CPPUNIT_LIBS)
polystyle_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

region_test_SOURCES = region_test.cc
region_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
region_test_LDFLAGS = $(CPPUNIT_LIBS)
region_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

schema_test_SOURCES = schema_test.cc
schema_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
schema_test_LDFLAGS = $(CPPUNIT_LIBS)
schema_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

snippet_test_SOURCES = snippet_test.cc
snippet_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
snippet_test_LDFLAGS = $(CPPUNIT_LIBS)
snippet_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

style_test_SOURCES = style_test.cc
style_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
style_test_LDFLAGS = $(CPPUNIT_LIBS)
style_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

stylemap_test_SOURCES = stylemap_test.cc
stylemap_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
stylemap_test_LDFLAGS = $(CPPUNIT_LIBS)
stylemap_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

styleselector_test_SOURCES = styleselector_test.cc
styleselector_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
styleselector_test_LDFLAGS = $(CPPUNIT_LIBS)
styleselector_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

substyle_test_SOURCES = substyle_test.cc
substyle_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
substyle_test_LDFLAGS = $(CPPUNIT_LIBS)
substyle_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

timeprimitive_test_SOURCES = timeprimitive_test.cc
timeprimitive_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
timeprimitive_test_LDFLAGS = $(CPPUNIT_LIBS)
timeprimitive_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

vec2_test_SOURCES = vec2_test.cc
vec2_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
vec2_test_LDFLAGS = $(CPPUNIT_LIBS)
vec2_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

unknown_test_SOURCES = unknown_test.cc
unknown_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
unknown_test_LDFLAGS = $(CPPUNIT_LIBS)
unknown_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

# Unit tests for internal classes.
attributes_test_SOURCES = attributes_test.cc
attributes_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
attributes_test_LDFLAGS = $(CPPUNIT_LIBS)
attributes_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

kml_handler_test_SOURCES = kml_handler_test.cc
kml_handler_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
kml_handler_test_LDFLAGS = $(CPPUNIT_LIBS)
kml_handler_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

parser_test_SOURCES = parser_test.cc
parser_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
parser_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
parser_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

serializer_test_SOURCES = serializer_test.cc
serializer_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
serializer_test_LDFLAGS = $(CPPUNIT_LIBS)
serializer_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

xml_serializer_test_SOURCES = xml_serializer_test.cc
xml_serializer_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
xml_serializer_test_LDFLAGS = $(CPPUNIT_LIBS)
xml_serializer_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

xsd_test_SOURCES = xsd_test.cc
xsd_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
xsd_test_LDFLAGS = $(CPPUNIT_LIBS)
xsd_test_LDADD= libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

CLEANFILES = check_PROGRAMS
endif
