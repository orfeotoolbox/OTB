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

#include "kml/base/unit_test.h"
#include "kml/dom/element.h"
#include "kml/dom/kml_funcs.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml22.h"
#include "kml/dom/region.h"

namespace kmldom {

class LatLonAltBoxTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(LatLonAltBoxTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST(TestParseAltitudeMode);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    latlonaltbox_ = KmlFactory::GetFactory()->CreateLatLonAltBox();
  }

  // Called after each test.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();
  void TestParseAltitudeMode();

 private:
  LatLonAltBoxPtr latlonaltbox_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(LatLonAltBoxTest);

void LatLonAltBoxTest::TestType() {
  CPPUNIT_ASSERT_EQUAL(Type_LatLonAltBox, latlonaltbox_->Type());
  CPPUNIT_ASSERT(true == latlonaltbox_->IsA(Type_LatLonAltBox));
  CPPUNIT_ASSERT(true == latlonaltbox_->IsA(Type_AbstractLatLonBox));
  CPPUNIT_ASSERT(true == latlonaltbox_->IsA(Type_Object));
}

// Verify proper defaults:
void LatLonAltBoxTest::TestDefaults() {
  CPPUNIT_ASSERT(false == latlonaltbox_->has_minaltitude());
  CPPUNIT_ASSERT(0.0 == latlonaltbox_->get_minaltitude());
  CPPUNIT_ASSERT(false == latlonaltbox_->has_maxaltitude());
  CPPUNIT_ASSERT(0.0 == latlonaltbox_->get_maxaltitude());
  CPPUNIT_ASSERT(false == latlonaltbox_->has_altitudemode());
  CPPUNIT_ASSERT(ALTITUDEMODE_CLAMPTOGROUND ==
                 latlonaltbox_->get_altitudemode());
}

// Verify setting default makes has_xxx() true:
void LatLonAltBoxTest::TestSetToDefaultValues() {
  // Verify the latlonaltbox_->is in default state:
  CPPUNIT_ASSERT(false == latlonaltbox_->has_minaltitude());
  CPPUNIT_ASSERT(false == latlonaltbox_->has_maxaltitude());
  CPPUNIT_ASSERT(false == latlonaltbox_->has_altitudemode());
  latlonaltbox_->set_minaltitude(latlonaltbox_->get_minaltitude());
  latlonaltbox_->set_maxaltitude(latlonaltbox_->get_maxaltitude());
  latlonaltbox_->set_altitudemode(latlonaltbox_->get_altitudemode());
  CPPUNIT_ASSERT(true == latlonaltbox_->has_minaltitude());
  CPPUNIT_ASSERT(true == latlonaltbox_->has_maxaltitude());
  CPPUNIT_ASSERT(true == latlonaltbox_->has_altitudemode());
}

// Verify set, get, has, clear:
void LatLonAltBoxTest::TestSetGetHasClear() {
  // Non-default values:
  const double minaltitude = 10101.5678;
  const double maxaltitude = 54321.1234566;
  const int altitudemode = ALTITUDEMODE_ABSOLUTE;

  // Set all fields:
  latlonaltbox_->set_minaltitude(minaltitude);
  latlonaltbox_->set_maxaltitude(maxaltitude);
  latlonaltbox_->set_altitudemode(altitudemode);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(true == latlonaltbox_->has_minaltitude());
  CPPUNIT_ASSERT(minaltitude == latlonaltbox_->get_minaltitude());
  CPPUNIT_ASSERT(true == latlonaltbox_->has_maxaltitude());
  CPPUNIT_ASSERT(maxaltitude == latlonaltbox_->get_maxaltitude());
  CPPUNIT_ASSERT(true == latlonaltbox_->has_altitudemode());
  CPPUNIT_ASSERT(altitudemode == latlonaltbox_->get_altitudemode());

  // Clear all fields:
  latlonaltbox_->clear_minaltitude();
  latlonaltbox_->clear_maxaltitude();
  latlonaltbox_->clear_altitudemode();

  // Verify now in default state:
  TestDefaults();
}

// Verify parse behavior of <altitudeMode> in <LatLonAltBox>
void LatLonAltBoxTest::TestParseAltitudeMode() {
  std::string kLatLonAltBoxAbsolute =
    "<LatLonAltBox>"
    "<north>2.5</north>"
    "<south>1.25</south>"
    "<east>1.25</east>"
    "<west>0</west>"
    "<minAltitude>101.101</minAltitude>"
    "<maxAltitude>202.202</maxAltitude>"
    "<altitudeMode>absolute</altitudeMode>"
    "</LatLonAltBox>";
  std::string errors;
  ElementPtr root = Parse(kLatLonAltBoxAbsolute, &errors);
  CPPUNIT_ASSERT(root);
  CPPUNIT_ASSERT(errors.empty());
  const LatLonAltBoxPtr llab_absolute = AsLatLonAltBox(root);
  CPPUNIT_ASSERT(llab_absolute);

  // Verify the proper values in the object model:
  CPPUNIT_ASSERT(llab_absolute->has_north());
  CPPUNIT_ASSERT_EQUAL(2.5, llab_absolute->get_north());
  CPPUNIT_ASSERT(llab_absolute->has_south());
  CPPUNIT_ASSERT_EQUAL(1.25, llab_absolute->get_south());
  CPPUNIT_ASSERT(llab_absolute->has_east());
  CPPUNIT_ASSERT_EQUAL(1.25, llab_absolute->get_east());
  CPPUNIT_ASSERT(llab_absolute->has_west());
  CPPUNIT_ASSERT_EQUAL(0., llab_absolute->get_west());
  CPPUNIT_ASSERT(llab_absolute->has_minaltitude());
  CPPUNIT_ASSERT_EQUAL(101.101, llab_absolute->get_minaltitude());
  CPPUNIT_ASSERT(llab_absolute->has_maxaltitude());
  CPPUNIT_ASSERT_EQUAL(202.202, llab_absolute->get_maxaltitude());
  CPPUNIT_ASSERT(llab_absolute->has_altitudemode());
  CPPUNIT_ASSERT_EQUAL(static_cast<int>(ALTITUDEMODE_ABSOLUTE),
                       llab_absolute->get_altitudemode());

  std::string kLatLonAltBoxClampToGround =
    "<LatLonAltBox>"
    "<altitudeMode>clampToGround</altitudeMode>"
    "</LatLonAltBox>";
  root = Parse(kLatLonAltBoxClampToGround, &errors);
  CPPUNIT_ASSERT(root);
  CPPUNIT_ASSERT(errors.empty());
  const LatLonAltBoxPtr llab_clamptoground = AsLatLonAltBox(root);
  CPPUNIT_ASSERT(llab_clamptoground);
  CPPUNIT_ASSERT(false == llab_clamptoground->has_north());
  CPPUNIT_ASSERT(false == llab_clamptoground->has_south());
  CPPUNIT_ASSERT(false == llab_clamptoground->has_east());
  CPPUNIT_ASSERT(false == llab_clamptoground->has_west());
  CPPUNIT_ASSERT(false == llab_clamptoground->has_minaltitude());
  CPPUNIT_ASSERT(false == llab_clamptoground->has_maxaltitude());
  CPPUNIT_ASSERT(llab_clamptoground->has_altitudemode());
  CPPUNIT_ASSERT_EQUAL(static_cast<int>(ALTITUDEMODE_CLAMPTOGROUND),
                       llab_clamptoground->get_altitudemode());

  std::string kLatLonAltBoxRelativeToGround =
    "<LatLonAltBox>"
    "<altitudeMode>relativeToGround</altitudeMode>"
    "</LatLonAltBox>";
  root = Parse(kLatLonAltBoxRelativeToGround, &errors);
  CPPUNIT_ASSERT(root);
  CPPUNIT_ASSERT(errors.empty());
  const LatLonAltBoxPtr llab_relativetoground = AsLatLonAltBox(root);
  CPPUNIT_ASSERT(llab_relativetoground);
  CPPUNIT_ASSERT(llab_relativetoground->has_altitudemode());
  CPPUNIT_ASSERT(llab_relativetoground->has_altitudemode());
  CPPUNIT_ASSERT_EQUAL(static_cast<int>(ALTITUDEMODE_RELATIVETOGROUND),
                       llab_relativetoground->get_altitudemode());
}

class LodTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(LodTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    lod_ = KmlFactory::GetFactory()->CreateLod();
  }

