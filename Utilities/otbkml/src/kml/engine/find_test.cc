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

// This file contains the unit tests for the GetElementsById() function
// and the ElementFinder class.

#include "kml/engine/find.h"
#include <string>
#include "kml/dom/attributes.h"
#include "kml/dom/kml_funcs.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml22.h"
#include "kml/dom/kmldom.h"
#include "kml/base/unit_test.h"

using kmldom::CoordinatesPtr;
using kmldom::FolderPtr;
using kmldom::GroundOverlayPtr;
using kmldom::KmlFactory;
using kmldom::PlacemarkPtr;
using kmldom::PointPtr;

namespace kmlengine {

class ElementFinderTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(ElementFinderTest);
  CPPUNIT_TEST(TestNullGetElementsById);
  CPPUNIT_TEST(TestBasicGetElementsById);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestNullGetElementsById();
  void TestBasicGetElementsById();

 public:
  // Called before each test.
  void setUp() {
    coordinates_ = KmlFactory::GetFactory()->CreateCoordinates();
    folder0_ = KmlFactory::GetFactory()->CreateFolder();
    folder1_ = KmlFactory::GetFactory()->CreateFolder();
    groundoverlay_ = KmlFactory::GetFactory()->CreateGroundOverlay();
    placemark0_ = KmlFactory::GetFactory()->CreatePlacemark();
    placemark1_ = KmlFactory::GetFactory()->CreatePlacemark();
    placemark2_ = KmlFactory::GetFactory()->CreatePlacemark();
    point0_ = KmlFactory::GetFactory()->CreatePoint();
    point1_ = KmlFactory::GetFactory()->CreatePoint();
  }

  // Called after each test.
  void tearDown() {
    // Smart pointers free everything.
  }

 private:
  CoordinatesPtr coordinates_;
  FolderPtr folder0_;
  FolderPtr folder1_;
  GroundOverlayPtr groundoverlay_;
  PlacemarkPtr placemark0_;
  PlacemarkPtr placemark1_;
  PlacemarkPtr placemark2_;
  PointPtr point0_;
  PointPtr point1_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(ElementFinderTest);

// Verify that GetElementsById() is well behaved when
// presented with NULL pointers.
void ElementFinderTest::TestNullGetElementsById() {
  GetElementsById(NULL, kmldom::Type_Folder, NULL);
}

// Verify a normal usage of GetElementsById().
void ElementFinderTest::TestBasicGetElementsById() {
  // Put a variety of elements in a hierarchy.
  const std::string kId0("id0");
  const std::string kId1("id1");
  point0_->set_coordinates(coordinates_);
  placemark0_->set_geometry(point0_);
  placemark0_->set_id(kId0);
  placemark1_->set_id(kId1);
  placemark1_->set_geometry(point1_);
  folder0_->add_feature(placemark0_);
  folder1_->add_feature(placemark2_);
  folder0_->add_feature(folder1_);
  folder0_->add_feature(placemark1_);
  folder0_->add_feature(groundoverlay_);

  // Ask for all Placemarks.
  ElementVector placemarks;
  GetElementsById(folder0_, kmldom::Type_Placemark, &placemarks);

  // Verify we got just what we asked for and in depth-first order.
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(3), placemarks.size());
  CPPUNIT_ASSERT_EQUAL(kId0, AsPlacemark(placemarks[0])->get_id());
  CPPUNIT_ASSERT(AsPlacemark(placemarks[1]));
  CPPUNIT_ASSERT_EQUAL(kId1, AsPlacemark(placemarks[2])->get_id());

  // Ask for all the Points.
  ElementVector points;
  GetElementsById(folder0_, kmldom::Type_Point, &points);

  // Verify we got the 2 Points in depth-first order.
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(2), points.size());
  PointPtr point = AsPoint(points[0]);
  CPPUNIT_ASSERT(point->has_coordinates());
  CPPUNIT_ASSERT(AsPoint(points[1]));

  // Verify that no elements are found if there are none
  // of this type in the element hierarchy.
  ElementVector no_documents;
  GetElementsById(folder0_, kmldom::Type_Document, &no_documents);
  CPPUNIT_ASSERT(no_documents.empty());
}

}  // end namespace kmlengine

TEST_MAIN
