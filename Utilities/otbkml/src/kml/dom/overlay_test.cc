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

// This file contains the unit tests for Overlay, GroundOverlay, PhotoOverlay,
// ScreenOverlay and their children.

#include "kml/dom/overlay.h"
#include "kml/dom/kml22.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_funcs.h"
#include "kml/dom/kml_ptr.h"
#include "kml/base/unit_test.h"

namespace kmldom {

// This tests the Overlay class.
class OverlayTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(OverlayTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

  // Derive a test class since Overlay is abstract.
  class TestOverlay : public Overlay {
  };

 public:
  // Called before each test.
  void setUp() {
    overlay_ = new TestOverlay();
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
  OverlayPtr overlay_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(OverlayTest);

// This tests the Type() and IsA() methods.
void OverlayTest::TestType() {
  CPPUNIT_ASSERT(Type_Overlay == overlay_->Type());
  CPPUNIT_ASSERT(true == overlay_->IsA(Type_Overlay));
  CPPUNIT_ASSERT(true == overlay_->IsA(Type_Feature));
  CPPUNIT_ASSERT(true == overlay_->IsA(Type_Object));
}

// This tests the default values of all fields.
void OverlayTest::TestDefaults() {
  CPPUNIT_ASSERT(false == overlay_->has_color());
  CPPUNIT_ASSERT("ffffffff" == overlay_->get_color());
  CPPUNIT_ASSERT(false == overlay_->has_draworder());
  CPPUNIT_ASSERT(0 == overlay_->get_draworder());
  CPPUNIT_ASSERT(false == overlay_->has_icon());
  CPPUNIT_ASSERT(NULL == overlay_->get_icon());
}

// This tests that has_xxx() is true even if the value is set to the default.
void OverlayTest::TestSetToDefaultValues() {
  overlay_->set_color(overlay_->get_color());
  CPPUNIT_ASSERT(true == overlay_->has_color());
  CPPUNIT_ASSERT("ffffffff" == overlay_->get_color());
  overlay_->set_draworder(overlay_->get_draworder());
  CPPUNIT_ASSERT(true == overlay_->has_draworder());
  CPPUNIT_ASSERT(0 == overlay_->get_draworder());
  overlay_->set_icon(NULL);
  CPPUNIT_ASSERT(false == overlay_->has_icon());
  CPPUNIT_ASSERT(NULL == overlay_->get_icon());
}

// This tests the set_xxx(), xxx() (getter), has_xxx(), and clear_xxx() methods.
void OverlayTest::TestSetGetHasClear() {
  // Non-default values.
  const std::string kNonDefaultColor = "ff336699";
  const int kNonDefaultDrawOrder = -10000;
  IconPtr icon = KmlFactory::GetFactory()->CreateIcon();

  // Set all fields.
  overlay_->set_color(kNonDefaultColor);
  overlay_->set_draworder(kNonDefaultDrawOrder);
  overlay_->set_icon(icon);  // overlay_ now owns Icon*.

  // Verify getter and has_xxx().
  CPPUNIT_ASSERT(overlay_->has_color());
  CPPUNIT_ASSERT_EQUAL(kNonDefaultColor, overlay_->get_color());
  CPPUNIT_ASSERT(overlay_->has_draworder());
  CPPUNIT_ASSERT_EQUAL(kNonDefaultDrawOrder, overlay_->get_draworder());
  CPPUNIT_ASSERT(overlay_->has_icon());
  CPPUNIT_ASSERT(icon == overlay_->get_icon());

  // Clear all fields.
  overlay_->clear_color();
  overlay_->clear_draworder();
  overlay_->clear_icon();

  // Verify now in default state.
  TestDefaults();
}

// This tests the LatLonBox class.
class LatLonBoxTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(LatLonBoxTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    latlonbox_ = KmlFactory::GetFactory()->CreateLatLonBox();
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
  LatLonBoxPtr latlonbox_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(LatLonBoxTest);

// This tests the Type() and IsA() methods.
void LatLonBoxTest::TestType() {
  CPPUNIT_ASSERT(Type_LatLonBox == latlonbox_->Type());
  CPPUNIT_ASSERT(true == latlonbox_->IsA(Type_LatLonBox));
  CPPUNIT_ASSERT(true == latlonbox_->IsA(Type_Object));
}

// This tests the default values of all fields.
void LatLonBoxTest::TestDefaults() {
  CPPUNIT_ASSERT(false == latlonbox_->has_rotation());
  CPPUNIT_ASSERT(0.0 == latlonbox_->get_rotation());
}

// This tests that has_xxx() is true even if the value is set to the default.
void LatLonBoxTest::TestSetToDefaultValues() {
  latlonbox_->set_rotation(latlonbox_->get_rotation());
  CPPUNIT_ASSERT(true == latlonbox_->has_rotation());
  CPPUNIT_ASSERT(0.0 == latlonbox_->get_rotation());
}

// This tests the set_xxx(), xxx() (getter), has_xxx(), and clear_xxx() methods.
void LatLonBoxTest::TestSetGetHasClear() {
  // Non-default values.
  const double kNonDefaultRotation = 180.0;

  // Set all fields.
  latlonbox_->set_rotation(kNonDefaultRotation);

  // Verify getter and has_xxx().
  CPPUNIT_ASSERT(latlonbox_->has_rotation());
  CPPUNIT_ASSERT_EQUAL(kNonDefaultRotation, latlonbox_->get_rotation());

  // Clear all fields.
  latlonbox_->clear_rotation();

  // Verify now in default state.
  TestDefaults();
}

// This tests the GroundOverlay class.
class GroundOverlayTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(GroundOverlayTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    groundoverlay_ = KmlFactory::GetFactory()->CreateGroundOverlay();
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
  GroundOverlayPtr groundoverlay_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(GroundOverlayTest);

// This tests the Type() and IsA() methods.
void GroundOverlayTest::TestType() {
  CPPUNIT_ASSERT(Type_GroundOverlay == groundoverlay_->Type());
  CPPUNIT_ASSERT(true == groundoverlay_->IsA(Type_GroundOverlay));
  CPPUNIT_ASSERT(true == groundoverlay_->IsA(Type_Overlay));
  CPPUNIT_ASSERT(true == groundoverlay_->IsA(Type_Feature));
  CPPUNIT_ASSERT(true == groundoverlay_->IsA(Type_Object));
}

// This tests the default values of all fields.
void GroundOverlayTest::TestDefaults() {
  // Defaults inheried from Overlay.
  CPPUNIT_ASSERT(false == groundoverlay_->has_color());
  CPPUNIT_ASSERT("ffffffff" == groundoverlay_->get_color());
  CPPUNIT_ASSERT(false == groundoverlay_->has_draworder());
  CPPUNIT_ASSERT(0 == groundoverlay_->get_draworder());

  CPPUNIT_ASSERT(false == groundoverlay_->has_latlonbox());
  CPPUNIT_ASSERT(NULL == groundoverlay_->get_latlonbox());
}

// This tests that has_xxx() is true even if the value is set to the default.
void GroundOverlayTest::TestSetToDefaultValues() {
  groundoverlay_->set_latlonbox(NULL);
  CPPUNIT_ASSERT(false == groundoverlay_->has_latlonbox());
  CPPUNIT_ASSERT(NULL == groundoverlay_->get_latlonbox());
}

// This tests the set_xxx(), xxx() (getter), has_xxx(), and clear_xxx() methods.
void GroundOverlayTest::TestSetGetHasClear() {
  // Non-default values.
  const std::string kNonDefaultColor = "ff223311";
  const int kNonDefaultDrawOrder = -1234;
  LatLonBoxPtr latlonbox = KmlFactory::GetFactory()->CreateLatLonBox();

  // Set all fields.
  groundoverlay_->set_color(kNonDefaultColor);
  groundoverlay_->set_draworder(kNonDefaultDrawOrder);
  groundoverlay_->set_latlonbox(latlonbox);

  // Verify getter and has_xxx().
  CPPUNIT_ASSERT(groundoverlay_->has_color());
  CPPUNIT_ASSERT_EQUAL(kNonDefaultColor, groundoverlay_->get_color());
  CPPUNIT_ASSERT(groundoverlay_->has_draworder());
  CPPUNIT_ASSERT_EQUAL(kNonDefaultDrawOrder, groundoverlay_->get_draworder());
  CPPUNIT_ASSERT(groundoverlay_->has_latlonbox());
  CPPUNIT_ASSERT(latlonbox == groundoverlay_->get_latlonbox());

  // Clear all fields.
  groundoverlay_->clear_color();
  groundoverlay_->clear_draworder();
  groundoverlay_->clear_latlonbox();

  // Verify now in default state.
  TestDefaults();
}

// This tests the OverlayXY class.
class OverlayXYTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(OverlayXYTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestParse);
  CPPUNIT_TEST(TestSerialize);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    overlayxy_ = KmlFactory::GetFactory()->CreateOverlayXY();
  }

  // Called after each test.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestParse();
  void TestSerialize();

 private:
  OverlayXYPtr overlayxy_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(OverlayXYTest);

void OverlayXYTest::TestType() {
  CPPUNIT_ASSERT_EQUAL(Type_overlayXY, overlayxy_->Type());
  CPPUNIT_ASSERT(true == overlayxy_->IsA(Type_overlayXY));
  CPPUNIT_ASSERT(true == overlayxy_->IsA(Type_Vec2));
}

void OverlayXYTest::TestParse() {
  std::string errors;
  ElementPtr root = Parse(
    "<overlayXY x=\"0.5\" y=\"123\" xunits=\"fraction\" yunits=\"pixels\"/>",
    &errors);
  CPPUNIT_ASSERT(root);
  CPPUNIT_ASSERT(errors.empty());
  const OverlayXYPtr overlayxy = AsOverlayXY(root);
  CPPUNIT_ASSERT(overlayxy);
  CPPUNIT_ASSERT_EQUAL(0.5, overlayxy->get_x());
  CPPUNIT_ASSERT_EQUAL(123., overlayxy->get_y());
  CPPUNIT_ASSERT_EQUAL(static_cast<int>(UNITS_FRACTION),
                       overlayxy->get_xunits());
  CPPUNIT_ASSERT_EQUAL(static_cast<int>(UNITS_PIXELS),
                       overlayxy->get_yunits());
}

void OverlayXYTest::TestSerialize() {
  overlayxy_->set_x(.66);
  overlayxy_->set_y(256);
  overlayxy_->set_xunits(UNITS_FRACTION);
  overlayxy_->set_yunits(UNITS_PIXELS);
  std::string expected =
    "<overlayXY x=\"0.66\" xunits=\"fraction\" y=\"256\" yunits=\"pixels\"/>";
  CPPUNIT_ASSERT_EQUAL(expected, SerializeRaw(overlayxy_));
}

// This tests the ScreenXY class.
class ScreenXYTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(ScreenXYTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestParse);
  CPPUNIT_TEST(TestSerialize);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    screenxy_ = KmlFactory::GetFactory()->CreateScreenXY();
  }

  // Called after each test.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestParse();
  void TestSerialize();

 private:
  ScreenXYPtr screenxy_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(ScreenXYTest);

void ScreenXYTest::TestType() {
  CPPUNIT_ASSERT(screenxy_->Type() == Type_screenXY);
  CPPUNIT_ASSERT(true == screenxy_->IsA(Type_screenXY));
  CPPUNIT_ASSERT(true == screenxy_->IsA(Type_Vec2));
}

void ScreenXYTest::TestParse() {
  std::string errors;
  ElementPtr root = Parse(
    "<screenXY x=\"0.5\" y=\"123\" xunits=\"fraction\" "
      "yunits=\"insetPixels\"/>",
    &errors);
  CPPUNIT_ASSERT(root);
  CPPUNIT_ASSERT(errors.empty());
  const ScreenXYPtr screenxy = AsScreenXY(root);
  CPPUNIT_ASSERT(screenxy);
  CPPUNIT_ASSERT_EQUAL(0.5, screenxy->get_x());
  CPPUNIT_ASSERT_EQUAL(123., screenxy->get_y());
  CPPUNIT_ASSERT_EQUAL(static_cast<int>(UNITS_FRACTION),
                       screenxy->get_xunits());
  CPPUNIT_ASSERT_EQUAL(static_cast<int>(UNITS_INSETPIXELS),
                       screenxy->get_yunits());
}

void ScreenXYTest::TestSerialize() {
  screenxy_->set_x(.66);
  screenxy_->set_y(256);
  screenxy_->set_xunits(UNITS_FRACTION);
  screenxy_->set_yunits(UNITS_INSETPIXELS);
  std::string expected =
    "<screenXY x=\"0.66\" xunits=\"fraction\" y=\"256\" "
      "yunits=\"insetPixels\"/>";
  CPPUNIT_ASSERT_EQUAL(expected, SerializeRaw(screenxy_));
}

// This tests the RotationXY class.
class RotationXYTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(RotationXYTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestParse);
  CPPUNIT_TEST(TestSerialize);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    rotationxy_ = KmlFactory::GetFactory()->CreateRotationXY();
  }

  // Called after each test.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestParse();
  void TestSerialize();

 private:
  RotationXYPtr rotationxy_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(RotationXYTest);

void RotationXYTest::TestType() {
  CPPUNIT_ASSERT_EQUAL(rotationxy_->Type(), Type_rotationXY);
  CPPUNIT_ASSERT(true == rotationxy_->IsA(Type_rotationXY));
  CPPUNIT_ASSERT(true == rotationxy_->IsA(Type_Vec2));
}

void RotationXYTest::TestParse() {
  std::string errors;
  ElementPtr root = Parse(
    "<rotationXY x=\"512\" y=\"0.7\" xunits=\"pixels\" yunits=\"fraction\"/>",
    &errors);
  CPPUNIT_ASSERT(root);
  CPPUNIT_ASSERT(errors.empty());
  const RotationXYPtr rotationxy = AsRotationXY(root);
  CPPUNIT_ASSERT(rotationxy);
  CPPUNIT_ASSERT_EQUAL(512., rotationxy->get_x());
  CPPUNIT_ASSERT_EQUAL(0.7, rotationxy->get_y());
  CPPUNIT_ASSERT_EQUAL(static_cast<int>(UNITS_PIXELS),
                       rotationxy->get_xunits());
  CPPUNIT_ASSERT_EQUAL(static_cast<int>(UNITS_FRACTION),
                       rotationxy->get_yunits());
}

void RotationXYTest::TestSerialize() {
  rotationxy_->set_x(.66);
  rotationxy_->set_y(.77);
  rotationxy_->set_xunits(UNITS_FRACTION);
  rotationxy_->set_yunits(UNITS_FRACTION);
  std::string expected =
    "<rotationXY x=\"0.66\" xunits=\"fraction\" y=\"0.77\" "
    "yunits=\"fraction\"/>";
  CPPUNIT_ASSERT_EQUAL(expected, SerializeRaw(rotationxy_));
}

// This tests the Size class.
class SizeTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(SizeTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestParse);
  CPPUNIT_TEST(TestSerialize);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    size_ = KmlFactory::GetFactory()->CreateSize();
  }

