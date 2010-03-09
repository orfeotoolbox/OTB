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

// This file contains the unit tests for the Atom elements.
// TODO: a few more tests

#include "kml/dom/atom.h"
#include "kml/base/string_util.h"
#include "kml/base/xml_namespaces.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_funcs.h"
#include "gtest/gtest.h"

namespace kmldom {

class AtomAuthorTest : public testing::Test {
 protected:
  virtual void SetUp() {
    atomauthor_ = KmlFactory::GetFactory()->CreateAtomAuthor();
  }

  AtomAuthorPtr atomauthor_;
};

TEST_F(AtomAuthorTest, TestType) {
  ASSERT_EQ(Type_AtomAuthor, atomauthor_->Type());
  ASSERT_FALSE(atomauthor_->IsA(Type_Object));
  ASSERT_TRUE(atomauthor_->IsA(Type_AtomAuthor));
}

TEST_F(AtomAuthorTest, TestXmlNamespace) {
  ASSERT_EQ(kmlbase::XMLNS_ATOM, atomauthor_->get_xmlns());
}

class AtomCategoryTest : public testing::Test {
 protected:
  virtual void SetUp() {
    atomcategory_ = KmlFactory::GetFactory()->CreateAtomCategory();
  }

  AtomCategoryPtr atomcategory_;
};

TEST_F(AtomCategoryTest, TestType) {
  ASSERT_EQ(Type_AtomCategory, atomcategory_->Type());
  ASSERT_FALSE(atomcategory_->IsA(Type_Object));
  ASSERT_TRUE(atomcategory_->IsA(Type_AtomCategory));
}

TEST_F(AtomCategoryTest, TestParseScheme) {
  const string kScheme("http://schemas.google.com/g/2005#kind");
  // ParseKml calls AddElement.
  atomcategory_ = AsAtomCategory(
      ParseKml(string("<atom:category scheme='") + kScheme + "'/>"));
  ASSERT_TRUE(atomcategory_);
  ASSERT_TRUE(atomcategory_->has_scheme());
  ASSERT_EQ(kScheme, atomcategory_->get_scheme());
}

TEST_F(AtomCategoryTest, TestParseLabel) {
  const string kLabel("document");
  // ParseKml calls AddElement.
  atomcategory_ = AsAtomCategory(
      ParseKml(string("<atom:category label='") + kLabel + "'/>"));
  ASSERT_TRUE(atomcategory_);
  ASSERT_TRUE(atomcategory_->has_label());
  ASSERT_EQ(kLabel, atomcategory_->get_label());
}

TEST_F(AtomCategoryTest, TestParseTerm) {
  const string kTerm("item");
  // ParseKml calls AddElement.
  atomcategory_ = AsAtomCategory(
      ParseKml(string("<atom:category term='") + kTerm + "'/>"));
  ASSERT_TRUE(atomcategory_);
  ASSERT_TRUE(atomcategory_->has_term());
  ASSERT_EQ(kTerm, atomcategory_->get_term());
}

class AtomContentTest : public testing::Test {
 protected:
  virtual void SetUp() {
    atomcontent_ = KmlFactory::GetFactory()->CreateAtomContent();
  }

