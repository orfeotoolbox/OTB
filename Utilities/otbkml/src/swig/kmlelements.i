// Copyright 2008, Google Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//  3. Neither the name of Google Inc. nor the names of its contributors may be
//     used to endorse or promote products derived from this software without
//     specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Generated from the KML 2.2 XSD.

// This is the SWIG interface file for the public API to the KML DOM.
// In addition to serving the needs of SWIG this also serves to document the
// public C++ API separate from the present implementation found in the
// private sections of the present C++ .h files.

// This is AbstractObjectType in the KML 2.2 XSD.
%nodefaultctor Object;
class Object : public Element {
public:
  // id=
  const std::string& get_id();
  void set_id(const std::string& id);
  bool has_id();
  void clear_id();

  // targetId=
  const std::string& get_targetid();
  void set_targetid(const std::string& targetid);
  bool has_targetid();
  void clear_targetid();
};

%nodefaultctor SubStyle;
class SubStyle : public Object {
};

%nodefaultctor ColorStyle;
class ColorStyle : public SubStyle {
public:
  // <color>
  const std::string& get_color();
  void set_color(const std::string& color);
  bool has_color();
  void clear_color();

  // <colorMode>
  int get_colormode();
  void set_colormode(int colormode);
  bool has_colormode();
  void clear_colormode();
};

%nodefaultctor SnippetCommon;
class SnippetCommon : public Element {
public:
  // Content.
  const std::string& get_text();
  void set_text(const std::string& text);
  bool has_text();
  void clear_text();

  // maxLines=
  int get_maxlines();
  void set_maxlines(int maxlines);
  bool has_maxlines();
  void clear_maxlines();
};

%nodefaultctor Snippet;
class Snippet : public SnippetCommon {
};

%nodefaultctor LinkSnippet;
class LinkSnippet : public SnippetCommon {
};

%nodefaultctor AbstractView;
class AbstractView : public Object {
};

%nodefaultctor TimePrimitive;
class TimePrimitive : public Object {
};

%nodefaultctor StyleSelector;
class StyleSelector : public Object {
};

%nodefaultctor AbstractLatLonBox;
class AbstractLatLonBox : public Object {
public:
  // <north>
  double get_north();
  bool has_north();
  void set_north(double north);
  void clear_north();

  // <south>
  double get_south();
  bool has_south();
  void set_south(double south);
  void clear_south();

  // <east>
  double get_east();
  bool has_east();
  void set_east(double east);
  void clear_east();

  // <west>
  double get_west();
  bool has_west();
  void set_west(double west);
  void clear_west();
};

%nodefaultctor LatLonAltBox;
class LatLonAltBox : public AbstractLatLonBox {
public:
  // <minAltitude>
  double get_minaltitude();
  void set_minaltitude(double minaltitude);
  bool has_minaltitude();
  void clear_minaltitude();

  // <maxAltitude>
  double get_maxaltitude();
  void set_maxaltitude(double maxaltitude);
  bool has_maxaltitude();
  void clear_maxaltitude();

  // <altitudeMode>
  int get_altitudemode();
  void set_altitudemode(int altitudemode);
  bool has_altitudemode();
  void clear_altitudemode();
};

%nodefaultctor Lod;
class Lod : public Object {
public:
  // <minLodPixels>
  double get_minlodpixels();
  void set_minlodpixels(double minlodpixels);
  bool has_minlodpixels();
  void clear_minlodpixels();

  // <maxLodPixels>
  double get_maxlodpixels();
  void set_maxlodpixels(double maxlodpixels);
  bool has_maxlodpixels();
  void clear_maxlodpixels();

  // <minFadeExtent>
  double get_minfadeextent();
  void set_minfadeextent(double minfadeextent);
  bool has_minfadeextent();
  void clear_minfadeextent();

  // <maxFadeExtent>
  double get_maxfadeextent();
  void set_maxfadeextent(double maxfadeextent);
  bool has_maxfadeextent();
  void clear_maxfadeextent();
};

%nodefaultctor Region;
class Region : public Object {
public:
  // <LatLonAltBox>
  const LatLonAltBoxPtr get_latlonaltbox();
  void set_latlonaltbox(LatLonAltBoxPtr latlonaltbox);
  bool has_latlonaltbox();
  void clear_latlonaltbox();

  // <Lod>
  const LodPtr get_lod();
  void set_lod(LodPtr lod);
  bool has_lod();
  void clear_lod();
};

