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

// This file contains the unit test for the abstract Feature element.

#include "boost/intrusive_ptr.hpp"
#include "kml/dom/feature.h"
#include "kml/dom/kmldom.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_funcs.h"
#include "kml/dom/kml_ptr.h"
#include "gtest/gtest.h"

namespace kmldom {

class FeatureTest : public testing::Test {
 protected:
  virtual void SetUp() {
    feature_ = new TestFeature();
  }

  // Feature is abstract, hence its constructor is protected.
  class TestFeature : public Feature {
  };
  boost::intrusive_ptr<TestFeature> feature_;
};

TEST_F(FeatureTest, TestType) {
  ASSERT_TRUE(feature_->IsA(Type_Feature));
  ASSERT_TRUE(feature_->IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(FeatureTest, TestDefaults) {
  ASSERT_FALSE(feature_->has_name());
  ASSERT_EQ(string(""), feature_->get_name());
  ASSERT_FALSE(feature_->has_visibility());
  ASSERT_TRUE(feature_->get_visibility());
  ASSERT_FALSE(feature_->has_open());
  ASSERT_FALSE(feature_->get_open());
  ASSERT_FALSE(feature_->has_atomauthor());
  ASSERT_FALSE(feature_->has_atomlink());
  ASSERT_FALSE(feature_->has_address());
  ASSERT_EQ(string(""), feature_->get_address());
  ASSERT_FALSE(feature_->has_xaladdressdetails());
  ASSERT_FALSE(feature_->has_phonenumber());
  ASSERT_EQ(string(""), feature_->get_phonenumber());
  ASSERT_FALSE(feature_->has_snippet());
  ASSERT_TRUE(NULL == feature_->get_snippet());
  ASSERT_FALSE(feature_->has_description());
  ASSERT_EQ(string(""), feature_->get_description());
  ASSERT_FALSE(feature_->has_abstractview());
  ASSERT_TRUE(NULL == feature_->get_abstractview());
  ASSERT_FALSE(feature_->has_timeprimitive());
  ASSERT_TRUE(NULL == feature_->get_timeprimitive());
  ASSERT_FALSE(feature_->has_styleurl());
  ASSERT_EQ(string(""), feature_->get_styleurl());
  ASSERT_FALSE(feature_->has_styleselector());
  ASSERT_TRUE(NULL == feature_->get_styleselector());
  ASSERT_FALSE(feature_->has_region());
  ASSERT_TRUE(NULL == feature_->get_region());
  ASSERT_FALSE(feature_->has_gx_balloonvisibility());
  ASSERT_FALSE(feature_->get_gx_balloonvisibility());
}

// Verify setting default makes has_xxx() true:
TEST_F(FeatureTest, TestSetToDefaultValues) {
  feature_->set_name(feature_->get_name());
  ASSERT_TRUE(feature_->has_name());
  feature_->set_visibility(feature_->get_visibility());
  ASSERT_TRUE(feature_->has_visibility());
  feature_->set_open(feature_->get_open());
  ASSERT_TRUE(feature_->has_open());
  feature_->set_atomauthor(KmlFactory::GetFactory()->CreateAtomAuthor());
  feature_->set_atomlink(KmlFactory::GetFactory()->CreateAtomLink());
  ASSERT_TRUE(feature_->has_atomauthor());  // ptr is still null
  feature_->set_address(feature_->get_address());
  ASSERT_TRUE(feature_->has_address());
  feature_->set_xaladdressdetails(
      KmlFactory::GetFactory()->CreateXalAddressDetails());
  feature_->set_phonenumber(feature_->get_phonenumber());
  ASSERT_TRUE(feature_->has_phonenumber());
  feature_->set_snippet(KmlFactory::GetFactory()->CreateSnippet());
  ASSERT_TRUE(feature_->has_snippet());  // ptr is still null
  feature_->set_description(feature_->get_description());
  ASSERT_TRUE(feature_->has_description());
  feature_->set_abstractview(KmlFactory::GetFactory()->CreateLookAt());
  ASSERT_TRUE(feature_->has_abstractview());
  feature_->set_timeprimitive(KmlFactory::GetFactory()->CreateTimeSpan());
  ASSERT_TRUE(feature_->has_timeprimitive());
  feature_->set_styleurl(feature_->get_styleurl());
  ASSERT_TRUE(feature_->has_styleurl());
  feature_->set_styleselector(KmlFactory::GetFactory()->CreateStyle());
  ASSERT_TRUE(feature_->has_styleselector());
  feature_->set_region(KmlFactory::GetFactory()->CreateRegion());
  ASSERT_TRUE(feature_->has_region());
  feature_->set_gx_balloonvisibility(feature_->get_gx_balloonvisibility());
  ASSERT_TRUE(feature_->has_gx_balloonvisibility());
}

// Verify set, get, has, clear:
TEST_F(FeatureTest, TestSetGetHasClear) {
  // Non-default values:
  string name("a");
  bool visibility = false;
  bool open = true;
  AtomAuthorPtr atomauthor = KmlFactory::GetFactory()->CreateAtomAuthor();
  AtomLinkPtr atomlink = KmlFactory::GetFactory()->CreateAtomLink();
  string address("b");
  XalAddressDetailsPtr xaladdressdetails =
      KmlFactory::GetFactory()->CreateXalAddressDetails();
  string phonenumber("c");
  SnippetPtr snippet = KmlFactory::GetFactory()->CreateSnippet();
  string description("d");
  LookAtPtr lookat = KmlFactory::GetFactory()->CreateLookAt();
  TimeSpanPtr timespan = KmlFactory::GetFactory()->CreateTimeSpan();
  string styleurl("e");
  StylePtr style = KmlFactory::GetFactory()->CreateStyle();
  RegionPtr region = KmlFactory::GetFactory()->CreateRegion();
  bool gx_balloonvisibility = true;

  // Set all fields:
  feature_->set_name(name);
  feature_->set_visibility(visibility);
  feature_->set_open(open);
  feature_->set_atomauthor(atomauthor);
  feature_->set_atomlink(atomlink);
  feature_->set_address(address);
  feature_->set_xaladdressdetails(xaladdressdetails);
  feature_->set_phonenumber(phonenumber);
  feature_->set_snippet(snippet);
  feature_->set_description(description);
  feature_->set_abstractview(lookat);
  feature_->set_timeprimitive(timespan);
  feature_->set_styleurl(styleurl);
  feature_->set_styleselector(style);
  feature_->set_region(region);
  feature_->set_gx_balloonvisibility(gx_balloonvisibility);

  // Verify getter and has_xxx():
  ASSERT_TRUE(name == feature_->get_name());
  ASSERT_TRUE(feature_->has_name());
  ASSERT_TRUE(visibility == feature_->get_visibility());
  ASSERT_TRUE(feature_->has_visibility());
  ASSERT_TRUE(open == feature_->get_open());
  ASSERT_TRUE(feature_->has_open());
  ASSERT_TRUE(atomauthor == feature_->get_atomauthor());
  ASSERT_TRUE(feature_->has_atomauthor());
  ASSERT_TRUE(atomlink == feature_->get_atomlink());
  ASSERT_TRUE(feature_->has_atomlink());
  ASSERT_TRUE(address == feature_->get_address());
  ASSERT_TRUE(feature_->has_address());
  ASSERT_TRUE(xaladdressdetails == feature_->get_xaladdressdetails());
  ASSERT_TRUE(feature_->has_xaladdressdetails());
  ASSERT_TRUE(phonenumber == feature_->get_phonenumber());
  ASSERT_TRUE(feature_->has_phonenumber());
  ASSERT_TRUE(snippet == feature_->get_snippet());
  ASSERT_TRUE(feature_->has_snippet());
  ASSERT_TRUE(description == feature_->get_description());
  ASSERT_TRUE(feature_->has_description());
  ASSERT_TRUE(lookat == feature_->get_abstractview());
  ASSERT_TRUE(feature_->has_abstractview());
  ASSERT_TRUE(timespan == feature_->get_timeprimitive());
  ASSERT_TRUE(feature_->has_timeprimitive());
  ASSERT_TRUE(styleurl == feature_->get_styleurl());
  ASSERT_TRUE(feature_->has_styleurl());
  ASSERT_TRUE(style == feature_->get_styleselector());
  ASSERT_TRUE(feature_->has_styleselector());
  ASSERT_TRUE(region == feature_->get_region());
  ASSERT_TRUE(feature_->has_region());
  ASSERT_TRUE(gx_balloonvisibility == feature_->get_gx_balloonvisibility());
  ASSERT_TRUE(feature_->has_gx_balloonvisibility());

  // Clear all fields:
  feature_->clear_name();
  feature_->clear_visibility();
  feature_->clear_open();
  feature_->clear_atomauthor();
  feature_->clear_atomlink();
  feature_->clear_address();
  feature_->clear_xaladdressdetails();
  feature_->clear_phonenumber();
  feature_->clear_snippet();
  feature_->clear_description();
  feature_->clear_abstractview();
  feature_->clear_timeprimitive();
  feature_->clear_styleurl();
  feature_->clear_styleselector();
  feature_->clear_region();
  feature_->clear_gx_balloonvisibility();

  // Verify default state.
  ASSERT_FALSE(feature_->has_name());
  ASSERT_EQ(string(""), feature_->get_name());
  ASSERT_FALSE(feature_->has_visibility());
  ASSERT_TRUE(feature_->get_visibility());
  ASSERT_FALSE(feature_->has_open());
  ASSERT_FALSE(feature_->get_open());
  ASSERT_FALSE(feature_->has_atomauthor());
  ASSERT_FALSE(feature_->has_atomlink());
  ASSERT_FALSE(feature_->has_address());
  ASSERT_EQ(string(""), feature_->get_address());
  ASSERT_FALSE(feature_->has_xaladdressdetails());
  ASSERT_FALSE(feature_->has_phonenumber());
  ASSERT_EQ(string(""), feature_->get_phonenumber());
  ASSERT_FALSE(feature_->has_snippet());
  ASSERT_TRUE(NULL == feature_->get_snippet());
  ASSERT_FALSE(feature_->has_description());
  ASSERT_EQ(string(""), feature_->get_description());
  ASSERT_FALSE(feature_->has_abstractview());
  ASSERT_TRUE(NULL == feature_->get_abstractview());
  ASSERT_FALSE(feature_->has_timeprimitive());
  ASSERT_TRUE(NULL == feature_->get_timeprimitive());
  ASSERT_FALSE(feature_->has_styleurl());
  ASSERT_EQ(string(""), feature_->get_styleurl());
  ASSERT_FALSE(feature_->has_styleselector());
  ASSERT_TRUE(NULL == feature_->get_styleselector());
  ASSERT_FALSE(feature_->has_region());
  ASSERT_TRUE(NULL == feature_->get_region());
  ASSERT_FALSE(feature_->has_gx_balloonvisibility());
  ASSERT_FALSE(feature_->get_gx_balloonvisibility());
}

// Verify <snippet> and <Metadata> are recognized in the small, and not in the
// large, but are still preserved.  This verifies the AddElement() and
// Serialize() methods.
TEST_F(FeatureTest, ParseSerializeMisplaced) {
  const string kPlacemark(
      "<Placemark>"
      "<name>my name</name>"
      "<Snippet maxLines=\"3\"></Snippet>"
      "<Point/>"
      "<snippet>little snippy</snippet>"
      "<Metadata><the>old<way>you see</way></the>\n"
      "</Metadata>"
      "</Placemark>");

  ASSERT_EQ(kPlacemark, kmldom::SerializeRaw(kmldom::Parse(kPlacemark, NULL)));
}

TEST_F(FeatureTest, TestSerializeGx) {
  PlacemarkPtr placemark = KmlFactory::GetFactory()->CreatePlacemark();
  placemark->set_name("my name");
  placemark->set_visibility(false);
  RegionPtr region = KmlFactory::GetFactory()->CreateRegion();
  placemark->set_region(region);
  placemark->set_gx_balloonvisibility(true);
  const string kExpected(
      "<Placemark>"
      "<name>my name</name>"
      "<visibility>0</visibility>"
      "<Region/>"
      "<gx:balloonVisibility>1</gx:balloonVisibility>"
      "</Placemark>");

  ASSERT_EQ(kExpected, SerializeRaw(placemark));
}

TEST_F(FeatureTest, TestSerializeFeature) {
  // TODO: all other Feature children.
  // Note that this KML has its children in a non-XSD-valid ordering. This is
  // to test that the elements are serialized and re-ordered correctly.
  const string kFeature(
      "<Document>"
      "<phoneNumber>867-5309</phoneNumber>"
      "<name>Jenny</name>"
      "</Document>"
      );
  const string kExpected(
      "<Document>"
      "<name>Jenny</name>"
      "<phoneNumber>867-5309</phoneNumber>"
      "</Document>"
      );

  ASSERT_EQ(kExpected, SerializeRaw(kmldom::Parse(kFeature, NULL)));
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
