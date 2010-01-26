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
#include <string>
#include "kml/dom.h"
#include "kml/base/unit_test.h"

using kmldom::CoordinatesPtr;
using kmldom::ElementPtr;
using kmldom::FolderPtr;
using kmldom::GroundOverlayPtr;
using kmldom::KmlFactory;
using kmldom::PlacemarkPtr;
using kmldom::PointPtr;
using kmldom::SnippetPtr;
using kmldom::Vec3;

namespace kmlengine {

class CloneTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(CloneTest);
  CPPUNIT_TEST(TestNullClone);
  CPPUNIT_TEST(TestEmptyClone);
  CPPUNIT_TEST(TestCloneFields);
  CPPUNIT_TEST(TestCloneChildren);
  CPPUNIT_TEST(TestCloneArray);
  CPPUNIT_TEST(TestClonePointCoordinates);
  CPPUNIT_TEST(TestCloneLineCoordinates);
  CPPUNIT_TEST(TestCloneSnippet);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestNullClone();
  void TestEmptyClone();
  void TestCloneFields();
  void TestCloneChildren();
  void TestCloneArray();
  void TestClonePointCoordinates();
  void TestCloneLineCoordinates();
  void TestCloneSnippet();

 public:
  // Called before each test.
  void setUp() {
    KmlFactory* factory = KmlFactory::GetFactory();
    coordinates_ = factory->CreateCoordinates();
    folder_ = factory->CreateFolder();
    groundoverlay_ = factory->CreateGroundOverlay();
    placemark_ = factory->CreatePlacemark();
    point_ = factory->CreatePoint();
    snippet_ = factory->CreateSnippet();
  }

  // Called after each test.
  void tearDown() {
    // Smart pointers free everything.
  }

 private:
  CoordinatesPtr coordinates_;
  FolderPtr folder_;
  GroundOverlayPtr groundoverlay_;
  PlacemarkPtr placemark_;
  PointPtr point_;
  SnippetPtr snippet_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(CloneTest);

// Verify that a NULL element is handled properly.
void CloneTest::TestNullClone() {
  ElementPtr clone = Clone(NULL);
  CPPUNIT_ASSERT(!clone);
}

// Verify that a empty complex element is cloned properly.
void CloneTest::TestEmptyClone() {
  ElementPtr clone = Clone(placemark_);
  CPPUNIT_ASSERT_EQUAL(clone->Type(), placemark_->Type());
}

// Verify that a complex element with some fields clones properly.
void CloneTest::TestCloneFields() {
  const std::string kName("clone my name");
  const std::string kId("clone-my-id");
  const bool kVisibility = false;

  // Set the fields.
  placemark_->set_name(kName);
  placemark_->set_id(kId);
  placemark_->set_visibility(kVisibility);

  // Clone it.
  ElementPtr clone = Clone(placemark_);

  // Verify all values were set in the clone.
  CPPUNIT_ASSERT_EQUAL(clone->Type(), placemark_->Type());
  PlacemarkPtr cloned_placemark = AsPlacemark(clone);
  CPPUNIT_ASSERT(cloned_placemark->has_id());
  CPPUNIT_ASSERT_EQUAL(kId, cloned_placemark->get_id());
  CPPUNIT_ASSERT(cloned_placemark->has_name());
  CPPUNIT_ASSERT_EQUAL(kName, cloned_placemark->get_name());
  CPPUNIT_ASSERT(cloned_placemark->has_visibility());
  CPPUNIT_ASSERT_EQUAL(kVisibility, cloned_placemark->get_visibility());
}

// Verify that a complex element with both some fields and complex children
// clones properly.
void CloneTest::TestCloneChildren() {
  const std::string kDescription("clone my description");
  const std::string kId("clone-my-id");
  const bool kOpen = false;

  // Set some fields.
  folder_->set_description(kDescription);
  folder_->set_id(kId);
  folder_->set_open(kOpen);
  folder_->add_feature(placemark_);

  // Clone it.
  ElementPtr clone = Clone(folder_);

  // Verify all values were set in the clone.
  CPPUNIT_ASSERT_EQUAL(clone->Type(), folder_->Type());
  FolderPtr cloned_folder = AsFolder(clone);
  CPPUNIT_ASSERT(cloned_folder->has_id());
  CPPUNIT_ASSERT_EQUAL(kId, cloned_folder->get_id());
  CPPUNIT_ASSERT(!cloned_folder->has_name());
  CPPUNIT_ASSERT(cloned_folder->has_description());
  CPPUNIT_ASSERT_EQUAL(kDescription, cloned_folder->get_description());
  CPPUNIT_ASSERT(!cloned_folder->has_visibility());
  CPPUNIT_ASSERT(cloned_folder->has_open());
  CPPUNIT_ASSERT_EQUAL(kOpen, cloned_folder->get_open());
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1),
                       cloned_folder->get_feature_array_size());
  CPPUNIT_ASSERT_EQUAL(placemark_->Type(),
                       cloned_folder->get_feature_array_at(0)->Type());
}