%nodefaultctor ExtendedData;
class ExtendedData : public Element {
public:
  // <Data>.
  void add_data(DataPtr data);
  const size_t get_data_array_size();
  const DataPtr get_data_array_at(unsigned int index);
  // <SchemaData>.
  void add_schemadata(SchemaDataPtr schemadata);
  const size_t get_schemadata_array_size();
  const SchemaDataPtr get_schemadata_array_at(unsigned int index);
};

%nodefaultctor Feature;
class Feature : public Object {
public:
  // <name>
  const std::string& get_name();
  void set_name(const std::string& name);
  bool has_name();
  void clear_name();

  // <visibility>
  bool get_visibility();
  void set_visibility(bool visibility);
  bool has_visibility();
  void clear_visibility();

  // <open>
  bool get_open();
  void set_open(bool open);
  bool has_open();
  void clear_open();

  // <address>
  const std::string& get_address();
  void set_address(const std::string& address);
  bool has_address();
  void clear_address();

  // <phoneNumber>
  const std::string& get_phonenumber();
  void set_phonenumber(const std::string& phonenumber);
  bool has_phonenumber();
  void clear_phonenumber();

  // <Snippet>
  const SnippetPtr get_snippet();
  void set_snippet(SnippetPtr snippet);
  bool has_snippet();
  void clear_snippet();

  // <description>
  const std::string& get_description();
  void set_description(const std::string& description);
  bool has_description();
  void clear_description();

  // AbstractView
  const AbstractViewPtr get_abstractview();
  void set_abstractview(AbstractViewPtr abstractview);
  bool has_abstractview();
  void clear_abstractview();

  // TimePrimitive
  const TimePrimitivePtr get_timeprimitive();
  void set_timeprimitive(TimePrimitivePtr timeprimitive);
  bool has_timeprimitive();
  void clear_timeprimitive();

  // <styleUrl>
  const std::string& get_styleurl();
  void set_styleurl(const std::string& styleurl);
  bool has_styleurl();
  void clear_styleurl();

  // StyleSelector
  const StyleSelectorPtr get_styleselector();
  void set_styleselector(StyleSelectorPtr styleselector);
  bool has_styleselector();
  void clear_styleselector();

  // <Region>
  const RegionPtr get_region();
  void set_region(RegionPtr region);
  bool has_region();
  void clear_region();

  // <ExtendedData>
  const ExtendedDataPtr get_extendeddata();
  void set_extendeddata(ExtendedDataPtr extendeddata);
  bool has_extendeddata();
  void clear_extendeddata();
};

%nodefaultctor Container;
class Container : public Feature {
public:
  // Feature...
  void add_feature(FeaturePtr feature);
  const size_t get_feature_array_size();
  const FeaturePtr get_feature_array_at(unsigned int index);
};

%nodefaultctor Geometry;
class Geometry : public Object {
};

%nodefaultctor BasicLink;
class BasicLink : public Object {
public:
  // <href>
  const std::string& get_href();
  bool has_href();
  void set_href(const std::string& href);
  void clear_href();
};

%nodefaultctor Icon;
class Icon : public BasicLink {
public:
  // <refreshMode>
  int get_refreshmode();
  void set_refreshmode(int refreshmode);
  bool has_refreshmode();
  void clear_refreshmode();

  // <refreshInterval>
  double get_refreshinterval();
  void set_refreshinterval(double refreshinterval);
  bool has_refreshinterval();
  void clear_refreshinterval();

  // <viewRefreshMode>
  int get_viewrefreshmode();
  void set_viewrefreshmode(int viewrefreshmode);
  bool has_viewrefreshmode();
  void clear_viewrefreshmode();

  // <viewRefreshTime>
  double get_viewrefreshtime();
  void set_viewrefreshtime(double viewrefreshtime);
  bool has_viewrefreshtime();
  void clear_viewrefreshtime();

  // <viewBoundScale>
  double get_viewboundscale();
  void set_viewboundscale(double viewboundscale);
  bool has_viewboundscale();
  void clear_viewboundscale();

  // <viewFormat>
  const std::string& get_viewformat();
  void set_viewformat(const std::string& viewformat);
  bool has_viewformat();
  void clear_viewformat();

  // <httpQuery>
  const std::string& get_httpquery();
  void set_httpquery(const std::string& httpquery);
  bool has_httpquery();
  void clear_httpquery();
};

%nodefaultctor IconStyleIcon;
class IconStyleIcon : public BasicLink {
public:
  // <href>
  const std::string& get_href();
  bool has_href();
  void set_href(const std::string& href);
  void clear_href();
};

