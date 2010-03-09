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

// This file contains the unit tests for the FeatureList class.

#include "kml/convenience/feature_list.h"
#include "gtest/gtest.h"
#include "kml/dom.h"
#include "kml/convenience/convenience.h"
#include "kml/engine/bbox.h"
#include "kml/engine/location_util.h"

namespace kmlconvenience {

using kmldom::FolderPtr;
using kmldom::KmlFactory;
using kmldom::PlacemarkPtr;
using kmldom::RegionPtr;
using kmlengine::Bbox;

// Make these contants to avoid cluttering the code with static_cast<size_t>.
static const size_t kSize0 = 0;
static const size_t kSize1 = 1;

static const struct {
  double lat, lon;
} kPoints[] = {
  { 46.6864,9.38456 },
  { 46.0807,9.00158 },
  { 46.6994,9.43047 },
  { 46.0706,8.97776 },
  { 46.6613,9.1914 },
  { 46.0843,9.00969 },
  { 46.0884,9.02766 },
  { 46.0961,9.05026 },
  { 46.078,8.99814 },
  { 46.6922,9.40111 },
  { 46.0695,8.97107 },
  { 46.6065,10.4327 },
  { 46.6904,9.18267 },
  { 46.0736,8.98457 },
  { 46.6938,9.43975 },
  { 46.4831,8.7907 },
  { 46.0922,9.05428 },
  { 46.6982,9.40493 },
  { 46.7011,9.407 },
  { 46.6247,9.1863 },
  { 46.6914,9.3952 }
};

class FeatureListTest : public testing::Test {
 protected:
  virtual void SetUp() {
    CreateFeatureList();
  }

