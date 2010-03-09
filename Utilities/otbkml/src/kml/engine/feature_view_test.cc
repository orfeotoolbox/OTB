// Copyright 2009, Google Inc. All rights reserved.
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

// This file contains the unit tests for the ComputeFeatureLookAt function.

#include "kml/engine/feature_view.h"
#include "gtest/gtest.h"
#include "kml/engine/kml_file.h"

using kmldom::CoordinatesPtr;
using kmldom::FolderPtr;
using kmldom::KmlFactory;
using kmldom::LineStringPtr;
using kmldom::LocationPtr;
using kmldom::LookAtPtr;
using kmldom::ModelPtr;
using kmldom::PhotoOverlayPtr;
using kmldom::PlacemarkPtr;
using kmldom::PointPtr;

namespace kmlengine {

TEST(FeatureViewTest, TestComputeFeatureLookAt) {
  KmlFactory* factory = KmlFactory::GetFactory();
  // Nothing happens when passed NULL.
  ASSERT_FALSE(ComputeFeatureLookAt(NULL));
  // Nothing happens when no abstract view can be computed.
  ASSERT_FALSE(ComputeFeatureLookAt(factory->CreateDocument()));
  ASSERT_FALSE(ComputeFeatureLookAt(factory->CreateFolder()));
  ASSERT_FALSE(ComputeFeatureLookAt(factory->CreatePlacemark()));
  ASSERT_FALSE(ComputeFeatureLookAt(factory->CreateNetworkLink()));
  ASSERT_FALSE(ComputeFeatureLookAt(factory->CreateGroundOverlay()));
  ASSERT_FALSE(ComputeFeatureLookAt(factory->CreateScreenOverlay()));
  ASSERT_FALSE(ComputeFeatureLookAt(factory->CreatePhotoOverlay()));
  ASSERT_FALSE(ComputeFeatureLookAt(factory->CreateGxTour()));

  PlacemarkPtr point_placemark = factory->CreatePlacemark();
  PointPtr point = factory->CreatePoint();
  CoordinatesPtr coordinates = factory->CreateCoordinates();
  const double kLat0 = 37.0;
  const double kLng0 = -122.0;
  coordinates->add_latlng(kLat0, kLng0);
  point->set_coordinates(coordinates);
  point_placemark->set_geometry(point);

  ASSERT_TRUE(point_placemark);
  const LookAtPtr point_lookat = ComputeFeatureLookAt(point_placemark);
  ASSERT_TRUE(point_lookat);
  ASSERT_DOUBLE_EQ(-122.0, point_lookat->get_longitude());
  ASSERT_DOUBLE_EQ(37.0, point_lookat->get_latitude());
  ASSERT_DOUBLE_EQ(0.0, point_lookat->get_altitude());
  ASSERT_DOUBLE_EQ(0.0, point_lookat->get_heading());
  ASSERT_DOUBLE_EQ(0.0, point_lookat->get_tilt());
  ASSERT_DOUBLE_EQ(1000.0, point_lookat->get_range());
  ASSERT_DOUBLE_EQ(kmldom::ALTITUDEMODE_RELATIVETOGROUND,
                   point_lookat->get_altitudemode());

  PlacemarkPtr line_placemark = factory->CreatePlacemark();
  LineStringPtr linestring = factory->CreateLineString();
  CoordinatesPtr line_coords = factory->CreateCoordinates();
  line_coords->add_latlng(kLat0, kLng0);
  const double kLat1 = 38.0;
  const double kLng1 = -121.0;
  line_coords->add_latlng(kLat1, kLng1);
  linestring->set_coordinates(line_coords);
  line_placemark->set_geometry(linestring);

  const LookAtPtr line_lookat = ComputeFeatureLookAt(line_placemark);
  ASSERT_TRUE(line_lookat);
  // We're looking at the center point of the line.
  ASSERT_DOUBLE_EQ(-121.5, line_lookat->get_longitude());
  ASSERT_DOUBLE_EQ(37.5, line_lookat->get_latitude());
  ASSERT_DOUBLE_EQ(0.0, line_lookat->get_altitude());
  ASSERT_DOUBLE_EQ(0.0, line_lookat->get_heading());
  ASSERT_DOUBLE_EQ(0.0, line_lookat->get_tilt());
  ASSERT_NEAR(135123.4361, line_lookat->get_range(), 0.0001);
  ASSERT_DOUBLE_EQ(kmldom::ALTITUDEMODE_RELATIVETOGROUND,
                   line_lookat->get_altitudemode());
}

TEST(FeatureViewTest, TestComputeFeatureLookAtFolder) {
  KmlFactory* factory = KmlFactory::GetFactory();
  FolderPtr folder = factory->CreateFolder();
  // No abstract view can be computed for this folder.
  ASSERT_FALSE(ComputeFeatureLookAt(folder));
  LocationPtr location = factory->CreateLocation();
  location->set_latitude(0);
  location->set_longitude(0);
  ModelPtr model = factory->CreateModel();
  model->set_location(location);
  PlacemarkPtr placemark = factory->CreatePlacemark();
  placemark->set_geometry(model);
  folder->add_feature(placemark);
  // Now an abstract view can be determined for the folder.
  LookAtPtr lookat = ComputeFeatureLookAt(folder);
  ASSERT_TRUE(lookat);
  ASSERT_DOUBLE_EQ(0.0, lookat->get_latitude());
  ASSERT_DOUBLE_EQ(0.0, lookat->get_longitude());
  // The range was clamped to 1000 meters.
  ASSERT_DOUBLE_EQ(1000.0, lookat->get_range());
  CoordinatesPtr coords = factory->CreateCoordinates();
  coords->add_latlng(10, 10);
  PointPtr point = factory->CreatePoint();
  point->set_coordinates(coords);
  PhotoOverlayPtr photooverlay = factory->CreatePhotoOverlay();
  photooverlay->set_point(point);
  folder->add_feature(photooverlay);
  // The folder's abstract view is now at the midpoint of 0,0 10,10 and
  // encompasses the view of both the model and the photo's point.
  lookat = ComputeFeatureLookAt(folder);
  ASSERT_DOUBLE_EQ(5.0, lookat->get_latitude());
  ASSERT_DOUBLE_EQ(5.0, lookat->get_longitude());
  // The range encompasses 0,0 to 10,10 for our field of view.
  ASSERT_NEAR(1494183.4444, lookat->get_range(), 0.0001);
}

}  // end namespace kmlengine

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
