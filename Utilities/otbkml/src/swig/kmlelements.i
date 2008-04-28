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

%nodefaultctor Object;

class Object : public Element {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // id=
  const std::string& id();
  void set_id(const std::string& id);
  bool has_id();
  void clear_id();

  // targetId=
  const std::string& targetid();
  void set_targetid(const std::string& targetid);
  bool has_targetid();
  void clear_targetid();
};

%nodefaultctor SubStyle;
class SubStyle : public Object {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);
};

%nodefaultctor ColorStyle;
class ColorStyle : public SubStyle {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);
};

%nodefaultctor SnippetCommon;
class SnippetCommon : public Element {
public:
  // Content.
  const std::string& text() const;
  void set_text(const std::string& text);
  bool has_text() const;
  void clear_text();

  // maxLines=
  int maxlines() const;
  void set_maxlines(int maxlines);
  bool has_maxlines();
  void clear_maxlines();
};

%nodefaultctor Snippet;
class Snippet : public SnippetCommon {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);
};

%nodefaultctor LinkSnippet;
class LinkSnippet : public SnippetCommon {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);
};

%nodefaultctor AbstractView;
class AbstractView : public Object {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);
};

%nodefaultctor TimePrimitive;
class TimePrimitive : public Object {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);
};

%nodefaultctor StyleSelector;
class StyleSelector : public Object {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);
};

%nodefaultctor AbstractLatLonBox;
class AbstractLatLonBox : public Object {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <north>
  double north() const;
  bool has_north() const;
  void set_north(double north);
  void clear_north();

  // <south>
  double south() const;
  bool has_south() const;
  void set_south(double south);
  void clear_south();

  // <east>
  double east() const;
  bool has_east() const;
  void set_east(double east);
  void clear_east();

  // <west>
  double west() const;
  bool has_west() const;
  void set_west(double west);
  void clear_west();
};

%nodefaultctor LatLonAltBox;
class LatLonAltBox : public AbstractLatLonBox {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <minAltitude>
  double minaltitude() const;
  void set_minaltitude(double minaltitude);
  bool has_minaltitude() const;
  void clear_minaltitude();

  // <maxAltitude>
  double maxaltitude() const;
  void set_maxaltitude(double maxaltitude);
  bool has_maxaltitude() const;
  void clear_maxaltitude();

  // <altitudeMode>
  int altitudemode() const;
  void set_altitudemode(int altitudemode);
  bool has_altitudemode() const;
  void clear_altitudemode();

};

%nodefaultctor Lod;
class Lod : public Object {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <minLodPixels>
  double minlodpixels() const;
  void set_minlodpixels(double minlodpixels);
  bool has_minlodpixels() const;
  void clear_minlodpixels();

  // <maxLodPixels>
  double maxlodpixels() const;
  void set_maxlodpixels(double maxlodpixels);
  bool has_maxlodpixels() const;
  void clear_maxlodpixels();

  // <minFadeExtent>
  double minfadeextent() const;
  void set_minfadeextent(double minfadeextent);
  bool has_minfadeextent() const;
  void clear_minfadeextent();

  // <maxFadeExtent>
  double maxfadeextent() const;
  void set_maxfadeextent(double maxfadeextent);
  bool has_maxfadeextent() const;
  void clear_maxfadeextent();

};

%nodefaultctor Region;
class Region : public Object {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <LatLonAltBox>
  const LatLonAltBox* latlonaltbox() const;
  void set_latlonaltbox(LatLonAltBox* latlonaltbox);
  bool has_latlonaltbox() const;
  void clear_latlonaltbox();

  // <Lod>
  const Lod* lod() const;
  void set_lod(Lod* lod);
  bool has_lod() const;
  void clear_lod();

};

%nodefaultctor ExtendedDataMember;
class ExtendedDataMember : public Object {
};

%nodefaultctor ExtendedData;
class ExtendedData : public Element {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <Data>, <SchemaData>...
  void add_extendeddatamember(ExtendedDataMember* extendeddatamember);
  const size_t extendeddatamember_array_size() const;
  const ExtendedDataMember* extendeddatamember_array_at(unsigned int index);
};

%nodefaultctor Feature;
class Feature : public Object {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <name>
  const std::string& name() const;
  void set_name(const std::string& name);
  bool has_name() const;
  void clear_name();