%nodefaultctor Overlay;
class Overlay : public Feature {
public:
  // <color>
  const std::string& get_color();
  void set_color(const std::string& color);
  bool has_color();
  void clear_color();

  // <drawOrder>
  int get_draworder();
  void set_draworder(int draworder);
  bool has_draworder();
  void clear_draworder();

  // <Icon>
  const IconPtr get_icon();
  void set_icon(IconPtr icon);
  bool has_icon();
  void clear_icon();
};

%nodefaultctor BalloonStyle;
class BalloonStyle : public SubStyle {
public:
  // <bgColor>
  const std::string& get_bgcolor();
  void set_bgcolor(const std::string& bgcolor);
  bool has_bgcolor();
  void clear_bgcolor();

  // <textColor>
  const std::string& get_textcolor();
  void set_textcolor(const std::string& textcolor);
  bool has_textcolor();
  void clear_textcolor();

  // <text>
  const std::string& get_text();
  void set_text(const std::string& text);
  bool has_text();
  void clear_text();

  // <displayMode>
  int get_displaymode();
  void set_displaymode(int displaymode);
  bool has_displaymode();
  void clear_displaymode();
};

%nodefaultctor Camera;
class Camera : public AbstractView {
public:
  // <longitude>
  double get_longitude();
  void set_longitude(double longitude);
  bool has_longitude();
  void clear_longitude();

  // <latitude>
  double get_latitude();
  void set_latitude(double latitude);
  bool has_latitude();
  void clear_latitude();

  // <altitude>
  double get_altitude();
  void set_altitude(double altitude);
  bool has_altitude();
  void clear_altitude();

  // <heading>
  double get_heading();
  void set_heading(double heading);
  bool has_heading();
  void clear_heading();

  // <tilt>
  double get_tilt();
  void set_tilt(double tilt);
  bool has_tilt();
  void clear_tilt();

  // <roll>
  double get_roll();
  void set_roll(double roll);
  bool has_roll();
  void clear_roll();

  // <altitudeMode>
  int get_altitudemode();
  void set_altitudemode(int altitudemode);
  bool has_altitudemode();
  void clear_altitudemode();
};

%nodefaultctor UpdateOperation;
class UpdateOperation : public Element {
};

%nodefaultctor Change;
class Change : public UpdateOperation {
public:
  // Object...
  void add_object(ObjectPtr object);
  const size_t get_object_array_size();
  const ObjectPtr get_object_array_at(unsigned int index);
};

%nodefaultctor Create;
class Create : public UpdateOperation {
public:
  // Container...
  void add_container(ContainerPtr container);
  const size_t get_container_array_size();
  const ContainerPtr get_container_array_at(unsigned int index);
};

%nodefaultctor Data;
class Data : public Object {
public:
  // <displayName>
  const std::string& get_displayname();
  void set_displayname(const std::string& displayname);
  bool has_displayname();
  void clear_displayname();
};

%nodefaultctor Delete;
class Delete : public UpdateOperation {
public:
  // Feature...
  void add_feature(FeaturePtr feature);
  const size_t get_feature_array_size();
  const FeaturePtr get_feature_array_at(unsigned int index);
};

%nodefaultctor SimpleField;
class SimpleField : public Element {
public:
  // type=
  const std::string& get_type();
  bool has_type();
  void set_type(const std::string& value);
  void clear_type();

  // name=
  const std::string& get_name();
  bool has_name();
  void set_name(const std::string& value);
  void clear_name();

  // <displayName>
  const std::string& get_displayname();
  void set_displayname(const std::string& displayname);
  bool has_displayname();
  void clear_displayname();
};

%nodefaultctor Schema;
class Schema : public Object {
public:
  // name=
  const std::string& get_name();
  bool has_name();
  void set_name(const std::string& value);
  void clear_name();

  // <SimpleField>...
  void add_simplefield(SimpleFieldPtr simplefield);
  const size_t get_simplefield_array_size();
  const SimpleFieldPtr get_simplefield_array_at(unsigned int index);
};

%nodefaultctor Document;
class Document : public Container {
public:
  // <Schema>...
  void add_schema(SchemaPtr schema);
  const size_t get_schema_array_size();
  const SchemaPtr get_schema_array_at(unsigned int index);

  // <Style>,<StyleMap>...
  void add_styleselector(StyleSelectorPtr styleselector);
  const size_t get_styleselector_array_size();
  const StyleSelectorPtr get_styleselector_array_at(unsigned int index);
};

