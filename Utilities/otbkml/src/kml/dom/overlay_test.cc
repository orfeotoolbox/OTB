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
#include "gtest/gtest.h"

using kmlbase::Color32;

namespace kmldom {

// This tests the Overlay class.
class OverlayTest : public testing::Test {
 protected:
  virtual void SetUp() {
    overlay_ = new TestOverlay();
  }

  // Derive a test class since Overlay is abstract.
  class TestOverlay : public Overlay {
  };

  OverlayPtr overlay_;
};

// This tests the Type() and IsA() methods.
TEST_F(OverlayTest, TestType) {
  ASSERT_EQ(Type_Overlay, 	overlay_->Type());
  ASSERT_TRUE(overlay_->IsA(Type_Overlay));
  ASSERT_TRUE(overlay_->IsA(Type_Feature));
  ASSERT_TRUE(overlay_->IsA(Type_Object));
}

// This tests the default values of all fields.
TEST_F(OverlayTest, TestDefaults) {
  ASSERT_FALSE(overlay_->has_color());
  ASSERT_TRUE(Color32(0xffffffff) == overlay_->get_color());
  ASSERT_FALSE(overlay_->has_draworder());
  ASSERT_EQ(0, overlay_->get_draworder());
  ASSERT_FALSE(overlay_->has_icon());
  ASSERT_TRUE(NULL == overlay_->get_icon());
}

// This tests that has_xxx() is true even if the value is set to the default.
TEST_F(OverlayTest, TestSetToDefaultValues) {
  overlay_->set_color(overlay_->get_color());
  ASSERT_TRUE(overlay_->has_color());
  ASSERT_TRUE(Color32(0xffffffff) == overlay_->get_color());
  overlay_->set_draworder(overlay_->get_draworder());
  ASSERT_TRUE(overlay_->has_draworder());
  ASSERT_EQ(0, overlay_->get_draworder());
  overlay_->set_icon(NULL);
  ASSERT_FALSE(overlay_->has_icon());
  ASSERT_TRUE(NULL == overlay_->get_icon());
}

// This tests the set_xxx(), xxx() (getter), has_xxx(), and clear_xxx() methods.
TEST_F(OverlayTest, TestSetGetHasClear) {
  // Non-default values.
  const Color32 kNonDefaultColor = Color32(0xff336699);
  const int kNonDefaultDrawOrder = -10000;
  IconPtr icon = KmlFactory::GetFactory()->CreateIcon();

  // Set all fields.
  overlay_->set_color(kNonDefaultColor);
  overlay_->set_draworder(kNonDefaultDrawOrder);
  overlay_->set_icon(icon);  // overlay_ now owns Icon*.

  // Verify getter and has_xxx().
  ASSERT_TRUE(overlay_->has_color());
  ASSERT_TRUE(kNonDefaultColor == overlay_->get_color());
  ASSERT_TRUE(overlay_->has_draworder());
  ASSERT_EQ(kNonDefaultDrawOrder, overlay_->get_draworder());
  ASSERT_TRUE(overlay_->has_icon());
  ASSERT_TRUE(icon == overlay_->get_icon());

  // Clear all fields.
  overlay_->clear_color();
  overlay_->clear_draworder();
  overlay_->clear_icon();

  // Verify now in default state.
}

// This tests the LatLonBox class.
class LatLonBoxTest : public testing::Test {
 protected:
  virtual void SetUp() {
    latlonbox_ = KmlFactory::GetFactory()->CreateLatLonBox();
  }

  LatLonBoxPtr latlonbox_;
};

// This tests the Type() and IsA() methods.
TEST_F(LatLonBoxTest, TestType) {
  ASSERT_EQ(Type_LatLonBox, latlonbox_->Type());
  ASSERT_TRUE(latlonbox_->IsA(Type_LatLonBox));
  ASSERT_TRUE(latlonbox_->IsA(Type_Object));
}

// This tests the default values of all fields.
TEST_F(LatLonBoxTest, TestDefaults) {
  ASSERT_FALSE(latlonbox_->has_rotation());
  ASSERT_DOUBLE_EQ(0.0, latlonbox_->get_rotation());
}

// This tests that has_xxx() is true even if the value is set to the default.
TEST_F(LatLonBoxTest, TestSetToDefaultValues) {
  latlonbox_->set_rotation(latlonbox_->get_rotation());
  ASSERT_TRUE(latlonbox_->has_rotation());
  ASSERT_DOUBLE_EQ(0.0, latlonbox_->get_rotation());
}

// This tests the set_xxx(), xxx() (getter), has_xxx(), and clear_xxx() methods.
TEST_F(LatLonBoxTest, TestSetGetHasClear) {
  // Non-default values.
  const double kNonDefaultRotation = 180.0;

  // Set all fields.
  latlonbox_->set_rotation(kNonDefaultRotation);

  // Verify getter and has_xxx().
  ASSERT_TRUE(latlonbox_->has_rotation());
  ASSERT_EQ(kNonDefaultRotation, latlonbox_->get_rotation());

  // Clear all fields.
  latlonbox_->clear_rotation();

  // Verify now in default state.
}

// This tests the LatLonQuad class.
class GxLatLonQuadTest : public testing::Test {
 protected:
  virtual void SetUp() {
    gx_latlonquad_ = KmlFactory::GetFactory()->CreateGxLatLonQuad();
  }