  // <visibility>
  bool visibility() const;
  void set_visibility(bool visibility);
  bool has_visibility() const;
  void clear_visibility();

  // <open>
  bool open() const;
  void set_open(bool open);
  bool has_open() const;
  void clear_open();

  // <address>
  const std::string& address() const;
  void set_address(const std::string& address);
  bool has_address() const;
  void clear_address();

  // <phoneNumber>
  const std::string& phonenumber() const;
  void set_phonenumber(const std::string& phonenumber);
  bool has_phonenumber() const;
  void clear_phonenumber();

  // <Snippet>
  const Snippet* snippet() const;
  void set_snippet(Snippet* snippet);
  bool has_snippet() const;
  void clear_snippet();

  // <description>
  const std::string& description() const;
  void set_description(const std::string& description);
  bool has_description() const;
  void clear_description();

  // AbstractView
  const AbstractView* abstractview() const;
  void set_abstractview(AbstractView* abstractview);
  bool has_abstractview() const;
  void clear_abstractview();

  // TimePrimitive
  const TimePrimitive* timeprimitive() const;
  void set_timeprimitive(TimePrimitive* timeprimitive);
  bool has_timeprimitive() const;
  void clear_timeprimitive();

  // <styleUrl>
  const std::string& styleurl() const;
  void set_styleurl(const std::string& styleurl);
  bool has_styleurl() const;
  void clear_styleurl();

  // StyleSelector
  const StyleSelector* styleselector() const;
  void set_styleselector(StyleSelector* styleselector);
  bool has_styleselector() const;
  void clear_styleselector();

  // <Region>
  const Region* region() const;
  void set_region(Region* region);
  bool has_region() const;
  void clear_region();

  // <ExtendedData>
  const ExtendedData* extendeddata() const;
  void set_extendeddata(ExtendedData* extendeddata);
  bool has_extendeddata() const;
  void clear_extendeddata();

};

%nodefaultctor Container;
class Container : public Feature {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  void add_feature(Feature* feature);
  const size_t feature_array_size() const;
  const kmldom::Feature* feature_array_at(unsigned int index) const;
};

%nodefaultctor Geometry;
class Geometry : public Object {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);
};

%nodefaultctor BasicLink;
class BasicLink : public Object {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <href>
  const std::string& href() const;
  bool has_href() const;
  void set_href(const std::string& href);
  void clear_href();
};

%nodefaultctor Icon;
class Icon : public BasicLink {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <refreshMode>
  int refreshmode() const;
  void set_refreshmode(int refreshmode);
  bool has_refreshmode() const;
  void clear_refreshmode();

  // <refreshInterval>
  double refreshinterval() const;
  void set_refreshinterval(double refreshinterval);
  bool has_refreshinterval() const;
  void clear_refreshinterval();

  // <viewRefreshMode>
  int viewrefreshmode() const;
  void set_viewrefreshmode(int viewrefreshmode);
  bool has_viewrefreshmode() const;
  void clear_viewrefreshmode();

  // <viewRefreshTime>
  double viewrefreshtime() const;
  void set_viewrefreshtime(double viewrefreshtime);
  bool has_viewrefreshtime() const;
  void clear_viewrefreshtime();

  // <viewBoundScale>
  double viewboundscale() const;
  void set_viewboundscale(double viewboundscale);
  bool has_viewboundscale() const;
  void clear_viewboundscale();

  // <viewFormat>
  const std::string& viewformat() const;
  void set_viewformat(const std::string& viewformat);
  bool has_viewformat() const;
  void clear_viewformat();

  // <httpQuery>
  const std::string& httpquery() const;
  void set_httpquery(const std::string& httpquery);
  bool has_httpquery() const;
  void clear_httpquery();

};

%nodefaultctor IconStyleIcon;
class IconStyleIcon : public BasicLink {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <href>
  const std::string& href() const;
  bool has_href() const;
  void set_href(const std::string& href);
  void clear_href();

};

%nodefaultctor Overlay;
class Overlay : public Feature {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <color>
  const std::string& color() const;
  void set_color(const std::string& color);
  bool has_color() const;
  void clear_color();

  // <drawOrder>
  int draworder() const;
  void set_draworder(int draworder);
  bool has_draworder() const;
  void clear_draworder();

  // <Icon>
  const Icon* icon() const;
  void set_icon(Icon* icon);
  bool has_icon() const;
  void clear_icon();

};