  AtomContentPtr atomcontent_;
};

TEST_F(AtomContentTest, TestType) {
  ASSERT_EQ(Type_AtomContent, atomcontent_->Type());
  ASSERT_FALSE(atomcontent_->IsA(Type_Object));
  ASSERT_TRUE(atomcontent_->IsA(Type_AtomContent));
}

TEST_F(AtomContentTest, TestXmlNamespace) {
  ASSERT_EQ(kmlbase::XMLNS_ATOM, atomcontent_->get_xmlns());
}

TEST_F(AtomContentTest, TestParseSrc) {
  const string kSrc("http://somewhere.com/over/the/rainbow");
  // ParseKml calls AddElement.
  atomcontent_ = AsAtomContent(
      ParseKml(string("<atom:content src='") + kSrc + "'/>"));
  ASSERT_TRUE(atomcontent_);
  ASSERT_TRUE(atomcontent_->has_src());
  ASSERT_EQ(kSrc, atomcontent_->get_src());
}

TEST_F(AtomContentTest, TestParseType) {
  const string kType("text/blah");
  // ParseKml calls AddElement.
  atomcontent_ = AsAtomContent(
      ParseKml(string("<atom:content type='") + kType + "'/>"));
  ASSERT_TRUE(atomcontent_);
  ASSERT_TRUE(atomcontent_->has_type());
  ASSERT_EQ(kType, atomcontent_->get_type());
}

TEST_F(AtomContentTest, TestParseUnknownContent) {
  const string kContent("<goo:bar>baz<goo:a>foo</goo:a></goo:bar>\n");
  atomcontent_ = AsAtomContent(
      ParseKml(string("<atom:content>") + kContent + "</atom:content>"));
  ASSERT_TRUE(atomcontent_);
  ASSERT_EQ(static_cast<size_t>(1),
            atomcontent_->get_unknown_elements_array_size());
  ASSERT_EQ(kContent, atomcontent_->get_unknown_elements_array_at(0));
}

// This test case is taken from the Atom RFC 4287, section 1.1
TEST_F(AtomContentTest, TestParseUnknownContentWithUnknownAttributes) {
  // Note that libkml emits attributes using double quotes and appends
  // a newline after each unknown item.
  const string kContent(
       "<div xmlns=\"http://www.w3.org/1999/xhtml\">"
         "<p><i>[Update: The Atom draft is finished.]</i></p>"
        "</div>\n");
  // ParseKml is _not_ namespace aware, hence the atom: prefix.
  atomcontent_ = AsAtomContent(ParseKml(
     string("<atom:content type='xhtml' xml:lang='en'") +
                    " xml:base='http://diveintomark.org/'>" + kContent +
                 "</atom:content>"));
  ASSERT_TRUE(atomcontent_);
  ASSERT_EQ(static_cast<size_t>(1),
            atomcontent_->get_unknown_elements_array_size());
  ASSERT_EQ(kContent, atomcontent_->get_unknown_elements_array_at(0));
  const kmlbase::Attributes* unknown_attributes =
      atomcontent_->GetUnknownAttributes();
  ASSERT_TRUE(unknown_attributes);
  ASSERT_EQ(static_cast<size_t>(2), unknown_attributes->GetSize());
  string val;
  ASSERT_TRUE(unknown_attributes->GetValue("xml:lang", &val));
  ASSERT_EQ(string("en"), val);
  ASSERT_TRUE(unknown_attributes->GetValue("xml:base", &val));
  ASSERT_EQ(string("http://diveintomark.org/"), val);
}

// Within libkml <Placemark> and any other kml element is only ever
// considered misplaced.  The <Placemark> itself _is_ fully parsed and
// available in dom form on the parents misplaced elements list.  Neat!
TEST_F(AtomContentTest, TestParseMisplacedContent) {
  const string kName("my name");
  const string kPlacemark(string("<Placemark><name>") + kName +
                               "</name></Placemark>");
  atomcontent_ = AsAtomContent(
      ParseKml(string("<atom:content>") + kPlacemark + "</atom:content>"));
  ASSERT_TRUE(atomcontent_);
  ASSERT_EQ(static_cast<size_t>(0),
            atomcontent_->get_unknown_elements_array_size());
  ASSERT_EQ(static_cast<size_t>(1),
            atomcontent_->get_misplaced_elements_array_size());
  PlacemarkPtr placemark = AsPlacemark(
      atomcontent_->get_misplaced_elements_array_at(0));
  ASSERT_TRUE(placemark);
  ASSERT_EQ(kName, placemark->get_name());
}

TEST_F(AtomContentTest, TestSerializeAttributes) {
  const string kSrc("http://somewhere.com/over/the/rainbow");
  const string kType("text/blah");
  atomcontent_->set_src(kSrc);
  atomcontent_->set_type(kType);
  const string kExpected(
    string("<atom:content") +
                " src=\"" + kSrc + "\""
                " type=\"" + kType + "\""
               "/>");
  ASSERT_EQ(kExpected, SerializeRaw(atomcontent_));
}

// TOOD: test serialize of <atom:content><arbitrary>mark<up>...
// TOOD: test serialize of <atom:content><Placemark>mark<up>...

// This tests elements comment to <atom:entry> and <atom:feed>
class AtomCommonTest : public testing::Test {
 protected:
  virtual void SetUp() {
    entry_ = KmlFactory::GetFactory()->CreateAtomEntry();
    feed_ = KmlFactory::GetFactory()->CreateAtomFeed();
  }

