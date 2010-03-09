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

// This file contains the unit tests for the StyleSplitter class.

#include "kml/engine/style_splitter.h"
#include "kml/engine/style_splitter_internal.h"
#include "boost/scoped_ptr.hpp"
#include "gtest/gtest.h"
#include "kml/base/net_cache_test_util.h"
#include "kml/base/string_util.h"
#include "kml/dom.h"
#include "kml/engine/kml_file.h"

using kmlbase::ToString;
using kmldom::AsDocument;
using kmldom::DocumentPtr;
using kmldom::ElementPtr;
using kmldom::FeaturePtr;
using kmldom::FolderPtr;
using kmldom::KmlFactory;
using kmldom::Parser;
using kmldom::PlacemarkPtr;
using kmldom::LineStylePtr;
using kmldom::StylePtr;
using kmldom::StyleMapPtr;
using kmldom::StyleSelectorPtr;

namespace kmlengine {

class StyleSplitterTest : public testing::Test {
 protected:
  virtual void SetUp() {
    kml_factory_ = KmlFactory::GetFactory();
    style_splitter_.reset(new StyleSplitter(&shared_style_map_));
    style_splitting_parser_.reset(new Parser);
    style_splitting_parser_->AddObserver(style_splitter_.get());
  }

  KmlFactory* kml_factory_;
  SharedStyleMap shared_style_map_;
  boost::scoped_ptr<StyleSplitter> style_splitter_;
  boost::scoped_ptr<Parser> style_splitting_parser_;
};

// Call StyleSplitter methods directly in a simple typical case.
TEST_F(StyleSplitterTest, CallStyleSplitterMethodsInTypicalUsage) {
  // Call the StyleSplitter methods involved in parsing this:
  // <Document>      <!-- 1 -->
  //   <Placemark>   <!-- 2 -->
  //     <Style/>    <!-- 3,4 -->
  //   </Placemark>  <!-- 5 -->
  // </Document>     <!-- 6 -->
  // ...to create this:
  const string kExpectedKml(
    "<Document id=\"d\">\n"
    "  <Style id=\"_0\"/>\n"
    "  <Placemark>\n"
    "    <styleUrl>#_0</styleUrl>\n"
    "  </Placemark>\n"
    "</Document>\n");

  // 1) On "<Document>" the parser creates a Document and calls NewElement().
  DocumentPtr document = kml_factory_->CreateDocument();
  const string kDocId("d");
  document->set_id(kDocId);
  ASSERT_TRUE(style_splitter_->NewElement(document));
  ASSERT_TRUE(style_splitter_->get_document());
  ASSERT_EQ(kDocId, style_splitter_->get_document()->get_id());

  // 2) On "<Placemark>" the parser creates a Placemark and calls NewElement().
  PlacemarkPtr placemark = kml_factory_->CreatePlacemark();
  ASSERT_TRUE(style_splitter_->NewElement(placemark));

  // 3) On "<Style>" the parser creates a Style and calls NewElement().
  StylePtr style = kml_factory_->CreateStyle();
  ASSERT_TRUE(style_splitter_->NewElement(style));

  // 4) On "</Style>" the parser calls EndElement() to ask if style should be
  // made a child of placemark.  Verify the answer is false.
  ASSERT_FALSE(style_splitter_->EndElement(placemark, style));

  // This is exactly the situation StyleSplitter handles.  Verify that the
  // Document now has a shared style and that the map has a pointer to it
  // as well.
  const string kId0("_0");
  ASSERT_EQ(static_cast<size_t>(1), shared_style_map_.size());
  ASSERT_TRUE(shared_style_map_[kId0]);
  ASSERT_EQ(kId0, shared_style_map_[kId0]->get_id());
  ASSERT_EQ(static_cast<size_t>(1), document->get_styleselector_array_size());
  ASSERT_EQ(kId0, document->get_styleselector_array_at(0)->get_id());

  // Verify that the placemark instead got a styleUrl
  ASSERT_TRUE(placemark->has_styleurl());
  ASSERT_EQ(string("#") + kId0, placemark->get_styleurl());

  // 5) On "</Placemark>" the parser calls EndElement() to ask if placemark
  // should be made a child of document.  Verify the answer is true.
  ASSERT_TRUE(style_splitter_->EndElement(document, placemark));
  // Play like the parser and add the placemark to the document:
  document->add_feature(placemark);

  // 6) StyleSplitter is not called by the parser on </Document> given that
  // this is the root element and has no parent.  But, the parser does add

  // Serialize the document and verify a proper overall result.
  ASSERT_EQ(kExpectedKml, SerializePretty(document));
}

TEST_F(StyleSplitterTest, CallStyleSplitterMethodsInComplexTypicalUsage) {
  // Call StyleSplitter methods directly in a more complex typical case.
  // Here the <Docment> already has a shared Style and we verify that this
  // is not disturbed.  And, there is an additional Placemark with no style.
  // Call the StyleSplitter methods involved in parsing this:
  // <Document>                    <!-- 1 -->
  //   <StyleMap id="stylemap0"/>  <!-- 2 -->
  //   <Placemark>                 <!-- 3 -->
  //     <name>no style</name>
  //   </Placemark>
  //   <Placemark>                 <!-- 4 -->
  //     <Style>                   <!-- 5 -->
  //       <LineStyle/>
  //     <Style/>
  //   </Placemark>
  //   <Placemark>                 <!-- 6 -->
  //      <name>has shared stylemap</name>
  //      <styleUrl>#stylemap0</styleUrl>
  //   </Placemark>
  // </Document>                   <!-- 7 -->
  // ...to create this:
  const string kExpectedKml(
    "<Document>\n"
    "  <StyleMap id=\"stylemap0\"/>\n"
    "  <Style id=\"_0\"/>\n"
    "  <Placemark>\n"
    "    <name>no style</name>\n"
    "  </Placemark>\n"
    "  <Placemark>\n"
    "    <styleUrl>#_0</styleUrl>\n"
    "  </Placemark>\n"
    "  <Placemark>\n"
    "    <name>has shared stylemap</name>\n"
    "    <styleUrl>#stylemap0</styleUrl>\n"
    "  </Placemark>\n"
    "</Document>\n");

  // 1) On "<Document>" the parser creates a Document and calls NewElement().
  DocumentPtr document = kml_factory_->CreateDocument();
  ASSERT_TRUE(style_splitter_->NewElement(document));

  // 2a) On "<StyleMap>" the parser creates a Style and calls NewElement().
  const string kStyleMap0("stylemap0");
  StyleMapPtr stylemap = kml_factory_->CreateStyleMap();
  stylemap->set_id(kStyleMap0);
  ASSERT_TRUE(style_splitter_->NewElement(stylemap));
  // 2b) On "</StyleMap>" the parser calls EndElement().
  ASSERT_TRUE(style_splitter_->EndElement(document, stylemap));
  document->add_styleselector(stylemap);  // Act like the parser.
  shared_style_map_[kStyleMap0] = stylemap;  // Act like KmlFile.
  ASSERT_EQ(static_cast<size_t>(1), document->get_styleselector_array_size());
  ASSERT_EQ(static_cast<size_t>(1), shared_style_map_.size());

  // 3) <Placemark> with name and no StyleSelector
  PlacemarkPtr placemark0 = kml_factory_->CreatePlacemark();
  placemark0->set_name("no style");
  ASSERT_TRUE(style_splitter_->NewElement(placemark0));
  ASSERT_TRUE(style_splitter_->EndElement(document, placemark0));
  document->add_feature(placemark0);  // Act like the parser.

  // 4) <Placemark>
  PlacemarkPtr placemark1 = kml_factory_->CreatePlacemark();
  ASSERT_TRUE(style_splitter_->NewElement(placemark1));

  // 5) <Style> to go inline in <Placemark>
  StylePtr style = kml_factory_->CreateStyle();
  style->set_linestyle(kml_factory_->CreateLineStyle());
  ASSERT_TRUE(style_splitter_->NewElement(style));
  ASSERT_FALSE(style_splitter_->EndElement(placemark1, style));
  // Act like the parser and do NOT add this style, but verify that
  // StyleSplitter created the expected styleUrl.
  ASSERT_TRUE(placemark1->has_styleurl());
  const string k0("_0");
  ASSERT_EQ(string("#") + k0, placemark1->get_styleurl());
  // Verify the Style was moved to the Document and to the shared style map.
  ASSERT_EQ(static_cast<size_t>(2), document->get_styleselector_array_size());
  ASSERT_EQ(static_cast<size_t>(2), shared_style_map_.size());
  ASSERT_EQ(kStyleMap0, shared_style_map_[kStyleMap0]->get_id());
  ASSERT_EQ(k0, shared_style_map_[k0]->get_id());

  PlacemarkPtr placemark2 = kml_factory_->CreatePlacemark();
}

// Verify that lack of <Document> is overall essentially a NOP.
TEST_F(StyleSplitterTest, TestNoDocument) {
  const string kNoStyleFolder(
    "<Folder>\n"
    "  <name>f</name>\n"
    "  <Placemark>\n"
    "    <Style>\n"
    "      <IconStyle/>\n"
    "    </Style>\n"
    "  </Placemark>\n"
    "</Folder>\n");
  string errors;
  ElementPtr root = style_splitting_parser_->Parse(kNoStyleFolder, &errors);
  // Verify that this still parses just fine.
  FolderPtr folder = AsFolder(root);
  ASSERT_TRUE(folder);
  ASSERT_TRUE(folder->has_name());
  ASSERT_FALSE(folder->has_description());
  ASSERT_EQ(static_cast<size_t>(1), folder->get_feature_array_size());
  PlacemarkPtr placemark = AsPlacemark(folder->get_feature_array_at(0));
  ASSERT_TRUE(placemark);
  // No <Document> to split this <Style> to so it remains in <Placemark>:
  ASSERT_TRUE(placemark->has_styleselector());
  ASSERT_EQ(kNoStyleFolder, SerializePretty(root));
}

TEST_F(StyleSplitterTest, TestBasicPlacemark) {
  string errors;
  const string kInlineStyle(
    "<Document>"
    "  <Placemark>"
    "    <name>split me</name>"
    "    <Style>"
    "      <LineStyle>"
    "        <color>7f123456</color>"
    "      </LineStyle>"
    "    </Style>"
    "    <LineString><coordinates/></LineString>"
    "  </Placemark>"
    "</Document>");
  ElementPtr root = style_splitting_parser_->Parse(kInlineStyle, &errors);
  DocumentPtr document = AsDocument(root);
  ASSERT_TRUE(document);
  PlacemarkPtr placemark = AsPlacemark(document->get_feature_array_at(0));
  ASSERT_TRUE(placemark);
  ASSERT_TRUE(placemark->has_name());
  ASSERT_FALSE(placemark->has_description());
  ASSERT_TRUE(placemark->has_styleurl());
  ASSERT_EQ(string("#_0"), placemark->get_styleurl());
  ASSERT_FALSE(placemark->has_styleselector());
  ASSERT_EQ(static_cast<size_t>(1), shared_style_map_.size());
  ASSERT_EQ(static_cast<size_t>(1), document->get_styleselector_array_size());
  StylePtr style = AsStyle(document->get_styleselector_array_at(0));
  ASSERT_TRUE(style);
  ASSERT_TRUE(style->has_id());
  ASSERT_EQ(string("_0"), style->get_id());
  ASSERT_TRUE(style->has_linestyle());
  ASSERT_TRUE(style->get_linestyle()->has_color());
  ASSERT_FALSE(style->get_linestyle()->has_width());
  const string kExpectedKml(
    "<Document>\n"
    "  <Style id=\"_0\">\n"
    "    <LineStyle>\n"
    "      <color>7f123456</color>\n"
    "    </LineStyle>\n"
    "  </Style>\n"
    "  <Placemark>\n"
    "    <name>split me</name>\n"
    "    <styleUrl>#_0</styleUrl>\n"
    "    <LineString>\n"
    "      <coordinates/>\n"
    "    </LineString>\n"
    "  </Placemark>\n"
    "</Document>\n");
  ASSERT_EQ(kExpectedKml, SerializePretty(root));
}

TEST_F(StyleSplitterTest, TestIdCollision) {
  const string kId("_0");
  StylePtr style = kml_factory_->CreateStyle();
  style->set_id(kId);
  shared_style_map_[kId] = style;
  const string kKml(
      "<Document>\n"
      "  <Placemark>\n"
      "    <Style/>\n"
      "  </Placemark>\n"
      "</Document>\n");
  DocumentPtr document = AsDocument(style_splitting_parser_->Parse(kKml, NULL));
  ASSERT_TRUE(document);
  PlacemarkPtr placemark = AsPlacemark(document->get_feature_array_at(0));
  ASSERT_TRUE(placemark);
  ASSERT_EQ(kKml, SerializePretty(document));
  ASSERT_FALSE(placemark->has_styleurl());
  ASSERT_TRUE(placemark->has_styleselector());
  ASSERT_TRUE(AsStyle(placemark->get_styleselector()));
  ASSERT_EQ(static_cast<size_t>(1), shared_style_map_.size());
  style = AsStyle(shared_style_map_[kId]);
  ASSERT_TRUE(style);
  ASSERT_EQ(kId, style->get_id());
  ASSERT_EQ(static_cast<size_t>(0), document->get_styleselector_array_size());
}

TEST_F(StyleSplitterTest, TestMultipleFeatures) {
  DocumentPtr document = kml_factory_->CreateDocument();
  const size_t kCount(123);
  for (size_t i = 0; i < kCount; ++i) {
    PlacemarkPtr placemark = kml_factory_->CreatePlacemark();
    placemark->set_name(ToString(i));
    StylePtr style = kml_factory_->CreateStyle();
    placemark->set_styleselector(style);
    document->add_feature(placemark);
  }
  const string kDocument(SerializePretty(document));
  document = AsDocument(style_splitting_parser_->Parse(kDocument, NULL));
  ASSERT_TRUE(document);
  ASSERT_EQ(kCount, document->get_styleselector_array_size());
  ASSERT_EQ(kCount, document->get_feature_array_size());
  for (size_t i = 0; i < kCount; ++i) {
    PlacemarkPtr placemark = AsPlacemark(document->get_feature_array_at(i));
    ASSERT_TRUE(placemark);
    ASSERT_EQ(ToString(i), placemark->get_name());
    ASSERT_FALSE(placemark->has_styleselector());
    ASSERT_TRUE(placemark->has_styleurl());
    const string kExpectedStyleId("_" + ToString(i));
    ASSERT_EQ(string("#") + kExpectedStyleId, placemark->get_styleurl());
    StylePtr style = AsStyle(document->get_styleselector_array_at(i));
    ASSERT_TRUE(style);
    ASSERT_EQ(kExpectedStyleId, style->get_id());
  }
}

// TODO: more stuff to test
#if 0
  // util
  StyleSplitter::AsNonDocumentFeature();

  StyleSplitter::CreateStyleSelector();

  // Document, Update...
  style_splitter_->NewElement();

  style_splitter_->CreateUniqueId();

  style_splitter_->EndElement();
#endif
}  // end namespace kmlengine

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