  // Called after each test.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();

 private:
  LodPtr lod_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(LodTest);

void LodTest::TestType() {
  CPPUNIT_ASSERT_EQUAL(Type_Lod, lod_->Type());
  CPPUNIT_ASSERT(true == lod_->IsA(Type_Lod));
  CPPUNIT_ASSERT(true == lod_->IsA(Type_Object));
}

// Verify proper defaults:
void LodTest::TestDefaults() {
  CPPUNIT_ASSERT(false == lod_->has_minlodpixels());
  CPPUNIT_ASSERT(0.0 == lod_->get_minlodpixels());
  CPPUNIT_ASSERT(false == lod_->has_maxlodpixels());
  CPPUNIT_ASSERT(-1.0 == lod_->get_maxlodpixels());
  CPPUNIT_ASSERT(false == lod_->has_minfadeextent());
  CPPUNIT_ASSERT(0.0 == lod_->get_minfadeextent());
  CPPUNIT_ASSERT(false == lod_->has_maxfadeextent());
  CPPUNIT_ASSERT(0.0 == lod_->get_maxfadeextent());
}

// Verify setting default makes has_xxx() true:
void LodTest::TestSetToDefaultValues() {
  // Verify the lod is in default state:
  CPPUNIT_ASSERT(false == lod_->has_minlodpixels());
  CPPUNIT_ASSERT(false == lod_->has_maxlodpixels());
  CPPUNIT_ASSERT(false == lod_->has_minfadeextent());
  CPPUNIT_ASSERT(false == lod_->has_maxfadeextent());
  lod_->set_minlodpixels(lod_->get_minlodpixels());
  lod_->set_maxlodpixels(lod_->get_maxlodpixels());
  lod_->set_minfadeextent(lod_->get_minfadeextent());
  lod_->set_maxfadeextent(lod_->get_maxfadeextent());
  CPPUNIT_ASSERT(true == lod_->has_minlodpixels());
  CPPUNIT_ASSERT(true == lod_->has_maxlodpixels());
  CPPUNIT_ASSERT(true == lod_->has_minfadeextent());
  CPPUNIT_ASSERT(true == lod_->has_maxfadeextent());
}

// Verify set, get, has, clear:
void LodTest::TestSetGetHasClear() {
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
  CPPUNIT_ASSERT(true == lod_->has_minlodpixels());
  CPPUNIT_ASSERT(minlodpixels == lod_->get_minlodpixels());
  CPPUNIT_ASSERT(true == lod_->has_maxlodpixels());
  CPPUNIT_ASSERT(maxlodpixels == lod_->get_maxlodpixels());
  CPPUNIT_ASSERT(true == lod_->has_minfadeextent());
  CPPUNIT_ASSERT(minfadeextent == lod_->get_minfadeextent());
  CPPUNIT_ASSERT(true == lod_->has_maxfadeextent());
  CPPUNIT_ASSERT(maxfadeextent == lod_->get_maxfadeextent());

  // Clear all fields:
  lod_->clear_minlodpixels();
  lod_->clear_maxlodpixels();
  lod_->clear_minfadeextent();
  lod_->clear_maxfadeextent();

  // Verify now in default state:
  TestDefaults();
}

class RegionTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(RegionTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetParent);
  CPPUNIT_TEST(TestParse);
  CPPUNIT_TEST(TestSerialize);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestType();
  void TestDefaults();
  void TestSetParent();
  void TestParse();
  void TestSerialize();

