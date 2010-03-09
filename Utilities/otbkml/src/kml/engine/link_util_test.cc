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

// This file contains the unit tests for the location utility functions.

#include "kml/engine/link_util.h"
#include "kml/dom.h"
#include "kml/base/net_cache_test_util.h"
#include "gtest/gtest.h"
#include "kml/engine/kml_cache.h"
#include "kml/engine/feature_visitor.h"
#include "kml/engine/find.h"

// The following define is a convenience for testing inside Google.
#ifdef GOOGLE_INTERNAL
#include "kml/base/google_internal_test.h"
#endif

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

using kmldom::GroundOverlayPtr;
using kmldom::IconPtr;
using kmldom::IconStylePtr;
using kmldom::IconStyleIconPtr;
using kmldom::ItemIconPtr;
using kmldom::KmlFactory;
using kmldom::LinkPtr;
using kmldom::ModelPtr;
using kmldom::NetworkLinkPtr;
using kmldom::OverlayPtr;
using kmldom::PhotoOverlayPtr;
using kmldom::ScreenOverlayPtr;

namespace kmlengine {

const char* kGroundOverlayHref = "ground.jpg";
const char* kIconHref = "goo.jpg";
const char* kIconStyleHref = "icon.png";
const char* kItemIconHref = "itemicon.png";
const char* kLinkHref = "foo.kml";
const char* kNetworkLinkHref = "http://example.com/cool/stuff/here.kml";
const char* kModelHref = "foo.kmz/hi.dae";
const char* kScreenOverlayHref = "screen.jpg";
const char* kPhotoOverlayHref = "photo.jpg";

static void SetOverlayIconHref(OverlayPtr overlay, const char* href) {
  IconPtr icon = KmlFactory::GetFactory()->CreateIcon();
  icon->set_href(href);
  overlay->set_icon(icon);
}

template<typename LP>
static void SetLinkParentLinkHref(LP link_parent, const char* href) {
  LinkPtr link = KmlFactory::GetFactory()->CreateLink();
  link->set_href(href);
  link_parent->set_icon(link);
}

class LinkUtilTest : public testing::Test {
 protected:
  virtual void SetUp() {
    factory_ = KmlFactory::GetFactory();
    groundoverlay_ = factory_->CreateGroundOverlay();
    SetOverlayIconHref(groundoverlay_, kGroundOverlayHref);
    icon_ = factory_->CreateIcon();
    icon_->set_href(kIconHref);
    iconstyle_ = factory_->CreateIconStyle();
    IconStyleIconPtr iconstyleicon = factory_->CreateIconStyleIcon();
    iconstyleicon->set_href(kIconStyleHref);
    iconstyle_->set_icon(iconstyleicon);
    iconstyleicon_ = factory_->CreateIconStyleIcon();
    itemicon_ = factory_->CreateItemIcon();
    itemicon_->set_href(kItemIconHref);
    link_ = factory_->CreateLink();
    link_->set_href(kLinkHref);
    model_ = factory_->CreateModel();
    networklink_ = factory_->CreateNetworkLink();
    photooverlay_ = factory_->CreatePhotoOverlay();
    SetOverlayIconHref(photooverlay_, kPhotoOverlayHref);
    screenoverlay_ = factory_->CreateScreenOverlay();
    SetOverlayIconHref(screenoverlay_, kScreenOverlayHref);
    kml_cache_.reset(new KmlCache(&testdata_net_fetcher_, 1));
  }

