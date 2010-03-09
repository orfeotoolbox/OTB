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

// This file contains the unit test for the AtomUtil API functions.

#include "kml/convenience/atom_util.h"
#include "gtest/gtest.h"
#include "kml/convenience/convenience.h"
#include "kml/convenience/http_client.h"
#include "kml/convenience/http_client_test_util.h"
#include "kml/dom.h"
#include "kml/engine/location_util.h"

// The following define is a convenience for testing inside Google.
#ifdef GOOGLE_INTERNAL
#include "kml/base/google_internal_test.h"
#endif

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

namespace kmlconvenience {

TEST(AtomUtilTest, TestCreateBasicEntry) {
  const string kTitle("War And Peace");
  const string kSummary("First there was war, then there was peace");
  const kmldom::AtomEntryPtr entry =
      AtomUtil::CreateBasicEntry(kTitle, kSummary);
  ASSERT_TRUE(entry.get());
  ASSERT_EQ(kTitle, entry->get_title());
  ASSERT_EQ(kSummary, entry->get_summary());
}

TEST(AtomUtilTest, TestCreateAtomEntryForFeature) {
  const string kName("a place");
  const string kDescription("This is a really great place");
  const double kLat(12.12);
  const double kLon(-38.38);
  kmldom::PlacemarkPtr placemark = CreatePointPlacemark(kName, kLat, kLon);
  placemark->set_description(kDescription);
  const kmldom::AtomEntryPtr entry = AtomUtil::CreateEntryForFeature(placemark);
  ASSERT_TRUE(entry.get());
  ASSERT_EQ(kName, entry->get_title());
  ASSERT_EQ(kDescription, entry->get_summary());
  ASSERT_TRUE(entry->has_content());
  const kmldom::AtomContentPtr content = entry->get_content();
  ASSERT_TRUE(content.get());
  ASSERT_EQ(static_cast<size_t>(1),
            content->get_misplaced_elements_array_size());
  placemark = kmldom::AsPlacemark(content->get_misplaced_elements_array_at(0));
  ASSERT_TRUE(placemark.get());
  double lat;
  double lon;
  ASSERT_TRUE(kmlengine::GetFeatureLatLon(placemark, &lat, &lon));
  ASSERT_EQ(kLat, lat);
  ASSERT_EQ(kLon, lon);
  ASSERT_EQ(kName, placemark->get_name());
  ASSERT_EQ(kDescription, placemark->get_description());
}

TEST(AtomUtilTest, TestBasicGetContentSrc) {
  kmldom::AtomContentPtr content =
      kmldom::KmlFactory::GetFactory()->CreateAtomContent();
  const string kSrc("http://somehost.com/some/path/feed/blah");
  content->set_src(kSrc);
  kmldom::AtomEntryPtr entry =
      kmldom::KmlFactory::GetFactory()->CreateAtomEntry();
  entry->set_content(content);
  string src;
  ASSERT_TRUE(AtomUtil::GetContentSrc(entry, &src));
  ASSERT_EQ(kSrc, src);
}

TEST(AtomUtilTest, TestFindRelUrl) {
  kmldom::AtomFeedPtr feed = kmldom::KmlFactory::GetFactory()->CreateAtomFeed();
  const string kRelType("fish");
  ASSERT_FALSE(AtomUtil::FindRelUrl(*feed.get(), kRelType, NULL));
  string href;
  ASSERT_FALSE(AtomUtil::FindRelUrl(*feed.get(), kRelType, &href));
}

TEST(AtomUtilTest, TestCloneEntryFeature) {
  const string kName("a place");
  const double kLat(12.12);
  const double kLon(-38.38);
  const kmldom::PlacemarkPtr placemark =
      CreatePointPlacemark(kName, kLat, kLon);
  kmldom::AtomEntryPtr entry = AtomUtil::CreateEntryForFeature(placemark);
  kmldom::FeaturePtr feature = AtomUtil::GetEntryFeature(entry);
  ASSERT_TRUE(feature.get());
}

TEST(AtomUtilTest, TestGetEntryFeature) {
  kmldom::AtomEntryPtr entry;
  kmldom::FeaturePtr feature = AtomUtil::GetEntryFeature(entry);
}

TEST(AtomUtilTest, TestGetFeedFeatures) {
  kmldom::AtomFeedPtr feed;
  kmldom::FolderPtr folder;
  AtomUtil::GetFeedFeatures(feed, folder);
}

TEST(AtomUtilTest, TestIsOfLinkRel) {
  kmldom::AtomLinkPtr link = kmldom::KmlFactory::GetFactory()->CreateAtomLink();
  // A link with no rel= at all.
  ASSERT_FALSE(AtomUtil::LinkIsOfRel(link, "post"));
  link->set_rel("post-is-not-at-the-end");
  ASSERT_FALSE(AtomUtil::LinkIsOfRel(link, "post"));
  link->set_rel("http://foo.com/goo/blah#post");
  ASSERT_TRUE(AtomUtil::LinkIsOfRel(link, "post"));
  link->set_rel("post");
  ASSERT_TRUE(AtomUtil::LinkIsOfRel(link, "post"));
  string empty;
  ASSERT_FALSE(AtomUtil::LinkIsOfRel(NULL, empty));
}

TEST(AtomUtilTest, TestFindLink) {
  kmldom::AtomFeedPtr feed = kmldom::KmlFactory::GetFactory()->CreateAtomFeed();
  const string kEmpty;
  // Empty/NULL everything just returns NULL w/o crashing.
  ASSERT_FALSE(AtomUtil::FindLink(*feed, kEmpty, kEmpty));
  const string kRel("alternate");
  const string kMimeType("text/html");
  // NULL AtomFeePtr just returns NULL w/o crashing.
  ASSERT_FALSE(AtomUtil::FindLink(*feed, kRel, kMimeType));
  kmldom::AtomLinkPtr link = kmldom::KmlFactory::GetFactory()->CreateAtomLink();
  feed->add_link(link);
  link->set_rel(kRel);
  // Have rel=, but not type=.
  ASSERT_FALSE(AtomUtil::FindLink(*feed, kRel, kMimeType));
  link->clear_rel();
  link->set_type(kMimeType);
  // Have type=, but not rel=.
  ASSERT_FALSE(AtomUtil::FindLink(*feed, kRel, kMimeType));
  link->set_rel(kRel);
  // Have both rel= and type=.
  kmldom::AtomLinkPtr got_link = AtomUtil::FindLink(*feed, kRel, kMimeType);
  ASSERT_TRUE(got_link.get());
  ASSERT_EQ(kRel, got_link->get_rel());
  ASSERT_EQ(kMimeType, got_link->get_type());
  kmldom::AtomEntryPtr entry =
      kmldom::KmlFactory::GetFactory()->CreateAtomEntry();
  ASSERT_FALSE(AtomUtil::FindLink(*entry, kRel, kMimeType));
  link = kmldom::KmlFactory::GetFactory()->CreateAtomLink();
  entry->add_link(link);
  link->set_rel(kRel);
  link->set_type(kMimeType);
  got_link = AtomUtil::FindLink(*feed, kRel, kMimeType);
  ASSERT_TRUE(got_link.get());
  ASSERT_EQ(kRel, got_link->get_rel());
  ASSERT_EQ(kMimeType, got_link->get_type());
}

TEST(AtomUtilTest, TestFindEntryByTitle) {
  kmldom::KmlFactory* factory = kmldom::KmlFactory::GetFactory();
  kmldom::AtomEntryPtr entry = factory->CreateAtomEntry();
  const string kTitle("War and Peace");
  entry->set_title(kTitle);
  kmldom::AtomFeedPtr feed = factory->CreateAtomFeed();
  feed->add_entry(entry);
  kmldom::AtomEntryPtr got_entry = AtomUtil::FindEntryByTitle(feed, kTitle);
  ASSERT_TRUE(got_entry.get());
  ASSERT_EQ(kTitle, got_entry->get_title());
  ASSERT_FALSE(AtomUtil::FindEntryByTitle(feed, "Peaceful Warring"));
}

TEST(AtomUtilTest, TestFindCategoryByScheme) {
  kmldom::KmlFactory* factory = kmldom::KmlFactory::GetFactory();
  kmldom::AtomEntryPtr entry = factory->CreateAtomEntry();
  kmldom::AtomCategoryPtr category = factory->CreateAtomCategory();
  const string kScheme("http://schemas.google.com/g/2005#kind");
  category->set_scheme(kScheme);
  const string kLabel("document");
  category->set_label(kLabel);
  entry->add_category(category);
  kmldom::AtomCategoryPtr got_category = AtomUtil::FindCategoryByScheme(
      *entry, "kind");
  ASSERT_TRUE(got_category.get());
  ASSERT_EQ(kLabel, got_category->get_label());
}

TEST(AtomUtilTest, TestGetAndParseFeed) {
  TestDataHttpClient test_data_http_client;
  kmldom::AtomFeedPtr feed = AtomUtil::GetAndParseFeed(
      "http://example.com/gdata/picasaweb-metafeed.xml",
      test_data_http_client);
  ASSERT_TRUE(feed);
  ASSERT_EQ(
      string("http://picasaweb.google.com/data/feed/user/ben.gardenfield"),
      feed->get_id());
  ASSERT_EQ(static_cast<size_t>(17), feed->get_entry_array_size());

  ASSERT_FALSE(AtomUtil::GetAndParseFeed("http://example.com/no/such/file",
                                         test_data_http_client));
  // This is an Atom <entry>.
  ASSERT_FALSE(AtomUtil::GetAndParseFeed(
      "http://example.com/gmaps/create-map-result.xml",
      test_data_http_client));
}

TEST(AtomUtilTest, TestGetNextFeed) {
  TestDataHttpClient test_data_http_client;
  kmldom::AtomFeedPtr feed = AtomUtil::GetAndParseFeed(
      "http://example.com/gdata/feed0.xml",
      test_data_http_client);
  ASSERT_TRUE(feed);
  // feed0.xml points to feed1.xml
  kmldom::AtomFeedPtr next = AtomUtil::GetNextFeed(feed, test_data_http_client);
  ASSERT_TRUE(next);
  ASSERT_EQ(static_cast<size_t>(1), next->get_link_array_size());
  kmldom::AtomLinkPtr link = next->get_link_array_at(0);
  ASSERT_TRUE(link);
  // feed1.xml points to feed2.xml
  ASSERT_EQ(string("gdata/feed2.xml"), link->get_href());
}

TEST(AtomUtilTest, TestGetGdResourceId) {
  TestDataHttpClient test_data_http_client;
  kmldom::AtomFeedPtr feed = AtomUtil::GetAndParseFeed(
      "http://example.com/gdata/doclist-metafeed.xml",
      test_data_http_client);
  ASSERT_TRUE(feed);
  ASSERT_EQ(static_cast<size_t>(4), feed->get_entry_array_size());
  string resource_id;
  // The 0th <atom:entry> in doclist-metafeed.xml has this child:
  // <gd:resourceId>document:0ARX2bBe7ATEpZHg1a3poY18xOWNwZ2NuN2Qy</gd:resourceId>
  ASSERT_TRUE(AtomUtil::GetGdResourceId(feed->get_entry_array_at(0),
                                        &resource_id));
  ASSERT_EQ(string("document:0ARX2bBe7ATEpZHg1a3poY18xOWNwZ2NuN2Qy"),
            resource_id);
}

}  // end namespace kmlconvenience

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
