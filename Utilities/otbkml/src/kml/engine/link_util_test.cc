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
#include "kml/base/unit_test.h"

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

class LinkUtilTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(LinkUtilTest);
  CPPUNIT_TEST(TestGetHref);
  CPPUNIT_TEST(TestGetIconParentHref);
  CPPUNIT_TEST(TestGetLinkParentHref);
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp() {
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
  }

 protected:
  void TestGetHref();
  void TestGetIconParentHref();
  void TestGetLinkParentHref();

 private:
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
};

template<typename HP>
static void VerifyGetHref(const HP& href_parent, const char* want_href) {
  std::string got_href;
  CPPUNIT_ASSERT(GetHref(href_parent, &got_href));
  CPPUNIT_ASSERT_EQUAL(std::string(want_href), got_href);
}

// This tests the GetHref() function template.
void LinkUtilTest::TestGetHref() {
  VerifyGetHref(itemicon_, kItemIconHref);
  VerifyGetHref(link_, kLinkHref);
  VerifyGetHref(icon_, kIconHref);
}

template<typename HP>
static void VerifyGetIconParentHref(const HP& icon_parent,
                                    const char* want_href) {
  std::string got_href;
  CPPUNIT_ASSERT(GetIconParentHref(icon_parent, &got_href));
  CPPUNIT_ASSERT_EQUAL(std::string(want_href), got_href);
}

// This tests the GetIconParentHref() function template.
void LinkUtilTest::TestGetIconParentHref() {
  VerifyGetIconParentHref(groundoverlay_, kGroundOverlayHref);
  VerifyGetIconParentHref(photooverlay_, kPhotoOverlayHref);
  VerifyGetIconParentHref(screenoverlay_, kScreenOverlayHref);
  VerifyGetIconParentHref(iconstyle_, kIconStyleHref);
}

template<typename LP>
static void VerifyGetLinkParentHref(const LP& link_parent,
                                    const char* want_href) {
  std::string got_href;
  CPPUNIT_ASSERT(GetLinkParentHref(link_parent, &got_href));
  CPPUNIT_ASSERT_EQUAL(std::string(want_href), got_href);
}

// This tests the GetLinkParentHref() function template.
void LinkUtilTest::TestGetLinkParentHref() {
  VerifyGetLinkParentHref(networklink_, kNetworkLinkHref);
  VerifyGetLinkParentHref(model_, kModelHref);
}

}  // end namespace kmlengine

TEST_MAIN
