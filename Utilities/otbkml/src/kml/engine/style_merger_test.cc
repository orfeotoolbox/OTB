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

// This file contains the unit tests for the StyleMerger class.

#include "kml/engine/style_merger.h"
#include "boost/scoped_ptr.hpp"
#include "kml/base/net_cache_test_util.h"
#include "kml/base/unit_test.h"
#include "kml/dom.h"
#include "kml/engine/kml_file.h"

using kmldom::FeaturePtr;
using kmldom::KmlFactory;
using kmldom::PairPtr;
using kmldom::LineStylePtr;
using kmldom::StylePtr;
using kmldom::StyleMapPtr;
using kmldom::StyleSelectorPtr;

namespace kmlengine {

static const size_t kKmlNetCacheCacheSize = 10;

class StyleMergerTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(StyleMergerTest);
  CPPUNIT_TEST(TestConstructor);
  CPPUNIT_TEST(TestMergeStyleNullEmptyBad);
  CPPUNIT_TEST(TestMergeStyleBasic);
  CPPUNIT_TEST(TestMergeStyleMapNullEmpty);
  CPPUNIT_TEST(TestMergeStyleMapEmptyStyle);
  CPPUNIT_TEST(TestMergeStyleMapEmptyStyleMap);
  CPPUNIT_TEST(TestMergeStyleMapBasic);
  CPPUNIT_TEST(TestMergeStyleSelector);

  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestConstructor();
  void TestMergeStyleNullEmptyBad();
  void TestMergeStyleBasic();
  void TestMergeStyleMapNullEmpty();
  void TestMergeStyleMapEmptyStyle();
  void TestMergeStyleMapEmptyStyleMap();
  void TestMergeStyleMapBasic();
  void TestMergeStyleSelector();

 public:
  // Called before each test.
  void setUp() {
    kml_file_net_cache_.reset(new KmlFileNetCache(&test_data_net_fetcher_,
                                                  kKmlNetCacheCacheSize));
    kml_file_ = KmlFile::Create();
    style_merger_normal_.reset(
        new StyleMerger(kml_file_, kmldom::STYLESTATE_NORMAL));
    style_merger_highlight_.reset(
        new StyleMerger(kml_file_, kmldom::STYLESTATE_HIGHLIGHT));
    factory_ = KmlFactory::GetFactory();
    style_ = factory_->CreateStyle();
    stylemap_ = factory_->CreateStyleMap();
  }

  // Called after each test.
  void tearDown() {
    // Smart pointers free everything.
  }