%nodefaultctor Folder;
class Folder : public Container {
};

%nodefaultctor LatLonBox;
class LatLonBox : public AbstractLatLonBox {
public:
  // <rotation>
  double get_rotation();
  void set_rotation(double rotation);
  bool has_rotation();
  void clear_rotation();
};

%nodefaultctor GroundOverlay;
class GroundOverlay : public Overlay {
public:
  // <altitude>
  double get_altitude();
  void set_altitude(double altitude);
  bool has_altitude();
  void clear_altitude();

  // <altitudeMode>
  int get_altitudemode();
  void set_altitudemode(int altitudemode);
  bool has_altitudemode();
  void clear_altitudemode();

  // <LatLonBox>
  const LatLonBoxPtr get_latlonbox();
  void set_latlonbox(LatLonBoxPtr latlonbox);
  bool has_latlonbox();
  void clear_latlonbox();
};

%nodefaultctor HotSpot;
class HotSpot : public Vec2 {
};

%nodefaultctor IconStyle;
class IconStyle : public ColorStyle {
public:
  // <scale>
  double get_scale();
  void set_scale(double scale);
  bool has_scale();
  void clear_scale();

  // <Icon> (different from Overlay Icon)
  const IconStyleIconPtr get_icon();
  void set_icon(IconStyleIconPtr icon);
  bool has_icon();
  void clear_icon();

  // <heading>
  double get_heading();
  void set_heading(double heading);
  bool has_heading();
  void clear_heading();

  // <hotSpot>
  const HotSpotPtr get_hotspot();
  void set_hotspot(HotSpotPtr hotspot);
  bool has_hotspot();
  void clear_hotspot();
};

%nodefaultctor ImagePyramid;
class ImagePyramid : public Object {
public:
  // <tileSize>
  int get_tilesize();
  void set_tilesize(int tilesize);
  bool has_tilesize();
  void clear_tilesize();

  // <maxWidth>
  int get_maxwidth();
  void set_maxwidth(int maxwidth);
  bool has_maxwidth();
  void clear_maxwidth();

  // <maxHeight>
  int get_maxheight();
  void set_maxheight(int maxheight);
  bool has_maxheight();
  void clear_maxheight();

  // <gridOrigin>
  int get_gridorigin();
  void set_gridorigin(int gridorigin);
  bool has_gridorigin();
  void clear_gridorigin();
};

%nodefaultctor ItemIcon;
class ItemIcon : public Object {
public:
  // TODO: <state>
  //itemIconStatePtr get_state();
  //void set_state(itemIconStatePtr state);
  //bool has_state();
  //void clear_state();

  // <href>
  const std::string& get_href();
  void set_href(const std::string& href);
  bool has_href();
  void clear_href();

};

%nodefaultctor LabelStyle;
class LabelStyle : public ColorStyle {
public:
  // <scale>
  double get_scale();
  void set_scale(double scale);
  bool has_scale();
  void clear_scale();
};

%nodefaultctor LineString;
class LineString : public Geometry {
public:
  // <extrude>
  bool get_extrude();
  void set_extrude(bool extrude);
  bool has_extrude();
  void clear_extrude();

  // <tessellate>
  bool get_tessellate();
  void set_tessellate(bool tessellate);
  bool has_tessellate();
  void clear_tessellate();

  // <altitudeMode>
  int get_altitudemode();
  void set_altitudemode(int altitudemode);
  bool has_altitudemode();
  void clear_altitudemode();

  // <coordinates>
  const CoordinatesPtr get_coordinates();
  void set_coordinates(CoordinatesPtr coordinates);
  bool has_coordinates();
  void clear_coordinates();
};

%nodefaultctor LineStyle;
class LineStyle : public ColorStyle {
public:
  // <width>
  double get_width();
  void set_width(double width);
  bool has_width();
  void clear_width();
};

%nodefaultctor LinearRing;
class LinearRing : public Geometry {
public:
  // <extrude>
  bool get_extrude();
  void set_extrude(bool extrude);
  bool has_extrude();
  void clear_extrude();

  // <tessellate>
  bool get_tessellate();
  void set_tessellate(bool tessellate);
  bool has_tessellate();
  void clear_tessellate();

  // <altitudeMode>
  int get_altitudemode();
  void set_altitudemode(int altitudemode);
  bool has_altitudemode();
  void clear_altitudemode();

  // <coordinates>
  const CoordinatesPtr get_coordinates();
  void set_coordinates(CoordinatesPtr coordinates);
  bool has_coordinates();
  void clear_coordinates();
};