 public:
  // Called before each test.
  void setUp() {
    region_ = KmlFactory::GetFactory()->CreateRegion();
  }

  // Called after each test.
  void tearDown() {
  }

 private:
  RegionPtr region_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(RegionTest);

void RegionTest::TestType() {
  CPPUNIT_ASSERT_EQUAL(Type_Region, region_->Type());
  CPPUNIT_ASSERT(true == region_->IsA(Type_Region));
  CPPUNIT_ASSERT(true == region_->IsA(Type_Object));
}

// Verify proper defaults:
void RegionTest::TestDefaults() {
  CPPUNIT_ASSERT(false == region_->has_latlonaltbox());
  CPPUNIT_ASSERT(false == region_->has_lod());
}

void RegionTest::TestParse() {
  std::string kRegion =
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
  std::string errors;
  ElementPtr root = Parse(kRegion, &errors);
  CPPUNIT_ASSERT(root);
  CPPUNIT_ASSERT(errors.empty());
  const RegionPtr region = AsRegion(root);
  CPPUNIT_ASSERT(region);
  CPPUNIT_ASSERT_EQUAL(std::string("region123"), region->get_id());
  CPPUNIT_ASSERT(region->has_latlonaltbox());
  CPPUNIT_ASSERT_EQUAL(101.101, region->get_latlonaltbox()->get_minaltitude());
  CPPUNIT_ASSERT_EQUAL(202.202, region->get_latlonaltbox()->get_maxaltitude());
  CPPUNIT_ASSERT_EQUAL(static_cast<int>(ALTITUDEMODE_ABSOLUTE),
                       region->get_latlonaltbox()->get_altitudemode());
  CPPUNIT_ASSERT(region->has_lod());
  CPPUNIT_ASSERT_EQUAL(128., region->get_lod()->get_minlodpixels());
}

// Verify that 2 Regions can't take the same Lod, LatLonAltBox.
// (This tests the internal set_parent() method.)
void RegionTest::TestSetParent() {
  KmlFactory* factory = KmlFactory::GetFactory();
  LodPtr lod = factory->CreateLod();
  LatLonAltBoxPtr latlonaltbox = factory->CreateLatLonAltBox();
  region_->set_lod(lod);
  region_->set_latlonaltbox(latlonaltbox);
  RegionPtr region2 = factory->CreateRegion();
  region2->set_lod(lod);
  region2->set_latlonaltbox(latlonaltbox);
  CPPUNIT_ASSERT(region_->has_lod());
  CPPUNIT_ASSERT(region_->has_latlonaltbox());
  // The 2nd Region should not have taken the children.
  CPPUNIT_ASSERT(false == region2->has_lod());
  CPPUNIT_ASSERT(false == region2->has_latlonaltbox());
  // Delete of region_ deletes the Lod and LatLonAltBox created here.
}

void RegionTest::TestSerialize() {
  std::string expecting_default ="<Region/>";
  CPPUNIT_ASSERT_EQUAL(expecting_default, SerializeRaw(region_));
  region_->set_lod(KmlFactory::GetFactory()->CreateLod());
  region_->set_latlonaltbox(KmlFactory::GetFactory()->CreateLatLonAltBox());
  region_->set_id("abc");
  std::string expecting_both_children =
    "<Region id=\"abc\">"
    "<LatLonAltBox/>"
    "<Lod/>"
    "</Region>";
  CPPUNIT_ASSERT_EQUAL(expecting_both_children, SerializeRaw(region_));
  region_->clear_id();  // Clears id attribute.
  region_->clear_lod();  // Deletes Lod.
  std::string expecting_llab =
    "<Region>"
    "<LatLonAltBox/>"
    "</Region>";
  CPPUNIT_ASSERT_EQUAL(expecting_llab, SerializeRaw(region_));
  // Delete of region_ deletes LatLonAltBox.
}

}  // end namespace kmldom

TEST_MAIN
