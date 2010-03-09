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

// This file contains the unit tests for the MergeFields() and MergeElements()
// functions.

#include "kml/engine/merge.h"
#include "kml/dom/kml_funcs.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml22.h"
#include "kml/dom/kmldom.h"
#include "gtest/gtest.h"

using kmlbase::Vec3;
using kmldom::CoordinatesPtr;
using kmldom::FolderPtr;
using kmldom::LineStringPtr;
using kmldom::IconStylePtr;
using kmldom::IconStyleIconPtr;
using kmldom::KmlFactory;
using kmldom::PlacemarkPtr;
using kmldom::PointPtr;
using kmldom::StylePtr;

namespace kmlengine {

class MergeTest : public testing::Test {
 protected:
  virtual void SetUp() {
    factory_ = KmlFactory::GetFactory();
    linestring_ = factory_->CreateLineString();
    source_placemark_ = factory_->CreatePlacemark();
    target_placemark_ = factory_->CreatePlacemark();
    point_ = factory_->CreatePoint();
    source_style_ = factory_->CreateStyle();
    target_style_ = factory_->CreateStyle();
  }

  StylePtr SetSubStyles(StylePtr style, double icon_style_scale,
                        const string& label_style_color,
                        double line_style_width, bool poly_style_fill,
                        const string& balloon_style_text,
                        kmldom::ListItemTypeEnum liststyle_style_listitemtype);
  void VerifySubStyles(StylePtr style, double icon_style_scale,
                       const string& label_style_color,
                       double line_style_width, bool poly_style_fill,
                       const string& balloon_style_text,
                       kmldom::ListItemTypeEnum liststyle_style_listitemtype);
  void SetPointPlacemark(PlacemarkPtr placemark, const string& name,
                         double lat, double lon);
  void VerifyPointPlacemark(PlacemarkPtr placemark, const string& name,
                            double lat, double lon);
  KmlFactory *factory_;
  LineStringPtr linestring_;
  PlacemarkPtr source_placemark_;
  PlacemarkPtr target_placemark_;
  StylePtr source_style_;
  StylePtr target_style_;
  PointPtr point_;
};

// Verify MergeFields() does not crash on NULL args.
TEST_F(MergeTest, TestMergeFieldsNull) {
  MergeFields(source_placemark_, NULL);
  MergeFields(NULL, target_placemark_);
  MergeFields(NULL, NULL);
}

// Verify MergeFields does not crash or harm source if source and target
// are the same element.
TEST_F(MergeTest, TestMergeFieldsSame) {
  // Verify that passing the same element for source and target does not crash.
  MergeFields(source_placemark_, source_placemark_);
  const string kDescription("description");
  source_placemark_->set_description(kDescription);
  MergeFields(source_placemark_, source_placemark_);
  // Placemark is still sane. Placemark had a description before and still
  // does and had no other fields and still does.
  ASSERT_EQ(kDescription, source_placemark_->get_description());
  ASSERT_FALSE(source_placemark_->has_name());
  ASSERT_FALSE(source_placemark_->has_region());
  ASSERT_FALSE(source_placemark_->has_geometry());
  MergeFields(point_, point_);
  // Point is still sane. Point had no coordinates before and none after.
  ASSERT_FALSE(point_->has_coordinates());
}

// Verify normal usage of MergeFields().
TEST_F(MergeTest, TestSimpleMergeFields) {
  // Set one field in the source.
  const string kSourceName("source name");
  source_placemark_->set_name(kSourceName);
  // Merge fields in source to target.
  MergeFields(source_placemark_, target_placemark_);
  // Verify target now has the expected value for the given field.
  ASSERT_EQ(kSourceName, target_placemark_->get_name());

  // Set the target's value for the given field.
  const string kTargetName("target name");
  target_placemark_->set_name(kTargetName);
  // Verify that the source still has the expected value.
  ASSERT_EQ(kSourceName, source_placemark_->get_name());
  // Merge source to target.
  MergeFields(source_placemark_, target_placemark_);
  // Verify that target now has the expected value.
  ASSERT_EQ(kSourceName, target_placemark_->get_name());
}

// Verify that several fields are properly set in the target.
TEST_F(MergeTest, TestMergeFieldsMany) {
  const string kName("name");
  const bool kVisibility(false);
  const bool kOpen(false);
  const string kDescription("name");
  source_placemark_->set_name(kName);
  source_placemark_->set_description(kDescription);
  source_placemark_->set_visibility(kVisibility);
  source_placemark_->set_open(kOpen);
  MergeFields(source_placemark_, target_placemark_);
  ASSERT_TRUE(target_placemark_->has_name());
  ASSERT_EQ(kName, target_placemark_->get_name());
  ASSERT_TRUE(target_placemark_->has_description());
  ASSERT_EQ(kDescription, target_placemark_->get_description());
  ASSERT_TRUE(target_placemark_->has_visibility());
  ASSERT_EQ(kVisibility, target_placemark_->get_visibility());
  ASSERT_TRUE(target_placemark_->has_open());
  ASSERT_EQ(kOpen, target_placemark_->get_open());
}

// Verify that MergeFields() does not effect complex children.
TEST_F(MergeTest, TestDontMergeComplexChildren) {
  // Give the source element a complex child.
  source_placemark_->set_geometry(point_);
  // Give the target a different type for the same complex child.
  target_placemark_->set_geometry(linestring_);
  // Set some simple element children in the source.
  const string kName("hi there");
  source_placemark_->set_name(kName);
  source_placemark_->set_visibility(false);
  // Merge fields from source to target.
  MergeFields(source_placemark_, target_placemark_);
  // Verify simple fields were copied.
  ASSERT_EQ(kName, target_placemark_->get_name());
  ASSERT_FALSE(target_placemark_->get_visibility());
  // Verify complex child of target was not touched.
  ASSERT_EQ(kmldom::Type_LineString,
                       target_placemark_->get_geometry()->Type());
}

// Verify MergeElements() does not crash on NULL args.
TEST_F(MergeTest, TestMergeElementsNull) {
  MergeElements(source_placemark_, NULL);
  MergeElements(NULL, target_placemark_);
  MergeElements(NULL, NULL);
}

TEST_F(MergeTest, TestBasicMergeIconStyle) {
  const string kHref("icon.png");
  IconStylePtr source = KmlFactory::GetFactory()->CreateIconStyle();
  IconStyleIconPtr icon = KmlFactory::GetFactory()->CreateIconStyleIcon();
  icon->set_href(kHref);
  source->set_icon(icon);

  IconStylePtr target = KmlFactory::GetFactory()->CreateIconStyle();
  MergeElements(source, target);
  ASSERT_TRUE(target->has_icon());
  ASSERT_TRUE(target->get_icon()->has_href());
  ASSERT_EQ(kHref, target->get_icon()->get_href());
}

// This verifies that an element hierarchy is properly merged from
// target to source.  The test of IconStyle/Icon in particular excercises
// some special handling of this element.
TEST_F(MergeTest, TestMergeIconStyle) {
  const double kScale(1.3);
  const double kHeading(123);
  const string kHref("cool.jpeg");

  // This is what we are merging in:
  // <Style>
  //   <IconStyle>
  //      <scale>1.3</scale>
  //      <Icon>
  //        <href>cool.jpeg</href>
  //      </Icon>
  //   </IconStyle>
  // </Style>
  kmldom::IconStylePtr iconstyle = factory_->CreateIconStyle();
  iconstyle->set_scale(kScale);
  kmldom::IconStyleIconPtr iconstyleicon = factory_->CreateIconStyleIcon();
  iconstyleicon->set_href(kHref);
  iconstyle->set_icon(iconstyleicon);
  source_style_->set_iconstyle(iconstyle);

  // This is what we are merging onto:
  // <Style>
  //   <IconStyle>
  //      <scale>1.5</scale>
  //      <heading>123</heading>
  //   </IconStyle>
  // </Style>
  iconstyle = factory_->CreateIconStyle();
  iconstyle->set_scale(1.5);  // Something that is not kScale.
  iconstyle->set_heading(kHeading);
  target_style_->set_iconstyle(iconstyle);

  // Since target_style_ _has_ an IconStyle, this _merges_ IconStyle from src.
  MergeElements(source_style_, target_style_);

  // Verify that the result is this:
  // <Style>
  //   <IconStyle>
  //      <scale>1.3</scale>        <!-- over-written -->
  //      <heading>123</heading>    <!-- preserved -->
  //      <Icon>                    <!-- added -->
  //        <href>cool.jpeg</href>
  //      </Icon>
  //   </IconStyle>
  // </Style>
  ASSERT_TRUE(target_style_->has_iconstyle());
  ASSERT_EQ(kScale, source_style_->get_iconstyle()->get_scale());
  ASSERT_EQ(kScale, target_style_->get_iconstyle()->get_scale());
  ASSERT_EQ(kHeading, target_style_->get_iconstyle()->get_heading());
  ASSERT_TRUE(source_style_->get_iconstyle()->has_icon());
  ASSERT_TRUE(target_style_->get_iconstyle()->has_icon());
  ASSERT_TRUE(source_style_->get_iconstyle()->get_icon()->has_href());
  ASSERT_TRUE(target_style_->get_iconstyle()->get_icon()->has_href());
  ASSERT_EQ(kHref, source_style_->get_iconstyle()->get_icon()->get_href());
  ASSERT_EQ(kHref, target_style_->get_iconstyle()->get_icon()->get_href());
}

// This is a utility function to create each substyle and set the given
// field to the given value.
StylePtr MergeTest::SetSubStyles(StylePtr style,
                                 double icon_style_scale,
                                 const string& label_style_color,
                                 double line_style_width,
                                 bool poly_style_fill,
                                 const string& balloon_style_text,
                                 kmldom::ListItemTypeEnum
                                     list_style_listitemtype) {
  // <IconStyle><scale>
  kmldom::IconStylePtr iconstyle = factory_->CreateIconStyle();
  iconstyle->set_scale(icon_style_scale);
  style->set_iconstyle(iconstyle);

  // <LabelStyle><color>
  kmldom::LabelStylePtr labelstyle = factory_->CreateLabelStyle();
  labelstyle->set_color(label_style_color);
  style->set_labelstyle(labelstyle);

  // <LineStyle><width>
  kmldom::LineStylePtr linestyle = factory_->CreateLineStyle();
  linestyle->set_width(line_style_width);
  style->set_linestyle(linestyle);

  // <PolyStyle><fill>
  kmldom::PolyStylePtr polystyle = factory_->CreatePolyStyle();
  polystyle->set_fill(poly_style_fill);
  style->set_polystyle(polystyle);

  // <BalloonStyle><text>
  kmldom::BalloonStylePtr balloonstyle = factory_->CreateBalloonStyle();
  balloonstyle->set_text(balloon_style_text);
  style->set_balloonstyle(balloonstyle);

  // <ListStyle><listItemType>
  kmldom::ListStylePtr liststyle = factory_->CreateListStyle();
  liststyle->set_listitemtype(list_style_listitemtype);
  style->set_liststyle(liststyle);

  return style;
}

// This is a utility function to verify the given SubStyle fields.
void MergeTest::VerifySubStyles(StylePtr style,
                                double icon_style_scale,
                                const string& label_style_color,
                                double line_style_width,
                                bool poly_style_fill,
                                const string& balloon_style_text,
                                kmldom::ListItemTypeEnum
                                    list_style_listitemtype) {
  ASSERT_TRUE(style->has_iconstyle());
  ASSERT_TRUE(style->get_iconstyle()->has_scale());
  ASSERT_EQ(icon_style_scale, style->get_iconstyle()->get_scale());
  ASSERT_TRUE(style->has_labelstyle());
  ASSERT_TRUE(style->get_labelstyle()->has_color());
  ASSERT_EQ(label_style_color,
                       style->get_labelstyle()->get_color().to_string_abgr());
  ASSERT_TRUE(style->has_linestyle());
  ASSERT_TRUE(style->get_linestyle()->has_width());
  ASSERT_EQ(line_style_width, style->get_linestyle()->get_width());
  ASSERT_TRUE(style->has_polystyle());
  ASSERT_TRUE(style->get_polystyle()->has_fill());
  ASSERT_EQ(poly_style_fill, style->get_polystyle()->get_fill());
  ASSERT_TRUE(style->has_balloonstyle());
  ASSERT_TRUE(style->get_balloonstyle()->has_text());
  ASSERT_EQ(balloon_style_text,
                       style->get_balloonstyle()->get_text());
  ASSERT_TRUE(style->has_liststyle());
  ASSERT_TRUE(style->get_liststyle()->has_listitemtype());
  ASSERT_EQ(static_cast<int>(list_style_listitemtype),
                       style->get_liststyle()->get_listitemtype());
}

// This verifies that all SubStyles are merged from a source to target Style.
// Note that most field types are represented in this test: double, string,
// bool and enum.
TEST_F(MergeTest, TestMergeFullStyle) {
  // Create a Style with the following SubStyle fields.
  const double kScale(2.2);
  const string kColor("ff112233");
  const double kWidth(3.3);
  const bool kFill(false);
  const string kText("This is a <b>bold $[name]</b>");
  const kmldom::ListItemTypeEnum
      kListItemType(kmldom::LISTITEMTYPE_CHECKHIDECHILDREN);
  SetSubStyles(source_style_, kScale, kColor, kWidth, kFill, kText,
               kListItemType);

  // For good measure verify everything was set.
  VerifySubStyles(source_style_, kScale, kColor, kWidth, kFill, kText,
                  kListItemType);

  // Make an empty Style and merge in everything from source.
  MergeElements(source_style_, target_style_);

  // Verify everything was merged:
  VerifySubStyles(target_style_, kScale, kColor, kWidth, kFill, kText,
                  kListItemType);

  // Set source sub style fields to different values.
  SetSubStyles(source_style_, 1.1, "00110011", 0.2, true, "xxx",
               kmldom::LISTITEMTYPE_RADIOFOLDER);

  // Verify target is unscathed.
  VerifySubStyles(target_style_, kScale, kColor, kWidth, kFill, kText,
                  kListItemType);
}

// This is a utility method to create a Placemark with a Point.
void MergeTest::SetPointPlacemark(PlacemarkPtr placemark,
                                  const string& name,
                                  double lat, double lon) {
  placemark->set_name(name);
  PointPtr point = factory_->CreatePoint();
  CoordinatesPtr coordinates = factory_->CreateCoordinates();
  coordinates->add_latlng(lat, lon);
  point->set_coordinates(coordinates);
  placemark->set_geometry(point);
}

// This is a utility method to verify the given fields in a Point Placemark.
void MergeTest::VerifyPointPlacemark(PlacemarkPtr placemark,
                                     const string& name,
                                     double lat, double lon) {
  ASSERT_TRUE(placemark->has_name());
  ASSERT_EQ(name, placemark->get_name());
  ASSERT_TRUE(placemark->has_geometry());
  PointPtr point = kmldom::AsPoint(placemark->get_geometry());
  ASSERT_TRUE(point);
  ASSERT_TRUE(point->has_coordinates());
  CoordinatesPtr coordinates = point->get_coordinates();
  ASSERT_EQ(static_cast<size_t>(1),
                       coordinates->get_coordinates_array_size());
  Vec3 vec3 = coordinates->get_coordinates_array_at(0);
  ASSERT_EQ(lat, vec3.get_latitude());
  ASSERT_EQ(lon, vec3.get_longitude());
}

// This verifies the merge of a Placemark Point element hierarchy.
TEST_F(MergeTest, TestMergePointPlacemark) {
  const string kName("source");
  const double kLat(1.1);
  const double kLon(-1.1);
  SetPointPlacemark(source_placemark_, kName, kLat, kLon);
  SetPointPlacemark(target_placemark_, "not source at all", -2.2, 2.2);

  // Merge the source onto the target.
  MergeElements(source_placemark_, target_placemark_);

  // Verify that the target has the expected values.
  VerifyPointPlacemark(target_placemark_, kName, kLat, kLon);
}

// This verifies that a group match causes a replacement in the target.
// In this case a LineString replaces the Point because both are in
// single-value substitution group: Placemark's Geometry.
TEST_F(MergeTest, TestMergeSubstitutionGroup) {
  target_placemark_->set_geometry(point_);
  source_placemark_->set_geometry(linestring_);
  MergeElements(source_placemark_, target_placemark_);
  ASSERT_EQ(kmldom::Type_LineString,
                       target_placemark_->get_geometry()->Type());
}

// This verifies that the merge does not introduce any extraneous elements.
TEST_F(MergeTest, TestMergeFieldsSerialize) {
  kmldom::ListStylePtr liststyle = factory_->CreateListStyle();
  liststyle->set_listitemtype(kmldom::LISTITEMTYPE_CHECKHIDECHILDREN);
  source_style_->set_liststyle(liststyle);
  MergeElements(source_style_, target_style_);
  ASSERT_EQ(kmldom::SerializeRaw(target_style_),
                       kmldom::SerializeRaw(source_style_));
}

// An early version of MergeElements did not properly preserve the state of
// previously set attributes.
TEST_F(MergeTest, TestMergeAttributes) {
  const string kId("style-id");
  const string kTargetId("style-target-id");
  source_style_->set_targetid(kTargetId);
  target_style_->set_id(kId);
  MergeElements(source_style_, target_style_);
  ASSERT_TRUE(target_style_->has_id());
  ASSERT_EQ(kId, target_style_->get_id());
  ASSERT_TRUE(target_style_->has_targetid());
  ASSERT_EQ(kTargetId, target_style_->get_targetid());
}

}  // end namespace kmlengine

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