%nodefaultctor Link;
class Link : public BasicLink {
public:
  // <refreshMode>
  int get_refreshmode();
  void set_refreshmode(int refreshmode);
  bool has_refreshmode();
  void clear_refreshmode();

  // <refreshInterval>
  double get_refreshinterval();
  void set_refreshinterval(double refreshinterval);
  bool has_refreshinterval();
  void clear_refreshinterval();

  // <viewRefreshMode>
  int get_viewrefreshmode();
  void set_viewrefreshmode(int viewrefreshmode);
  bool has_viewrefreshmode();
  void clear_viewrefreshmode();

  // <viewRefreshTime>
  double get_viewrefreshtime();
  void set_viewrefreshtime(double viewrefreshtime);
  bool has_viewrefreshtime();
  void clear_viewrefreshtime();

  // <viewBoundScale>
  double get_viewboundscale();
  void set_viewboundscale(double viewboundscale);
  bool has_viewboundscale();
  void clear_viewboundscale();

  // <viewFormat>
  const std::string& get_viewformat();
  void set_viewformat(const std::string& viewformat);
  bool has_viewformat();
  void clear_viewformat();

  // <httpQuery>
  const std::string& get_httpquery();
  void set_httpquery(const std::string& httpquery);
  bool has_httpquery();
  void clear_httpquery();
};

%nodefaultctor ListStyle;
class ListStyle : public SubStyle {
public:
  // <listItemType>
  int get_listitemtype();
  void set_listitemtype(int listitemtype);
  bool has_listitemtype();
  void clear_listitemtype();

  // <bgColor>
  const std::string& get_bgcolor();
  void set_bgcolor(const std::string& bgcolor);
  bool has_bgcolor();
  void clear_bgcolor();

  // <ItemIcon>...
  void add_itemicon(ItemIconPtr itemicon);
  const size_t get_itemicon_array_size();
  const ItemIconPtr get_itemicon_array_at(unsigned int index);
};

%nodefaultctor Location;
class Location : public Object {
public:
  // <longitude>
  double get_longitude();
  void set_longitude(double longitude);
  bool has_longitude();
  void clear_longitude();

  // <latitude>
  double get_latitude();
  void set_latitude(double latitude);
  bool has_latitude();
  void clear_latitude();

  // <altitude>
  double get_altitude();
  void set_altitude(double altitude);
  bool has_altitude();
  void clear_altitude();
};

%nodefaultctor LookAt;
class LookAt : public AbstractView {
public:
  // <longitude>
  double get_longitude();
  void set_longitude(double longitude);
  bool has_longitude();
  void clear_longitude();

  // <latitude>
  double get_latitude();
  void set_latitude(double latitude);
  bool has_latitude();
  void clear_latitude();

  // <altitude>
  double get_altitude();
  void set_altitude(double altitude);
  bool has_altitude();
  void clear_altitude();

  // <heading>
  double get_heading();
  void set_heading(double heading);
  bool has_heading();
  void clear_heading();

  // <tilt>
  double get_tilt();
  void set_tilt(double tilt);
  bool has_tilt();
  void clear_tilt();

  // <range>
  double get_range();
  void set_range(double range);
  bool has_range();
  void clear_range();

  // <altitudeMode>
  int get_altitudemode();
  void set_altitudemode(int altitudemode);
  bool has_altitudemode();
  void clear_altitudemode();
};

%nodefaultctor Orientation;
class Orientation : public Object {
public:
  // <heading>
  double get_heading();
  void set_heading(double heading);
  bool has_heading();
  void clear_heading();

  // <tilt>
  double get_tilt();
  void set_tilt(double tilt);
  bool has_tilt();
  void clear_tilt();

  // <roll>
  double get_roll();
  void set_roll(double roll);
  bool has_roll();
  void clear_roll();
};

%nodefaultctor Scale;
class Scale : public Object {
public:
  // <x>
  double get_x();
  void set_x(double x);
  bool has_x();
  void clear_x();

  // <y>
  double get_y();
  void set_y(double y);
  bool has_y();
  void clear_y();

  // <z>
  double get_z();
  void set_z(double z);
  bool has_z();
  void clear_z();
};

%nodefaultctor Alias;
class Alias : public Object {
public:
  // <targetHref>
  const std::string& get_targethref();
  void set_targethref(const std::string& targethref);
  bool has_targethref();
  void clear_targethref();

