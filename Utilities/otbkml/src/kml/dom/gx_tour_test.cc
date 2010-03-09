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

// This file contains the unit tests for the <gx:Tour> elements.

#include "kml/dom/gx_tour.h"
#include "gtest/gtest.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_funcs.h"

namespace kmldom {

class GxTourTest : public testing::Test {
 protected:
  virtual void SetUp() {
    gx_tour_ = KmlFactory::GetFactory()->CreateGxTour();
  }

  GxTourPtr gx_tour_;
};

TEST_F(GxTourTest, TestType) {
  ASSERT_EQ(Type_GxTour, gx_tour_->Type());
  ASSERT_TRUE(gx_tour_->IsA(Type_GxTour));
  ASSERT_TRUE(gx_tour_->IsA(Type_Feature));
  ASSERT_FALSE(gx_tour_->IsA(Type_Container));
  ASSERT_FALSE(gx_tour_->IsA(Type_Geometry));
  ASSERT_TRUE(gx_tour_->IsA(Type_Object));
  ASSERT_TRUE(AsGxTour(gx_tour_));
  ASSERT_TRUE(AsFeature(gx_tour_));
  ASSERT_TRUE(AsObject(gx_tour_));
}

// Verify proper defaults:
TEST_F(GxTourTest, TestDefaults) {
  ASSERT_FALSE(gx_tour_->has_gx_playlist());
  ASSERT_TRUE(NULL == gx_tour_->get_gx_playlist());
}

// Verify setting default makes has_xxx() true:
TEST_F(GxTourTest, TestSetToDefaultValues) {
  gx_tour_->set_gx_playlist(NULL);  // should not crash
  ASSERT_FALSE(gx_tour_->has_gx_playlist());  // ptr is null
}

// Verify set, get, has, clear:
TEST_F(GxTourTest, TestSetGetHasClear) {
  // Set child elements to non-default values:
  gx_tour_->set_gx_playlist(KmlFactory::GetFactory()->CreateGxPlaylist());

  // Verify getter and has_xxx():
  ASSERT_TRUE(gx_tour_->has_gx_playlist());
  ASSERT_EQ(Type_GxPlaylist, gx_tour_->get_gx_playlist()->Type());

  // Clear all fields:
  gx_tour_->clear_gx_playlist();
}

TEST_F(GxTourTest, TestParse) {
  string kName = "My Favorite GxTour";
  string kSnippet = "Left panel stuff about my favorite place...";
  string kGxTour =
    "<gx:Tour>"
    "<name>" + kName + "</name>"
    "<Snippet>" + kSnippet + "</Snippet>"
    "<gx:Playlist/>"
    "</gx:Tour>";
  string errors;
  ElementPtr root = Parse(kGxTour, &errors);
  ASSERT_TRUE(root);
  ASSERT_TRUE(errors.empty());
  const GxTourPtr tour = AsGxTour(root);
  ASSERT_TRUE(tour);
  ASSERT_FALSE(tour->has_id());
  ASSERT_FALSE(tour->has_targetid());
  ASSERT_TRUE(tour->has_name());
  ASSERT_EQ(kName, tour->get_name());
  ASSERT_FALSE(tour->has_visibility());
  ASSERT_FALSE(tour->has_open());
  ASSERT_FALSE(tour->has_address());
  ASSERT_TRUE(tour->has_snippet());
  ASSERT_TRUE(tour->get_snippet()->has_text());
  ASSERT_EQ(kSnippet, tour->get_snippet()->get_text());
  ASSERT_FALSE(tour->has_description());
  ASSERT_FALSE(tour->has_abstractview());
  ASSERT_FALSE(tour->has_timeprimitive());
  ASSERT_FALSE(tour->has_styleurl());
  ASSERT_FALSE(tour->has_styleselector());
  ASSERT_FALSE(tour->has_region());
  ASSERT_FALSE(tour->has_extendeddata());
  const GxPlaylistPtr gx_playlist = AsGxPlaylist(tour->get_gx_playlist());
  ASSERT_EQ(static_cast<size_t>(0), gx_playlist->get_gx_tourprimitive_array_size());
}

TEST_F(GxTourTest, TestSerialize) {
  gx_tour_->set_name("name");
  gx_tour_->set_visibility(true);
  gx_tour_->set_description("description");
  gx_tour_->set_styleurl("#styleurl");
  gx_tour_->set_region(KmlFactory::GetFactory()->CreateRegion());
  gx_tour_->set_gx_playlist(KmlFactory::GetFactory()->CreateGxPlaylist());

  string expected(
    "<gx:Tour>"
    "<name>name</name>"
    "<visibility>1</visibility>"
    "<description>description</description>"
    "<styleUrl>#styleurl</styleUrl>"
    "<Region/>"
    "<gx:Playlist/>"
    "</gx:Tour>"
  );
  ASSERT_EQ(expected, SerializeRaw(gx_tour_));
}

class GxPlaylistTest : public testing::Test {
 protected:
  virtual void SetUp() {
    gx_playlist_ = KmlFactory::GetFactory()->CreateGxPlaylist();
  }