  AtomFeedPtr feed_;
  AtomEntryPtr entry_;
};

TEST_F(AtomCommonTest, TestDefault) {
  ASSERT_FALSE(feed_->has_id());
  ASSERT_FALSE(entry_->has_id());
  ASSERT_FALSE(feed_->has_title());
  ASSERT_FALSE(entry_->has_title());
  ASSERT_FALSE(feed_->has_updated());
  ASSERT_FALSE(entry_->has_updated());
  ASSERT_EQ(static_cast<size_t>(0), feed_->get_link_array_size());
  ASSERT_EQ(static_cast<size_t>(0), entry_->get_link_array_size());
}

TEST_F(AtomCommonTest, TestSetGetClear) {
  const string kId("very-mostly-highly-unique");
  const string kTitle("Your Lordship");
  const string kUpdated("today!");
  feed_->set_id(kId);
  entry_->set_id(kId);
  feed_->set_title(kTitle);
  entry_->set_title(kTitle);
  feed_->set_updated(kUpdated);
  entry_->set_updated(kUpdated);
  ASSERT_TRUE(feed_->has_id());
  ASSERT_TRUE(entry_->has_id());
  ASSERT_TRUE(feed_->has_title());
  ASSERT_TRUE(entry_->has_title());
  ASSERT_TRUE(feed_->has_updated());
  ASSERT_TRUE(entry_->has_updated());
  ASSERT_EQ(kId, feed_->get_id());
  ASSERT_EQ(kId, entry_->get_id());
  ASSERT_EQ(kTitle, feed_->get_title());
  ASSERT_EQ(kTitle, entry_->get_title());
  ASSERT_EQ(kUpdated, feed_->get_updated());
  ASSERT_EQ(kUpdated, entry_->get_updated());
  feed_->clear_id();
  entry_->clear_id();
  feed_->clear_title();
  entry_->clear_title();
  feed_->clear_updated();
  entry_->clear_updated();
  ASSERT_FALSE(feed_->has_id());
  ASSERT_FALSE(entry_->has_id());
  ASSERT_FALSE(feed_->has_title());
  ASSERT_FALSE(entry_->has_title());
  ASSERT_FALSE(feed_->has_updated());
  ASSERT_FALSE(entry_->has_updated());
}

TEST_F(AtomCommonTest, TestAddOneLink) {
  entry_->add_link(KmlFactory::GetFactory()->CreateAtomLink());
  ASSERT_EQ(static_cast<size_t>(1), entry_->get_link_array_size());
  feed_->add_link(KmlFactory::GetFactory()->CreateAtomLink());
  ASSERT_EQ(static_cast<size_t>(1), feed_->get_link_array_size());
}

TEST_F(AtomCommonTest, TestAddMultipleLinks) {
  const int kLinkCount(49);
  AtomLinkPtr link;
  for (int i = 0; i < kLinkCount; ++i) {
    link = KmlFactory::GetFactory()->CreateAtomLink();
    link->set_href(kmlbase::ToString(i));
    entry_->add_link(link);
    // Can't give the same element to 2 parents.
    link = KmlFactory::GetFactory()->CreateAtomLink();
    link->set_href(kmlbase::ToString(i));
    feed_->add_link(link);
  }
  ASSERT_EQ(static_cast<size_t>(kLinkCount), entry_->get_link_array_size());
  ASSERT_EQ(static_cast<size_t>(kLinkCount), feed_->get_link_array_size());
  for (int i = 0; i < kLinkCount; ++i) {
    ASSERT_EQ(kmlbase::ToString(i), entry_->get_link_array_at(i)->get_href());
    ASSERT_EQ(kmlbase::ToString(i), feed_->get_link_array_at(i)->get_href());
  }
}

// This tests AtomCommon::AddElement()
TEST_F(AtomCommonTest, TestParse) {
  const string kId("very-mostly-highly-unique");
  const string kTitle("Your Lordship");
  const string kUpdated("today!");
  const string kHrefA("http://a.com");
  const string kHrefB("http://b.net");
  const string kLinks(string("<atom:link href='" + kHrefA + "'/>"
                                       "<atom:link href='" + kHrefB + "'/>"));
  const string kChildren(
      string("<atom:id>") + kId + "</atom:id>"
      "<atom:title>" + kTitle + "</atom:title>"
      "<atom:updated>" + kUpdated + "</atom:updated>" +
      kLinks);
  entry_ = AsAtomEntry(ParseKml(string("<atom:entry>") + kChildren +
                               "</atom:entry>"));
  ASSERT_TRUE(entry_.get());
  ASSERT_TRUE(entry_->has_id());
  ASSERT_EQ(kId, entry_->get_id());
  ASSERT_TRUE(entry_->has_title());
  ASSERT_EQ(kTitle, entry_->get_title());
  ASSERT_TRUE(entry_->has_updated());
  ASSERT_EQ(kUpdated, entry_->get_updated());
  ASSERT_EQ(static_cast<size_t>(2), entry_->get_link_array_size());
  ASSERT_EQ(kHrefA, entry_->get_link_array_at(0)->get_href());
  ASSERT_EQ(kHrefB, entry_->get_link_array_at(1)->get_href());
  feed_ = AsAtomFeed(ParseKml(string("<atom:feed>") + kChildren +
                               "</atom:feed>"));
  ASSERT_TRUE(feed_.get());
  ASSERT_TRUE(feed_->has_id());
  ASSERT_EQ(kId, feed_->get_id());
  ASSERT_TRUE(feed_->has_title());
  ASSERT_EQ(kTitle, feed_->get_title());
  ASSERT_TRUE(feed_->has_updated());
  ASSERT_EQ(kUpdated, feed_->get_updated());
  ASSERT_EQ(static_cast<size_t>(2), feed_->get_link_array_size());
  ASSERT_EQ(kHrefA, feed_->get_link_array_at(0)->get_href());
  ASSERT_EQ(kHrefB, feed_->get_link_array_at(1)->get_href());
}

TEST_F(AtomCommonTest, TestSerialize) {
  const string kId("very-mostly-highly-unique");
  const string kTitle("Your Lordship");
  const string kUpdated("today!");
  const string kHrefA("http://a.com");
  const string kHrefB("http://b.net");
  const string kLinks(string("<atom:link href=\"" + kHrefA + "\"/>"
                                       "<atom:link href=\"" + kHrefB + "\"/>"));
  const string kChildren(
      string("<atom:id>") + kId + "</atom:id>"
      "<atom:title>" + kTitle + "</atom:title>"
      "<atom:updated>" + kUpdated + "</atom:updated>" +
      kLinks);
  entry_ = AsAtomEntry(ParseKml(string("<atom:entry>") + kChildren +
                               "</atom:entry>"));
  feed_ = AsAtomFeed(ParseKml(string("<atom:feed>") + kChildren +
                               "</atom:feed>"));
  ASSERT_EQ(string("<atom:entry>") + kChildren + "</atom:entry>",
            SerializeRaw(entry_));
  ASSERT_EQ(string("<atom:feed>") + kChildren + "</atom:feed>",
            SerializeRaw(feed_));
}

class AtomLinkTest : public testing::Test {
 protected:
  virtual void SetUp() {
    atomlink_ = KmlFactory::GetFactory()->CreateAtomLink();
  }

