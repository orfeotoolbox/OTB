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
#include <string>
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_funcs.h"
#include "kml/base/unit_test.h"

namespace kmldom {

// This tests <Snippet>.
class SnippetTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(SnippetTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST(TestBasicParse);
  CPPUNIT_TEST(TestParseMaxLines);
  CPPUNIT_TEST(TestSerializeCdata);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before all tests.
  void setUp() {
    snippet_ = KmlFactory::GetFactory()->CreateSnippet();
  }

  // Called after all tests.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();
  void TestBasicParse();
  void TestParseMaxLines();
  void TestSerializeCdata();

 private:
  SnippetPtr snippet_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(SnippetTest);

void SnippetTest::TestType() {
  CPPUNIT_ASSERT_EQUAL(Type_Snippet, snippet_->Type());
  CPPUNIT_ASSERT(snippet_->IsA(Type_Snippet));
}

// Verify proper defaults:
void SnippetTest::TestDefaults() {
  CPPUNIT_ASSERT(false == snippet_->has_text());
  CPPUNIT_ASSERT(snippet_->get_text().empty());
  CPPUNIT_ASSERT(false == snippet_->has_maxlines());
  CPPUNIT_ASSERT(2 == snippet_->get_maxlines());
}

// Verify setting default makes has_xxx() true:
void SnippetTest::TestSetToDefaultValues() {
  TestDefaults();
  snippet_->set_text(snippet_->get_text());
  CPPUNIT_ASSERT(snippet_->has_text());
  snippet_->set_maxlines(snippet_->get_maxlines());
  CPPUNIT_ASSERT(snippet_->has_maxlines());
}

// Verify set, get, has, clear:
void SnippetTest::TestSetGetHasClear() {
  // Non-default values:
  const std::string kText = "snippet body";
  const unsigned int kMaxlines = 10;

  // Set all fields:
  snippet_->set_text(kText);
  snippet_->set_maxlines(kMaxlines);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(snippet_->has_text());
  CPPUNIT_ASSERT(kText == snippet_->get_text());
  CPPUNIT_ASSERT(snippet_->has_maxlines());
  CPPUNIT_ASSERT(kMaxlines == snippet_->get_maxlines());

  // Clear all fields:
  snippet_->clear_text();
  snippet_->clear_maxlines();

  // Verify now in default state:
  TestDefaults();
}

// This tests parsing of Snippet with no maxLines attribute.
void SnippetTest::TestBasicParse() {
  const std::string kText = "left panel only please";
  const std::string kSnippet = "<Snippet>" + kText + "</Snippet>";
  const std::string kPlacemark = "<Placemark>" + kSnippet + "</Placemark>";
  std::string errors;
  ElementPtr root = Parse(kPlacemark, &errors);
  CPPUNIT_ASSERT(root);
  const PlacemarkPtr placemark = AsPlacemark(root);
  CPPUNIT_ASSERT(placemark);
  CPPUNIT_ASSERT(placemark->has_snippet());
  CPPUNIT_ASSERT(false == placemark->get_snippet()->has_maxlines());
  CPPUNIT_ASSERT_EQUAL(static_cast<unsigned int>(2),  // The default.
                       placemark->get_snippet()->get_maxlines());
  CPPUNIT_ASSERT(placemark->get_snippet()->has_text());
  CPPUNIT_ASSERT_EQUAL(kText, placemark->get_snippet()->get_text());
}

// This tests parsing of Snippet with a maxLines attribute.
void SnippetTest::TestParseMaxLines() {
  const std::string kText =
      "left panel only please"
      "left panel only please"
      "left panel only please"
      "left panel only please"
      "left panel only please";
  const std::string kSnippet = "<Snippet maxLines=\"5\">" + kText +
    "</Snippet>";
  const std::string kFolder = "<Folder>" + kSnippet + "</Folder>";
  std::string errors;
  ElementPtr root = Parse(kFolder, &errors);
  CPPUNIT_ASSERT(root);
  CPPUNIT_ASSERT(errors.empty());
  const FolderPtr folder = AsFolder(root);
  CPPUNIT_ASSERT(folder->has_snippet());
  const SnippetPtr snippet = folder->get_snippet();
  CPPUNIT_ASSERT(snippet);
  CPPUNIT_ASSERT(snippet->has_maxlines());
  CPPUNIT_ASSERT_EQUAL(static_cast<unsigned int>(5), snippet->get_maxlines());
}

void SnippetTest::TestSerializeCdata() {
  snippet_->set_text("&");
  CPPUNIT_ASSERT_EQUAL(std::string("<Snippet><![CDATA[&]]></Snippet>"),
                       SerializeRaw(snippet_));

  snippet_->set_text("a");
  CPPUNIT_ASSERT_EQUAL(std::string("<Snippet>a</Snippet>"),
                       SerializeRaw(snippet_));
}

// This tests <linkSnippet>.
class LinkSnippetTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(LinkSnippetTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST(TestBasicParse);
  CPPUNIT_TEST(TestParseMaxLines);
  CPPUNIT_TEST(TestSerializeCdata);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before all tests.
  void setUp() {
    linksnippet_ = KmlFactory::GetFactory()->CreateLinkSnippet();
  }

  // Called after all tests.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();
  void TestBasicParse();
  void TestParseMaxLines();
  void TestSerializeCdata();

 private:
  LinkSnippetPtr linksnippet_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(LinkSnippetTest);

void LinkSnippetTest::TestType() {
  CPPUNIT_ASSERT_EQUAL(Type_linkSnippet, linksnippet_->Type());
  CPPUNIT_ASSERT(linksnippet_->IsA(Type_linkSnippet));
}

// Verify proper defaults:
void LinkSnippetTest::TestDefaults() {
  CPPUNIT_ASSERT(false == linksnippet_->has_text());
  CPPUNIT_ASSERT(linksnippet_->get_text().empty());
  CPPUNIT_ASSERT(false == linksnippet_->has_maxlines());
  CPPUNIT_ASSERT(2 == linksnippet_->get_maxlines());
}

// Verify setting default makes has_xxx() true:
void LinkSnippetTest::TestSetToDefaultValues() {
  TestDefaults();
  linksnippet_->set_text(linksnippet_->get_text());
  CPPUNIT_ASSERT(linksnippet_->has_text());
  linksnippet_->set_maxlines(linksnippet_->get_maxlines());
  CPPUNIT_ASSERT(linksnippet_->has_maxlines());
}

// Verify set, get, has, clear:
void LinkSnippetTest::TestSetGetHasClear() {
  // Non-default values:
  const std::string kText = "snippet body";
  const unsigned int kMaxlines = 11;

  // Set all fields:
  linksnippet_->set_text(kText);
  linksnippet_->set_maxlines(kMaxlines);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(linksnippet_->has_text());
  CPPUNIT_ASSERT(kText == linksnippet_->get_text());
  CPPUNIT_ASSERT(linksnippet_->has_maxlines());
  CPPUNIT_ASSERT(kMaxlines == linksnippet_->get_maxlines());

  // Clear all fields:
  linksnippet_->clear_text();
  linksnippet_->clear_maxlines();

  // Verify now in default state:
  TestDefaults();
}

// This tests parsing of linkSnippet with no maxLines attribute.
void LinkSnippetTest::TestBasicParse() {
  const std::string kText = "change my left panel";
  const std::string kLinkSnippet = "<linkSnippet>" + kText + "</linkSnippet>";
  const std::string kNetworkLinkControl = "<NetworkLinkControl>" +
    kLinkSnippet + "</NetworkLinkControl>";
  std::string errors;
  ElementPtr root = Parse(kNetworkLinkControl, &errors);
  CPPUNIT_ASSERT(root);
  const NetworkLinkControlPtr networklinkcontrol = AsNetworkLinkControl(root);
  CPPUNIT_ASSERT(networklinkcontrol->has_linksnippet());
  const LinkSnippetPtr linksnippet = networklinkcontrol->get_linksnippet();
  CPPUNIT_ASSERT(false == linksnippet->has_maxlines());
  CPPUNIT_ASSERT_EQUAL(static_cast<unsigned int>(2),
                       linksnippet->get_maxlines());
  CPPUNIT_ASSERT(linksnippet->has_text());
  CPPUNIT_ASSERT_EQUAL(kText, linksnippet->get_text());
}

// This tests parsing of linkSnippet with a maxLines attribute.
void LinkSnippetTest::TestParseMaxLines() {
  const std::string kText =
      "left panel only please"
      "left panel only please"
      "left panel only please";
  const std::string kLinkSnippet = "<linkSnippet maxLines=\"7\">" + kText +
    "</linkSnippet>";
  const std::string kNetworkLinkControl = "<NetworkLinkControl>" +
    kLinkSnippet + "</NetworkLinkControl>";
  std::string errors;
  ElementPtr root = Parse(kNetworkLinkControl, &errors);
  CPPUNIT_ASSERT(root);
  CPPUNIT_ASSERT(errors.empty());
  const NetworkLinkControlPtr networklinkcontrol = AsNetworkLinkControl(root);
  CPPUNIT_ASSERT(networklinkcontrol->has_linksnippet());
  const LinkSnippetPtr linksnippet = networklinkcontrol->get_linksnippet();
  CPPUNIT_ASSERT(linksnippet);
  CPPUNIT_ASSERT(linksnippet->has_maxlines());
  CPPUNIT_ASSERT_EQUAL(static_cast<unsigned int>(7),
                       linksnippet->get_maxlines());
}

void LinkSnippetTest::TestSerializeCdata() {
  linksnippet_->set_text("&");
  CPPUNIT_ASSERT_EQUAL(std::string("<linkSnippet><![CDATA[&]]></linkSnippet>"),
                       SerializeRaw(linksnippet_));

  linksnippet_->set_text("a");
  CPPUNIT_ASSERT_EQUAL(std::string("<linkSnippet>a</linkSnippet>"),
                       SerializeRaw(linksnippet_));
}

}  // end namespace kmldom

TEST_MAIN