  // <sourceHref>
  const std::string& get_sourcehref();
  void set_sourcehref(const std::string& sourcehref);
  bool has_sourcehref();
  void clear_sourcehref();
};

%nodefaultctor ResourceMap;
class ResourceMap : public Object {
public:
  // <Alias>...
  void add_alias(AliasPtr alias);
  const size_t get_alias_array_size();
  const AliasPtr get_alias_array_at(unsigned int index);
};

%nodefaultctor Model;
class Model : public Geometry {
public:
  // <altitudeMode>
  int get_altitudemode();
  void set_altitudemode(int altitudemode);
  bool has_altitudemode();
  void clear_altitudemode();

  // <Location>
  const LocationPtr get_location();
  void set_location(LocationPtr location);
  bool has_location();
  void clear_location();

  // <Orientation>
  const OrientationPtr get_orientation();
  void set_orientation(OrientationPtr orientation);
  bool has_orientation();
  void clear_orientation();

  // <Scale>
  const ScalePtr get_scale();
  void set_scale(ScalePtr scale);
  bool has_scale();
  void clear_scale();

  // <Link>
  const LinkPtr get_link();
  void set_link(LinkPtr link);
  bool has_link();
  void clear_link();

  // <ResourceMap>
  const ResourceMapPtr get_resourcemap();
  void set_resourcemap(ResourceMapPtr resourcemap);
  bool has_resourcemap();
  void clear_resourcemap();
};

%nodefaultctor MultiGeometry;
class MultiGeometry : public Geometry {
public:
  // Geometry...
  void add_geometry(GeometryPtr geometry);
  const size_t get_geometry_array_size();
  const GeometryPtr get_geometry_array_at(unsigned int index);
};

%nodefaultctor NetworkLink;
class NetworkLink : public Feature {
public:
  // <refreshVisibility>
  bool get_refreshvisibility();
  void set_refreshvisibility(bool refreshvisibility);
  bool has_refreshvisibility();
  void clear_refreshvisibility();

  // <flyToView>
  bool get_flytoview();
  void set_flytoview(bool flytoview);
  bool has_flytoview();
  void clear_flytoview();

  // <Link>
  const LinkPtr get_link();
  void set_link(LinkPtr link);
  bool has_link();
  void clear_link();
};

%nodefaultctor Update;
class Update : public Element {
public:
  // <Change>,<Create>,<Delete>...
  void add_updateoperation(UpdateOperationPtr updateoperation);
  const size_t get_updateoperation_array_size();
  const UpdateOperationPtr get_updateoperation_array_at(unsigned int index);
};

%nodefaultctor NetworkLinkControl;
class NetworkLinkControl : public Element {
public:
  // <minRefreshPeriod>
  double get_minrefreshperiod();
  void set_minrefreshperiod(double minrefreshperiod);
  bool has_minrefreshperiod();
  void clear_minrefreshperiod();

  // <maxSessionLength>
  double get_maxsessionlength();
  void set_maxsessionlength(double maxsessionlength);
  bool has_maxsessionlength();
  void clear_maxsessionlength();

  // <cookie>
  const std::string& get_cookie();
  void set_cookie(const std::string& cookie);
  bool has_cookie();
  void clear_cookie();

  // <message>
  const std::string& get_message();
  void set_message(const std::string& message);
  bool has_message();
  void clear_message();

  // <linkName>
  const std::string& get_linkname();
  void set_linkname(const std::string& linkname);
  bool has_linkname();
  void clear_linkname();

  // <linkDescription>
  const std::string& get_linkdescription();
  void set_linkdescription(const std::string& linkdescription);
  bool has_linkdescription();
  void clear_linkdescription();

  // <linkSnippet>
  const LinkSnippetPtr get_linksnippet();
  void set_linksnippet(LinkSnippetPtr linksnippet);
  bool has_linksnippet();
  void clear_linksnippet();

  // <expires>
  const std::string& get_expires();
  void set_expires(const std::string& expires);
  bool has_expires();
  void clear_expires();

  // <Update>
  const UpdatePtr get_update();
  void set_update(UpdatePtr update);
  bool has_update();
  void clear_update();

  // AbstractView
  const AbstractViewPtr get_abstractview();
  void set_abstractview(AbstractViewPtr abstractview);
  bool has_abstractview();
  void clear_abstractview();
};

%nodefaultctor Pair;
class Pair : public Object {
public:
  // <key>
  int get_key();
  void set_key(int key);
  bool has_key();
  void clear_key();

