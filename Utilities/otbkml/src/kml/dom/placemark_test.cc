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

// This file contains the unit tests for the Placemark element.

#include "kml/dom/placemark.h"
#include "kml/dom/kml_funcs.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_ptr.h"
#include "kml/dom/serializer.h"
#include "gtest/gtest.h"

using kmlbase::Vec3;

namespace kmldom {

class PlacemarkTest : public testing::Test {
 protected:
  virtual void SetUp() {
    placemark_ = KmlFactory::GetFactory()->CreatePlacemark();
  }

  PlacemarkPtr placemark_;
};

TEST_F(PlacemarkTest, TestType) {
  ASSERT_EQ(Type_Placemark, placemark_->Type());
  ASSERT_TRUE(placemark_->IsA(Type_Placemark));
  ASSERT_TRUE(placemark_->IsA(Type_Feature));
  ASSERT_TRUE(placemark_->IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(PlacemarkTest, TestDefaults) {
  ASSERT_FALSE(placemark_->has_geometry());
  ASSERT_TRUE(NULL == placemark_->get_geometry());
}

// Verify setting default makes has_xxx() true:
TEST_F(PlacemarkTest, TestSetToDefaultValues) {
  placemark_->set_geometry(NULL);  // should not crash
  ASSERT_FALSE(placemark_->has_geometry());  // ptr is null
}

// Verify set, get, has, clear:
TEST_F(PlacemarkTest, TestSetGetHasClear) {
  // Non-default values:
  PointPtr point = KmlFactory::GetFactory()->CreatePoint();

  // Set geometry to point:
  placemark_->set_geometry(point);

  // Verify getter and has_xxx():
  ASSERT_TRUE(placemark_->has_geometry());
  ASSERT_EQ(Type_Point, placemark_->get_geometry()->Type());

  // Set geometry to model.  This frees point.
  ModelPtr model= KmlFactory::GetFactory()->CreateModel();
  placemark_->set_geometry(model);

  // Verify getter and has_xxx():
  ASSERT_TRUE(placemark_->has_geometry());
  ASSERT_EQ(Type_Model, placemark_->get_geometry()->Type());

  // Clear all fields:
  placemark_->clear_geometry();
}

TEST_F(PlacemarkTest, TestParse) {
  string kName = "My Favorite Place";
  string kSnippet = "Left panel stuff about my favorite place...";
  string kPlacemark =
    "<Placemark>"
    "<name>" + kName + "</name>"
    "<Snippet>" + kSnippet + "</Snippet>"
    "<Point><coordinates>1.1,2.2,3.3</coordinates></Point>"
    "</Placemark>";
  string errors;
  ElementPtr root = Parse(kPlacemark, &errors);
  ASSERT_TRUE(root);
  ASSERT_TRUE(errors.empty());
  const PlacemarkPtr placemark = AsPlacemark(root);
  ASSERT_TRUE(placemark);
  ASSERT_FALSE(placemark->has_id());
  ASSERT_FALSE(placemark->has_targetid());
  ASSERT_TRUE(placemark->has_name());
  ASSERT_EQ(kName, placemark->get_name());
  ASSERT_FALSE(placemark->has_visibility());
  ASSERT_FALSE(placemark->has_open());
  ASSERT_FALSE(placemark->has_address());
  ASSERT_TRUE(placemark->has_snippet());
  ASSERT_TRUE(placemark->get_snippet()->has_text());
  ASSERT_EQ(kSnippet, placemark->get_snippet()->get_text());
  ASSERT_FALSE(placemark->has_description());
  ASSERT_FALSE(placemark->has_abstractview());
  ASSERT_FALSE(placemark->has_timeprimitive());
  ASSERT_FALSE(placemark->has_styleurl());
  ASSERT_FALSE(placemark->has_styleselector());
  ASSERT_FALSE(placemark->has_region());
  ASSERT_FALSE(placemark->has_extendeddata());
  const PointPtr point = AsPoint(placemark->get_geometry());
  Vec3 vec3 = point->get_coordinates()->get_coordinates_array_at(0);
  ASSERT_DOUBLE_EQ(1.1, vec3.get_longitude());
  ASSERT_DOUBLE_EQ(2.2, vec3.get_latitude());
  ASSERT_DOUBLE_EQ(3.3, vec3.get_altitude());
}

TEST_F(PlacemarkTest, TestSerialize) {
  placemark_->set_name("name");
  placemark_->set_visibility(true);
  placemark_->set_description("description");
  placemark_->set_styleurl("#styleurl");
  placemark_->set_region(KmlFactory::GetFactory()->CreateRegion());
  placemark_->set_geometry(KmlFactory::GetFactory()->CreatePoint());

  string expected(
    "<Placemark>"
    "<name>name</name>"
    "<visibility>1</visibility>"
    "<description>description</description>"
    "<styleUrl>#styleurl</styleUrl>"
    "<Region/>"
    "<Point/>"
    "</Placemark>"
  );
  ASSERT_EQ(expected, SerializeRaw(placemark_));
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
