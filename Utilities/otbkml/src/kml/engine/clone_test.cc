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

// This file contains the unit tests for the Clone() function.

#include "kml/engine/clone.h"
#include "kml/dom.h"
#include "gtest/gtest.h"

using kmlbase::Vec3;
using kmldom::CoordinatesPtr;
using kmldom::ElementPtr;
using kmldom::FolderPtr;
using kmldom::GroundOverlayPtr;
using kmldom::IconStylePtr;
using kmldom::IconStyleIconPtr;
using kmldom::KmlFactory;
using kmldom::PlacemarkPtr;
using kmldom::PointPtr;
using kmldom::SnippetPtr;

namespace kmlengine {

class CloneTest : public testing::Test {
 protected:
  virtual void SetUp() {
    KmlFactory* factory = KmlFactory::GetFactory();
    coordinates_ = factory->CreateCoordinates();
    folder_ = factory->CreateFolder();
    groundoverlay_ = factory->CreateGroundOverlay();
    placemark_ = factory->CreatePlacemark();
    point_ = factory->CreatePoint();
    snippet_ = factory->CreateSnippet();
  }

  CoordinatesPtr coordinates_;
  FolderPtr folder_;
  GroundOverlayPtr groundoverlay_;
  PlacemarkPtr placemark_;
  PointPtr point_;
  SnippetPtr snippet_;
};

// Verify that a NULL element is handled properly.
TEST_F(CloneTest, TestNullClone) {
  ElementPtr clone = Clone(NULL);
  ASSERT_FALSE(clone);
}

// Verify that a empty complex element is cloned properly.
TEST_F(CloneTest, TestEmptyClone) {
  ElementPtr clone = Clone(placemark_);
  ASSERT_EQ(clone->Type(), placemark_->Type());
}

// Verify that a complex element with some fields clones properly.
TEST_F(CloneTest, TestCloneFields) {
  const string kName("clone my name");
  const string kId("clone-my-id");
  const bool kVisibility = false;

  // Set the fields.
  placemark_->set_name(kName);
  placemark_->set_id(kId);
  placemark_->set_visibility(kVisibility);

  // Clone it.
  ElementPtr clone = Clone(placemark_);

  // Verify all values were set in the clone.
  ASSERT_EQ(clone->Type(), placemark_->Type());
  PlacemarkPtr cloned_placemark = AsPlacemark(clone);
  ASSERT_TRUE(cloned_placemark->has_id());
  ASSERT_EQ(kId, cloned_placemark->get_id());
  ASSERT_TRUE(cloned_placemark->has_name());
  ASSERT_EQ(kName, cloned_placemark->get_name());
  ASSERT_TRUE(cloned_placemark->has_visibility());
  ASSERT_EQ(kVisibility, cloned_placemark->get_visibility());
}

// Verify that a complex element with both some fields and complex children
// clones properly.
TEST_F(CloneTest, TestCloneChildren) {
  const string kDescription("clone my description");
  const string kId("clone-my-id");
  const bool kOpen = false;

  // Set some fields.
  folder_->set_description(kDescription);
  folder_->set_id(kId);
  folder_->set_open(kOpen);
  folder_->add_feature(placemark_);

  // Clone it.
  ElementPtr clone = Clone(folder_);

  // Verify all values were set in the clone.
  ASSERT_EQ(clone->Type(), folder_->Type());
  FolderPtr cloned_folder = AsFolder(clone);
  ASSERT_TRUE(cloned_folder->has_id());
  ASSERT_EQ(kId, cloned_folder->get_id());
  ASSERT_FALSE(cloned_folder->has_name());
  ASSERT_TRUE(cloned_folder->has_description());
  ASSERT_EQ(kDescription, cloned_folder->get_description());
  ASSERT_FALSE(cloned_folder->has_visibility());
  ASSERT_TRUE(cloned_folder->has_open());
  ASSERT_EQ(kOpen, cloned_folder->get_open());
  ASSERT_EQ(static_cast<size_t>(1),
                       cloned_folder->get_feature_array_size());
  ASSERT_EQ(placemark_->Type(),
                       cloned_folder->get_feature_array_at(0)->Type());
}

// Verify that an element with an array of complex children and fields
// clones properly.
TEST_F(CloneTest, TestCloneArray) {
  // <Folder><Placemark><Point/><Placemark/><GroundOverlay/></Folder>
  placemark_->set_geometry(point_);
  folder_->add_feature(placemark_);
  folder_->add_feature(groundoverlay_);

  // Clone the Folder.
  ElementPtr clone = Clone(folder_);

  // Verify the contents of the Feature array child in the Folder.
  FolderPtr cloned_folder = AsFolder(clone);
  ASSERT_EQ(static_cast<size_t>(2),
                       cloned_folder->get_feature_array_size());
  ASSERT_EQ(placemark_->Type(),
                       cloned_folder->get_feature_array_at(0)->Type());
  ASSERT_EQ(groundoverlay_->Type(),
                       cloned_folder->get_feature_array_at(1)->Type());

  // Verify the Placemark has a Point Geometry.
  PlacemarkPtr cloned_placemark =
      AsPlacemark(cloned_folder->get_feature_array_at(0));
  ASSERT_TRUE(cloned_placemark->has_geometry());
  ASSERT_EQ(point_->Type(),
                       cloned_placemark->get_geometry()->Type());
}

// Verify that <Point>-style <coordinates> is cloned properly.
TEST_F(CloneTest, TestClonePointCoordinates) {
  // Make a point.
  const double kLat(38.38);
  const double kLon(-122.122);
  const double kAlt(42.42);
  coordinates_->add_latlngalt(kLat, kLon, kAlt);

  // Clone <coordinates>
  ElementPtr clone = Clone(coordinates_);

  // Verify that a new <coordinates> was created and has the expected content.
  CoordinatesPtr coordinates_clone = AsCoordinates(clone);
  ASSERT_TRUE(coordinates_clone);
  ASSERT_EQ(static_cast<size_t>(1),
            coordinates_clone->get_coordinates_array_size());
  Vec3 vec3 = coordinates_clone->get_coordinates_array_at(0);
  ASSERT_DOUBLE_EQ(kLat, vec3.get_latitude());
  ASSERT_DOUBLE_EQ(kLon, vec3.get_longitude());
  ASSERT_DOUBLE_EQ(kAlt, vec3.get_altitude());
}

// Verify that LineString/LinearRing-style <coordinates> is cloned properly.
TEST_F(CloneTest, TestCloneLineCoordinates) {
  // Create a <coordinates> as might be found in <LineString>/<LinearRing>.
  const size_t kNumPoints(1001);
  size_t i;
  for (i = 0; i < kNumPoints; ++i) {
    coordinates_->add_latlngalt(static_cast<double>(i % 90),
                                static_cast<double>(i % 180),
                                static_cast<double>(i));
  }

  // Clone the <coordinates>
  ElementPtr element = Clone(coordinates_);

  // Verify all the points came over okay.
  CoordinatesPtr cloned_coordinates = AsCoordinates(element);
  ASSERT_TRUE(cloned_coordinates);
  ASSERT_EQ(static_cast<size_t>(kNumPoints),
                       cloned_coordinates->get_coordinates_array_size());
  for (i = 0; i < kNumPoints; ++i) {
    Vec3 vec3 = cloned_coordinates->get_coordinates_array_at(i);
    ASSERT_EQ(static_cast<double>(i % 90), vec3.get_latitude());
    ASSERT_EQ(static_cast<double>(i % 180), vec3.get_longitude());
    ASSERT_EQ(static_cast<double>(i), vec3.get_altitude());
  }
}

// Verify that <Snippet> is cloned properly.
TEST_F(CloneTest, TestCloneSnippet) {
  // Clone an empty/default Snippet.
  ElementPtr element = Clone(snippet_);
  SnippetPtr cloned_snippet = AsSnippet(element);
  ASSERT_TRUE(cloned_snippet);
  ASSERT_EQ(snippet_->get_maxlines(),
                       cloned_snippet->get_maxlines());
  ASSERT_EQ(snippet_->get_text(), cloned_snippet->get_text());

  const string kText("some snippet text");
  snippet_->set_text(kText);
  cloned_snippet = AsSnippet(Clone(snippet_));
  ASSERT_EQ(kText, cloned_snippet->get_text());
}

// <IconStyle>'s <Icon> uses has_icon(), etc but is Type_IconStyleIcon.
TEST_F(CloneTest, TestCloneIconStyle) {
  IconStyleIconPtr icon = KmlFactory::GetFactory()->CreateIconStyleIcon();
  const string kImage("icon.png");
  icon->set_href(kImage);
  IconStylePtr iconstyle = KmlFactory::GetFactory()->CreateIconStyle();
  iconstyle->set_icon(icon);
  ASSERT_TRUE(iconstyle->has_icon());
  ASSERT_EQ(kmldom::Type_IconStyleIcon, iconstyle->get_icon()->Type());

  IconStylePtr clone = AsIconStyle(Clone(iconstyle));
  ASSERT_TRUE(clone);
  ASSERT_EQ(kmldom::Type_IconStyle, clone->Type());
  ASSERT_TRUE(clone->has_icon());
  ASSERT_TRUE(clone->get_icon()->has_href());
  ASSERT_EQ(kmldom::Type_IconStyleIcon, clone->get_icon()->Type());
  ASSERT_EQ(kImage, clone->get_icon()->get_href());
}

TEST_F(CloneTest, TestCloneWithMisplacedChild) {
  kmldom::IconPtr icon =
    kmldom::AsIcon(kmldom::Parse("<Icon><x>64</x></Icon>", NULL));
  ASSERT_TRUE(icon);
  ASSERT_EQ(static_cast<size_t>(1), icon->get_misplaced_elements_array_size());
  ASSERT_EQ(static_cast<size_t>(0), icon->get_unknown_elements_array_size());
  kmldom::IconPtr clone = kmldom::AsIcon(Clone(icon));
  ASSERT_TRUE(clone);
  ASSERT_EQ(static_cast<size_t>(1), clone->get_misplaced_elements_array_size());
  ASSERT_EQ(static_cast<size_t>(0), clone->get_unknown_elements_array_size());
  ASSERT_FALSE(kmldom::SerializePretty(clone).empty());
}

TEST_F(CloneTest, TestCloneWithFullyUnknownChild) {
  // This originally appeared as IconStyle Icon's child, but the bug is
  // manifested in cloning any element with a fully unknown child.
  kmldom::IconPtr icon =
      kmldom::AsIcon(kmldom::Parse("<Icon><w>64</w></Icon>", NULL));
  ASSERT_TRUE(icon);
  ASSERT_EQ(static_cast<size_t>(0), icon->get_misplaced_elements_array_size());
  ASSERT_EQ(static_cast<size_t>(1), icon->get_unknown_elements_array_size());
  kmldom::IconPtr clone = kmldom::AsIcon(Clone(icon));
  ASSERT_TRUE(clone);
  ASSERT_EQ(static_cast<size_t>(0), clone->get_misplaced_elements_array_size());
  ASSERT_EQ(static_cast<size_t>(1), clone->get_unknown_elements_array_size());
  ASSERT_FALSE(kmldom::SerializePretty(clone).empty());
}

}  // end namespace kmlengine

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