%nodefaultctor BalloonStyle;
class BalloonStyle : public SubStyle {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <bgColor>
  const std::string& bgcolor() const;
  void set_bgcolor(const std::string& bgcolor);
  bool has_bgcolor() const;
  void clear_bgcolor();

  // <textColor>
  const std::string& textcolor() const;
  void set_textcolor(const std::string& textcolor);
  bool has_textcolor() const;
  void clear_textcolor();

  // <text>
  const std::string& text() const;
  void set_text(const std::string& text);
  bool has_text() const;
  void clear_text();

  // <displayMode>
  int displaymode() const;
  void set_displaymode(int displaymode);
  bool has_displaymode() const;
  void clear_displaymode();

};

%nodefaultctor Camera;
class Camera : public AbstractView {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <longitude>
  double longitude() const;
  void set_longitude(double longitude);
  bool has_longitude() const;
  void clear_longitude();

  // <latitude>
  double latitude() const;
  void set_latitude(double latitude);
  bool has_latitude() const;
  void clear_latitude();

  // <altitude>
  double altitude() const;
  void set_altitude(double altitude);
  bool has_altitude() const;
  void clear_altitude();

  // <heading>
  double heading() const;
  void set_heading(double heading);
  bool has_heading() const;
  void clear_heading();

  // <tilt>
  double tilt() const;
  void set_tilt(double tilt);
  bool has_tilt() const;
  void clear_tilt();

  // <roll>
  double roll() const;
  void set_roll(double roll);
  bool has_roll() const;
  void clear_roll();

  // <altitudeMode>
  int altitudemode() const;
  void set_altitudemode(int altitudemode);
  bool has_altitudemode() const;
  void clear_altitudemode();

};

%nodefaultctor UpdateOperation;
class UpdateOperation : public Element {
};

%nodefaultctor Change;
class Change : public UpdateOperation {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  void add_object(Object* object);
  const size_t object_array_size() const;
  const Object* object_array_at(unsigned int index) const;
};

%nodefaultctor Create;
class Create : public UpdateOperation {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  void add_container(Container* container);
  const size_t container_array_size() const;
  const Container* container_array_at(unsigned int index) const;
};

%nodefaultctor Data;
class Data : public ExtendedDataMember {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <displayName>
  const std::string& displayname() const;
  void set_displayname(const std::string& displayname);
  bool has_displayname() const;
  void clear_displayname();

};

%nodefaultctor Delete;
class Delete : public UpdateOperation {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  void add_feature(Feature* feature);
  const size_t feature_array_size() const;
  const Feature* feature_array_at(unsigned int index) const;
};

%nodefaultctor SimpleField;
class SimpleField : public Element {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // type=
  const std::string& type() const;
  bool has_type() const;
  void set_type(const std::string& value);
  void clear_type();

  // name=
  const std::string& name() const;
  bool has_name() const;
  void set_name(const std::string& value);
  void clear_name();

  // <displayName>
  const std::string& displayname() const;
  void set_displayname(const std::string& displayname);
  bool has_displayname() const;
  void clear_displayname();

};

%nodefaultctor Schema;
class Schema : public Element {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // name=
  const std::string& name() const;
  bool has_name() const;
  void set_name(const std::string& value);
  void clear_name();

  // id=
  const std::string& id() const;
  bool has_id() const;
  void set_id(const std::string& value);
  void clear_id();

  // <SimpleField>...
  void add_simplefield(SimpleField* simplefield);
  const size_t simplefield_array_size() const;
  const SimpleField* simplefield_array_at(unsigned int index) const;
};

%nodefaultctor Document;
class Document : public Container {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <Schema>
  void add_schema(Schema* schema);
  const size_t schema_array_size() const;
  const Feature* schema_array_at(unsigned int index) const;

  // <Style> and <StyleMap>
  void add_styleselector(StyleSelector* styleselector);
  const size_t styleselector_array_size() const;
  const StyleSelector* styleselector_array_at(unsigned int index) const;
};

%nodefaultctor Folder;
class Folder : public Container {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);
};

%nodefaultctor LatLonBox;
class LatLonBox : public AbstractLatLonBox {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <rotation>
  double rotation() const;
  void set_rotation(double rotation);
  bool has_rotation() const;
  void clear_rotation();

};