  void CreateFeatureList();
  int CountPointsInBbox(const Bbox& bbox) const;
  static RegionPtr CreateRegionFromBbox(const Bbox& bbox, double minlodpixels,
                                        double maxlodpixels);
  size_t initial_input_point_count_;
  Bbox initial_input_bbox_;
  FeatureList input_;
  FeatureList output_;
};

// This is an internal utility to count the number of points in the kPoints
// array contained by the given bounding box.
int FeatureListTest::CountPointsInBbox(const Bbox& bbox) const {
  int point_count = 0;
  for (size_t i = 0; i < initial_input_point_count_; ++i) {
    if (bbox.Contains(kPoints[i].lat, kPoints[i].lon)) {
      ++point_count;
    }
  }
  return point_count;
}

// This is an internal utility to create the input_ FeatureList from the
// kPoints array.
void FeatureListTest::CreateFeatureList() {
  initial_input_point_count_ = sizeof(kPoints)/sizeof(kPoints[0]);
  for (size_t i = 0; i < initial_input_point_count_; ++i) {
    input_.PushBack(kmlconvenience::CreatePointPlacemark("hi",
                                                         kPoints[i].lat,
                                                         kPoints[i].lon));
  }
  // This is an internal assertion to verify the FeatureList was created
  // properly.
  ASSERT_EQ(initial_input_point_count_, input_.Size());
  for (size_t i = 0; i < initial_input_point_count_; ++i) {
    initial_input_bbox_.ExpandLatLon(kPoints[i].lat, kPoints[i].lon);
  }
}

// This verifies that the two FeatureLists within the test fixture are in
// the expected state.
TEST_F(FeatureListTest, TestDefault) {
  ASSERT_EQ(initial_input_point_count_, input_.Size());
  ASSERT_EQ(kSize0, output_.Size());
}

// This verifies the Save method including preservation of order.
TEST_F(FeatureListTest, TestSave) {
  // Create a Folder and save out to it.
  FolderPtr folder = KmlFactory::GetFactory()->CreateFolder();
  input_.Save(folder);

  // Verify the state of the input_ FeatureList was not changed.
  ASSERT_EQ(initial_input_point_count_, input_.Size());

  // Verify everything was saved.
  ASSERT_EQ(initial_input_point_count_,
            folder->get_feature_array_size());
  for (size_t i = 0; i < initial_input_point_count_; ++i) {
    // Verify that these are all Placemarks
    PlacemarkPtr placemark =
        kmldom::AsPlacemark(folder->get_feature_array_at(i));
    ASSERT_TRUE(placemark);

    // Verify that each is a proper Point Placemark with lat and lon.
    double lat, lon;
    ASSERT_TRUE(kmlengine::GetPlacemarkLatLon(placemark, &lat, &lon));

    // Verify that the order from the input_ FeatureList is preserved.
    ASSERT_EQ(lat, kPoints[i].lat);
    ASSERT_EQ(lon, kPoints[i].lon);
  }
}

// This verifies that the BboxSplit and RegionSplit methods are well behaved
// when given a NULL output FeatureList.
TEST_F(FeatureListTest, TestNull) {
  Bbox bbox;
  ASSERT_EQ(kSize0, input_.BboxSplit(bbox, 0, NULL));
  RegionPtr region;
  ASSERT_EQ(kSize0, input_.RegionSplit(region, 0, NULL));
  input_.PushBack(NULL);
  ASSERT_EQ(initial_input_point_count_, input_.Size());
}

// This verifies the PushBack() method.
TEST_F(FeatureListTest, TestPushBack) {
  // Create a point and a bounding box that contains it.
  const double kLat = 12.34;
  const double kLon = -65.43;
  PlacemarkPtr placemark = kmlconvenience::CreatePointPlacemark("hi", kLat,
                                                                kLon);
  Bbox bbox;
  bbox.ExpandLatLon(kLat, kLon);

  // Remove any points already in this bounding box.  This makes the test
  // more robust in the face of any changes to kPoints.
  input_.BboxSplit(bbox, 0, NULL);
  // Verify no points remain within this bounding box.
  ASSERT_EQ(kSize0, input_.BboxSplit(bbox, 0, NULL));

  size_t previous_size = input_.Size();

  // Add the new point.
  input_.PushBack(kmlconvenience::CreatePointPlacemark("hi", kLat, kLon));
  ASSERT_EQ(previous_size + 1, input_.Size());

  // Split on the point's bounding box and verify exactly it was split out.
  ASSERT_EQ(kSize1, input_.BboxSplit(bbox, 0, &output_));

  // Verify the input is back to its previous size.
  ASSERT_EQ(previous_size, input_.Size());
  // Verify that splitting on this bbox again results in no output
  // and no change in size.
  ASSERT_EQ(kSize0, input_.BboxSplit(bbox, 0, &output_));
  ASSERT_EQ(previous_size, input_.Size());
}

// This is an internal utility to create a KML Region from a Bbox.
RegionPtr FeatureListTest::CreateRegionFromBbox(const Bbox& bbox,
                                                double minlodpixels,
                                                double maxlodpixels) {
  return kmlconvenience::CreateRegion2d(bbox.get_north(), bbox.get_south(),
                                        bbox.get_east(), bbox.get_west(),
                                        minlodpixels, maxlodpixels);
}

// This verifies that splitting on the bounding box for the FeatureList removes
// all features from the FeatureList.
TEST_F(FeatureListTest, TestRegionSplitAll) {
  ASSERT_EQ(static_cast<int>(initial_input_point_count_),
            CountPointsInBbox(initial_input_bbox_));
  RegionPtr region = CreateRegionFromBbox(initial_input_bbox_, 128, -1);
  input_.RegionSplit(region, 0, &output_);
  ASSERT_EQ(initial_input_point_count_, output_.Size());
  ASSERT_EQ(kSize0, input_.Size());
}

// This verifies that splitting a bounded number of items from the FeatureList
// behaves properly.
TEST_F(FeatureListTest, TestRegionSplitSome) {
  RegionPtr region = CreateRegionFromBbox(initial_input_bbox_, 128, -1);
  size_t split_count = initial_input_point_count_/4;
  ASSERT_TRUE(split_count);
  input_.RegionSplit(region, split_count, &output_);
  ASSERT_EQ(split_count, output_.Size());
  ASSERT_EQ(initial_input_point_count_ - split_count, input_.Size());
}

// This verifies that successive RegionSplit() calls on several Regions
// covering the extent of the FeatureList's bounding box splits out all
// proper features properly.
TEST_F(FeatureListTest, TestRegionSplitIncrementally) {
  double mid_lat, mid_lon;
  initial_input_bbox_.GetCenter(&mid_lat, &mid_lon);
  Bbox ne_bbox(initial_input_bbox_.get_north(), mid_lat,
               initial_input_bbox_.get_east(), mid_lon);
  size_t ne_point_count = CountPointsInBbox(ne_bbox);
  RegionPtr ne_region = CreateRegionFromBbox(ne_bbox, 128, -1);
  ASSERT_EQ(ne_point_count,
            input_.RegionSplit(ne_region, 0, &output_));
  ASSERT_EQ(ne_point_count, output_.Size());
  ASSERT_EQ(initial_input_point_count_ - ne_point_count,
            input_.Size());

  Bbox nw_bbox(initial_input_bbox_.get_north(), mid_lat,
               mid_lon, initial_input_bbox_.get_west());
  size_t nw_point_count = CountPointsInBbox(nw_bbox);
  RegionPtr nw_region = CreateRegionFromBbox(nw_bbox, 128, -1);
  ASSERT_EQ(nw_point_count,
            input_.RegionSplit(nw_region, 0, &output_));
  ASSERT_EQ(initial_input_point_count_ - ne_point_count - nw_point_count,
            input_.Size());

  Bbox south_bbox(mid_lat, initial_input_bbox_.get_south(),
                  initial_input_bbox_.get_east(),
                  initial_input_bbox_.get_west());
  size_t south_point_count = CountPointsInBbox(south_bbox);
  RegionPtr south_region = CreateRegionFromBbox(south_bbox, 128, -1);
  ASSERT_EQ(south_point_count,
            input_.RegionSplit(south_region, 0, &output_));
  ASSERT_EQ(kSize0, input_.Size());
  ASSERT_EQ(initial_input_point_count_, output_.Size());
}

}  // namespace kmlconvenience

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