  void Init();
  GroundOverlayPtr groundoverlay_;
  KmlFactory* factory_;
  IconPtr icon_;
  IconStylePtr iconstyle_;
  IconStyleIconPtr iconstyleicon_;
  ItemIconPtr itemicon_;
  LinkPtr link_;
  ModelPtr model_;
  NetworkLinkPtr networklink_;
  PhotoOverlayPtr photooverlay_;
  ScreenOverlayPtr screenoverlay_;
  kmlbase::TestDataNetFetcher testdata_net_fetcher_;
  boost::scoped_ptr<KmlCache> kml_cache_;
};

template<typename HP>
static void VerifyGetHref(const HP& href_parent, const char* want_href) {
  string got_href;
  ASSERT_TRUE(GetHref(href_parent, &got_href));
  ASSERT_EQ(string(want_href), got_href);
}

// This tests the GetHref() function template.
TEST_F(LinkUtilTest, TestGetHref) {
  VerifyGetHref(itemicon_, kItemIconHref);
  VerifyGetHref(link_, kLinkHref);
  VerifyGetHref(icon_, kIconHref);
}

template<typename HP>
static void VerifyGetIconParentHref(const HP& icon_parent,
                                    const char* want_href) {
  string got_href;
  ASSERT_TRUE(GetIconParentHref(icon_parent, &got_href));
  ASSERT_EQ(string(want_href), got_href);
}

// This tests the GetIconParentHref() function template.
TEST_F(LinkUtilTest, TestGetIconParentHref) {
  VerifyGetIconParentHref(groundoverlay_, kGroundOverlayHref);
  VerifyGetIconParentHref(photooverlay_, kPhotoOverlayHref);
  VerifyGetIconParentHref(screenoverlay_, kScreenOverlayHref);
  VerifyGetIconParentHref(iconstyle_, kIconStyleHref);
}

template<typename LP>
static void VerifyGetLinkParentHref(const LP& link_parent,
                                    const char* want_href) {
  string got_href;
  ASSERT_TRUE(GetLinkParentHref(link_parent, &got_href));
  ASSERT_EQ(string(want_href), got_href);
}

template<typename LP>
static void SetLinkHref(const LP& link_parent, LinkPtr link,
                        const string& href) {
  link->set_href(href);
  link_parent->set_link(link);
}

// This tests the GetLinkParentHref() function template.
TEST_F(LinkUtilTest, TestGetLinkParentHref) {
  SetLinkHref(networklink_, factory_->CreateLink(), kNetworkLinkHref);
  SetLinkHref(model_, factory_->CreateLink(), kModelHref);
  VerifyGetLinkParentHref(networklink_, kNetworkLinkHref);
  VerifyGetLinkParentHref(model_, kModelHref);
}

TEST_F(LinkUtilTest, TestFetchLink) {
  const string kBase("http://host.com/kmz/radar-animation.kmz");
  KmlFilePtr base_kml_file = kml_cache_->FetchKmlAbsolute(kBase);
  ASSERT_TRUE(base_kml_file);
  ElementVector networklink_vector;
  GetElementsById(base_kml_file->get_root(), kmldom::Type_NetworkLink,
                  &networklink_vector);
  // The default KML file in radar-animation.kmz has 1 NetworkLink.
  ASSERT_EQ(static_cast<size_t>(1), networklink_vector.size());
  KmlFilePtr target_kml_file = FetchLink(base_kml_file,
                                         AsNetworkLink(networklink_vector[0]));
  ASSERT_TRUE(target_kml_file);
  kmldom::DocumentPtr document =
      AsDocument(GetRootFeature(target_kml_file->get_root()));
  ASSERT_TRUE(document);
  // This is kmz/radar-animation.kmz/level00/0.kml.
  ASSERT_EQ(string("0130_256_-1"), document->get_name());
}

TEST_F(LinkUtilTest, TestFetchIcon) {
  const string kBase("http://host.com/kmz/rumsey/kml/lc01.kmz");
  KmlFilePtr kml_file = kml_cache_->FetchKmlAbsolute(kBase);
  ASSERT_TRUE(kml_file);
  ElementVector groundoverlay_vector;
  GetElementsById(kml_file->get_root(), kmldom::Type_GroundOverlay,
                  &groundoverlay_vector);
  // The default KML file in lc01.kmz has 2 GroundOverlays.
  ASSERT_EQ(static_cast<size_t>(2), groundoverlay_vector.size());
  string data;
  ASSERT_TRUE(FetchIcon(kml_file, AsGroundOverlay(groundoverlay_vector[0]),
                           &data));
  // This is kmz/rumsey/imagery/01_4.png
  ASSERT_EQ(static_cast<size_t>(6742), data.size());
  ASSERT_TRUE(FetchIcon(kml_file, AsGroundOverlay(groundoverlay_vector[1]),
                           &data));
  // This is kmz/rumsey/imagery/01_8.png
  ASSERT_EQ(static_cast<size_t>(7364), data.size());
}

}  // end namespace kmlengine

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