%nodefaultctor GroundOverlay;
class GroundOverlay : public Overlay {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <altitude>
  double altitude() const;
  void set_altitude(double altitude);
  bool has_altitude() const;
  void clear_altitude();

  // <altitudeMode>
  int altitudemode() const;
  void set_altitudemode(int altitudemode);
  bool has_altitudemode() const;
  void clear_altitudemode();

  // <LatLonBox>
  const LatLonBox* latlonbox() const;
  void set_latlonbox(LatLonBox* latlonbox);
  bool has_latlonbox() const;
  void clear_latlonbox();

};

%nodefaultctor HotSpot;
class HotSpot : public Vec2 {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);
};

%nodefaultctor IconStyle;
class IconStyle : public ColorStyle {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <scale>
  double scale() const;
  void set_scale(double scale);
  bool has_scale() const;
  void clear_scale();

  // <Icon> (different from Overlay Icon)
  const IconStyleIcon* icon() const;
  void set_icon(IconStyleIcon* icon);
  bool has_icon() const;
  void clear_icon();

  // <heading>
  double heading() const;
  void set_heading(double heading);
  bool has_heading() const;
  void clear_heading();

  // <hotSpot>
  const HotSpot* hotspot() const;
  void set_hotspot(HotSpot* hotspot);
  bool has_hotspot() const;
  void clear_hotspot();

};

%nodefaultctor ImagePyramid;
class ImagePyramid : public Object {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <tileSize>
  int tilesize() const;
  void set_tilesize(int tilesize);
  bool has_tilesize() const;
  void clear_tilesize();

  // <maxWidth>
  int maxwidth() const;
  void set_maxwidth(int maxwidth);
  bool has_maxwidth() const;
  void clear_maxwidth();

  // <maxHeight>
  int maxheight() const;
  void set_maxheight(int maxheight);
  bool has_maxheight() const;
  void clear_maxheight();

  // <gridOrigin>
  int gridorigin() const;
  void set_gridorigin(int gridorigin);
  bool has_gridorigin() const;
  void clear_gridorigin();

};

%nodefaultctor ItemIcon;
class ItemIcon : public Object {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // TODO: <state>
  //itemIconState* state() const;
  //void set_state(itemIconState* state);
  //bool has_state() const;
  //void clear_state();

  // <href>
  const std::string& href() const;
  void set_href(const std::string& href);
  bool has_href() const;
  void clear_href();

};

%nodefaultctor LabelStyle;
class LabelStyle : public ColorStyle {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <scale>
  double scale() const;
  void set_scale(double scale);
  bool has_scale() const;
  void clear_scale();

};

%nodefaultctor LineString;
class LineString : public Geometry {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <extrude>
  bool extrude() const;
  void set_extrude(bool extrude);
  bool has_extrude() const;
  void clear_extrude();

  // <tessellate>
  bool tessellate() const;
  void set_tessellate(bool tessellate);
  bool has_tessellate() const;
  void clear_tessellate();

  // <altitudeMode>
  int altitudemode() const;
  void set_altitudemode(int altitudemode);
  bool has_altitudemode() const;
  void clear_altitudemode();

  // <coordinates>
  const Coordinates* coordinates() const;
  void set_coordinates(Coordinates* coordinates);
  bool has_coordinates() const;
  void clear_coordinates();

};

%nodefaultctor LineStyle;
class LineStyle : public ColorStyle {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <width>
  double width() const;
  void set_width(double width);
  bool has_width() const;
  void clear_width();

};

%nodefaultctor LinearRing;
class LinearRing : public Geometry {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <extrude>
  bool extrude() const;
  void set_extrude(bool extrude);
  bool has_extrude() const;
  void clear_extrude();

  // <tessellate>
  bool tessellate() const;
  void set_tessellate(bool tessellate);
  bool has_tessellate() const;
  void clear_tessellate();

  // <altitudeMode>
  int altitudemode() const;
  void set_altitudemode(int altitudemode);
  bool has_altitudemode() const;
  void clear_altitudemode();

  // <coordinates>
  const Coordinates* coordinates() const;
  void set_coordinates(Coordinates* coordinates);
  bool has_coordinates() const;
  void clear_coordinates();

};

%nodefaultctor Link;
class Link : public BasicLink {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <refreshMode>
  int refreshmode() const;
  void set_refreshmode(int refreshmode);
  bool has_refreshmode() const;
  void clear_refreshmode();