  AtomLinkPtr atomlink_;
};

// This tests elements particular to <atom:entry> not common with <atom:feed>.
// See AtomCommonTest for elements common with <atom:feed>.
class AtomEntryTest : public testing::Test {
 protected:
  virtual void SetUp() {
    atomentry_ = KmlFactory::GetFactory()->CreateAtomEntry();
  }

  AtomEntryPtr atomentry_;
};

TEST_F(AtomEntryTest, TestType) {
  ASSERT_EQ(Type_AtomEntry, atomentry_->Type());
  ASSERT_FALSE(atomentry_->IsA(Type_Object));
  ASSERT_TRUE(atomentry_->IsA(Type_AtomEntry));
}

TEST_F(AtomEntryTest, TestXmlNamespace) {
  ASSERT_EQ(kmlbase::XMLNS_ATOM, atomentry_->get_xmlns());
}

TEST_F(AtomEntryTest, TestSetSummary) {
  const string kSummary("and in summary");
  atomentry_->set_summary(kSummary);
  ASSERT_TRUE(atomentry_->has_summary());
  ASSERT_EQ(kSummary, atomentry_->get_summary());
}

TEST_F(AtomEntryTest, TestSetContent) {
  atomentry_->set_content(KmlFactory::GetFactory()->CreateAtomContent());
  ASSERT_TRUE(atomentry_->has_content());
  ASSERT_TRUE(atomentry_->get_content());
}

TEST_F(AtomEntryTest, TestParseSummary) {
  const string kSummary("and in summary");
  atomentry_ = AsAtomEntry(ParseKml(string("<atom:entry><atom:summary>") +
                                    kSummary + "</atom:summary></atom:entry>"));
  ASSERT_TRUE(atomentry_.get());
  ASSERT_TRUE(atomentry_->has_summary());
  ASSERT_EQ(kSummary, atomentry_->get_summary());
}

// This tests elements particular to <atom:feed> not common with <atom:entry>.
TEST_F(AtomEntryTest, TestParseContent) {
  atomentry_ = AsAtomEntry(ParseKml(
      "<atom:entry><atom:content/></atom:entry>"));
  ASSERT_TRUE(atomentry_.get());
  ASSERT_TRUE(atomentry_->has_content());
}

// This tests elements particular to <atom:feed> not common with <atom:entry>.
// See AtomCommonTest for elements common with <atom:entry>.
class AtomFeedTest : public testing::Test {
 protected:
  virtual void SetUp() {
    atomfeed_ = KmlFactory::GetFactory()->CreateAtomFeed();
  }