  GxLatLonQuadPtr gx_latlonquad_;
};

// This tests the Type() and IsA() methods.
TEST_F(GxLatLonQuadTest, TestType) {
  ASSERT_EQ(Type_GxLatLonQuad, gx_latlonquad_->Type());
  ASSERT_TRUE(gx_latlonquad_->IsA(Type_GxLatLonQuad));
  ASSERT_TRUE(gx_latlonquad_->IsA(Type_Object));
}

// This tests the default values of all fields.
TEST_F(GxLatLonQuadTest, TestDefaults) {
  ASSERT_FALSE(gx_latlonquad_->has_coordinates());
}

TEST_F(GxLatLonQuadTest, TestSetGetHasClear) {
  // TODO
}

TEST_F(GxLatLonQuadTest, TestParse) {
  // TODO
}

TEST_F(GxLatLonQuadTest, TestSerialize) {
  // TODO
}


// This tests the GroundOverlay class.
class GroundOverlayTest : public testing::Test {
 protected:
  virtual void SetUp() {
    groundoverlay_ = KmlFactory::GetFactory()->CreateGroundOverlay();
  }

  GroundOverlayPtr groundoverlay_;
};

// This tests the Type() and IsA() methods.
TEST_F(GroundOverlayTest, TestType) {
  ASSERT_EQ(Type_GroundOverlay, groundoverlay_->Type());
  ASSERT_TRUE(groundoverlay_->IsA(Type_GroundOverlay));
  ASSERT_TRUE(groundoverlay_->IsA(Type_Overlay));
  ASSERT_TRUE(groundoverlay_->IsA(Type_Feature));
  ASSERT_TRUE(groundoverlay_->IsA(Type_Object));
}

// This tests the default values of all fields.
TEST_F(GroundOverlayTest, TestDefaults) {
  // Defaults inheried from Overlay.
  ASSERT_FALSE(groundoverlay_->has_color());
  ASSERT_TRUE(Color32(0xffffffff) == groundoverlay_->get_color());
  ASSERT_FALSE(groundoverlay_->has_draworder());
  ASSERT_EQ(0, groundoverlay_->get_draworder());

  ASSERT_FALSE(groundoverlay_->has_altitude());
  ASSERT_EQ(0.0, groundoverlay_->get_altitude());
  ASSERT_FALSE(groundoverlay_->has_altitudemode());
  ASSERT_EQ(static_cast<int>(ALTITUDEMODE_CLAMPTOGROUND),
                       groundoverlay_->get_altitudemode());
  ASSERT_FALSE(groundoverlay_->has_gx_altitudemode());
  ASSERT_EQ(static_cast<int>(GX_ALTITUDEMODE_CLAMPTOSEAFLOOR),
                       groundoverlay_->get_gx_altitudemode());
  ASSERT_FALSE(groundoverlay_->has_latlonbox());
  ASSERT_TRUE(NULL == groundoverlay_->get_latlonbox());
  ASSERT_FALSE(groundoverlay_->has_gx_latlonquad());
  ASSERT_TRUE(NULL == groundoverlay_->get_gx_latlonquad());
}

// This tests that has_xxx() is true even if the value is set to the default.
TEST_F(GroundOverlayTest, TestSetToDefaultValues) {
  groundoverlay_->set_altitude(0.0);
  groundoverlay_->set_altitudemode(ALTITUDEMODE_CLAMPTOGROUND);
  groundoverlay_->set_gx_altitudemode(GX_ALTITUDEMODE_CLAMPTOSEAFLOOR);
  groundoverlay_->set_latlonbox(NULL);
  ASSERT_TRUE(groundoverlay_->has_altitude());
  ASSERT_TRUE(groundoverlay_->has_altitudemode());
  ASSERT_TRUE(groundoverlay_->has_gx_altitudemode());
  ASSERT_FALSE(groundoverlay_->has_latlonbox());
  ASSERT_TRUE(NULL == groundoverlay_->get_latlonbox());
  ASSERT_FALSE(groundoverlay_->has_gx_latlonquad());
  ASSERT_TRUE(NULL == groundoverlay_->get_gx_latlonquad());
}

// This tests the set_xxx(), xxx() (getter), has_xxx(), and clear_xxx() methods.
TEST_F(GroundOverlayTest, TestSetGetHasClear) {
  // Non-default values.
  const Color32 kNonDefaultColor = Color32(0xff223311);
  const int kNonDefaultDrawOrder = -1234;
  const double kAltitude = 314.56;
  const int kAltitudeMode = ALTITUDEMODE_ABSOLUTE;
  const int kGxAltitudeMode = GX_ALTITUDEMODE_RELATIVETOSEAFLOOR;
  LatLonBoxPtr latlonbox = KmlFactory::GetFactory()->CreateLatLonBox();
  GxLatLonQuadPtr gx_latlonquad =
      KmlFactory::GetFactory()->CreateGxLatLonQuad();

  // Set all fields.
  groundoverlay_->set_color(kNonDefaultColor);
  groundoverlay_->set_draworder(kNonDefaultDrawOrder);
  groundoverlay_->set_altitude(kAltitude);
  groundoverlay_->set_altitudemode(kAltitudeMode);
  groundoverlay_->set_gx_altitudemode(kGxAltitudeMode);
  groundoverlay_->set_latlonbox(latlonbox);
  groundoverlay_->set_gx_latlonquad(gx_latlonquad);

  // Verify getter and has_xxx().
  ASSERT_TRUE(groundoverlay_->has_altitude());
  ASSERT_EQ(kAltitude, groundoverlay_->get_altitude());
  ASSERT_TRUE(groundoverlay_->has_altitudemode());
  ASSERT_EQ(kAltitudeMode, groundoverlay_->get_altitudemode());
  ASSERT_TRUE(groundoverlay_->has_gx_altitudemode());
  ASSERT_EQ(kGxAltitudeMode, groundoverlay_->get_gx_altitudemode());
  ASSERT_TRUE(groundoverlay_->has_color());
  ASSERT_TRUE(kNonDefaultColor == groundoverlay_->get_color());
  ASSERT_TRUE(groundoverlay_->has_draworder());
  ASSERT_EQ(kNonDefaultDrawOrder, groundoverlay_->get_draworder());
  ASSERT_TRUE(groundoverlay_->has_latlonbox());
  ASSERT_TRUE(latlonbox == groundoverlay_->get_latlonbox());
  ASSERT_TRUE(groundoverlay_->has_gx_latlonquad());
  ASSERT_TRUE(gx_latlonquad == groundoverlay_->get_gx_latlonquad());

  // Clear all fields.
  groundoverlay_->clear_color();
  groundoverlay_->clear_draworder();
  groundoverlay_->clear_altitude();
  groundoverlay_->clear_altitudemode();
  groundoverlay_->clear_gx_altitudemode();
  groundoverlay_->clear_latlonbox();
  groundoverlay_->clear_gx_latlonquad();

  // Verify now in default state.
}

// This tests the Serialize method.
TEST_F(GroundOverlayTest, TestSerialize) {
  groundoverlay_->set_altitude(123.456);
  groundoverlay_->set_altitudemode(ALTITUDEMODE_ABSOLUTE);
  groundoverlay_->set_latlonbox(KmlFactory::GetFactory()->CreateLatLonBox()) ;

  const string expected =
      "<GroundOverlay>"
      "<altitude>123.456</altitude>"
      "<altitudeMode>absolute</altitudeMode>"
      "<LatLonBox/>"
      "</GroundOverlay>";
  ASSERT_EQ(expected, SerializeRaw(groundoverlay_));
}

TEST_F(GroundOverlayTest, TestSerializeGxAltitudeMode) {
  groundoverlay_->set_altitude(123.456);
  groundoverlay_->set_gx_altitudemode(GX_ALTITUDEMODE_RELATIVETOSEAFLOOR);
  groundoverlay_->set_latlonbox(KmlFactory::GetFactory()->CreateLatLonBox()) ;

  const string expected =
      "<GroundOverlay>"
      "<altitude>123.456</altitude>"
      "<gx:altitudeMode>relativeToSeaFloor</gx:altitudeMode>"
      "<LatLonBox/>"
      "</GroundOverlay>";
  ASSERT_EQ(expected, SerializeRaw(groundoverlay_));
}

// This tests the Serialize method on gx:LatLonQuad.
TEST_F(GroundOverlayTest, TestSerializeGxLatLonQuad) {
  groundoverlay_->set_altitude(123.456);
  groundoverlay_->set_altitudemode(ALTITUDEMODE_ABSOLUTE);
  groundoverlay_->set_gx_latlonquad(KmlFactory::GetFactory()->CreateGxLatLonQuad());

  const string expected =
      "<GroundOverlay>"
      "<altitude>123.456</altitude>"
      "<altitudeMode>absolute</altitudeMode>"
      "<gx:LatLonQuad/>"
      "</GroundOverlay>";
  ASSERT_EQ(expected, SerializeRaw(groundoverlay_));
}

// This tests the OverlayXY class.
class OverlayXYTest : public testing::Test {
 protected:
  virtual void SetUp() {
    overlayxy_ = KmlFactory::GetFactory()->CreateOverlayXY();
  }