  GxPlaylistPtr gx_playlist_;
};

TEST_F(GxPlaylistTest, TestType) {
  ASSERT_EQ(Type_GxPlaylist, gx_playlist_->Type());
  ASSERT_TRUE(gx_playlist_->IsA(Type_GxPlaylist));
  ASSERT_FALSE(gx_playlist_->IsA(Type_Feature));
  ASSERT_FALSE(gx_playlist_->IsA(Type_Geometry));
  ASSERT_TRUE(gx_playlist_->IsA(Type_Object));
  ASSERT_TRUE(AsGxPlaylist(gx_playlist_));
  ASSERT_FALSE(AsFeature(gx_playlist_));
  ASSERT_TRUE(AsObject(gx_playlist_));
}

// Verify proper defaults:
TEST_F(GxPlaylistTest, TestDefaults) {
  // TODO:
}

// Verify setting default makes has_xxx() true:
TEST_F(GxPlaylistTest, TestSetToDefaultValues) {
  // TODO:
}

// Verify set, get, has, clear:
TEST_F(GxPlaylistTest, TestSetGetHasClear) {
  // TODO:
}

TEST_F(GxPlaylistTest, TestParse) {
  string kName = "My Favorite Place";
  string kSnippet = "Left panel stuff about my favorite place...";
  string kGxPlaylist =
    "<gx:Playlist id=\"gx_playlist123\">"
    "<gx:AnimatedUpdate/>"
    "<gx:FlyTo/>"
    "<gx:Wait/>"
    "</gx:Playlist>";
  string errors;
  ElementPtr root = Parse(kGxPlaylist, &errors);
  ASSERT_TRUE(root);
  ASSERT_TRUE(errors.empty());
  const GxPlaylistPtr gx_playlist = AsGxPlaylist(root);
  ASSERT_TRUE(gx_playlist);
  ASSERT_TRUE(gx_playlist->has_id());
  ASSERT_EQ(string("gx_playlist123"), gx_playlist->get_id());
  ASSERT_FALSE(gx_playlist->has_targetid());
  ASSERT_EQ(static_cast<size_t>(3),
      gx_playlist->get_gx_tourprimitive_array_size());
  ASSERT_TRUE(AsGxAnimatedUpdate(
        gx_playlist->get_gx_tourprimitive_array_at(0)));
  ASSERT_TRUE(AsGxFlyTo(gx_playlist->get_gx_tourprimitive_array_at(1)));
  ASSERT_TRUE(AsGxWait(gx_playlist->get_gx_tourprimitive_array_at(2)));
}

TEST_F(GxPlaylistTest, TestSerialize) {
  // TODO:
}

class GxAnimatedUpdateTest : public testing::Test {
 protected:
  virtual void SetUp() {
    gx_animatedupdate_ = KmlFactory::GetFactory()->CreateGxAnimatedUpdate();
  }

