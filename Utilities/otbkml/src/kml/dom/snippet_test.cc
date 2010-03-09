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

// This file contains the unit tests for the Snippet and linkSnippet elements.

#include "kml/dom/snippet.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_funcs.h"
#include "gtest/gtest.h"

namespace kmldom {

// This tests <Snippet>.
class SnippetTest : public testing::Test {
 protected:
  virtual void SetUp() {
    snippet_ = KmlFactory::GetFactory()->CreateSnippet();
  }

  SnippetPtr snippet_;
};

TEST_F(SnippetTest, TestType) {
  ASSERT_EQ(Type_Snippet, snippet_->Type());
  ASSERT_TRUE(snippet_->IsA(Type_Snippet));
}

// Verify proper defaults:
TEST_F(SnippetTest, TestDefaults) {
  ASSERT_FALSE(snippet_->has_text());
  ASSERT_TRUE(snippet_->get_text().empty());
  ASSERT_FALSE(snippet_->has_maxlines());
  ASSERT_EQ(2, snippet_->get_maxlines());
}

// Verify setting default makes has_xxx() true:
TEST_F(SnippetTest, TestSetToDefaultValues) {
  snippet_->set_text(snippet_->get_text());
  ASSERT_TRUE(snippet_->has_text());
  snippet_->set_maxlines(snippet_->get_maxlines());
  ASSERT_TRUE(snippet_->has_maxlines());
}

// Verify set, get, has, clear:
TEST_F(SnippetTest, TestSetGetHasClear) {
  // Non-default values:
  const string kText = "snippet body";
  const int kMaxlines = 10;

  // Set all fields:
  snippet_->set_text(kText);
  snippet_->set_maxlines(kMaxlines);

  // Verify getter and has_xxx():
  ASSERT_TRUE(snippet_->has_text());
  ASSERT_TRUE(kText == snippet_->get_text());
  ASSERT_TRUE(snippet_->has_maxlines());
  ASSERT_TRUE(kMaxlines == snippet_->get_maxlines());

  // Clear all fields:
  snippet_->clear_text();
  snippet_->clear_maxlines();
}

// This tests parsing of Snippet with no maxLines attribute.
TEST_F(SnippetTest, TestBasicParse) {
  const string kText = "left panel only please";
  const string kSnippet = "<Snippet>" + kText + "</Snippet>";
  const string kPlacemark = "<Placemark>" + kSnippet + "</Placemark>";
  string errors;
  ElementPtr root = Parse(kPlacemark, &errors);
  ASSERT_TRUE(root);
  const PlacemarkPtr placemark = AsPlacemark(root);
  ASSERT_TRUE(placemark);
  ASSERT_TRUE(placemark->has_snippet());
  ASSERT_FALSE(placemark->get_snippet()->has_maxlines());
  ASSERT_EQ(2, placemark->get_snippet()->get_maxlines());  // The default.
  ASSERT_TRUE(placemark->get_snippet()->has_text());
  ASSERT_EQ(kText, placemark->get_snippet()->get_text());
}

// This tests parsing of Snippet with a maxLines attribute.
TEST_F(SnippetTest, TestParseMaxLines) {
  const string kText =
      "left panel only please"
      "left panel only please"
      "left panel only please"
      "left panel only please"
      "left panel only please";
  const string kSnippet = "<Snippet maxLines=\"5\">" + kText +
    "</Snippet>";
  const string kFolder = "<Folder>" + kSnippet + "</Folder>";
  string errors;
  ElementPtr root = Parse(kFolder, &errors);
  ASSERT_TRUE(root);
  ASSERT_TRUE(errors.empty());
  const FolderPtr folder = AsFolder(root);
  ASSERT_TRUE(folder->has_snippet());
  const SnippetPtr snippet = folder->get_snippet();
  ASSERT_TRUE(snippet);
  ASSERT_TRUE(snippet->has_maxlines());
  ASSERT_EQ(5, snippet->get_maxlines());
}

TEST_F(SnippetTest, TestSerializeCdata) {
  snippet_->set_text("&");
  ASSERT_EQ(string("<Snippet><![CDATA[&]]></Snippet>"),
                       SerializeRaw(snippet_));

  snippet_->set_text("a");
  ASSERT_EQ(string("<Snippet>a</Snippet>"),
                       SerializeRaw(snippet_));
}

// This tests <linkSnippet>.
class LinkSnippetTest : public testing::Test {
 protected:
  virtual void SetUp() {
    linksnippet_ = KmlFactory::GetFactory()->CreateLinkSnippet();
  }