  OverlayXYPtr overlayxy_;
};

TEST_F(OverlayXYTest, TestType) {
  ASSERT_EQ(Type_overlayXY, overlayxy_->Type());
  ASSERT_TRUE(overlayxy_->IsA(Type_overlayXY));
  ASSERT_TRUE(overlayxy_->IsA(Type_Vec2));
}

TEST_F(OverlayXYTest, TestParse) {
  string errors;
  ElementPtr root = Parse(
    "<overlayXY x=\"0.5\" y=\"123\" xunits=\"fraction\" yunits=\"pixels\"/>",
    &errors);
  ASSERT_TRUE(root);
  ASSERT_TRUE(errors.empty());
  const OverlayXYPtr overlayxy = AsOverlayXY(root);
  ASSERT_TRUE(overlayxy);
  ASSERT_EQ(0.5, overlayxy->get_x());
  ASSERT_EQ(123, overlayxy->get_y());
  ASSERT_EQ(static_cast<int>(UNITS_FRACTION), overlayxy->get_xunits());
  ASSERT_EQ(static_cast<int>(UNITS_PIXELS), overlayxy->get_yunits());
}

TEST_F(OverlayXYTest, TestSerialize) {
  overlayxy_->set_x(.66);
  overlayxy_->set_y(256);
  overlayxy_->set_xunits(UNITS_FRACTION);
  overlayxy_->set_yunits(UNITS_PIXELS);
  string expected =
    "<overlayXY x=\"0.66\" xunits=\"fraction\" y=\"256\" yunits=\"pixels\"/>";
  ASSERT_EQ(expected, SerializeRaw(overlayxy_));
}

// This tests the ScreenXY class.
class ScreenXYTest : public testing::Test {
 protected:
  virtual void SetUp() {
    screenxy_ = KmlFactory::GetFactory()->CreateScreenXY();
  }