  AtomFeedPtr atomfeed_;
};

TEST_F(AtomFeedTest, TestType) {
  ASSERT_EQ(Type_AtomFeed, atomfeed_->Type());
  ASSERT_FALSE(atomfeed_->IsA(Type_Object));
  ASSERT_TRUE(atomfeed_->IsA(Type_AtomFeed));
}

TEST_F(AtomFeedTest, TestXmlNamespace) {
  ASSERT_EQ(kmlbase::XMLNS_ATOM, atomfeed_->get_xmlns());
}

TEST_F(AtomFeedTest, TestAddOneEntry) {
  atomfeed_->add_entry(KmlFactory::GetFactory()->CreateAtomEntry());
  ASSERT_EQ(static_cast<size_t>(1), atomfeed_->get_entry_array_size());
}

TEST_F(AtomFeedTest, TestAddMultipleEntries) {
  const int kEntryCount(49);
  AtomEntryPtr entry;
  for (int i = 0; i < kEntryCount; ++i) {
    entry = KmlFactory::GetFactory()->CreateAtomEntry();
    entry->set_id(kmlbase::ToString(i));
    atomfeed_->add_entry(entry);
  }
  ASSERT_EQ(static_cast<size_t>(kEntryCount),
            atomfeed_->get_entry_array_size());
  for (int i = 0; i < kEntryCount; ++i) {
    ASSERT_EQ(kmlbase::ToString(i), atomfeed_->get_entry_array_at(i)->get_id());
  }
}

TEST_F(AtomLinkTest, TestType) {
  ASSERT_EQ(Type_AtomLink, atomlink_->Type());
  ASSERT_FALSE(atomlink_->IsA(Type_Object));
  ASSERT_TRUE(atomlink_->IsA(Type_AtomLink));
}

TEST_F(AtomLinkTest, TestXmlNamespace) {
  ASSERT_EQ(kmlbase::XMLNS_ATOM, atomlink_->get_xmlns());
}

}  // namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