  GxAnimatedUpdatePtr gx_animatedupdate_;
};

TEST_F(GxAnimatedUpdateTest, TestType) {
  ASSERT_EQ(Type_GxAnimatedUpdate, gx_animatedupdate_->Type());
  ASSERT_TRUE(gx_animatedupdate_->IsA(Type_GxAnimatedUpdate));
  ASSERT_TRUE(gx_animatedupdate_->IsA(Type_GxTourPrimitive));
  ASSERT_TRUE(gx_animatedupdate_->IsA(Type_Object));
  ASSERT_TRUE(AsGxAnimatedUpdate(gx_animatedupdate_));
  ASSERT_TRUE(AsGxTourPrimitive(gx_animatedupdate_));
  ASSERT_TRUE(AsObject(gx_animatedupdate_));
}

// Verify proper defaults:
TEST_F(GxAnimatedUpdateTest, TestDefaults) {
  ASSERT_FALSE(gx_animatedupdate_->has_gx_duration());
  ASSERT_DOUBLE_EQ(0.0, gx_animatedupdate_->get_gx_duration());
  ASSERT_FALSE(gx_animatedupdate_->has_update());
  ASSERT_TRUE(NULL == gx_animatedupdate_->get_update());
}

// Verify setting default makes has_xxx() true:
TEST_F(GxAnimatedUpdateTest, TestSetToDefaultValues) {
  gx_animatedupdate_->set_gx_duration(gx_animatedupdate_->get_gx_duration());
  ASSERT_TRUE(gx_animatedupdate_->has_gx_duration());
  gx_animatedupdate_->set_update(gx_animatedupdate_->get_update());
  ASSERT_TRUE(NULL == gx_animatedupdate_->get_update());
}

// Verify set, get, has, clear:
TEST_F(GxAnimatedUpdateTest, TestSetGetHasClear) {
  const double kDuration = 1.2;
  const UpdatePtr kUpdate = KmlFactory::GetFactory()->CreateUpdate();
  gx_animatedupdate_->set_gx_duration(kDuration);
  gx_animatedupdate_->set_update(kUpdate);
  ASSERT_TRUE(gx_animatedupdate_->has_gx_duration());
  ASSERT_DOUBLE_EQ(kDuration, gx_animatedupdate_->get_gx_duration());
  ASSERT_TRUE(gx_animatedupdate_->has_update());
  ASSERT_EQ(kUpdate, gx_animatedupdate_->get_update());
}

TEST_F(GxAnimatedUpdateTest, TestParse) {
  const string kGxAnimatedUpdate =
    "<gx:AnimatedUpdate>"
    "<gx:duration>1.1</gx:duration>"
    "<Update/>"
    "</gx:AnimatedUpdate>";
  string errors;
  ElementPtr root = Parse(kGxAnimatedUpdate, &errors);
  ASSERT_TRUE(root);
  ASSERT_TRUE(errors.empty());
  const GxAnimatedUpdatePtr animatedupdate = AsGxAnimatedUpdate(root);
  ASSERT_TRUE(animatedupdate->has_gx_duration());
  ASSERT_DOUBLE_EQ(1.1, animatedupdate->get_gx_duration());
  ASSERT_TRUE(animatedupdate);
  ASSERT_TRUE(animatedupdate->has_update());
  ASSERT_EQ(kGxAnimatedUpdate, SerializeRaw(Parse(kGxAnimatedUpdate, NULL)));
}

TEST_F(GxAnimatedUpdateTest, TestSerialize) {
  const double kDuration = 1.2;
  const UpdatePtr kUpdate = KmlFactory::GetFactory()->CreateUpdate();
  gx_animatedupdate_->set_gx_duration(kDuration);
  gx_animatedupdate_->set_update(kUpdate);
  const string kExpected =
    "<gx:AnimatedUpdate>"
    "<gx:duration>1.2</gx:duration>"
    "<Update/>"
    "</gx:AnimatedUpdate>";
  ASSERT_EQ(kExpected, SerializeRaw(gx_animatedupdate_));
}

class GxFlyToTest : public testing::Test {
 protected:
  virtual void SetUp() {
    gx_flyto_ = KmlFactory::GetFactory()->CreateGxFlyTo();
  }