  ScreenXYPtr screenxy_;
};

TEST_F(ScreenXYTest, TestType) {
  ASSERT_EQ(screenxy_->Type(), Type_screenXY);
  ASSERT_TRUE(screenxy_->IsA(Type_screenXY));
  ASSERT_TRUE(screenxy_->IsA(Type_Vec2));
}

TEST_F(ScreenXYTest, TestParse) {
  string errors;
  ElementPtr root = Parse(
    "<screenXY x=\"0.5\" y=\"123\" xunits=\"fraction\" "
      "yunits=\"insetPixels\"/>",
    &errors);
  ASSERT_TRUE(root);
  ASSERT_TRUE(errors.empty());
  const ScreenXYPtr screenxy = AsScreenXY(root);
  ASSERT_TRUE(screenxy);
  ASSERT_EQ(0.5, screenxy->get_x());
  ASSERT_EQ(123., screenxy->get_y());
  ASSERT_EQ(static_cast<int>(UNITS_FRACTION),
                       screenxy->get_xunits());
  ASSERT_EQ(static_cast<int>(UNITS_INSETPIXELS),
                       screenxy->get_yunits());
}

TEST_F(ScreenXYTest, TestSerialize) {
  screenxy_->set_x(.66);
  screenxy_->set_y(256);
  screenxy_->set_xunits(UNITS_FRACTION);
  screenxy_->set_yunits(UNITS_INSETPIXELS);
  string expected =
    "<screenXY x=\"0.66\" xunits=\"fraction\" y=\"256\" "
      "yunits=\"insetPixels\"/>";
  ASSERT_EQ(expected, SerializeRaw(screenxy_));
}

// This tests the RotationXY class.
class RotationXYTest : public testing::Test {
 protected:
  virtual void SetUp() {
    rotationxy_ = KmlFactory::GetFactory()->CreateRotationXY();
  }

