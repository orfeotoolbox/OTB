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

// This file contains the unit tests for the StyleInliner class.

#include "kml/engine/style_inliner.h"
#include "kml/engine/style_inliner_internal.h"
#include "gtest/gtest.h"
#include "kml/base/file.h"

// The following define is a convenience for testing inside Google.
#ifdef GOOGLE_INTERNAL
#include "kml/base/google_internal_test.h"
#endif

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

using kmlbase::File;
using kmldom::DocumentPtr;
using kmldom::ElementPtr;
using kmldom::FieldPtr;
using kmldom::KmlFactory;
using kmldom::PlacemarkPtr;
using kmldom::StylePtr;
using kmldom::Type_styleUrl;
using kmldom::UpdatePtr;

namespace kmlengine {

class StyleInlinerTest : public testing::Test {
 protected:
  virtual void SetUp() {
    kml_factory_ = KmlFactory::GetFactory();
    style_inliner_.reset(new StyleInliner);
  }

  KmlFactory* kml_factory_;
  boost::scoped_ptr<StyleInliner> style_inliner_;
};

TEST_F(StyleInlinerTest, CallStyleInlinerMethodsInTypicalUsage) {
  // Call the StyleInliner methods involved in parsing this:
  // <Document>
  //   <Style id="_0"/>
  //   <Placemark>
  //     <styleUrl>#_0</styleUrl>
  //   </Placemark>
  // </Document>
  // ...to generate this:
  const string kExpectedKml(
    "<Document>\n"
    "  <Placemark>\n"
    "    <StyleMap>\n"
    "      <Pair>\n"
    "        <key>normal</key>\n"
    "        <Style/>\n"
    "      </Pair>\n"
    "      <Pair>\n"
    "        <key>highlight</key>\n"
    "        <Style/>\n"
    "      </Pair>\n"
    "    </StyleMap>\n"
    "  </Placemark>\n"
    "</Document>\n");

  // 0) Initial conditions.
  ASSERT_FALSE(style_inliner_->get_document());
  ASSERT_EQ(static_cast<size_t>(0),
            style_inliner_->get_shared_styles().size());
  ASSERT_FALSE(style_inliner_->in_update());

  // 1) <Document>
  DocumentPtr document = kml_factory_->CreateDocument();
  ASSERT_TRUE(style_inliner_->NewElement(document));
  ASSERT_TRUE(style_inliner_->get_document());

  // 2) <Style id="_0">
  StylePtr style = kml_factory_->CreateStyle();
  const string kStyleId("_0");
  style->set_id(kStyleId);
  ASSERT_TRUE(style_inliner_->NewElement(style));

  // 3) </Style>
  ASSERT_FALSE(style_inliner_->EndElement(document, style));

  // 4) <Placemark>
  PlacemarkPtr placemark = kml_factory_->CreatePlacemark();
  ASSERT_TRUE(style_inliner_->NewElement(placemark));

  // 5) <styleUrl>#_0</styleUrl>
  FieldPtr styleurl = kml_factory_->CreateFieldById(Type_styleUrl);
  styleurl->set_char_data(string("#") + kStyleId);
  ASSERT_TRUE(style_inliner_->NewElement(styleurl));
  ASSERT_FALSE(style_inliner_->EndElement(placemark, styleurl));
  // StyleInliner::EndElement() gives the placemark a style selector
  // representing that which the styleUrl pointed to.
  ASSERT_TRUE(placemark->has_styleselector());
  // The local style should have no ids.
  ASSERT_FALSE(placemark->get_styleselector()->has_id());

  // 6) </Placemark>
  ASSERT_TRUE(style_inliner_->EndElement(document, placemark));
  document->add_feature(placemark);

  // Verify the shared style was captured.
  const SharedStyleMap& shared_styles = style_inliner_->get_shared_styles();
  ASSERT_EQ(static_cast<size_t>(1), shared_styles.size());
  ASSERT_EQ(kStyleId, shared_styles.find(kStyleId)->second->get_id());

  // Serialize the document and verify a proper overall result.
  ASSERT_EQ(kExpectedKml, SerializePretty(document));
}

TEST_F(StyleInlinerTest, VerifyRemoteStyleUrlNotInlined) {
  PlacemarkPtr placemark = kml_factory_->CreatePlacemark();
  FieldPtr styleurl = kml_factory_->CreateFieldById(Type_styleUrl);
  styleurl->set_char_data("http://example.com/style.kml#cool-style");
  // true == "proceed to add this styleurl to its feature"
  ASSERT_TRUE(style_inliner_->EndElement(placemark, styleurl));

  // true == "proceed to add this styleurl to its feature"
  styleurl->set_char_data("#non-existent-local-reference");
  ASSERT_TRUE(style_inliner_->EndElement(placemark, styleurl));
}

TEST_F(StyleInlinerTest, VerifyNoInliningWithinUpdate) {
  // <Update>
  UpdatePtr update = kml_factory_->CreateUpdate();
  ASSERT_TRUE(style_inliner_->NewElement(update));
  ASSERT_TRUE(style_inliner_->in_update());

  // Since this is somewhere inside an <Update> the <Style> will be added
  // to the <Document> as normal.
  DocumentPtr document = kml_factory_->CreateDocument();
  StylePtr style = kml_factory_->CreateStyle();
  const string kStyleId("style0");
  style->set_id(kStyleId);
  ASSERT_TRUE(style_inliner_->EndElement(document, style));

  // </Update>
  ASSERT_TRUE(style_inliner_->AddChild(kml_factory_->CreateNetworkLinkControl(),
                                       update));
  ASSERT_FALSE(style_inliner_->in_update());

  // Now that we're no longer within an <Update> a <Style> with an id _will_
  // be captured into the SharedStyleMap and _not_ parented to a <Document>.
  ASSERT_FALSE(style_inliner_->EndElement(document, style));
  ASSERT_EQ(static_cast<size_t>(1), style_inliner_->get_shared_styles().size());
  SharedStyleMap::const_iterator find =
      style_inliner_->get_shared_styles().find(kStyleId);
  ASSERT_EQ(kStyleId, find->second->get_id());
}

// This is a utility function to read a file relative to the testdata directory.
static bool ReadDataDirFileToString(const string& dir,
                                    const string& filename,
                                    string* content) {
  const string pathname =
    File::JoinPaths(File::JoinPaths(string(DATADIR), dir), filename);
  return File::ReadFileToString(pathname, content);
}

// This is a table of test cases of input files with shared styles checked
// against the expected inline result.
static const struct {
  const char* source_subdir_;
  const char* source_file_;
  const char* check_subdir_;
  const char* check_file_;
} kTestCases [] = {
  { "kml", "kmlsamples.kml", "style", "kmlsamples-inline-style-check.kml" },
  { "kml", "ge-point.kml", "style", "ge-point-inline-style-check.kml" }
};

TEST_F(StyleInlinerTest, TestFiles) {
  const size_t size = sizeof(kTestCases)/sizeof(kTestCases[0]);
  for (size_t i = 0; i < size; ++i) {
    string input;
    ASSERT_TRUE(ReadDataDirFileToString(kTestCases[i].source_subdir_,
                                        kTestCases[i].source_file_, &input))
      << kTestCases[i].source_subdir_ << "/" << kTestCases[i].source_file_;
    string errors;

    // Call the function under test.
    ElementPtr root = InlineStyles(input, &errors);
    ASSERT_TRUE(root);
    ASSERT_TRUE(errors.empty());
    string check;
    ASSERT_TRUE(ReadDataDirFileToString(kTestCases[i].check_subdir_,
                                        kTestCases[i].check_file_, &check))
      << kTestCases[i].check_subdir_ << "/" << kTestCases[i].check_file_;
    ASSERT_EQ(check, kmldom::SerializePretty(root));
  }
}

}  // end namespace kmlengine

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