  GxFlyToPtr gx_flyto_;
};

TEST_F(GxFlyToTest, TestType) {
  ASSERT_EQ(Type_GxFlyTo, gx_flyto_->Type());
  ASSERT_TRUE(gx_flyto_->IsA(Type_GxFlyTo));
  ASSERT_TRUE(gx_flyto_->IsA(Type_GxTourPrimitive));
  ASSERT_TRUE(gx_flyto_->IsA(Type_Object));
  ASSERT_TRUE(AsGxFlyTo(gx_flyto_));
  ASSERT_TRUE(AsGxTourPrimitive(gx_flyto_));
  ASSERT_TRUE(AsObject(gx_flyto_));
}

TEST_F(GxFlyToTest, TestDefaults) {
  ASSERT_FALSE(gx_flyto_->has_gx_duration());
  ASSERT_EQ(0.0, gx_flyto_->get_gx_duration());
  ASSERT_FALSE(gx_flyto_->has_gx_flytomode());
  ASSERT_EQ(GX_FLYTOMODE_BOUNCE, gx_flyto_->get_gx_flytomode());
  ASSERT_FALSE(gx_flyto_->has_abstractview());
  ASSERT_TRUE(NULL == gx_flyto_->get_abstractview());
}

TEST_F(GxFlyToTest, TestSetToDefaultValues) {
  gx_flyto_->set_gx_duration(gx_flyto_->get_gx_duration());
  gx_flyto_->set_gx_flytomode(gx_flyto_->get_gx_flytomode());
  gx_flyto_->set_abstractview(NULL);
  ASSERT_TRUE(gx_flyto_->has_gx_duration());
  ASSERT_TRUE(gx_flyto_->has_gx_flytomode());
  ASSERT_FALSE(gx_flyto_->has_abstractview());
}

TEST_F(GxFlyToTest, TestSetGetHasClear) {
  // Non-default values.
  const double kDuration = 10;
  const GxFlyToModeEnum kFlytomode = GX_FLYTOMODE_SMOOTH;
  const CameraPtr kCamera = KmlFactory::GetFactory()->CreateCamera();

  // Set all fields.
  gx_flyto_->set_gx_duration(kDuration);
  gx_flyto_->set_gx_flytomode(kFlytomode);
  gx_flyto_->set_abstractview(kCamera);

  // Verify getter and has_xxx().
  ASSERT_TRUE(gx_flyto_->has_gx_duration());
  ASSERT_TRUE(gx_flyto_->has_gx_flytomode());
  ASSERT_TRUE(gx_flyto_->has_abstractview());
  ASSERT_EQ(kDuration, gx_flyto_->get_gx_duration());
  ASSERT_EQ(kFlytomode, gx_flyto_->get_gx_flytomode());
  ASSERT_EQ(kCamera, gx_flyto_->get_abstractview());

  // Clear all fields.
  gx_flyto_->clear_gx_duration();
  gx_flyto_->clear_gx_flytomode();
  gx_flyto_->clear_abstractview();

  // Verify final state.
  ASSERT_FALSE(gx_flyto_->has_gx_duration());
  ASSERT_EQ(0.0, gx_flyto_->get_gx_duration());
  ASSERT_FALSE(gx_flyto_->has_gx_flytomode());
  ASSERT_EQ(GX_FLYTOMODE_BOUNCE, gx_flyto_->get_gx_flytomode());
  ASSERT_FALSE(gx_flyto_->has_abstractview());
  ASSERT_TRUE(NULL == gx_flyto_->get_abstractview());
}

TEST_F(GxFlyToTest, TestParse) {
  const string kGxFlyTo =
    "<gx:FlyTo>"
    "<gx:duration>10</gx:duration>"
    "<gx:flyToMode>smooth</gx:flyToMode>"
    "<Camera/>"
    "</gx:FlyTo>";
  string errors;
  ElementPtr root = Parse(kGxFlyTo, &errors);
  ASSERT_TRUE(root);
  ASSERT_TRUE(errors.empty());
  const GxFlyToPtr flyto = AsGxFlyTo(root);
  ASSERT_TRUE(flyto);
  ASSERT_TRUE(flyto->has_gx_duration());
  ASSERT_DOUBLE_EQ(10.0, flyto->get_gx_duration());
  ASSERT_TRUE(flyto->has_gx_flytomode());
  ASSERT_TRUE(GX_FLYTOMODE_SMOOTH == flyto->get_gx_flytomode());
  ASSERT_TRUE(flyto->has_abstractview());
  ASSERT_TRUE(AsCamera(flyto->get_abstractview()));
}

TEST_F(GxFlyToTest, TestSerialize) {
  gx_flyto_->set_gx_duration(10.0);
  gx_flyto_->set_gx_flytomode(GX_FLYTOMODE_SMOOTH);
  gx_flyto_->set_abstractview(KmlFactory::GetFactory()->CreateCamera());
  const string kExpected =
    "<gx:FlyTo>"
    "<gx:duration>10</gx:duration>"
    "<gx:flyToMode>smooth</gx:flyToMode>"
    "<Camera/>"
    "</gx:FlyTo>";
  ASSERT_EQ(kExpected, SerializeRaw(gx_flyto_));
}

class GxSoundCueTest : public testing::Test {
 protected:
  virtual void SetUp() {
    gx_soundcue_ = KmlFactory::GetFactory()->CreateGxSoundCue();
  }