  RotationXYPtr rotationxy_;
};

TEST_F(RotationXYTest, TestType) {
  ASSERT_EQ(rotationxy_->Type(), Type_rotationXY);
  ASSERT_TRUE(rotationxy_->IsA(Type_rotationXY));
  ASSERT_TRUE(rotationxy_->IsA(Type_Vec2));
}

TEST_F(RotationXYTest, TestParse) {
  string errors;
  ElementPtr root = Parse(
    "<rotationXY x=\"512\" y=\"0.7\" xunits=\"pixels\" yunits=\"fraction\"/>",
    &errors);
  ASSERT_TRUE(root);
  ASSERT_TRUE(errors.empty());
  const RotationXYPtr rotationxy = AsRotationXY(root);
  ASSERT_TRUE(rotationxy);
  ASSERT_EQ(512., rotationxy->get_x());
  ASSERT_EQ(0.7, rotationxy->get_y());
  ASSERT_EQ(static_cast<int>(UNITS_PIXELS),
                       rotationxy->get_xunits());
  ASSERT_EQ(static_cast<int>(UNITS_FRACTION),
                       rotationxy->get_yunits());
}

TEST_F(RotationXYTest, TestSerialize) {
  rotationxy_->set_x(.66);
  rotationxy_->set_y(.77);
  rotationxy_->set_xunits(UNITS_FRACTION);
  rotationxy_->set_yunits(UNITS_FRACTION);
  string expected =
    "<rotationXY x=\"0.66\" xunits=\"fraction\" y=\"0.77\" "
    "yunits=\"fraction\"/>";
  ASSERT_EQ(expected, SerializeRaw(rotationxy_));
}

// This tests the Size class.
class SizeTest : public testing::Test {
 protected:
  virtual void SetUp() {
    size_ = KmlFactory::GetFactory()->CreateSize();
  }

  SizePtr size_;
};

TEST_F(SizeTest, TestType) {
  ASSERT_EQ(size_->Type(), Type_size);
  ASSERT_TRUE(size_->IsA(Type_size));
  ASSERT_TRUE(size_->IsA(Type_Vec2));
}

TEST_F(SizeTest, TestParse) {
  string errors;
  ElementPtr root = Parse(
    "<size x=\"512\" y=\"0.7\" xunits=\"pixels\" yunits=\"fraction\"/>",
    &errors);
  ASSERT_TRUE(root);
  ASSERT_TRUE(errors.empty());
  const SizePtr size = AsSize(root);
  ASSERT_TRUE(size);
  ASSERT_EQ(512., size->get_x());
  ASSERT_EQ(0.7, size->get_y());
  ASSERT_EQ(static_cast<int>(UNITS_PIXELS), size->get_xunits());
  ASSERT_EQ(static_cast<int>(UNITS_FRACTION), size->get_yunits());
}

TEST_F(SizeTest, TestSerialize) {
  size_->set_x(66);
  size_->set_y(.5);
  size_->set_xunits(UNITS_PIXELS);
  size_->set_yunits(UNITS_FRACTION);
  string expected =
    "<size x=\"66\" xunits=\"pixels\" y=\"0.5\" yunits=\"fraction\"/>";
  ASSERT_EQ(expected, SerializeRaw(size_));
}

// This tests the ScreenOverlay class.
class ScreenOverlayTest : public testing::Test {
 protected:
  virtual void SetUp() {
    screenoverlay_ = KmlFactory::GetFactory()->CreateScreenOverlay();
  }