 private:
  PairPtr CreateStylePair(kmldom::StyleStateEnum style_state,
                          const StyleSelectorPtr& styleselector) const;
  StylePtr CreateStyleLineStyle(const std::string& id,
                                const std::string& color,
                                double width) const;
  void VerifyEmptyStyle(const StylePtr& style) const;
  void VerifyStyleLineStyle(const StylePtr& style, const std::string& id,
                            const std::string& color, double width) const;
  void VerifyStyleMergersEmpty() const;
  KmlFilePtr kml_file_;
  kmlbase::TestDataNetFetcher test_data_net_fetcher_;
  boost::scoped_ptr<KmlFileNetCache> kml_file_net_cache_;
  KmlFactory* factory_;
  boost::scoped_ptr<StyleMerger> style_merger_normal_;
  boost::scoped_ptr<StyleMerger> style_merger_highlight_;
  StylePtr style_;
  StyleMapPtr stylemap_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(StyleMergerTest);

// This is a helper function to verify that Style has no SubStyles.
void StyleMergerTest::VerifyEmptyStyle(const StylePtr& style) const {
  CPPUNIT_ASSERT(!style->has_iconstyle());
  CPPUNIT_ASSERT(!style->has_labelstyle());
  CPPUNIT_ASSERT(!style->has_linestyle());
  CPPUNIT_ASSERT(!style->has_polystyle());
  CPPUNIT_ASSERT(!style->has_balloonstyle());
  CPPUNIT_ASSERT(!style->has_liststyle());
}

// This is a helper function to verify empty state of the StyleMergers in the
// test fixture.
void StyleMergerTest::VerifyStyleMergersEmpty() const {
  StylePtr style = style_merger_normal_->GetResolvedStyle();
  CPPUNIT_ASSERT(style);
  VerifyEmptyStyle(style);
  style = style_merger_highlight_->GetResolvedStyle();
  CPPUNIT_ASSERT(style);
  VerifyEmptyStyle(style);
}

// Verify that the Style has the given id and LineStyle with the given color
// and width.
void StyleMergerTest::VerifyStyleLineStyle(const StylePtr& style,
                                           const std::string& id,
                                           const std::string& color,
                                           double width) const {
  CPPUNIT_ASSERT_EQUAL(id, style->get_id());
  CPPUNIT_ASSERT(style->get_linestyle());
  CPPUNIT_ASSERT_EQUAL(color, style->get_linestyle()->get_color());
  CPPUNIT_ASSERT_EQUAL(width, style->get_linestyle()->get_width());
}

// This is a helper function to create a Pair with key and StyleSelector.
PairPtr StyleMergerTest::CreateStylePair(
      kmldom::StyleStateEnum style_state,
      const StyleSelectorPtr& styleselector) const {
  PairPtr pair = factory_->CreatePair();
  pair->set_key(style_state);
  pair->set_styleselector(styleselector);
  return pair;
}

// This helper function creates Style with the given id and gives it a
// LineStyle of the given color and width.
StylePtr StyleMergerTest::CreateStyleLineStyle(const std::string& id,
                                               const std::string& color,
                                               double width) const {
  StylePtr style = factory_->CreateStyle();
  style->set_id(id);
  LineStylePtr linestyle = factory_->CreateLineStyle();
  linestyle->set_color(color);
  linestyle->set_width(width);
  style->set_linestyle(linestyle);
  return style;
}

// Verify a freshly constructed StyleMerger's GetResolvedStyle() returns an
// empty Style.
void StyleMergerTest::TestConstructor() {
  VerifyStyleMergersEmpty();
}

// Verify the MergeStyle() method on NULL, empty and bad arguments.
void StyleMergerTest::TestMergeStyleNullEmptyBad() {
  std::string empty_styleurl;

  // Verify that an empty styleurl and NULL StyleSelector behaves fine.
  style_merger_normal_->MergeStyle(empty_styleurl, NULL);
  VerifyStyleMergersEmpty();

  // Verify that MergeStyle takes an empty Style and empty StyleMap.
  style_merger_normal_->MergeStyle(empty_styleurl, style_);
  style_merger_normal_->MergeStyle(empty_styleurl, stylemap_);
  VerifyStyleMergersEmpty();

  // Verify that MergeStyle() is well behaved with a garbage styleUrl.
  std::string garbage("not at all a URL");
  style_merger_normal_->MergeStyle(empty_styleurl, NULL);
  VerifyStyleMergersEmpty();

  // Verify that MergeStyle() is well behaved with a validly formed styleurl
  // for an object that does not exist.
  std::string valid_fragment("#no-such-object");
  style_merger_normal_->MergeStyle(valid_fragment, NULL);
  VerifyStyleMergersEmpty();
}

// Test basic usage of the MergeStyle() method with various valid arguments.
void StyleMergerTest::TestMergeStyleBasic() {
  // Set one SubStyle (LineStyle), merge it in and verify it comes out the
  // other end.  No shared styles are in the KML file.
  style_->set_linestyle(factory_->CreateLineStyle());
  style_merger_normal_->MergeStyle("", style_);
  StylePtr style = style_merger_normal_->GetResolvedStyle();
  CPPUNIT_ASSERT(!style->has_iconstyle());
  CPPUNIT_ASSERT(!style->has_labelstyle());
  CPPUNIT_ASSERT(style->has_linestyle());
  CPPUNIT_ASSERT(!style->has_polystyle());
  CPPUNIT_ASSERT(!style->has_balloonstyle());
  CPPUNIT_ASSERT(!style->has_liststyle());

  // Set another.
  style_->set_polystyle(factory_->CreatePolyStyle());
  style_merger_normal_->MergeStyle("", style_);
  style = style_merger_normal_->GetResolvedStyle();
  CPPUNIT_ASSERT(!style->has_iconstyle());
  CPPUNIT_ASSERT(!style->has_labelstyle());
  CPPUNIT_ASSERT(style->has_linestyle());
  CPPUNIT_ASSERT(style->has_polystyle());
  CPPUNIT_ASSERT(!style->has_balloonstyle());
  CPPUNIT_ASSERT(!style->has_liststyle());

  // Set all SubStyles.
  style_->set_iconstyle(factory_->CreateIconStyle());
  style_->set_labelstyle(factory_->CreateLabelStyle());
  style_->set_balloonstyle(factory_->CreateBalloonStyle());
  style_->set_liststyle(factory_->CreateListStyle());

  style_merger_normal_->MergeStyle("", style_);
  style = style_merger_normal_->GetResolvedStyle();
  CPPUNIT_ASSERT(style->has_iconstyle());
  CPPUNIT_ASSERT(style->has_labelstyle());
  CPPUNIT_ASSERT(style->has_linestyle());
  CPPUNIT_ASSERT(style->has_polystyle());
  CPPUNIT_ASSERT(style->has_balloonstyle());
  CPPUNIT_ASSERT(style->has_liststyle());

  // Verify all behaves well a StyleMerger in highlight mode.
  style_merger_highlight_->MergeStyle("", style_);
  style = style_merger_highlight_->GetResolvedStyle();
  CPPUNIT_ASSERT(style->has_iconstyle());
  CPPUNIT_ASSERT(style->has_labelstyle());
  CPPUNIT_ASSERT(style->has_linestyle());
  CPPUNIT_ASSERT(style->has_polystyle());
  CPPUNIT_ASSERT(style->has_balloonstyle());
  CPPUNIT_ASSERT(style->has_liststyle());
}

// Verify the MergeStyleMap() method on NULL and empty args.
void StyleMergerTest::TestMergeStyleMapNullEmpty() {
  style_merger_normal_->MergeStyleMap(NULL);
  style_merger_highlight_->MergeStyleMap(NULL);
  VerifyStyleMergersEmpty();

  // An internal check to verify that the StyleMap is empty.
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0),
                       stylemap_->get_pair_array_size());
  style_merger_normal_->MergeStyleMap(stylemap_);
  style_merger_highlight_->MergeStyleMap(stylemap_);
  VerifyStyleMergersEmpty();

  // Create a StyleMap with no StyleSelector for the normal and highlight keys.
  stylemap_->add_pair(CreateStylePair(kmldom::STYLESTATE_NORMAL, NULL));
  stylemap_->add_pair(CreateStylePair(kmldom::STYLESTATE_HIGHLIGHT, NULL));
  style_merger_normal_->MergeStyleMap(stylemap_);
  style_merger_highlight_->MergeStyleMap(stylemap_);
  VerifyStyleMergersEmpty();
}

