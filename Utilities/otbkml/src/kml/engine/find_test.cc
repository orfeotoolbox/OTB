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
#include "kml/dom/kml_funcs.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml22.h"
#include "kml/dom/kmldom.h"
#include "gtest/gtest.h"

using kmldom::CoordinatesPtr;
using kmldom::FolderPtr;
using kmldom::GroundOverlayPtr;
using kmldom::KmlFactory;
using kmldom::PlacemarkPtr;
using kmldom::PointPtr;

namespace kmlengine {

class ElementFinderTest : public testing::Test {
 protected:
  virtual void SetUp() {
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

// Verify that GetElementsById() is well behaved when
// presented with NULL pointers.
TEST_F(ElementFinderTest, TestNullGetElementsById) {
  GetElementsById(NULL, kmldom::Type_Folder, NULL);
}

// Verify a normal usage of GetElementsById().
TEST_F(ElementFinderTest, TestBasicGetElementsById) {
  // Put a variety of elements in a hierarchy.
  const string kId0("id0");
  const string kId1("id1");
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
  ASSERT_EQ(static_cast<size_t>(3), placemarks.size());
  ASSERT_EQ(kId0, AsPlacemark(placemarks[0])->get_id());
  ASSERT_TRUE(AsPlacemark(placemarks[1]));
  ASSERT_EQ(kId1, AsPlacemark(placemarks[2])->get_id());

  // Ask for all the Points.
  ElementVector points;
  GetElementsById(folder0_, kmldom::Type_Point, &points);

  // Verify we got the 2 Points in depth-first order.
  ASSERT_EQ(static_cast<size_t>(2), points.size());
  PointPtr point = AsPoint(points[0]);
  ASSERT_TRUE(point->has_coordinates());
  ASSERT_TRUE(AsPoint(points[1]));

  // Verify that no elements are found if there are none
  // of this type in the element hierarchy.
  ElementVector no_documents;
  GetElementsById(folder0_, kmldom::Type_Document, &no_documents);
  ASSERT_TRUE(no_documents.empty());

  // As for all Overlays.
  ElementVector overlays;
  GetElementsById(folder0_, kmldom::Type_Overlay, &overlays);
  ASSERT_EQ(static_cast<size_t>(1), overlays.size());
  GroundOverlayPtr groundoverlay = AsGroundOverlay(overlays[0]);

  // As for all Features.
  ElementVector features;
  GetElementsById(folder0_, kmldom::Type_Feature, &features);
  ASSERT_EQ(static_cast<size_t>(5), features.size());

  // As for all Objects.
  ElementVector objects;
  GetElementsById(folder0_, kmldom::Type_Object, &objects);
  ASSERT_EQ(static_cast<size_t>(7), objects.size());
}

// Verify a normal usage of GetChildElements().
TEST_F(ElementFinderTest, TestBasicGetChildElements) {
  // Put a variety of elements in a hierarchy.
  const string kId0("id0");
  const string kId1("id1");
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

  ElementVector all_elements;
  ASSERT_EQ(8, GetChildElements(folder0_, true, &all_elements));
  ASSERT_EQ(static_cast<size_t>(8), all_elements.size());
  ASSERT_EQ(8, GetChildElements(folder0_, true, NULL));

  ElementVector folder0_children;
  GetChildElements(folder0_, false, &folder0_children);
  ASSERT_EQ(static_cast<size_t>(4), folder0_children.size());
}

}  // end namespace kmlengine

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
