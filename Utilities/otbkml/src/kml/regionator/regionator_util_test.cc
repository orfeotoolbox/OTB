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

// This file contains the unit tests for the functions in regionator_util.cc.

#include "kml/regionator/regionator_util.h"
#include "kml/engine.h"
#include "kml/convenience/convenience.h"
#include "gtest/gtest.h"

namespace kmlregionator {

// This class is the unit test fixture for the KmlHandler class.
class RegionatorUtilTest : public testing::Test {
 protected:
  virtual void SetUp() {
    factory_ = kmldom::KmlFactory::GetFactory();
  }

  kmldom::KmlFactory* factory_;
  void AssertEqualAbstractLatLonBox(const kmldom::AbstractLatLonBoxPtr& a,
                                    const kmldom::AbstractLatLonBoxPtr& b)
                                    const;
  void AssertEqualLatLonAltBox(const kmldom::LatLonAltBoxPtr& a,
                               const kmldom::LatLonAltBoxPtr& b,
                               bool alt_fields_only) const;
  void AssertEqualLod(const kmldom::LodPtr& a, const kmldom::LodPtr& b) const;
  void AssertEqualRegion(const kmldom::RegionPtr& a,
                         const kmldom::RegionPtr& b) const;
  void AssertEqualRegionValues(const kmldom::RegionPtr& a,
                               double north, double south,
                               double east, double west,
                               double minlodpixels, double maxlodpixels) const;
};

void RegionatorUtilTest::AssertEqualAbstractLatLonBox(
    const kmldom::AbstractLatLonBoxPtr& a,
    const kmldom::AbstractLatLonBoxPtr& b) const {
  // NOTE: id= and targetid= are not cloned.
  ASSERT_EQ(a->has_north(), b->has_north());
  ASSERT_EQ(a->has_south(), b->has_south());
  ASSERT_EQ(a->has_east(), b->has_east());
  ASSERT_EQ(a->has_west(), b->has_west());
  ASSERT_EQ(a->get_north(), b->get_north());
  ASSERT_EQ(a->get_south(), b->get_south());
  ASSERT_EQ(a->get_east(), b->get_east());
  ASSERT_EQ(a->get_west(), b->get_west());
}

void RegionatorUtilTest::AssertEqualLatLonAltBox(
    const kmldom::LatLonAltBoxPtr& a,
    const kmldom::LatLonAltBoxPtr& b,
    bool alt_fields_only) const {
  ASSERT_EQ(a->has_minaltitude(), b->has_minaltitude());
  ASSERT_EQ(a->has_maxaltitude(), b->has_maxaltitude());
  ASSERT_EQ(a->has_altitudemode(), b->has_altitudemode());
  ASSERT_EQ(a->get_minaltitude(), b->get_minaltitude());
  ASSERT_EQ(a->get_maxaltitude(), b->get_maxaltitude());
  ASSERT_EQ(a->get_altitudemode(), b->get_altitudemode());
  if (alt_fields_only) {
    return;
  }
  AssertEqualAbstractLatLonBox(a, b);
}

void RegionatorUtilTest::AssertEqualLod(const kmldom::LodPtr& a,
                                        const kmldom::LodPtr& b) const {
  // NOTE: id= and targetid= are not cloned.
  ASSERT_EQ(a->has_minlodpixels(), b->has_minlodpixels());
  ASSERT_EQ(a->has_maxlodpixels(), b->has_maxlodpixels());
  ASSERT_EQ(a->has_minfadeextent(), b->has_minfadeextent());
  ASSERT_EQ(a->has_maxfadeextent(), b->has_maxfadeextent());
  ASSERT_EQ(a->get_minlodpixels(), b->get_minlodpixels());
  ASSERT_EQ(a->get_maxlodpixels(), b->get_maxlodpixels());
  ASSERT_EQ(a->get_minfadeextent(), b->get_minfadeextent());
  ASSERT_EQ(a->get_maxfadeextent(), b->get_maxfadeextent());
}

void RegionatorUtilTest::AssertEqualRegion(const kmldom::RegionPtr& a,
                                           const kmldom::RegionPtr& b) const {
  // NOTE: id= and targetid= are not cloned.
  ASSERT_EQ(a->has_latlonaltbox(), b->has_latlonaltbox());
  ASSERT_EQ(a->has_lod(), b->has_lod());
  if (a->has_latlonaltbox()) {
    AssertEqualLatLonAltBox(a->get_latlonaltbox(), b->get_latlonaltbox(),
                            false);
  }
  if (a->has_lod()) {
    AssertEqualLod(a->get_lod(), b->get_lod());
  }
}

// This tests the CloneLatLonAltBox() function.
TEST_F(RegionatorUtilTest, TestCloneLatLonAltBox) {
  kmldom::LatLonAltBoxPtr llab = factory_->CreateLatLonAltBox();
  kmldom::LatLonAltBoxPtr clone = CloneLatLonAltBox(llab);
  // Verify cloning of a default LatLonAltBox.
  AssertEqualLatLonAltBox(clone, llab, false);

  // See a couple of fields and clone that and compare.
  llab->set_north(12.34);
  llab->set_south(-22.34);
  clone = CloneLatLonAltBox(llab);  // Assigment releases ref to previous.
  AssertEqualLatLonAltBox(clone, llab, false);

  // Set the rest of the fields and compare.
  llab->set_east(45.67);
  llab->set_west(-98.86);
  llab->set_minaltitude(10101.999);
  llab->set_maxaltitude(20202.77777);
  llab->set_altitudemode(kmldom::ALTITUDEMODE_ABSOLUTE);
  clone = CloneLatLonAltBox(llab);  // Assigment releases ref to previous.
  AssertEqualLatLonAltBox(clone, llab, false);
}

// This tests the CloneLod() function.
TEST_F(RegionatorUtilTest, TestCloneLod) {
  // Verify clone of an empty Lod.
  kmldom::LodPtr lod = factory_->CreateLod();
  kmldom::LodPtr clone = CloneLod(lod);
  AssertEqualLod(lod, clone);

  // Verify clone of a fully populated Lod.
  lod->set_minlodpixels(128);
  lod->set_maxlodpixels(1024);
  lod->set_minfadeextent(128);
  lod->set_maxfadeextent(129);
  clone = CloneLod(lod);  // Assignment releases previous clone.
  AssertEqualLod(lod, clone);
}

// This tests the CloneRegion() function.
TEST_F(RegionatorUtilTest, TestCloneRegion) {
  // Verify clone of an empty Region.
  kmldom::RegionPtr region = factory_->CreateRegion();
  kmldom::RegionPtr clone = CloneRegion(region);
  AssertEqualRegion(region, clone);

  // Verify clone of a Region with empty LatLonAltBox and Lod.
  region->set_latlonaltbox(factory_->CreateLatLonAltBox());
  region->set_lod(factory_->CreateLod());
  clone = CloneRegion(region);  // Assignment releases previous clone.
  AssertEqualRegion(region, clone);
}

// This tests the CreateChildRegion() function.
TEST_F(RegionatorUtilTest, TestCreateChildRegion) {
  kmldom::LatLonAltBoxPtr llab = factory_->CreateLatLonAltBox();
  const double north(38.98);
  const double south(-71.98);
  const double mid_lat = (north + south)/2.0;
  const double east(112.334);
  const double west(-172.1111);
  const double mid_lon = (east + west)/2.0;
  llab->set_north(north);
  llab->set_south(south);
  llab->set_east(east);
  llab->set_west(west);
  llab->set_minaltitude(100001);
  llab->set_altitudemode(kmldom::ALTITUDEMODE_RELATIVETOGROUND);
  kmldom::LodPtr lod = factory_->CreateLod();
  lod->set_minlodpixels(128);
  kmldom::RegionPtr parent = factory_->CreateRegion();
  parent->set_latlonaltbox(llab);
  parent->set_lod(lod);

  // Create the four child regions.
  kmldom::RegionPtr ne = CreateChildRegion(parent, NE);
  kmldom::RegionPtr nw = CreateChildRegion(parent, NW);
  kmldom::RegionPtr se = CreateChildRegion(parent, SE);
  kmldom::RegionPtr sw = CreateChildRegion(parent, SW);
  // Verify Lod cloned from parent.
  AssertEqualLod(ne->get_lod(), parent->get_lod());
  AssertEqualLod(nw->get_lod(), parent->get_lod());
  AssertEqualLod(se->get_lod(), parent->get_lod());
  AssertEqualLod(sw->get_lod(), parent->get_lod());
  // XXX Verify same alt stuff
  ASSERT_EQ(north, ne->get_latlonaltbox()->get_north());
  // Verify n,s,e,w is proper for each child.
  ASSERT_EQ(north, ne->get_latlonaltbox()->get_north());
  ASSERT_EQ(mid_lat, ne->get_latlonaltbox()->get_south());
  ASSERT_EQ(east, ne->get_latlonaltbox()->get_east());
  ASSERT_EQ(mid_lon, ne->get_latlonaltbox()->get_west());
  ASSERT_EQ(north, nw->get_latlonaltbox()->get_north());
  ASSERT_EQ(mid_lat, nw->get_latlonaltbox()->get_south());
  ASSERT_EQ(south, se->get_latlonaltbox()->get_south());
  ASSERT_EQ(mid_lat, se->get_latlonaltbox()->get_north());
  ASSERT_EQ(south, sw->get_latlonaltbox()->get_south());
  ASSERT_EQ(mid_lat, sw->get_latlonaltbox()->get_north());
}

// This tests the CreateLineStringBox() function.
TEST_F(RegionatorUtilTest, TestCreateLineStringBox) {
  const string name("my linestring box");
  kmldom::RegionPtr region = factory_->CreateRegion();
  kmldom::PlacemarkPtr placemark = CreateLineStringBox(name, region);
}

// This tests the CreateRegionNetworkLink() utility function.
TEST_F(RegionatorUtilTest, TestCreateRegionNetworkLink) {
  double north(.12121212);
  double south(.0454545454);
  double east(-.0878787);
  double west(-.9898981234);
  double minlodpixels(256);
  double maxlodpixels(-1);
  string href("child.kml");
  kmldom::NetworkLinkPtr networklink = CreateRegionNetworkLink(
      kmlconvenience::CreateRegion2d(north, south, east, west, minlodpixels,
                                     maxlodpixels),href);
  ASSERT_TRUE(networklink->has_region());
  const kmldom::RegionPtr& region= networklink->get_region();
  // TODO assert
  ASSERT_TRUE(networklink->has_link());
  const kmldom::LinkPtr& link = networklink->get_link();
  ASSERT_EQ(href, link->get_href());
  ASSERT_EQ(static_cast<int>(kmldom::VIEWREFRESHMODE_ONREGION),
            link->get_viewrefreshmode());
  ASSERT_EQ(north, region->get_latlonaltbox()->get_north());
}

// This tests the CreateRegionDocument() utility function.
TEST_F(RegionatorUtilTest, TestCreateRegionDocument) {
  double north(.12121212);
  double south(.0454545454);
  double east(-.0878787);
  double west(-.9898981234);
  double minlodpixels(256);
  double maxlodpixels(-1);
  kmldom::DocumentPtr document = CreateRegionDocument(
      kmlconvenience::CreateRegion2d(north, south, east, west, minlodpixels,
                                     maxlodpixels));
  // XXX assert region values
  ASSERT_EQ(north, document->get_region()->get_latlonaltbox()->get_north());
}

// This tests the GetCenter() function.
TEST_F(RegionatorUtilTest, TestGetCenter) {
  // NULL output pointer(s) should not crash.
  kmldom::LatLonBoxPtr llb = factory_->CreateLatLonBox();
  kmlengine::GetCenter(llb, NULL, NULL);
  double lat, lon;
  kmlengine::GetCenter(llb, &lat, NULL);
  // Missing lon pointer still saves a result for lat.
  ASSERT_EQ(0.0, lat);
  kmlengine::GetCenter(llb, NULL, &lon);
  // Missing lat pointer still saves a result for lon.
  ASSERT_EQ(0.0, lat);
  // A default LatLonBox is well defined thus so is its center.
  kmlengine::GetCenter(llb, &lat, &lon);
  ASSERT_EQ(0.0, lat);
  ASSERT_EQ(0.0, lon);
  // A default LatLonAltBox is well defined thus so is its center.
  kmldom::LatLonAltBoxPtr llab = factory_->CreateLatLonAltBox();
  kmlengine::GetCenter(llab, &lat, &lon);
  ASSERT_EQ(0.0, lat);
  ASSERT_EQ(0.0, lon);
}

TEST_F(RegionatorUtilTest, TestCreateAlignedAbstractLatLonBox) {
  kmldom::LatLonAltBoxPtr target = factory_->CreateLatLonAltBox();
  target->set_north(1);
  target->set_south(-1);
  target->set_east(1);
  target->set_west(-1);
  kmldom::LatLonAltBoxPtr aligned = factory_->CreateLatLonAltBox();
  ASSERT_TRUE(CreateAlignedAbstractLatLonBox(target, aligned));
  ASSERT_EQ(180, aligned->get_north());
  ASSERT_EQ(-180, aligned->get_south());
  ASSERT_EQ(180, aligned->get_east());
  ASSERT_EQ(-180, aligned->get_west());

  target->set_north(37.786807);  // Lincoln Park 3
  target->set_south(37.781563);  // Lincoln Park 7
  target->set_east(-122.494135);  // Lincoln Park 18
  target->set_west(-122.504031);  // Lincoln Park 5
  aligned = factory_->CreateLatLonAltBox();
  ASSERT_TRUE(CreateAlignedAbstractLatLonBox(target, aligned));
  ASSERT_EQ(37.79296875, aligned->get_north());
  ASSERT_EQ(37.7490234375, aligned->get_south());
  ASSERT_EQ(-122.4755859375, aligned->get_east());
  ASSERT_EQ(-122.51953125, aligned->get_west());
}

}  // end namespace kmlregionator

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