  // <refreshInterval>
  double refreshinterval() const;
  void set_refreshinterval(double refreshinterval);
  bool has_refreshinterval() const;
  void clear_refreshinterval();

  // <viewRefreshMode>
  int viewrefreshmode() const;
  void set_viewrefreshmode(int viewrefreshmode);
  bool has_viewrefreshmode() const;
  void clear_viewrefreshmode();

  // <viewRefreshTime>
  double viewrefreshtime() const;
  void set_viewrefreshtime(double viewrefreshtime);
  bool has_viewrefreshtime() const;
  void clear_viewrefreshtime();

  // <viewBoundScale>
  double viewboundscale() const;
  void set_viewboundscale(double viewboundscale);
  bool has_viewboundscale() const;
  void clear_viewboundscale();

  // <viewFormat>
  const std::string& viewformat() const;
  void set_viewformat(const std::string& viewformat);
  bool has_viewformat() const;
  void clear_viewformat();

  // <httpQuery>
  const std::string& httpquery() const;
  void set_httpquery(const std::string& httpquery);
  bool has_httpquery() const;
  void clear_httpquery();

};

%nodefaultctor ListStyle;
class ListStyle : public SubStyle {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <listItemType>
  int listitemtype() const;
  void set_listitemtype(int listitemtype);
  bool has_listitemtype() const;
  void clear_listitemtype();

  // <bgColor>
  const std::string& bgcolor() const;
  void set_bgcolor(const std::string& bgcolor);
  bool has_bgcolor() const;
  void clear_bgcolor();

  // <ItemIcon>...
  void add_itemicon(ItemIcon* itemicon);
  const size_t itemicon_array_size() const;
  const ItemIcon* itemicon_array_at(unsigned int index) const;
};

%nodefaultctor Location;
class Location : public Object {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <longitude>
  double longitude() const;
  void set_longitude(double longitude);
  bool has_longitude() const;
  void clear_longitude();

  // <latitude>
  double latitude() const;
  void set_latitude(double latitude);
  bool has_latitude() const;
  void clear_latitude();

  // <altitude>
  double altitude() const;
  void set_altitude(double altitude);
  bool has_altitude() const;
  void clear_altitude();

};

%nodefaultctor LookAt;
class LookAt : public AbstractView {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <longitude>
  double longitude() const;
  void set_longitude(double longitude);
  bool has_longitude() const;
  void clear_longitude();

  // <latitude>
  double latitude() const;
  void set_latitude(double latitude);
  bool has_latitude() const;
  void clear_latitude();

  // <altitude>
  double altitude() const;
  void set_altitude(double altitude);
  bool has_altitude() const;
  void clear_altitude();

  // <heading>
  double heading() const;
  void set_heading(double heading);
  bool has_heading() const;
  void clear_heading();

  // <tilt>
  double tilt() const;
  void set_tilt(double tilt);
  bool has_tilt() const;
  void clear_tilt();

  // <range>
  double range() const;
  void set_range(double range);
  bool has_range() const;
  void clear_range();

  // <altitudeMode>
  int altitudemode() const;
  void set_altitudemode(int altitudemode);
  bool has_altitudemode() const;
  void clear_altitudemode();

};

%nodefaultctor Orientation;
class Orientation : public Object {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <heading>
  double heading() const;
  void set_heading(double heading);
  bool has_heading() const;
  void clear_heading();

  // <tilt>
  double tilt() const;
  void set_tilt(double tilt);
  bool has_tilt() const;
  void clear_tilt();

  // <roll>
  double roll() const;
  void set_roll(double roll);
  bool has_roll() const;
  void clear_roll();

};

%nodefaultctor Scale;
class Scale : public Object {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <x>
  double x() const;
  void set_x(double x);
  bool has_x() const;
  void clear_x();

  // <y>
  double y() const;
  void set_y(double y);
  bool has_y() const;
  void clear_y();

  // <z>
  double z() const;
  void set_z(double z);
  bool has_z() const;
  void clear_z();

};

%nodefaultctor Alias;
class Alias : public Object {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <targetHref>
  const std::string& targethref() const;
  void set_targethref(const std::string& targethref);
  bool has_targethref() const;
  void clear_targethref();

  // <sourceHref>
  const std::string& sourcehref() const;
  void set_sourcehref(const std::string& sourcehref);
  bool has_sourcehref() const;
  void clear_sourcehref();

};

