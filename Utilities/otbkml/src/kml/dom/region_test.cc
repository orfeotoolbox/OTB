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

// This file contains the unit tests for LatLonAltBox, Lod, and Region.

#include "gtest/gtest.h"
#include "kml/dom/element.h"
#include "kml/dom/kml_funcs.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml22.h"
#include "kml/dom/region.h"

namespace kmldom {

class LatLonAltBoxTest : public testing::Test {
 protected:
  virtual void SetUp() {
    latlonaltbox_ = KmlFactory::GetFactory()->CreateLatLonAltBox();
  }

  LatLonAltBoxPtr latlonaltbox_;
};

TEST_F(LatLonAltBoxTest, TestType) {
  ASSERT_EQ(Type_LatLonAltBox, latlonaltbox_->Type());
  ASSERT_TRUE(latlonaltbox_->IsA(Type_LatLonAltBox));
  ASSERT_TRUE(latlonaltbox_->IsA(Type_AbstractLatLonBox));
  ASSERT_TRUE(latlonaltbox_->IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(LatLonAltBoxTest, TestDefaults) {
  ASSERT_FALSE(latlonaltbox_->has_minaltitude());
  ASSERT_DOUBLE_EQ(0.0, latlonaltbox_->get_minaltitude());
  ASSERT_FALSE(latlonaltbox_->has_maxaltitude());
  ASSERT_DOUBLE_EQ(0.0, latlonaltbox_->get_maxaltitude());
  ASSERT_FALSE(latlonaltbox_->has_altitudemode());
  ASSERT_TRUE(ALTITUDEMODE_CLAMPTOGROUND ==
              latlonaltbox_->get_altitudemode());
  ASSERT_FALSE(latlonaltbox_->has_gx_altitudemode());
  ASSERT_TRUE(GX_ALTITUDEMODE_CLAMPTOSEAFLOOR ==
              latlonaltbox_->get_gx_altitudemode());
}

// Verify setting default makes has_xxx() true:
TEST_F(LatLonAltBoxTest, TestSetToDefaultValues) {
  // Verify the latlonaltbox_->is in default state:
  ASSERT_FALSE(latlonaltbox_->has_minaltitude());
  ASSERT_FALSE(latlonaltbox_->has_maxaltitude());
  ASSERT_FALSE(latlonaltbox_->has_altitudemode());
  ASSERT_FALSE(latlonaltbox_->has_gx_altitudemode());
  latlonaltbox_->set_minaltitude(latlonaltbox_->get_minaltitude());
  latlonaltbox_->set_maxaltitude(latlonaltbox_->get_maxaltitude());
  latlonaltbox_->set_altitudemode(latlonaltbox_->get_altitudemode());
  latlonaltbox_->set_gx_altitudemode(latlonaltbox_->get_gx_altitudemode());
  ASSERT_TRUE(latlonaltbox_->has_minaltitude());
  ASSERT_TRUE(latlonaltbox_->has_maxaltitude());
  ASSERT_TRUE(latlonaltbox_->has_altitudemode());
  ASSERT_TRUE(latlonaltbox_->has_gx_altitudemode());
}

// Verify set, get, has, clear:
TEST_F(LatLonAltBoxTest, TestSetGetHasClear) {
  // Non-default values:
  const double minaltitude = 10101.5678;
  const double maxaltitude = 54321.1234566;
  const int altitudemode = ALTITUDEMODE_ABSOLUTE;
  const int gx_altitudemode = GX_ALTITUDEMODE_RELATIVETOSEAFLOOR;

  // Set all fields:
  latlonaltbox_->set_minaltitude(minaltitude);
  latlonaltbox_->set_maxaltitude(maxaltitude);
  latlonaltbox_->set_altitudemode(altitudemode);
  latlonaltbox_->set_gx_altitudemode(gx_altitudemode);

  // Verify getter and has_xxx():
  ASSERT_TRUE(latlonaltbox_->has_minaltitude());
  ASSERT_TRUE(minaltitude == latlonaltbox_->get_minaltitude());
  ASSERT_TRUE(latlonaltbox_->has_maxaltitude());
  ASSERT_TRUE(maxaltitude == latlonaltbox_->get_maxaltitude());
  ASSERT_TRUE(latlonaltbox_->has_altitudemode());
  ASSERT_TRUE(altitudemode == latlonaltbox_->get_altitudemode());
  ASSERT_TRUE(latlonaltbox_->has_gx_altitudemode());
  ASSERT_TRUE(gx_altitudemode == latlonaltbox_->get_gx_altitudemode());

  // Clear all fields:
  latlonaltbox_->clear_minaltitude();
  latlonaltbox_->clear_maxaltitude();
  latlonaltbox_->clear_altitudemode();
  latlonaltbox_->clear_gx_altitudemode();
}

// Verify parse behavior of <altitudeMode> in <LatLonAltBox>
TEST_F(LatLonAltBoxTest, TestParseAltitudeMode) {
  string kLatLonAltBoxAbsolute =
    "<LatLonAltBox>"
    "<north>2.5</north>"
    "<south>1.25</south>"
    "<east>1.25</east>"
    "<west>0</west>"
    "<minAltitude>101.101</minAltitude>"
    "<maxAltitude>202.202</maxAltitude>"
    "<altitudeMode>absolute</altitudeMode>"
    "</LatLonAltBox>";
  string errors;
  ElementPtr root = Parse(kLatLonAltBoxAbsolute, &errors);
  ASSERT_TRUE(root);
  ASSERT_TRUE(errors.empty());
  const LatLonAltBoxPtr llab_absolute = AsLatLonAltBox(root);
  ASSERT_TRUE(llab_absolute);

  // Verify the proper values in the object model:
  ASSERT_TRUE(llab_absolute->has_north());
  ASSERT_DOUBLE_EQ(2.5, llab_absolute->get_north());
  ASSERT_TRUE(llab_absolute->has_south());
  ASSERT_DOUBLE_EQ(1.25, llab_absolute->get_south());
  ASSERT_TRUE(llab_absolute->has_east());
  ASSERT_DOUBLE_EQ(1.25, llab_absolute->get_east());
  ASSERT_TRUE(llab_absolute->has_west());
  ASSERT_DOUBLE_EQ(0., llab_absolute->get_west());
  ASSERT_TRUE(llab_absolute->has_minaltitude());
  ASSERT_DOUBLE_EQ(101.101, llab_absolute->get_minaltitude());
  ASSERT_TRUE(llab_absolute->has_maxaltitude());
  ASSERT_DOUBLE_EQ(202.202, llab_absolute->get_maxaltitude());
  ASSERT_TRUE(llab_absolute->has_altitudemode());
  ASSERT_EQ(static_cast<int>(ALTITUDEMODE_ABSOLUTE),
            llab_absolute->get_altitudemode());

  string kLatLonAltBoxClampToGround =
    "<LatLonAltBox>"
    "<altitudeMode>clampToGround</altitudeMode>"
    "</LatLonAltBox>";
  root = Parse(kLatLonAltBoxClampToGround, &errors);
  ASSERT_TRUE(root);
  ASSERT_TRUE(errors.empty());
  const LatLonAltBoxPtr llab_clamptoground = AsLatLonAltBox(root);
  ASSERT_TRUE(llab_clamptoground);
  ASSERT_FALSE(llab_clamptoground->has_north());
  ASSERT_FALSE(llab_clamptoground->has_south());
  ASSERT_FALSE(llab_clamptoground->has_east());
  ASSERT_FALSE(llab_clamptoground->has_west());
  ASSERT_FALSE(llab_clamptoground->has_minaltitude());
  ASSERT_FALSE(llab_clamptoground->has_maxaltitude());
  ASSERT_TRUE(llab_clamptoground->has_altitudemode());
  ASSERT_FALSE(llab_clamptoground->has_gx_altitudemode());
  ASSERT_EQ(static_cast<int>(ALTITUDEMODE_CLAMPTOGROUND),
            llab_clamptoground->get_gx_altitudemode());

  string kLatLonAltBoxRelativeToGround =
    "<LatLonAltBox>"
    "<altitudeMode>relativeToGround</altitudeMode>"
    "</LatLonAltBox>";
  root = Parse(kLatLonAltBoxRelativeToGround, &errors);
  ASSERT_TRUE(root);
  ASSERT_TRUE(errors.empty());
  const LatLonAltBoxPtr llab_relativetoground = AsLatLonAltBox(root);
  ASSERT_TRUE(llab_relativetoground);
  ASSERT_TRUE(llab_relativetoground->has_altitudemode());
  ASSERT_FALSE(llab_relativetoground->has_gx_altitudemode());
  ASSERT_EQ(static_cast<int>(ALTITUDEMODE_RELATIVETOGROUND),
            llab_relativetoground->get_altitudemode());

  string kLatLonAltBoxRelativeToSeaFloor =
    "<LatLonAltBox>"
    "<gx:altitudeMode>relativeToSeaFloor</gx:altitudeMode>"
    "</LatLonAltBox>";
  root = Parse(kLatLonAltBoxRelativeToSeaFloor, &errors);
  ASSERT_TRUE(root);
  ASSERT_TRUE(errors.empty());
  const LatLonAltBoxPtr llab_relativetoseafloor = AsLatLonAltBox(root);
  ASSERT_TRUE(llab_relativetoseafloor);
  ASSERT_FALSE(llab_relativetoseafloor->has_north());
  ASSERT_FALSE(llab_relativetoseafloor->has_south());
  ASSERT_FALSE(llab_relativetoseafloor->has_east());
  ASSERT_FALSE(llab_relativetoseafloor->has_west());
  ASSERT_FALSE(llab_relativetoseafloor->has_minaltitude());
  ASSERT_FALSE(llab_relativetoseafloor->has_maxaltitude());
  ASSERT_FALSE(llab_relativetoseafloor->has_altitudemode());
  ASSERT_TRUE(llab_relativetoseafloor->has_gx_altitudemode());
  ASSERT_EQ(static_cast<int>(GX_ALTITUDEMODE_RELATIVETOSEAFLOOR),
            llab_relativetoseafloor->get_gx_altitudemode());
}

class LodTest : public testing::Test {
 protected:
  virtual void SetUp() {
    lod_ = KmlFactory::GetFactory()->CreateLod();
  }

  LodPtr lod_;
};

TEST_F(LodTest, TestType) {
  ASSERT_EQ(Type_Lod, lod_->Type());
  ASSERT_TRUE(lod_->IsA(Type_Lod));
  ASSERT_TRUE(lod_->IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(LodTest, TestDefaults) {
  ASSERT_FALSE(lod_->has_minlodpixels());
  ASSERT_DOUBLE_EQ(0.0, lod_->get_minlodpixels());
  ASSERT_FALSE(lod_->has_maxlodpixels());
  ASSERT_TRUE(-1.0 == lod_->get_maxlodpixels());
  ASSERT_FALSE(lod_->has_minfadeextent());
  ASSERT_DOUBLE_EQ(0.0, lod_->get_minfadeextent());
  ASSERT_FALSE(lod_->has_maxfadeextent());
  ASSERT_DOUBLE_EQ(0.0, lod_->get_maxfadeextent());
}

// Verify setting default makes has_xxx() true:
TEST_F(LodTest, TestSetToDefaultValues) {
  // Verify the lod is in default state:
  ASSERT_FALSE(lod_->has_minlodpixels());
  ASSERT_FALSE(lod_->has_maxlodpixels());
  ASSERT_FALSE(lod_->has_minfadeextent());
  ASSERT_FALSE(lod_->has_maxfadeextent());
  lod_->set_minlodpixels(lod_->get_minlodpixels());
  lod_->set_maxlodpixels(lod_->get_maxlodpixels());
  lod_->set_minfadeextent(lod_->get_minfadeextent());
  lod_->set_maxfadeextent(lod_->get_maxfadeextent());
  ASSERT_TRUE(lod_->has_minlodpixels());
  ASSERT_TRUE(lod_->has_maxlodpixels());
  ASSERT_TRUE(lod_->has_minfadeextent());
  ASSERT_TRUE(lod_->has_maxfadeextent());
}

// Verify set, get, has, clear:
TEST_F(LodTest, TestSetGetHasClear) {
  const double minlodpixels = 128;
  const double maxlodpixels = 1024;
  const double minfadeextent = 127;
  const double maxfadeextent = 129;

  // Set all fields:
  lod_->set_minlodpixels(minlodpixels);
  lod_->set_maxlodpixels(maxlodpixels);
  lod_->set_minfadeextent(minfadeextent);
  lod_->set_maxfadeextent(maxfadeextent);

  // Verify getter and has_xxx():
  ASSERT_TRUE(lod_->has_minlodpixels());
  ASSERT_DOUBLE_EQ(minlodpixels, lod_->get_minlodpixels());
  ASSERT_TRUE(lod_->has_maxlodpixels());
  ASSERT_DOUBLE_EQ(maxlodpixels, lod_->get_maxlodpixels());
  ASSERT_TRUE(lod_->has_minfadeextent());
  ASSERT_DOUBLE_EQ(minfadeextent, lod_->get_minfadeextent());
  ASSERT_TRUE(lod_->has_maxfadeextent());
  ASSERT_DOUBLE_EQ(maxfadeextent, lod_->get_maxfadeextent());

  // Clear all fields:
  lod_->clear_minlodpixels();
  lod_->clear_maxlodpixels();
  lod_->clear_minfadeextent();
  lod_->clear_maxfadeextent();
}

class RegionTest : public testing::Test {
 protected:
  virtual void SetUp() {
    region_ = KmlFactory::GetFactory()->CreateRegion();
  }

  RegionPtr region_;
};

TEST_F(RegionTest, TestType) {
  ASSERT_EQ(Type_Region, region_->Type());
  ASSERT_TRUE(region_->IsA(Type_Region));
  ASSERT_TRUE(region_->IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(RegionTest, TestDefaults) {
  ASSERT_FALSE(region_->has_latlonaltbox());
  ASSERT_FALSE(region_->has_lod());
}

TEST_F(RegionTest, TestParse) {
  string kRegion =
    "<Region id=\"region123\">"
    "<LatLonAltBox>"
    "<minAltitude>101.101</minAltitude>"
    "<maxAltitude>202.202</maxAltitude>"
    "<altitudeMode>absolute</altitudeMode>"
    "</LatLonAltBox>"
    "<Lod>"
    "<minLodPixels>128</minLodPixels>"
    "</Lod>"
    "</Region>";
  string errors;
  ElementPtr root = Parse(kRegion, &errors);
  ASSERT_TRUE(root);
  ASSERT_TRUE(errors.empty());
  const RegionPtr region = AsRegion(root);
  ASSERT_TRUE(region);
  ASSERT_EQ(string("region123"), region->get_id());
  ASSERT_TRUE(region->has_latlonaltbox());
  ASSERT_DOUBLE_EQ(101.101, region->get_latlonaltbox()->get_minaltitude());
  ASSERT_DOUBLE_EQ(202.202, region->get_latlonaltbox()->get_maxaltitude());
  ASSERT_EQ(static_cast<int>(ALTITUDEMODE_ABSOLUTE),
            region->get_latlonaltbox()->get_altitudemode());
  ASSERT_TRUE(region->has_lod());
  ASSERT_EQ(128, region->get_lod()->get_minlodpixels());
}

// Verify that 2 Regions can't take the same Lod, LatLonAltBox.
// (This tests the internal set_parent() method.)
TEST_F(RegionTest, TestSetParent) {
  KmlFactory* factory = KmlFactory::GetFactory();
  LodPtr lod = factory->CreateLod();
  LatLonAltBoxPtr latlonaltbox = factory->CreateLatLonAltBox();
  region_->set_lod(lod);
  region_->set_latlonaltbox(latlonaltbox);
  RegionPtr region2 = factory->CreateRegion();
  region2->set_lod(lod);
  region2->set_latlonaltbox(latlonaltbox);
  ASSERT_TRUE(region_->has_lod());
  ASSERT_TRUE(region_->has_latlonaltbox());
  // The 2nd Region should not have taken the children.
  ASSERT_FALSE(region2->has_lod());
  ASSERT_FALSE(region2->has_latlonaltbox());
  // Delete of region_ deletes the Lod and LatLonAltBox created here.
}

TEST_F(RegionTest, TestSerialize) {
  string expecting_default ="<Region/>";
  ASSERT_EQ(expecting_default, SerializeRaw(region_));
  region_->set_lod(KmlFactory::GetFactory()->CreateLod());
  region_->set_latlonaltbox(KmlFactory::GetFactory()->CreateLatLonAltBox());
  region_->set_id("abc");
  string expecting_both_children =
    "<Region id=\"abc\">"
    "<LatLonAltBox/>"
    "<Lod/>"
    "</Region>";
  ASSERT_EQ(expecting_both_children, SerializeRaw(region_));
  region_->clear_id();  // Clears id attribute.
  region_->clear_lod();  // Deletes Lod.
  string expecting_llab =
    "<Region>"
    "<LatLonAltBox/>"
    "</Region>";
  ASSERT_EQ(expecting_llab, SerializeRaw(region_));
  // Delete of region_ deletes LatLonAltBox.
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