  LinkSnippetPtr linksnippet_;
};

TEST_F(LinkSnippetTest, TestType) {
  ASSERT_EQ(Type_linkSnippet, linksnippet_->Type());
  ASSERT_TRUE(linksnippet_->IsA(Type_linkSnippet));
}

// Verify proper defaults:
TEST_F(LinkSnippetTest, TestDefaults) {
  ASSERT_FALSE(linksnippet_->has_text());
  ASSERT_TRUE(linksnippet_->get_text().empty());
  ASSERT_FALSE(linksnippet_->has_maxlines());
  ASSERT_EQ(2, linksnippet_->get_maxlines());
}

// Verify setting default makes has_xxx() true:
TEST_F(LinkSnippetTest, TestSetToDefaultValues) {
  linksnippet_->set_text(linksnippet_->get_text());
  ASSERT_TRUE(linksnippet_->has_text());
  linksnippet_->set_maxlines(linksnippet_->get_maxlines());
  ASSERT_TRUE(linksnippet_->has_maxlines());
}

// Verify set, get, has, clear:
TEST_F(LinkSnippetTest, TestSetGetHasClear) {
  // Non-default values:
  const string kText = "snippet body";
  const int kMaxlines = 11;

  // Set all fields:
  linksnippet_->set_text(kText);
  linksnippet_->set_maxlines(kMaxlines);

  // Verify getter and has_xxx():
  ASSERT_TRUE(linksnippet_->has_text());
  ASSERT_TRUE(kText == linksnippet_->get_text());
  ASSERT_TRUE(linksnippet_->has_maxlines());
  ASSERT_TRUE(kMaxlines == linksnippet_->get_maxlines());

  // Clear all fields:
  linksnippet_->clear_text();
  linksnippet_->clear_maxlines();
}

// This tests parsing of linkSnippet with no maxLines attribute.
TEST_F(LinkSnippetTest, TestBasicParse) {
  const string kText = "change my left panel";
  const string kLinkSnippet = "<linkSnippet>" + kText + "</linkSnippet>";
  const string kNetworkLinkControl = "<NetworkLinkControl>" +
    kLinkSnippet + "</NetworkLinkControl>";
  string errors;
  ElementPtr root = Parse(kNetworkLinkControl, &errors);
  ASSERT_TRUE(root);
  const NetworkLinkControlPtr networklinkcontrol = AsNetworkLinkControl(root);
  ASSERT_TRUE(networklinkcontrol->has_linksnippet());
  const LinkSnippetPtr linksnippet = networklinkcontrol->get_linksnippet();
  ASSERT_FALSE(linksnippet->has_maxlines());
  ASSERT_EQ(2, linksnippet->get_maxlines());
  ASSERT_TRUE(linksnippet->has_text());
  ASSERT_EQ(kText, linksnippet->get_text());
}

// This tests parsing of linkSnippet with a maxLines attribute.
TEST_F(LinkSnippetTest, TestParseMaxLines) {
  const string kText =
      "left panel only please"
      "left panel only please"
      "left panel only please";
  const string kLinkSnippet = "<linkSnippet maxLines=\"7\">" + kText +
    "</linkSnippet>";
  const string kNetworkLinkControl = "<NetworkLinkControl>" +
    kLinkSnippet + "</NetworkLinkControl>";
  string errors;
  ElementPtr root = Parse(kNetworkLinkControl, &errors);
  ASSERT_TRUE(root);
  ASSERT_TRUE(errors.empty());
  const NetworkLinkControlPtr networklinkcontrol = AsNetworkLinkControl(root);
  ASSERT_TRUE(networklinkcontrol->has_linksnippet());
  const LinkSnippetPtr linksnippet = networklinkcontrol->get_linksnippet();
  ASSERT_TRUE(linksnippet);
  ASSERT_TRUE(linksnippet->has_maxlines());
  ASSERT_EQ(7, linksnippet->get_maxlines());
}

TEST_F(LinkSnippetTest, TestSerializeCdata) {
  linksnippet_->set_text("&");
  ASSERT_EQ(string("<linkSnippet><![CDATA[&]]></linkSnippet>"),
                       SerializeRaw(linksnippet_));

  linksnippet_->set_text("a");
  ASSERT_EQ(string("<linkSnippet>a</linkSnippet>"),
                       SerializeRaw(linksnippet_));
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