// Create a StyleMap with an empty Style for the normal and highlight keys.
void StyleMergerTest::TestMergeStyleMapEmptyStyle() {
  stylemap_->add_pair(CreateStylePair(kmldom::STYLESTATE_NORMAL,
                                      factory_->CreateStyle()));
  stylemap_->add_pair(CreateStylePair(kmldom::STYLESTATE_HIGHLIGHT,
                                      factory_->CreateStyle()));
  style_merger_normal_->MergeStyleMap(stylemap_);
  style_merger_highlight_->MergeStyleMap(stylemap_);
  VerifyStyleMergersEmpty();
}

// Create a StyleMap with an empty StyleMap for the normal and highlight keys.
void StyleMergerTest::TestMergeStyleMapEmptyStyleMap() {
  stylemap_->add_pair(CreateStylePair(kmldom::STYLESTATE_NORMAL,
                                      factory_->CreateStyleMap()));
  stylemap_->add_pair(CreateStylePair(kmldom::STYLESTATE_HIGHLIGHT,
                                      factory_->CreateStyleMap()));
  style_merger_normal_->MergeStyleMap(stylemap_);
  style_merger_highlight_->MergeStyleMap(stylemap_);
  VerifyStyleMergersEmpty();
}

// Verify basic usage of the MergeStyleMap() method with a simple StyleMap.
void StyleMergerTest::TestMergeStyleMapBasic() {
  // Create a StyleMap with different values for normal and highlight LineStyle.
  const std::string kNormalId("normal-id");           
  const std::string kNormalColor("1f331122");         
  const double kNormalWidth(2.3);
  const std::string kHighlightId("highlight-id");
  const std::string kHighlightColor("7f112233");
  const double kHighlightWidth(4.4);
  stylemap_->add_pair(
      CreateStylePair(kmldom::STYLESTATE_NORMAL,
                      CreateStyleLineStyle(kNormalId, kNormalColor,
                                           kNormalWidth)));
  stylemap_->add_pair(
      CreateStylePair(kmldom::STYLESTATE_HIGHLIGHT,
                      CreateStyleLineStyle(kHighlightId, kHighlightColor,
                                           kHighlightWidth)));
  // Merge the StyleMap into the normal merger.
  style_merger_normal_->MergeStyleMap(stylemap_);
  // Merge the same StyleMap into the highlight merger.
  style_merger_highlight_->MergeStyleMap(stylemap_);
  // Verify the normal values appear properly in the normal resolved style.
  VerifyStyleLineStyle(style_merger_normal_->GetResolvedStyle(), kNormalId,
                       kNormalColor, kNormalWidth);
  // Verify the highlight values appear properly in the highlight resolved
  // style.
  VerifyStyleLineStyle(style_merger_highlight_->GetResolvedStyle(),
                       kHighlightId, kHighlightColor, kHighlightWidth);
}

// Verify the MergeStyleSelector() method with NULL and empty arguments.
void StyleMergerTest::TestMergeStyleSelector() {
  style_merger_normal_->MergeStyleSelector(NULL);
  style_merger_highlight_->MergeStyleSelector(NULL);
  VerifyStyleMergersEmpty();

  style_merger_normal_->MergeStyleSelector(style_);
  style_merger_highlight_->MergeStyleSelector(style_);
  VerifyStyleMergersEmpty();

  style_merger_normal_->MergeStyleSelector(stylemap_);
  style_merger_highlight_->MergeStyleSelector(stylemap_);
  VerifyStyleMergersEmpty();
}

}  // endnamespace kmlengine

TEST_MAIN