%nodefaultctor ResourceMap;
class ResourceMap : public Object {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <Alias>...
  void add_alias(Alias* alias);
  const size_t alias_array_size() const;
  const Alias* alias_array_at(unsigned int index) const;
};

%nodefaultctor Model;
class Model : public Geometry {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <altitudeMode>
  int altitudemode() const;
  void set_altitudemode(int altitudemode);
  bool has_altitudemode() const;
  void clear_altitudemode();

  // <Location>
  const Location* location() const;
  void set_location(Location* location);
  bool has_location() const;
  void clear_location();

  // <Orientation>
  const Orientation* orientation() const;
  void set_orientation(Orientation* orientation);
  bool has_orientation() const;
  void clear_orientation();

  // <Scale>
  const Scale* scale() const;
  void set_scale(Scale* scale);
  bool has_scale() const;
  void clear_scale();

  // <Link>
  const Link* link() const;
  void set_link(Link* link);
  bool has_link() const;
  void clear_link();

  // <ResourceMap>
  const ResourceMap* resourcemap() const;
  void set_resourcemap(ResourceMap* resourcemap);
  bool has_resourcemap() const;
  void clear_resourcemap();

};

%nodefaultctor MultiGeometry;
class MultiGeometry : public Geometry {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // Geometry...
  void add_geometry(Geometry* geometry);
  const size_t geometry_array_size() const;
  const Geometry* geometry_array_at(unsigned int index) const;
};

%nodefaultctor NetworkLink;
class NetworkLink : public Feature {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <refreshVisibility>
  bool refreshvisibility() const;
  void set_refreshvisibility(bool refreshvisibility);
  bool has_refreshvisibility() const;
  void clear_refreshvisibility();

  // <flyToView>
  bool flytoview() const;
  void set_flytoview(bool flytoview);
  bool has_flytoview() const;
  void clear_flytoview();

  // <Link>
  const Link* link() const;
  void set_link(Link* link);
  bool has_link() const;
  void clear_link();

};

%nodefaultctor Update;
class Update : public Element {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  void add_updateoperation(UpdateOperation* updateoperation);
  const size_t updateoperation_array_size() const;
  const UpdateOperation* updateoperation_array_at(unsigned int index) const;
};

%nodefaultctor NetworkLinkControl;
class NetworkLinkControl : public Element {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <minRefreshPeriod>
  double minrefreshperiod() const;
  void set_minrefreshperiod(double minrefreshperiod);
  bool has_minrefreshperiod() const;
  void clear_minrefreshperiod();

  // <maxSessionLength>
  double maxsessionlength() const;
  void set_maxsessionlength(double maxsessionlength);
  bool has_maxsessionlength() const;
  void clear_maxsessionlength();

  // <cookie>
  const std::string& cookie() const;
  void set_cookie(const std::string& cookie);
  bool has_cookie() const;
  void clear_cookie();

  // <message>
  const std::string& message() const;
  void set_message(const std::string& message);
  bool has_message() const;
  void clear_message();

  // <linkName>
  const std::string& linkname() const;
  void set_linkname(const std::string& linkname);
  bool has_linkname() const;
  void clear_linkname();

  // <linkDescription>
  const std::string& linkdescription() const;
  void set_linkdescription(const std::string& linkdescription);
  bool has_linkdescription() const;
  void clear_linkdescription();

  // <linkSnippet>
  const LinkSnippet* linksnippet() const;
  void set_linksnippet(LinkSnippet* linksnippet);
  bool has_linksnippet() const;
  void clear_linksnippet();

  // <expires>
  const std::string& expires() const;
  void set_expires(const std::string& expires);
  bool has_expires() const;
  void clear_expires();

  // <Update>
  const Update* update() const;
  void set_update(Update* update);
  bool has_update() const;
  void clear_update();

  // AbstractView
  const AbstractView* abstractview() const;
  void set_abstractview(AbstractView* abstractview);
  bool has_abstractview() const;
  void clear_abstractview();

};

%nodefaultctor Pair;
class Pair : public Object {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <key>
  int key() const;
  void set_key(int key);
  bool has_key() const;
  void clear_key();

  // <styleUrl>
  const std::string& styleurl() const;
  void set_styleurl(const std::string& styleurl);
  bool has_styleurl() const;
  void clear_styleurl();