  GxSoundCuePtr gx_soundcue_;
};

TEST_F(GxSoundCueTest, TestType) {
  ASSERT_EQ(Type_GxSoundCue, gx_soundcue_->Type());
  ASSERT_TRUE(gx_soundcue_->IsA(Type_GxSoundCue));
  ASSERT_TRUE(gx_soundcue_->IsA(Type_GxTourPrimitive));
  ASSERT_TRUE(gx_soundcue_->IsA(Type_Object));
  ASSERT_TRUE(AsGxSoundCue(gx_soundcue_));
  ASSERT_TRUE(AsGxTourPrimitive(gx_soundcue_));
  ASSERT_TRUE(AsObject(gx_soundcue_));
}

TEST_F(GxSoundCueTest, TestDefaults) {
  ASSERT_FALSE(gx_soundcue_->has_href());
  ASSERT_EQ("", gx_soundcue_->get_href());
}

TEST_F(GxSoundCueTest, TestSetToDefaultValues) {
  gx_soundcue_->set_href(gx_soundcue_->get_href());
  ASSERT_TRUE(gx_soundcue_->has_href());
}

TEST_F(GxSoundCueTest, TestSetGetHasClear) {
  // Non-default values.
  const string kHref("my/cool/music.mp3");

  // Set all fields.
  gx_soundcue_->set_href(kHref);

  // Verify getter and has_xxx().
  ASSERT_TRUE(gx_soundcue_->has_href());
  ASSERT_EQ(kHref, gx_soundcue_->get_href());

  // Clear all fields.
  gx_soundcue_->clear_href();

  // Verify final state.
  ASSERT_FALSE(gx_soundcue_->has_href());
  ASSERT_EQ("", gx_soundcue_->get_href());
}

TEST_F(GxSoundCueTest, TestParse) {
  const string kGxSoundCue =
    "<gx:SoundCue>"
    "<href>some/cool/file.mp3</href>"
    "</gx:SoundCue>";
  string errors;
  ElementPtr root = Parse(kGxSoundCue, &errors);
  ASSERT_TRUE(root);
  ASSERT_TRUE(errors.empty());
  const GxSoundCuePtr soundcue = AsGxSoundCue(root);
  ASSERT_TRUE(soundcue);
  ASSERT_TRUE(soundcue->has_href());
  ASSERT_EQ("some/cool/file.mp3", soundcue->get_href());
}

TEST_F(GxSoundCueTest, TestSerialize) {
  gx_soundcue_->set_href("some/cool/file.mp3");
  const string kExpected =
    "<gx:SoundCue>"
    "<href>some/cool/file.mp3</href>"
    "</gx:SoundCue>";
  ASSERT_EQ(kExpected, SerializeRaw(gx_soundcue_));
}

class GxTourControlTest : public testing::Test {
 protected:
  virtual void SetUp() {
    gx_tourcontrol_ = KmlFactory::GetFactory()->CreateGxTourControl();
  }