// Verify that an element with an array of complex children and fields
// clones properly.
void CloneTest::TestCloneArray() {
  // <Folder><Placemark><Point/><Placemark/><GroundOverlay/></Folder>
  placemark_->set_geometry(point_);
  folder_->add_feature(placemark_);
  folder_->add_feature(groundoverlay_);

  // Clone the Folder.
  ElementPtr clone = Clone(folder_);

  // Verify the contents of the Feature array child in the Folder.
  FolderPtr cloned_folder = AsFolder(clone);
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(2),
                       cloned_folder->get_feature_array_size());
  CPPUNIT_ASSERT_EQUAL(placemark_->Type(),
                       cloned_folder->get_feature_array_at(0)->Type());
  CPPUNIT_ASSERT_EQUAL(groundoverlay_->Type(),
                       cloned_folder->get_feature_array_at(1)->Type());

  // Verify the Placemark has a Point Geometry.
  PlacemarkPtr cloned_placemark =
      AsPlacemark(cloned_folder->get_feature_array_at(0));
  CPPUNIT_ASSERT(cloned_placemark->has_geometry());
  CPPUNIT_ASSERT_EQUAL(point_->Type(),
                       cloned_placemark->get_geometry()->Type());
}

// Verify that <Point>-style <coordinates> is cloned properly.
void CloneTest::TestClonePointCoordinates() {
  // Make a point.
  const double kLat(38.38);
  const double kLon(-122.122);
  const double kAlt(42.42);
  coordinates_->add_latlngalt(kLat, kLon, kAlt);

  // Clone <coordinates>
  ElementPtr clone = Clone(coordinates_);

  // Verify that a new <coordinates> was created and has the expected content.
  CoordinatesPtr coordinates_clone = AsCoordinates(clone);
  CPPUNIT_ASSERT(coordinates_clone);
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1),
                       coordinates_clone->get_coordinates_array_size());
  Vec3 vec3 = coordinates_clone->get_coordinates_array_at(0);
  CPPUNIT_ASSERT_EQUAL(kLat, vec3.get_latitude());
  CPPUNIT_ASSERT_EQUAL(kLon, vec3.get_longitude());
  CPPUNIT_ASSERT_EQUAL(kAlt, vec3.get_altitude());
}

// Verify that LineString/LinearRing-style <coordinates> is cloned properly.
void CloneTest::TestCloneLineCoordinates() {
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
  CPPUNIT_ASSERT(cloned_coordinates);
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(kNumPoints),
                       cloned_coordinates->get_coordinates_array_size());
  for (i = 0; i < kNumPoints; ++i) {
    Vec3 vec3 = cloned_coordinates->get_coordinates_array_at(i);
    CPPUNIT_ASSERT_EQUAL(static_cast<double>(i % 90), vec3.get_latitude());
    CPPUNIT_ASSERT_EQUAL(static_cast<double>(i % 180), vec3.get_longitude());
    CPPUNIT_ASSERT_EQUAL(static_cast<double>(i), vec3.get_altitude());
  }
}

// Verify that <Snippet> is cloned properly.
void CloneTest::TestCloneSnippet() {
  // Clone an empty/default Snippet.
  ElementPtr element = Clone(snippet_);
  SnippetPtr cloned_snippet = AsSnippet(element);
  CPPUNIT_ASSERT(cloned_snippet);
  CPPUNIT_ASSERT_EQUAL(snippet_->get_maxlines(),
                       cloned_snippet->get_maxlines());
  CPPUNIT_ASSERT_EQUAL(snippet_->get_text(), cloned_snippet->get_text());

  const std::string kText("some snippet text");
  snippet_->set_text(kText);
  cloned_snippet = AsSnippet(Clone(snippet_));
  CPPUNIT_ASSERT_EQUAL(kText, cloned_snippet->get_text());
}

}  // end namespace kmlengine

TEST_MAIN