  // <StyleSelector>
  const StyleSelector* styleselector() const;
  void set_styleselector(StyleSelector* styleselector);
  bool has_styleselector() const;
  void clear_styleselector();

};

%nodefaultctor ViewVolume;
class ViewVolume : public Object {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <leftFov>
  double leftfov() const;
  void set_leftfov(double leftfov);
  bool has_leftfov() const;
  void clear_leftfov();

  // <rightFov>
  double rightfov() const;
  void set_rightfov(double rightfov);
  bool has_rightfov() const;
  void clear_rightfov();

  // <bottomFov>
  double bottomfov() const;
  void set_bottomfov(double bottomfov);
  bool has_bottomfov() const;
  void clear_bottomfov();

  // <topFov>
  double topfov() const;
  void set_topfov(double topfov);
  bool has_topfov() const;
  void clear_topfov();

  // <near>
  double near() const;
  void set_near(double near);
  bool has_near() const;
  void clear_near();

};

%nodefaultctor Point;
class Point : public Geometry {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <extrude>
  bool extrude() const;
  void set_extrude(bool extrude);
  bool has_extrude() const;
  void clear_extrude();

  // <altitudeMode>
  int altitudemode() const;
  void set_altitudemode(int altitudemode);
  bool has_altitudemode() const;
  void clear_altitudemode();

  // <coordinates>
  const Coordinates* coordinates() const;
  void set_coordinates(Coordinates* coordinates);
  bool has_coordinates() const;
  void clear_coordinates();

};

%nodefaultctor PhotoOverlay;
class PhotoOverlay : public Overlay {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <rotation>
  double rotation() const;
  void set_rotation(double rotation);
  bool has_rotation() const;
  void clear_rotation();

  // <ViewVolume>
  const ViewVolume* viewvolume() const;
  void set_viewvolume(ViewVolume* viewvolume);
  bool has_viewvolume() const;
  void clear_viewvolume();

  // <ImagePyramid>
  const ImagePyramid* imagepyramid() const;
  void set_imagepyramid(ImagePyramid* imagepyramid);
  bool has_imagepyramid() const;
  void clear_imagepyramid();

  // <Point>
  const Point* point() const;
  void set_point(Point* point);
  bool has_point() const;
  void clear_point();

  // <shape>
  int shape() const;
  bool has_shape() const;
  void set_shape(int shape);
  void clear_shape();
};

%nodefaultctor Placemark;
class Placemark : public Feature {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // Geometry
  const Geometry* geometry() const;
  void set_geometry(Geometry* geometry);
  bool has_geometry() const;
  void clear_geometry();

};

%nodefaultctor PolyStyle;
class PolyStyle : public ColorStyle {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <fill>
  bool fill() const;
  void set_fill(bool fill);
  bool has_fill() const;
  void clear_fill();

  // <outline>
  bool outline() const;
  void set_outline(bool outline);
  bool has_outline() const;
  void clear_outline();

};

%nodefaultctor OuterBoundaryIs;
class OuterBoundaryIs : public Element {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <LinearRing>
  const LinearRing* linearring() const;
  void set_linearring(LinearRing* linearring);
  bool has_linearring() const;
  void clear_linearring();
};

%nodefaultctor InnerBoundaryIs;
class InnerBoundaryIs : public Element {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <LinearRing>
  const LinearRing* linearring() const;
  void set_linearring(LinearRing* linearring);
  bool has_linearring() const;
  void clear_linearring();
};

%nodefaultctor Polygon;
class Polygon : public Geometry {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <extrude>
  bool extrude() const;
  void set_extrude(bool extrude);
  bool has_extrude() const;
  void clear_extrude();

  // <tessellate>
  bool tessellate() const;
  void set_tessellate(bool tessellate);
  bool has_tessellate() const;
  void clear_tessellate();

  // <altitudeMode>
  int altitudemode() const;
  void set_altitudemode(int altitudemode);
  bool has_altitudemode() const;
  void clear_altitudemode();

  // <outerBoundaryIs>
  const OuterBoundaryIs* outerboundaryis() const;
  void set_outerboundaryis(OuterBoundaryIs* outerboundaryis);
  bool has_outerboundaryis() const;
  void clear_outerboundaryis();

  // <innerBoundaryIs>
  void add_innerboundaryis(InnerBoundaryIs* innerboundaryis);
  const size_t innerboundaryis_array_size() const;
  const InnerBoundaryIs* innerboundaryis_array_at(unsigned int index);
};