  ScreenOverlayPtr screenoverlay_;
};

// This tests the Type() and IsA() methods.
TEST_F(ScreenOverlayTest, TestType) {
  ASSERT_EQ(Type_ScreenOverlay, screenoverlay_->Type());
  ASSERT_TRUE(screenoverlay_->IsA(Type_ScreenOverlay));
  ASSERT_TRUE(screenoverlay_->IsA(Type_Overlay));
  ASSERT_TRUE(screenoverlay_->IsA(Type_Feature));
  ASSERT_TRUE(screenoverlay_->IsA(Type_Object));
}

// This tests the default values of all fields.
TEST_F(ScreenOverlayTest, TestDefaults) {
  ASSERT_FALSE(screenoverlay_->has_overlayxy());
  ASSERT_TRUE(NULL == screenoverlay_->get_overlayxy());
  ASSERT_FALSE(screenoverlay_->has_screenxy());
  ASSERT_TRUE(NULL == screenoverlay_->get_screenxy());
  ASSERT_FALSE(screenoverlay_->has_rotationxy());
  ASSERT_TRUE(NULL == screenoverlay_->get_rotationxy());
  ASSERT_FALSE(screenoverlay_->has_size());
  ASSERT_TRUE(NULL == screenoverlay_->get_size());
  ASSERT_FALSE(screenoverlay_->has_rotation());
  ASSERT_DOUBLE_EQ(0.0, screenoverlay_->get_rotation());
}

// This tests that has_xxx() is true even if the value is set to the default.
TEST_F(ScreenOverlayTest, TestSetToDefaultValues) {
  screenoverlay_->set_overlayxy(NULL);
  ASSERT_FALSE(screenoverlay_->has_overlayxy());
  ASSERT_TRUE(NULL == screenoverlay_->get_overlayxy());
  screenoverlay_->set_screenxy(NULL);
  ASSERT_FALSE(screenoverlay_->has_screenxy());
  ASSERT_TRUE(NULL == screenoverlay_->get_screenxy());
  screenoverlay_->set_rotationxy(NULL);
  ASSERT_FALSE(screenoverlay_->has_rotationxy());
  ASSERT_TRUE(NULL == screenoverlay_->get_rotationxy());
  screenoverlay_->set_size(NULL);
  ASSERT_FALSE(screenoverlay_->has_size());
  ASSERT_TRUE(NULL == screenoverlay_->get_size());
}

// This tests the set_xxx(), xxx() (getter), has_xxx(), and clear_xxx() methods.
TEST_F(ScreenOverlayTest, TestSetGetHasClear) {
  // Create all possible complex children and give to the ScreenOverlay:
  screenoverlay_->set_overlayxy(KmlFactory::GetFactory()->CreateOverlayXY());
  screenoverlay_->set_screenxy(KmlFactory::GetFactory()->CreateScreenXY());
  screenoverlay_->set_rotationxy(KmlFactory::GetFactory()->CreateRotationXY());
  screenoverlay_->set_size(KmlFactory::GetFactory()->CreateSize());
  screenoverlay_->set_rotation(15.15);
  ASSERT_TRUE(screenoverlay_->has_overlayxy());
  ASSERT_TRUE(screenoverlay_->has_screenxy());
  ASSERT_TRUE(screenoverlay_->has_rotationxy());
  ASSERT_TRUE(screenoverlay_->has_size());
  ASSERT_TRUE(screenoverlay_->has_rotation());

  // Clear all children:
  screenoverlay_->clear_overlayxy();
  screenoverlay_->clear_screenxy();
  screenoverlay_->clear_rotationxy();
  screenoverlay_->clear_size();
  screenoverlay_->clear_rotation();

  // Verify ScreenOverlay is now in default state:
}

// This tests the ViewVolume class.
class ViewVolumeTest : public testing::Test {
 protected:
  virtual void SetUp() {
    viewvolume_ = KmlFactory::GetFactory()->CreateViewVolume();
  }

  ViewVolumePtr viewvolume_;
};

// This tests the Type() and IsA() methods.
TEST_F(ViewVolumeTest, TestType) {
  ASSERT_EQ(Type_ViewVolume, viewvolume_->Type());
  ASSERT_TRUE(viewvolume_->IsA(Type_ViewVolume));
  ASSERT_TRUE(viewvolume_->IsA(Type_Object));
}

// This tests the default values of all fields.
TEST_F(ViewVolumeTest, TestDefaults) {
  ASSERT_FALSE(viewvolume_->has_leftfov());
  ASSERT_DOUBLE_EQ(0.0, viewvolume_->get_leftfov());
  ASSERT_FALSE(viewvolume_->has_rightfov());
  ASSERT_DOUBLE_EQ(0.0, viewvolume_->get_rightfov());
  ASSERT_FALSE(viewvolume_->has_bottomfov());
  ASSERT_DOUBLE_EQ(0.0, viewvolume_->get_bottomfov());
  ASSERT_FALSE(viewvolume_->has_topfov());
  ASSERT_DOUBLE_EQ(0.0, viewvolume_->get_topfov());
  ASSERT_FALSE(viewvolume_->has_near());
  ASSERT_DOUBLE_EQ(0.0, viewvolume_->get_near());
}

// This tests that has_xxx() is true even if the value is set to the default.
TEST_F(ViewVolumeTest, TestSetToDefaultValues) {
  viewvolume_->set_leftfov(viewvolume_->get_leftfov());
  ASSERT_TRUE(viewvolume_->has_leftfov());
  ASSERT_DOUBLE_EQ(0.0, viewvolume_->get_leftfov());
  viewvolume_->set_rightfov(viewvolume_->get_rightfov());
  ASSERT_TRUE(viewvolume_->has_rightfov());
  ASSERT_DOUBLE_EQ(0.0, viewvolume_->get_rightfov());
  viewvolume_->set_bottomfov(viewvolume_->get_bottomfov());
  ASSERT_TRUE(viewvolume_->has_bottomfov());
  ASSERT_DOUBLE_EQ(0.0, viewvolume_->get_bottomfov());
  viewvolume_->set_topfov(viewvolume_->get_topfov());
  ASSERT_TRUE(viewvolume_->has_topfov());
  ASSERT_DOUBLE_EQ(0.0, viewvolume_->get_topfov());
  viewvolume_->set_near(viewvolume_->get_near());
  ASSERT_TRUE(viewvolume_->has_near());
  ASSERT_DOUBLE_EQ(0.0, viewvolume_->get_near());
}

// This tests the set_xxx(), xxx() (getter), has_xxx(), and clear_xxx() methods.
TEST_F(ViewVolumeTest, TestSetGetHasClear) {
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
  ASSERT_TRUE(viewvolume_->has_leftfov());
  ASSERT_DOUBLE_EQ(kNonDefaultLeftFov, viewvolume_->get_leftfov());
  ASSERT_TRUE(viewvolume_->has_rightfov());
  ASSERT_DOUBLE_EQ(kNonDefaultRightFov, viewvolume_->get_rightfov());
  ASSERT_TRUE(viewvolume_->has_bottomfov());
  ASSERT_DOUBLE_EQ(kNonDefaultBottomFov, viewvolume_->get_bottomfov());
  ASSERT_TRUE(viewvolume_->has_topfov());
  ASSERT_DOUBLE_EQ(kNonDefaultTopFov, viewvolume_->get_topfov());
  ASSERT_TRUE(viewvolume_->has_near());
  ASSERT_DOUBLE_EQ(kNonDefaultNear, viewvolume_->get_near());

  // Clear all fields.
  viewvolume_->clear_leftfov();
  viewvolume_->clear_rightfov();
  viewvolume_->clear_bottomfov();
  viewvolume_->clear_topfov();
  viewvolume_->clear_near();

  // Verify now in default state.
}

// This tests the ImagePyramid class.
class ImagePyramidTest : public testing::Test {
 protected:
  virtual void SetUp() {
    imagepyramid_ = KmlFactory::GetFactory()->CreateImagePyramid();
  }