  // <styleUrl>
  const std::string& get_styleurl();
  void set_styleurl(const std::string& styleurl);
  bool has_styleurl();
  void clear_styleurl();

  // <StyleSelector>
  const StyleSelectorPtr get_styleselector();
  void set_styleselector(StyleSelectorPtr styleselector);
  bool has_styleselector();
  void clear_styleselector();
};

%nodefaultctor ViewVolume;
class ViewVolume : public Object {
public:
  // <leftFov>
  double get_leftfov();
  void set_leftfov(double leftfov);
  bool has_leftfov();
  void clear_leftfov();

  // <rightFov>
  double get_rightfov();
  void set_rightfov(double rightfov);
  bool has_rightfov();
  void clear_rightfov();

  // <bottomFov>
  double get_bottomfov();
  void set_bottomfov(double bottomfov);
  bool has_bottomfov();
  void clear_bottomfov();

  // <topFov>
  double get_topfov();
  void set_topfov(double topfov);
  bool has_topfov();
  void clear_topfov();

  // <near>
  double get_near();
  void set_near(double val);
  bool has_near();
  void clear_near();
};

%nodefaultctor Point;
class Point : public Geometry {
public:
  // <extrude>
  bool get_extrude();
  void set_extrude(bool extrude);
  bool has_extrude();
  void clear_extrude();

  // <altitudeMode>
  int get_altitudemode();
  void set_altitudemode(int altitudemode);
  bool has_altitudemode();
  void clear_altitudemode();

  // <coordinates>
  const CoordinatesPtr get_coordinates();
  void set_coordinates(CoordinatesPtr coordinates);
  bool has_coordinates();
  void clear_coordinates();
};

%nodefaultctor PhotoOverlay;
class PhotoOverlay : public Overlay {
public:
  // <rotation>
  double get_rotation();
  void set_rotation(double rotation);
  bool has_rotation();
  void clear_rotation();

  // <ViewVolume>
  const ViewVolumePtr get_viewvolume();
  void set_viewvolume(ViewVolumePtr viewvolume);
  bool has_viewvolume();
  void clear_viewvolume();

  // <ImagePyramid>
  const ImagePyramidPtr get_imagepyramid();
  void set_imagepyramid(ImagePyramidPtr imagepyramid);
  bool has_imagepyramid();
  void clear_imagepyramid();

  // <Point>
  const PointPtr get_point();
  void set_point(PointPtr point);
  bool has_point();
  void clear_point();

  // <shape>
  int get_shape();
  bool has_shape();
  void set_shape(int shape);
  void clear_shape();
};

%nodefaultctor Placemark;
class Placemark : public Feature {
public:
  // Geometry
  const GeometryPtr get_geometry();
  void set_geometry(GeometryPtr geometry);
  bool has_geometry();
  void clear_geometry();
};

%nodefaultctor PolyStyle;
class PolyStyle : public ColorStyle {
public:
  // <fill>
  bool get_fill();
  void set_fill(bool fill);
  bool has_fill();
  void clear_fill();

  // <outline>
  bool get_outline();
  void set_outline(bool outline);
  bool has_outline();
  void clear_outline();
};

%nodefaultctor OuterBoundaryIs;
class OuterBoundaryIs : public Element {
public:
  // <LinearRing>
  const LinearRingPtr get_linearring();
  void set_linearring(LinearRingPtr linearring);
  bool has_linearring();
  void clear_linearring();
};

%nodefaultctor InnerBoundaryIs;
class InnerBoundaryIs : public Element {
public:
  // <LinearRing>
  const LinearRingPtr get_linearring();
  void set_linearring(LinearRingPtr linearring);
  bool has_linearring();
  void clear_linearring();
};

%nodefaultctor Polygon;
class Polygon : public Geometry {
public:
  // <extrude>
  bool get_extrude();
  void set_extrude(bool extrude);
  bool has_extrude();
  void clear_extrude();

  // <tessellate>
  bool get_tessellate();
  void set_tessellate(bool tessellate);
  bool has_tessellate();
  void clear_tessellate();

  // <altitudeMode>
  int get_altitudemode();
  void set_altitudemode(int altitudemode);
  bool has_altitudemode();
  void clear_altitudemode();

  // <outerBoundaryIs>
  const OuterBoundaryIsPtr get_outerboundaryis();
  void set_outerboundaryis(OuterBoundaryIsPtr outerboundaryis);
  bool has_outerboundaryis();
  void clear_outerboundaryis();