  GxTourControlPtr gx_tourcontrol_;
};

TEST_F(GxTourControlTest, TestType) {
  ASSERT_EQ(Type_GxTourControl, gx_tourcontrol_->Type());
  ASSERT_TRUE(gx_tourcontrol_->IsA(Type_GxTourControl));
  ASSERT_TRUE(gx_tourcontrol_->IsA(Type_GxTourPrimitive));
  ASSERT_TRUE(gx_tourcontrol_->IsA(Type_Object));
  ASSERT_TRUE(AsGxTourControl(gx_tourcontrol_));
  ASSERT_TRUE(AsGxTourPrimitive(gx_tourcontrol_));
  ASSERT_TRUE(AsObject(gx_tourcontrol_));
}

TEST_F(GxTourControlTest, TestDefaults) {
  ASSERT_FALSE(gx_tourcontrol_->has_gx_playmode());
  ASSERT_EQ(GX_PLAYMODE_PAUSE, gx_tourcontrol_->get_gx_playmode());
}

TEST_F(GxTourControlTest, TestSetToDefaultValues) {
  gx_tourcontrol_->set_gx_playmode(gx_tourcontrol_->get_gx_playmode());
  ASSERT_TRUE(gx_tourcontrol_->has_gx_playmode());
}

TEST_F(GxTourControlTest, TestSetGetHasClear) {
  // gx:playMode has only one enum at present.
  const GxPlayModeEnum kPlayMode = GX_PLAYMODE_PAUSE;

  // Set all fields.
  gx_tourcontrol_->set_gx_playmode(kPlayMode);

  // Verify getter and has_xxx().
  ASSERT_TRUE(gx_tourcontrol_->has_gx_playmode());
  ASSERT_EQ(kPlayMode, gx_tourcontrol_->get_gx_playmode());

  // Clear all fields.
  gx_tourcontrol_->clear_gx_playmode();

  // Verify final state.
  ASSERT_FALSE(gx_tourcontrol_->has_gx_playmode());
  ASSERT_EQ(GX_PLAYMODE_PAUSE, gx_tourcontrol_->get_gx_playmode());
}

TEST_F(GxTourControlTest, TestParse) {
  const string kGxTourControl =
    "<gx:TourControl>"
    "<gx:playMode>pause</gx:playMode>"
    "</gx:TourControl>";
  string errors;
  ElementPtr root = Parse(kGxTourControl, &errors);
  ASSERT_TRUE(root);
  ASSERT_TRUE(errors.empty());
  const GxTourControlPtr tourcontrol = AsGxTourControl(root);
  ASSERT_TRUE(tourcontrol);
  ASSERT_TRUE(tourcontrol->has_gx_playmode());
  ASSERT_TRUE(GX_PLAYMODE_PAUSE == tourcontrol->get_gx_playmode());
}

TEST_F(GxTourControlTest, TestSerialize) {
  gx_tourcontrol_->set_gx_playmode(GX_PLAYMODE_PAUSE);
  const string kExpected =
    "<gx:TourControl>"
    "<gx:playMode>pause</gx:playMode>"
    "</gx:TourControl>";
  ASSERT_EQ(kExpected, SerializeRaw(gx_tourcontrol_));
}

class GxWaitTest : public testing::Test {
 protected:
  virtual void SetUp() {
    gx_wait_ = KmlFactory::GetFactory()->CreateGxWait();
  }

  GxWaitPtr gx_wait_;
};

TEST_F(GxWaitTest, TestType) {
  ASSERT_EQ(Type_GxWait, gx_wait_->Type());
  ASSERT_TRUE(gx_wait_->IsA(Type_GxWait));
  ASSERT_TRUE(gx_wait_->IsA(Type_GxTourPrimitive));
  ASSERT_TRUE(gx_wait_->IsA(Type_Object));
  ASSERT_TRUE(AsGxWait(gx_wait_));
  ASSERT_TRUE(AsGxTourPrimitive(gx_wait_));
  ASSERT_TRUE(AsObject(gx_wait_));
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