  // Called after each test.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestParse();
  void TestSerialize();

 private:
  SizePtr size_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(SizeTest);

void SizeTest::TestType() {
  CPPUNIT_ASSERT(size_->Type() == Type_size);
  CPPUNIT_ASSERT(true == size_->IsA(Type_size));
  CPPUNIT_ASSERT(true == size_->IsA(Type_Vec2));
}

void SizeTest::TestParse() {
  std::string errors;
  ElementPtr root = Parse(
    "<size x=\"512\" y=\"0.7\" xunits=\"pixels\" yunits=\"fraction\"/>",
    &errors);
  CPPUNIT_ASSERT(root);
  CPPUNIT_ASSERT(errors.empty());
  const SizePtr size = AsSize(root);
  CPPUNIT_ASSERT(size);
  CPPUNIT_ASSERT_EQUAL(512., size->get_x());
  CPPUNIT_ASSERT_EQUAL(0.7, size->get_y());
  CPPUNIT_ASSERT_EQUAL(static_cast<int>(UNITS_PIXELS), size->get_xunits());
  CPPUNIT_ASSERT_EQUAL(static_cast<int>(UNITS_FRACTION), size->get_yunits());
}

void SizeTest::TestSerialize() {
  size_->set_x(66);
  size_->set_y(.5);
  size_->set_xunits(UNITS_PIXELS);
  size_->set_yunits(UNITS_FRACTION);
  std::string expected =
    "<size x=\"66\" xunits=\"pixels\" y=\"0.5\" yunits=\"fraction\"/>";
  CPPUNIT_ASSERT_EQUAL(expected, SerializeRaw(size_));
}

// This tests the ScreenOverlay class.
class ScreenOverlayTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(ScreenOverlayTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    screenoverlay_ = KmlFactory::GetFactory()->CreateScreenOverlay();
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
  ScreenOverlayPtr screenoverlay_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(ScreenOverlayTest);

// This tests the Type() and IsA() methods.
void ScreenOverlayTest::TestType() {
  CPPUNIT_ASSERT(Type_ScreenOverlay == screenoverlay_->Type());
  CPPUNIT_ASSERT(true == screenoverlay_->IsA(Type_ScreenOverlay));
  CPPUNIT_ASSERT(true == screenoverlay_->IsA(Type_Overlay));
  CPPUNIT_ASSERT(true == screenoverlay_->IsA(Type_Feature));
  CPPUNIT_ASSERT(true == screenoverlay_->IsA(Type_Object));
}

// This tests the default values of all fields.
void ScreenOverlayTest::TestDefaults() {
  CPPUNIT_ASSERT(false == screenoverlay_->has_overlayxy());
  CPPUNIT_ASSERT(NULL == screenoverlay_->get_overlayxy());
  CPPUNIT_ASSERT(false == screenoverlay_->has_screenxy());
  CPPUNIT_ASSERT(NULL == screenoverlay_->get_screenxy());
  CPPUNIT_ASSERT(false == screenoverlay_->has_rotationxy());
  CPPUNIT_ASSERT(NULL == screenoverlay_->get_rotationxy());
  CPPUNIT_ASSERT(false == screenoverlay_->has_size());
  CPPUNIT_ASSERT(NULL == screenoverlay_->get_size());
  CPPUNIT_ASSERT(false == screenoverlay_->has_rotation());
  CPPUNIT_ASSERT(0.0 == screenoverlay_->get_rotation());
}

// This tests that has_xxx() is true even if the value is set to the default.
void ScreenOverlayTest::TestSetToDefaultValues() {
  screenoverlay_->set_overlayxy(NULL);
  CPPUNIT_ASSERT(false == screenoverlay_->has_overlayxy());
  CPPUNIT_ASSERT(NULL == screenoverlay_->get_overlayxy());
  screenoverlay_->set_screenxy(NULL);
  CPPUNIT_ASSERT(false == screenoverlay_->has_screenxy());
  CPPUNIT_ASSERT(NULL == screenoverlay_->get_screenxy());
  screenoverlay_->set_rotationxy(NULL);
  CPPUNIT_ASSERT(false == screenoverlay_->has_rotationxy());
  CPPUNIT_ASSERT(NULL == screenoverlay_->get_rotationxy());
  screenoverlay_->set_size(NULL);
  CPPUNIT_ASSERT(false == screenoverlay_->has_size());
  CPPUNIT_ASSERT(NULL == screenoverlay_->get_size());
}

// This tests the set_xxx(), xxx() (getter), has_xxx(), and clear_xxx() methods.
void ScreenOverlayTest::TestSetGetHasClear() {
  // Create all possible complex children and give to the ScreenOverlay:
  screenoverlay_->set_overlayxy(KmlFactory::GetFactory()->CreateOverlayXY());
  screenoverlay_->set_screenxy(KmlFactory::GetFactory()->CreateScreenXY());
  screenoverlay_->set_rotationxy(KmlFactory::GetFactory()->CreateRotationXY());
  screenoverlay_->set_size(KmlFactory::GetFactory()->CreateSize());
  screenoverlay_->set_rotation(15.15);
  CPPUNIT_ASSERT(true == screenoverlay_->has_overlayxy());
  CPPUNIT_ASSERT(true == screenoverlay_->has_screenxy());
  CPPUNIT_ASSERT(true == screenoverlay_->has_rotationxy());
  CPPUNIT_ASSERT(true == screenoverlay_->has_size());
  CPPUNIT_ASSERT(true == screenoverlay_->has_rotation());

  // Clear all children:
  screenoverlay_->clear_overlayxy();
  screenoverlay_->clear_screenxy();
  screenoverlay_->clear_rotationxy();
  screenoverlay_->clear_size();
  screenoverlay_->clear_rotation();

  // Verify ScreenOverlay is now in default state:
  TestDefaults();
}

// This tests the ViewVolume class.
class ViewVolumeTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(ViewVolumeTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    viewvolume_ = KmlFactory::GetFactory()->CreateViewVolume();
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
  ViewVolumePtr viewvolume_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(ViewVolumeTest);

// This tests the Type() and IsA() methods.
void ViewVolumeTest::TestType() {
  CPPUNIT_ASSERT(Type_ViewVolume == viewvolume_->Type());
  CPPUNIT_ASSERT(true == viewvolume_->IsA(Type_ViewVolume));
  CPPUNIT_ASSERT(true == viewvolume_->IsA(Type_Object));
}

// This tests the default values of all fields.
void ViewVolumeTest::TestDefaults() {
  CPPUNIT_ASSERT(false == viewvolume_->has_leftfov());
  CPPUNIT_ASSERT(0.0 == viewvolume_->get_leftfov());
  CPPUNIT_ASSERT(false == viewvolume_->has_rightfov());
  CPPUNIT_ASSERT(0.0 == viewvolume_->get_rightfov());
  CPPUNIT_ASSERT(false == viewvolume_->has_bottomfov());
  CPPUNIT_ASSERT(0.0 == viewvolume_->get_bottomfov());
  CPPUNIT_ASSERT(false == viewvolume_->has_topfov());
  CPPUNIT_ASSERT(0.0 == viewvolume_->get_topfov());
  CPPUNIT_ASSERT(false == viewvolume_->has_near());
  CPPUNIT_ASSERT(0.0 == viewvolume_->get_near());
}

// This tests that has_xxx() is true even if the value is set to the default.
void ViewVolumeTest::TestSetToDefaultValues() {
  viewvolume_->set_leftfov(viewvolume_->get_leftfov());
  CPPUNIT_ASSERT(viewvolume_->has_leftfov());
  CPPUNIT_ASSERT(0.0 == viewvolume_->get_leftfov());
  viewvolume_->set_rightfov(viewvolume_->get_rightfov());
  CPPUNIT_ASSERT(viewvolume_->has_rightfov());
  CPPUNIT_ASSERT(0.0 == viewvolume_->get_rightfov());
  viewvolume_->set_bottomfov(viewvolume_->get_bottomfov());
  CPPUNIT_ASSERT(viewvolume_->has_bottomfov());
  CPPUNIT_ASSERT(0.0 == viewvolume_->get_bottomfov());
  viewvolume_->set_topfov(viewvolume_->get_topfov());
  CPPUNIT_ASSERT(viewvolume_->has_topfov());
  CPPUNIT_ASSERT(0.0 == viewvolume_->get_topfov());
  viewvolume_->set_near(viewvolume_->get_near());
  CPPUNIT_ASSERT(viewvolume_->has_near());
  CPPUNIT_ASSERT(0.0 == viewvolume_->get_near());
}

// This tests the set_xxx(), xxx() (getter), has_xxx(), and clear_xxx() methods.
void ViewVolumeTest::TestSetGetHasClear() {
  // Non-default values.
  const double kNonDefaultLeftFov = -40.0;
  const double kNonDefaultRightFov = 40.0;
  const double kNonDefaultBottomFov = -30.0;
  const double kNonDefaultTopFov = 30.0;
  const double kNonDefaultNear= 100.0;

  // Set all fields.
  viewvolume_->set_leftfov(kNonDefaultLeftFov);
  viewvolume_->set_rightfov(kNonDefaultRightFov);
  viewvolume_->set_bottomfov(kNonDefaultBottomFov);
  viewvolume_->set_topfov(kNonDefaultTopFov);
  viewvolume_->set_near(kNonDefaultNear);

  // Verify getter and has_xxx().
  CPPUNIT_ASSERT(viewvolume_->has_leftfov());
  CPPUNIT_ASSERT_EQUAL(kNonDefaultLeftFov, viewvolume_->get_leftfov());
  CPPUNIT_ASSERT(viewvolume_->has_rightfov());
  CPPUNIT_ASSERT_EQUAL(kNonDefaultRightFov, viewvolume_->get_rightfov());
  CPPUNIT_ASSERT(viewvolume_->has_bottomfov());
  CPPUNIT_ASSERT_EQUAL(kNonDefaultBottomFov, viewvolume_->get_bottomfov());
  CPPUNIT_ASSERT(viewvolume_->has_topfov());
  CPPUNIT_ASSERT_EQUAL(kNonDefaultTopFov, viewvolume_->get_topfov());
  CPPUNIT_ASSERT(viewvolume_->has_near());
  CPPUNIT_ASSERT_EQUAL(kNonDefaultNear, viewvolume_->get_near());

  // Clear all fields.
  viewvolume_->clear_leftfov();
  viewvolume_->clear_rightfov();
  viewvolume_->clear_bottomfov();
  viewvolume_->clear_topfov();
  viewvolume_->clear_near();

  // Verify now in default state.
  TestDefaults();
}

// This tests the ImagePyramid class.
class ImagePyramidTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(ImagePyramidTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    imagepyramid_ = KmlFactory::GetFactory()->CreateImagePyramid();
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
  ImagePyramidPtr imagepyramid_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(ImagePyramidTest);

// This tests the Type() and IsA() methods.
void ImagePyramidTest::TestType() {
  CPPUNIT_ASSERT(Type_ImagePyramid == imagepyramid_->Type());
  CPPUNIT_ASSERT(true == imagepyramid_->IsA(Type_ImagePyramid));
  CPPUNIT_ASSERT(true == imagepyramid_->IsA(Type_Object));
}

// This tests the default values of all fields.
void ImagePyramidTest::TestDefaults() {
  CPPUNIT_ASSERT(false == imagepyramid_->has_tilesize());
  CPPUNIT_ASSERT(256 == imagepyramid_->get_tilesize());
  CPPUNIT_ASSERT(false == imagepyramid_->has_maxwidth());
  CPPUNIT_ASSERT(0 == imagepyramid_->get_maxwidth());
  CPPUNIT_ASSERT(false == imagepyramid_->has_maxheight());
  CPPUNIT_ASSERT(0 == imagepyramid_->get_maxheight());
  CPPUNIT_ASSERT(false == imagepyramid_->has_gridorigin());
  CPPUNIT_ASSERT(GRIDORIGIN_LOWERLEFT == imagepyramid_->get_gridorigin());
}

// This tests that has_xxx() is true even if the value is set to the default.
void ImagePyramidTest::TestSetToDefaultValues() {
  imagepyramid_->set_tilesize(imagepyramid_->get_tilesize());
  CPPUNIT_ASSERT(imagepyramid_->has_tilesize());
  CPPUNIT_ASSERT_EQUAL(256, imagepyramid_->get_tilesize());
  imagepyramid_->set_maxheight(imagepyramid_->get_maxheight());
  CPPUNIT_ASSERT(imagepyramid_->has_maxheight());
  CPPUNIT_ASSERT_EQUAL(0, imagepyramid_->get_maxheight());
  imagepyramid_->set_maxwidth(imagepyramid_->get_maxwidth());
  CPPUNIT_ASSERT(imagepyramid_->has_maxwidth());
  CPPUNIT_ASSERT_EQUAL(0, imagepyramid_->get_maxwidth());
  imagepyramid_->set_gridorigin(imagepyramid_->get_gridorigin());
  CPPUNIT_ASSERT(imagepyramid_->has_gridorigin());
  CPPUNIT_ASSERT(GRIDORIGIN_LOWERLEFT == imagepyramid_->get_gridorigin());
}

// This tests the set_xxx(), xxx() (getter), has_xxx(), and clear_xxx() methods.
void ImagePyramidTest::TestSetGetHasClear() {
  const int kNonDefaultTilesize = 512;
  const int kNonDefaultMaxheight = 64;
  const int kNonDefaultMaxwidth = 128;
  GridOriginEnum gridorigin = GRIDORIGIN_UPPERLEFT;

  // Set all fields.
  imagepyramid_->set_tilesize(kNonDefaultTilesize);
  imagepyramid_->set_maxheight(kNonDefaultMaxheight);
  imagepyramid_->set_maxwidth(kNonDefaultMaxwidth);
  imagepyramid_->set_gridorigin(gridorigin);

  // Verify getter and has_xxx().
  CPPUNIT_ASSERT(imagepyramid_->has_tilesize());
  CPPUNIT_ASSERT_EQUAL(kNonDefaultTilesize, imagepyramid_->get_tilesize());
  CPPUNIT_ASSERT(imagepyramid_->has_maxheight());
  CPPUNIT_ASSERT_EQUAL(kNonDefaultMaxheight, imagepyramid_->get_maxheight());
  CPPUNIT_ASSERT(imagepyramid_->has_maxwidth());
  CPPUNIT_ASSERT_EQUAL(kNonDefaultMaxwidth, imagepyramid_->get_maxwidth());
  CPPUNIT_ASSERT(imagepyramid_->has_gridorigin());
  CPPUNIT_ASSERT(gridorigin == imagepyramid_->get_gridorigin());

  // Clear all fields.
  imagepyramid_->clear_tilesize();
  imagepyramid_->clear_maxheight();
  imagepyramid_->clear_maxwidth();
  imagepyramid_->clear_gridorigin();

  // Verify now in default state.
  TestDefaults();
}

// This tests the PhotoOverlay class.
class PhotoOverlayTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(PhotoOverlayTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    photooverlay_ = KmlFactory::GetFactory()->CreatePhotoOverlay();
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
  PhotoOverlayPtr photooverlay_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(PhotoOverlayTest);

// This tests the Type() and IsA() methods.
void PhotoOverlayTest::TestType() {
  CPPUNIT_ASSERT(Type_PhotoOverlay == photooverlay_->Type());
  CPPUNIT_ASSERT(true == photooverlay_->IsA(Type_PhotoOverlay));
  CPPUNIT_ASSERT(true == photooverlay_->IsA(Type_Overlay));
  CPPUNIT_ASSERT(true == photooverlay_->IsA(Type_Feature));
  CPPUNIT_ASSERT(true == photooverlay_->IsA(Type_Object));
}

// This tests the default values of all fields.
void PhotoOverlayTest::TestDefaults() {
  // rotation, ViewVolume, ImagePyramid, Point, shape
  CPPUNIT_ASSERT(false == photooverlay_->has_rotation());
  CPPUNIT_ASSERT(0.0 == photooverlay_->get_rotation());
  CPPUNIT_ASSERT(false == photooverlay_->has_viewvolume());
  CPPUNIT_ASSERT(NULL == photooverlay_->get_viewvolume());
  CPPUNIT_ASSERT(false == photooverlay_->has_imagepyramid());
  CPPUNIT_ASSERT(NULL == photooverlay_->get_imagepyramid());
  CPPUNIT_ASSERT(false == photooverlay_->has_point());
  CPPUNIT_ASSERT(NULL == photooverlay_->get_point());
  CPPUNIT_ASSERT(false == photooverlay_->has_shape());
  CPPUNIT_ASSERT(SHAPE_RECTANGLE == photooverlay_->get_shape());
}

// This tests that has_xxx() is true even if the value is set to the default.
void PhotoOverlayTest::TestSetToDefaultValues() {
  photooverlay_->set_rotation(photooverlay_->get_rotation());
  CPPUNIT_ASSERT(photooverlay_->has_rotation());
  CPPUNIT_ASSERT(0.0 == photooverlay_->get_rotation());
  photooverlay_->set_viewvolume(NULL);
  CPPUNIT_ASSERT(false == photooverlay_->has_viewvolume());
  CPPUNIT_ASSERT(NULL == photooverlay_->get_viewvolume());
  photooverlay_->set_imagepyramid(NULL);
  CPPUNIT_ASSERT(false == photooverlay_->has_imagepyramid());
  CPPUNIT_ASSERT(NULL == photooverlay_->get_imagepyramid());
  photooverlay_->set_point(NULL);
  CPPUNIT_ASSERT(false == photooverlay_->has_point());
  CPPUNIT_ASSERT(NULL == photooverlay_->get_point());
  photooverlay_->set_shape(photooverlay_->get_shape());
  CPPUNIT_ASSERT(photooverlay_->has_shape());
  CPPUNIT_ASSERT(SHAPE_RECTANGLE == photooverlay_->get_shape());
}

// This tests the set_xxx(), xxx() (getter), has_xxx(), and clear_xxx() methods.
void PhotoOverlayTest::TestSetGetHasClear() {
  // Non-default values.
  const double kNonDefaultRotation = 90.0;
  ViewVolumePtr viewvolume = KmlFactory::GetFactory()->CreateViewVolume();
  ImagePyramidPtr imagepyramid = KmlFactory::GetFactory()->CreateImagePyramid();
  PointPtr point = KmlFactory::GetFactory()->CreatePoint();
  ShapeEnum shape = SHAPE_SPHERE;

  // Set all fields.
  photooverlay_->set_rotation(kNonDefaultRotation);
  photooverlay_->set_viewvolume(viewvolume);
  photooverlay_->set_imagepyramid(imagepyramid);
  photooverlay_->set_point(point);
  photooverlay_->set_shape(shape);

  // Verify getter and has_xxx().
  CPPUNIT_ASSERT(photooverlay_->has_rotation());
  CPPUNIT_ASSERT_EQUAL(kNonDefaultRotation, photooverlay_->get_rotation());
  CPPUNIT_ASSERT(photooverlay_->has_viewvolume());
  CPPUNIT_ASSERT(viewvolume == photooverlay_->get_viewvolume());
  CPPUNIT_ASSERT(photooverlay_->has_imagepyramid());
  CPPUNIT_ASSERT(imagepyramid == photooverlay_->get_imagepyramid());
  CPPUNIT_ASSERT(photooverlay_->has_point());
  CPPUNIT_ASSERT(point == photooverlay_->get_point());
  CPPUNIT_ASSERT(photooverlay_->has_shape());
  CPPUNIT_ASSERT(shape == photooverlay_->get_shape());

  // Clear all fields.
  photooverlay_->clear_rotation();
  photooverlay_->clear_viewvolume();
  photooverlay_->clear_imagepyramid();
  photooverlay_->clear_point();
  photooverlay_->clear_shape();

  // Verify now in default state.
  TestDefaults();
}

}  // end namespace kmldom

TEST_MAIN