  ImagePyramidPtr imagepyramid_;
};

// This tests the Type() and IsA() methods.
TEST_F(ImagePyramidTest, TestType) {
  ASSERT_EQ(Type_ImagePyramid, imagepyramid_->Type());
  ASSERT_TRUE(imagepyramid_->IsA(Type_ImagePyramid));
  ASSERT_TRUE(imagepyramid_->IsA(Type_Object));
}

// This tests the default values of all fields.
TEST_F(ImagePyramidTest, TestDefaults) {
  ASSERT_FALSE(imagepyramid_->has_tilesize());
  ASSERT_TRUE(256 == imagepyramid_->get_tilesize());
  ASSERT_FALSE(imagepyramid_->has_maxwidth());
  ASSERT_EQ(0, imagepyramid_->get_maxwidth());
  ASSERT_FALSE(imagepyramid_->has_maxheight());
  ASSERT_EQ(0, imagepyramid_->get_maxheight());
  ASSERT_FALSE(imagepyramid_->has_gridorigin());
  ASSERT_TRUE(GRIDORIGIN_LOWERLEFT == imagepyramid_->get_gridorigin());
}

// This tests that has_xxx() is true even if the value is set to the default.
TEST_F(ImagePyramidTest, TestSetToDefaultValues) {
  imagepyramid_->set_tilesize(imagepyramid_->get_tilesize());
  ASSERT_TRUE(imagepyramid_->has_tilesize());
  ASSERT_EQ(256, imagepyramid_->get_tilesize());
  imagepyramid_->set_maxheight(imagepyramid_->get_maxheight());
  ASSERT_TRUE(imagepyramid_->has_maxheight());
  ASSERT_EQ(0, imagepyramid_->get_maxheight());
  imagepyramid_->set_maxwidth(imagepyramid_->get_maxwidth());
  ASSERT_TRUE(imagepyramid_->has_maxwidth());
  ASSERT_EQ(0, imagepyramid_->get_maxwidth());
  imagepyramid_->set_gridorigin(imagepyramid_->get_gridorigin());
  ASSERT_TRUE(imagepyramid_->has_gridorigin());
  ASSERT_TRUE(GRIDORIGIN_LOWERLEFT == imagepyramid_->get_gridorigin());
}

// This tests the set_xxx(), xxx() (getter), has_xxx(), and clear_xxx() methods.
TEST_F(ImagePyramidTest, TestSetGetHasClear) {
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
  ASSERT_TRUE(imagepyramid_->has_tilesize());
  ASSERT_EQ(kNonDefaultTilesize, imagepyramid_->get_tilesize());
  ASSERT_TRUE(imagepyramid_->has_maxheight());
  ASSERT_EQ(kNonDefaultMaxheight, imagepyramid_->get_maxheight());
  ASSERT_TRUE(imagepyramid_->has_maxwidth());
  ASSERT_EQ(kNonDefaultMaxwidth, imagepyramid_->get_maxwidth());
  ASSERT_TRUE(imagepyramid_->has_gridorigin());
  ASSERT_TRUE(gridorigin == imagepyramid_->get_gridorigin());

  // Clear all fields.
  imagepyramid_->clear_tilesize();
  imagepyramid_->clear_maxheight();
  imagepyramid_->clear_maxwidth();
  imagepyramid_->clear_gridorigin();

  // Verify now in default state.
}

// This tests the PhotoOverlay class.
class PhotoOverlayTest : public testing::Test {
 protected:
  virtual void SetUp() {
    photooverlay_ = KmlFactory::GetFactory()->CreatePhotoOverlay();
  }