  // <innerBoundaryIs>...
  void add_innerboundaryis(InnerBoundaryIsPtr innerboundaryis);
  const size_t get_innerboundaryis_array_size();
  const InnerBoundaryIsPtr get_innerboundaryis_array_at(unsigned int index);
};

%nodefaultctor SimpleData;
class SimpleData : public Element {
};

%nodefaultctor SchemaData;
class SchemaData : public Object {
public:
  // <SimpleData>...
  void add_simpledata(SimpleDataPtr simpledata);
  const size_t get_simpledata_array_size();
  const SimpleDataPtr get_simpledata_array_at(unsigned int index);
};

%nodefaultctor OverlayXY;
class OverlayXY : public Vec2 {
};

%nodefaultctor ScreenXY;
class ScreenXY : public Vec2 {
};

%nodefaultctor RotationXY;
class RotationXY : public Vec2 {
};

%nodefaultctor Size;
class Size : public Vec2 {
};

%nodefaultctor ScreenOverlay;
class ScreenOverlay : public Overlay {
public:
  // <OverlayXY>
  const OverlayXYPtr get_overlayxy();
  void set_overlayxy(OverlayXYPtr overlayxy);
  bool has_overlayxy();
  void clear_overlayxy();

  // <ScreenXY>
  const ScreenXYPtr get_screenxy();
  void set_screenxy(ScreenXYPtr screenxy);
  bool has_screenxy();
  void clear_screenxy();

  // <RotationXY>
  const RotationXYPtr get_rotationxy();
  void set_rotationxy(RotationXYPtr rotationxy);
  bool has_rotationxy();
  void clear_rotationxy();

  // <size>
  const SizePtr get_size();
  void set_size(SizePtr size);
  bool has_size();
  void clear_size();

  // <rotation>
  double get_rotation();
  void set_rotation(double rotation);
  bool has_rotation();
  void clear_rotation();
};

%nodefaultctor Style;
class Style : public StyleSelector {
public:
  // <IconStyle>
  const IconStylePtr get_iconstyle();
  void set_iconstyle(IconStylePtr iconstyle);
  bool has_iconstyle();
  void clear_iconstyle();

  // <LabelStyle>
  const LabelStylePtr get_labelstyle();
  void set_labelstyle(LabelStylePtr labelstyle);
  bool has_labelstyle();
  void clear_labelstyle();

  // <LineStyle>
  const LineStylePtr get_linestyle();
  void set_linestyle(LineStylePtr linestyle);
  bool has_linestyle();
  void clear_linestyle();

  // <PolyStyle>
  const PolyStylePtr get_polystyle();
  void set_polystyle(PolyStylePtr polystyle);
  bool has_polystyle();
  void clear_polystyle();

  // <BalloonStyle>
  const BalloonStylePtr get_balloonstyle();
  void set_balloonstyle(BalloonStylePtr balloonstyle);
  bool has_balloonstyle();
  void clear_balloonstyle();

  // <ListStyle>
  const ListStylePtr get_liststyle();
  void set_liststyle(ListStylePtr liststyle);
  bool has_liststyle();
  void clear_liststyle();
};

%nodefaultctor StyleMap;
class StyleMap : public StyleSelector {
public:
  // <Pair>...
  void add_pair(PairPtr pair);
  const size_t get_pair_array_size();
  const PairPtr get_pair_array_at(unsigned int index);
};

%nodefaultctor TimeSpan;
class TimeSpan : public TimePrimitive {
public:
  // <begin>
  const std::string& get_begin();
  void set_begin(const std::string& begin);
  bool has_begin();
  void clear_begin();

  // <end>
  const std::string& get_end();
  void set_end(const std::string& end);
  bool has_end();
  void clear_end();
};

%nodefaultctor TimeStamp;
class TimeStamp : public TimePrimitive {
public:
  // <when>
  const std::string& get_when();
  void set_when(const std::string& when);
  bool has_when();
  void clear_when();
};

%nodefaultctor Kml;
class Kml : public Element {
public:
  // hint=
  const std::string& get_hint();
  void set_hint(const std::string& hint);
  bool has_hint();
  void clear_hint();

  // <NetworkLinkControl>
  const NetworkLinkControlPtr get_networklinkcontrol();
  void set_networklinkcontrol(NetworkLinkControlPtr networklinkcontrol);
  bool has_networklinkcontrol();
  void clear_networklinkcontrol();

  // Feature
  const FeaturePtr get_feature();
  void set_feature(FeaturePtr feature);
  bool has_feature();
  void clear_feature();
};