%nodefaultctor SimpleData;
class SimpleData : public Element {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);
};

%nodefaultctor SchemaData;
class SchemaData : public Object {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <SimpleData>...
  void add_simpledata(SimpleData* simpledata);
  const size_t simpledata_array_size() const;
  const SimpleData* simpledata_array_at(unsigned int index) const;
};

%nodefaultctor OverlayXY;
class OverlayXY : public Vec2 {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);
};

%nodefaultctor ScreenXY;
class ScreenXY : public Vec2 {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);
};

%nodefaultctor RotationXY;
class RotationXY : public Vec2 {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);
};

%nodefaultctor Size;
class Size : public Vec2 {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);
};

%nodefaultctor ScreenOverlay;
class ScreenOverlay : public Overlay {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <OverlayXY>
  const OverlayXY* overlayxy() const;
  void set_overlayxy(OverlayXY* overlayxy);
  bool has_overlayxy() const;
  void clear_overlayxy();

  // <ScreenXY>
  const ScreenXY* screenxy() const;
  void set_screenxy(ScreenXY* screenxy);
  bool has_screenxy() const;
  void clear_screenxy();

  // <RotationXY>
  const RotationXY* rotationxy() const;
  void set_rotationxy(RotationXY* rotationxy);
  bool has_rotationxy() const;
  void clear_rotationxy();

  // <size>
  const Size* size() const;
  void set_size(Size* size);
  bool has_size() const;
  void clear_size();

  // <rotation>
  double rotation() const;
  void set_rotation(double rotation);
  bool has_rotation() const;
  void clear_rotation();

};

%nodefaultctor Style;
class Style : public StyleSelector {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <IconStyle>
  const IconStyle* iconstyle() const;
  void set_iconstyle(IconStyle* iconstyle);
  bool has_iconstyle() const;
  void clear_iconstyle();

  // <LabelStyle>
  const LabelStyle* labelstyle() const;
  void set_labelstyle(LabelStyle* labelstyle);
  bool has_labelstyle() const;
  void clear_labelstyle();

  // <LineStyle>
  const LineStyle* linestyle() const;
  void set_linestyle(LineStyle* linestyle);
  bool has_linestyle() const;
  void clear_linestyle();

  // <PolyStyle>
  const PolyStyle* polystyle() const;
  void set_polystyle(PolyStyle* polystyle);
  bool has_polystyle() const;
  void clear_polystyle();

  // <BalloonStyle>
  const BalloonStyle* balloonstyle() const;
  void set_balloonstyle(BalloonStyle* balloonstyle);
  bool has_balloonstyle() const;
  void clear_balloonstyle();

  // <ListStyle>
  const ListStyle* liststyle() const;
  void set_liststyle(ListStyle* liststyle);
  bool has_liststyle() const;
  void clear_liststyle();

};

%nodefaultctor StyleMap;
class StyleMap : public StyleSelector {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <Pair>
  void add_pair(Pair* pair);
  const size_t pair_array_size() const;
  const Pair* pair_array_at(unsigned int index) const;
};

%nodefaultctor TimeSpan;
class TimeSpan : public TimePrimitive {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <begin>
  const std::string& begin() const;
  void set_begin(const std::string& begin);
  bool has_begin() const;
  void clear_begin();

  // <end>
  const std::string& end() const;
  void set_end(const std::string& end);
  bool has_end() const;
  void clear_end();

};

%nodefaultctor TimeStamp;
class TimeStamp : public TimePrimitive {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // <when>
  const std::string& when() const;
  void set_when(const std::string& when);
  bool has_when() const;
  void clear_when();

};

%nodefaultctor Kml;
class Kml : public Element {
public:
  virtual kmldom::KmlDomType Type();
  virtual bool IsA(kmldom::KmlDomType type_id);

  // hint=
  const std::string& hint();
  void set_hint(const std::string& hint);
  bool has_hint();
  void clear_hint();

  // <NetworkLinkControl>
  const NetworkLinkControl* networklinkcontrol() const;
  void set_networklinkcontrol(NetworkLinkControl* networklinkcontrol);
  bool has_networklinkcontrol() const;
  void clear_networklinkcontrol();

  // Feature
  const Feature* feature() const;
  void set_feature(Feature* feature);
  bool has_feature() const;
  void clear_feature();

};