  PhotoOverlayPtr photooverlay_;
};

// This tests the Type() and IsA() methods.
TEST_F(PhotoOverlayTest, TestType) {
  ASSERT_EQ(Type_PhotoOverlay, photooverlay_->Type());
  ASSERT_TRUE(photooverlay_->IsA(Type_PhotoOverlay));
  ASSERT_TRUE(photooverlay_->IsA(Type_Overlay));
  ASSERT_TRUE(photooverlay_->IsA(Type_Feature));
  ASSERT_TRUE(photooverlay_->IsA(Type_Object));
}

// This tests the default values of all fields.
TEST_F(PhotoOverlayTest, TestDefaults) {
  // rotation, ViewVolume, ImagePyramid, Point, shape
  ASSERT_FALSE(photooverlay_->has_rotation());
  ASSERT_DOUBLE_EQ(0.0, photooverlay_->get_rotation());
  ASSERT_FALSE(photooverlay_->has_viewvolume());
  ASSERT_TRUE(NULL == photooverlay_->get_viewvolume());
  ASSERT_FALSE(photooverlay_->has_imagepyramid());
  ASSERT_TRUE(NULL == photooverlay_->get_imagepyramid());
  ASSERT_FALSE(photooverlay_->has_point());
  ASSERT_TRUE(NULL == photooverlay_->get_point());
  ASSERT_FALSE(photooverlay_->has_shape());
  ASSERT_EQ(SHAPE_RECTANGLE, photooverlay_->get_shape());
}

// This tests that has_xxx() is true even if the value is set to the default.
TEST_F(PhotoOverlayTest, TestSetToDefaultValues) {
  photooverlay_->set_rotation(photooverlay_->get_rotation());
  ASSERT_TRUE(photooverlay_->has_rotation());
  ASSERT_DOUBLE_EQ(0.0, photooverlay_->get_rotation());
  photooverlay_->set_viewvolume(NULL);
  ASSERT_FALSE(photooverlay_->has_viewvolume());
  ASSERT_TRUE(NULL == photooverlay_->get_viewvolume());
  photooverlay_->set_imagepyramid(NULL);
  ASSERT_FALSE(photooverlay_->has_imagepyramid());
  ASSERT_TRUE(NULL == photooverlay_->get_imagepyramid());
  photooverlay_->set_point(NULL);
  ASSERT_FALSE(photooverlay_->has_point());
  ASSERT_TRUE(NULL == photooverlay_->get_point());
  photooverlay_->set_shape(photooverlay_->get_shape());
  ASSERT_TRUE(photooverlay_->has_shape());
  ASSERT_EQ(SHAPE_RECTANGLE, photooverlay_->get_shape());
}

// This tests the set_xxx(), xxx() (getter), has_xxx(), and clear_xxx() methods.
TEST_F(PhotoOverlayTest, TestSetGetHasClear) {
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
  ASSERT_TRUE(photooverlay_->has_rotation());
  ASSERT_DOUBLE_EQ(kNonDefaultRotation, photooverlay_->get_rotation());
  ASSERT_TRUE(photooverlay_->has_viewvolume());
  ASSERT_TRUE(viewvolume == photooverlay_->get_viewvolume());
  ASSERT_TRUE(photooverlay_->has_imagepyramid());
  ASSERT_TRUE(imagepyramid == photooverlay_->get_imagepyramid());
  ASSERT_TRUE(photooverlay_->has_point());
  ASSERT_TRUE(point == photooverlay_->get_point());
  ASSERT_TRUE(photooverlay_->has_shape());
  ASSERT_TRUE(shape == photooverlay_->get_shape());

  // Clear all fields.
  photooverlay_->clear_rotation();
  photooverlay_->clear_viewvolume();
  photooverlay_->clear_imagepyramid();
  photooverlay_->clear_point();
  photooverlay_->clear_shape();

  // Verify now in default state.
}

TEST_F(PhotoOverlayTest, TestParseSerialize) {
  const string kPhotoOverlay(
      "<PhotoOverlay>"
      "<name>hi</name>"
      "<Icon><href>pretty.jpg</href></Icon>"
      "<rotation>12.3</rotation>"
      "<ViewVolume/>"
      "<ImagePyramid/>"
      "<Point/>"
      "<shape>sphere</shape>"
      "</PhotoOverlay>");
  ASSERT_EQ(kPhotoOverlay,
            kmldom::SerializeRaw(kmldom::Parse(kPhotoOverlay, NULL)));
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
