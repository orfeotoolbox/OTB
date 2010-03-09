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

// This file contains the unit tests for the Link, Url, Overlay Icon
// and IconStyle Icon elements.

#include "kml/dom/link.h"
#include "kml/dom/kml22.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_funcs.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_ptr.h"
#include "kml/dom/serializer.h"
#include "gtest/gtest.h"

using kmlbase::Attributes;

namespace kmldom {

class LinkTest : public testing::Test {
 protected:
  virtual void SetUp() {
    link_ = KmlFactory::GetFactory()->CreateLink();
  }

  LinkPtr link_;
};

TEST_F(LinkTest, TestType) {
  ASSERT_TRUE(Type_Link == link_->Type());
  ASSERT_TRUE(link_->IsA(Type_Link));
  ASSERT_TRUE(link_->IsA(Type_BasicLink));
  ASSERT_TRUE(link_->IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(LinkTest, TestDefaults) {
  ASSERT_EQ(string(""), link_->get_href());
  ASSERT_FALSE(link_->has_href());
  ASSERT_EQ(REFRESHMODE_ONCHANGE, link_->get_refreshmode());
  ASSERT_FALSE(link_->has_refreshmode());
  ASSERT_DOUBLE_EQ(4.0, link_->get_refreshinterval());
  ASSERT_FALSE(link_->has_refreshinterval());
  ASSERT_EQ(VIEWREFRESHMODE_NEVER, link_->get_viewrefreshmode());
  ASSERT_FALSE(link_->has_viewrefreshmode());
  ASSERT_DOUBLE_EQ(4.0, link_->get_viewrefreshtime());
  ASSERT_FALSE(link_->has_viewrefreshtime());
  ASSERT_DOUBLE_EQ(1.0, link_->get_viewboundscale());
  ASSERT_FALSE(link_->has_viewboundscale());
  ASSERT_EQ("", link_->get_viewformat());
  ASSERT_FALSE(link_->has_viewformat());
  ASSERT_EQ("", link_->get_httpquery());
  ASSERT_FALSE(link_->has_httpquery());
}

// Verify setting default makes has_xxx() true:
TEST_F(LinkTest, TestSetToDefaultValues) {
  // Verify default state:
  link_->set_href(link_->get_href());
  ASSERT_TRUE(link_->has_href());
  link_->set_refreshmode(link_->get_refreshmode());
  ASSERT_TRUE(link_->has_refreshmode());
  link_->set_refreshinterval(link_->get_refreshinterval());
  ASSERT_TRUE(link_->has_refreshinterval());
  link_->set_viewrefreshmode(link_->get_viewrefreshmode());
  ASSERT_TRUE(link_->has_viewrefreshmode());
  link_->set_viewrefreshtime(link_->get_viewrefreshtime());
  ASSERT_TRUE(link_->has_viewrefreshtime());
  link_->set_viewboundscale(link_->get_viewboundscale());
  ASSERT_TRUE(link_->has_viewboundscale());
  link_->set_viewformat(link_->get_viewformat());
  ASSERT_TRUE(link_->has_viewformat());
  link_->set_httpquery(link_->get_httpquery());
  ASSERT_TRUE(link_->has_httpquery());
}

// Verify set, get, has, clear:
TEST_F(LinkTest, TestSetGetHasClear) {
  // Non-default values:
  string href("href");
  RefreshModeEnum refreshmode = REFRESHMODE_ONINTERVAL;
  double refreshinterval = 1.0;
  ViewRefreshModeEnum viewrefreshmode = VIEWREFRESHMODE_ONREQUEST;
  double viewrefreshtime = 1.0;
  double viewboundscale = 0.5;
  string viewformat("viewformat");
  string httpquery("httpquery");

  // Set all fields:
  link_->set_href(href);
  link_->set_refreshmode(refreshmode);
  link_->set_refreshinterval(refreshinterval);
  link_->set_viewrefreshmode(viewrefreshmode);
  link_->set_viewrefreshtime(viewrefreshtime);
  link_->set_viewboundscale(viewboundscale);
  link_->set_viewformat(viewformat);
  link_->set_httpquery(httpquery);

  // Verify getter and has_xxx():
  ASSERT_TRUE(link_->has_href());
  ASSERT_TRUE(link_->has_refreshmode());
  ASSERT_TRUE(link_->has_refreshinterval());
  ASSERT_TRUE(link_->has_viewrefreshmode());
  ASSERT_TRUE(link_->has_viewrefreshtime());
  ASSERT_TRUE(link_->has_viewboundscale());
  ASSERT_TRUE(link_->has_viewformat());
  ASSERT_TRUE(link_->has_httpquery());

  ASSERT_EQ(href, link_->get_href());
  ASSERT_EQ(refreshmode, link_->get_refreshmode());
  ASSERT_EQ(refreshinterval, link_->get_refreshinterval());
  ASSERT_EQ(viewrefreshmode, link_->get_viewrefreshmode());
  ASSERT_EQ(viewrefreshtime, link_->get_viewrefreshtime());
  ASSERT_EQ(viewboundscale, link_->get_viewboundscale());
  ASSERT_EQ(viewformat, link_->get_viewformat());
  ASSERT_EQ(httpquery, link_->get_httpquery());

  // Clear all fields:
  link_->clear_href();
  link_->clear_refreshmode();
  link_->clear_refreshinterval();
  link_->clear_viewrefreshmode();
  link_->clear_viewrefreshtime();
  link_->clear_viewboundscale();
  link_->clear_viewformat();
  link_->clear_httpquery();
}


TEST_F(LinkTest, TestParse) {
  const string kContent = "foo.kml";
  const string kHref = "<href>" + kContent + "</href>";
  const string kLink = "<Link>" + kHref + "</Link>";
  string errors;
  ElementPtr root = Parse(kLink, &errors);
  ASSERT_TRUE(root);
  ASSERT_TRUE(errors.empty());
  const LinkPtr link = AsLink(root);
  ASSERT_TRUE(link);
  ASSERT_TRUE(link->has_href());
  ASSERT_EQ(kContent, link->get_href());
}

TEST_F(LinkTest, TestAcceptCdataInHref) {
  const string kContent = "abl?output=kml&ab_cl=erth&fname=p7_8_9.kmz";
  const string kCdata = "<![CDATA[" + kContent + "]]>";
  const string kHref = "<href>" + kCdata + "</href>";
  const string kLink = "<Link>" + kHref + "</Link>";
  string errors;
  ElementPtr root = Parse(kLink, &errors);
  ASSERT_TRUE(root);
  ASSERT_TRUE(errors.empty());
  const LinkPtr link = AsLink(root);
  ASSERT_TRUE(link);
  ASSERT_TRUE(link->has_href());
  ASSERT_TRUE(kContent == link->get_href());
}

class UrlTest : public testing::Test {
 protected:
  virtual void SetUp() {
    url_ = KmlFactory::GetFactory()->CreateUrl();
  }

  UrlPtr url_;
};

TEST_F(UrlTest, TestType) {
  ASSERT_EQ(Type_Url, url_->Type());
  ASSERT_TRUE(url_->IsA(Type_Url));
  ASSERT_TRUE(url_->IsA(Type_BasicLink));
  ASSERT_TRUE(url_->IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(UrlTest, TestDefaults) {
  ASSERT_EQ("", url_->get_href());
  ASSERT_FALSE(url_->has_href());
  ASSERT_EQ(REFRESHMODE_ONCHANGE, url_->get_refreshmode());
  ASSERT_FALSE(url_->has_refreshmode());
  ASSERT_DOUBLE_EQ(4.0, url_->get_refreshinterval());
  ASSERT_FALSE(url_->has_refreshinterval());
  ASSERT_TRUE(VIEWREFRESHMODE_NEVER == url_->get_viewrefreshmode());
  ASSERT_FALSE(url_->has_viewrefreshmode());
  ASSERT_DOUBLE_EQ(4.0, url_->get_viewrefreshtime());
  ASSERT_FALSE(url_->has_viewrefreshtime());
  ASSERT_DOUBLE_EQ(1.0, url_->get_viewboundscale());
  ASSERT_FALSE(url_->has_viewboundscale());
  ASSERT_EQ(string(""), url_->get_viewformat());
  ASSERT_FALSE(url_->has_viewformat());
  ASSERT_EQ(string(""), url_->get_httpquery());
  ASSERT_FALSE(url_->has_httpquery());
}

// Verify setting default makes has_xxx() true:
TEST_F(UrlTest, TestSetToDefaultValues) {
  // Verify default state:
  url_->set_href(url_->get_href());
  ASSERT_TRUE(url_->has_href());
  url_->set_refreshmode(url_->get_refreshmode());
  ASSERT_TRUE(url_->has_refreshmode());
  url_->set_refreshinterval(url_->get_refreshinterval());
  ASSERT_TRUE(url_->has_refreshinterval());
  url_->set_viewrefreshmode(url_->get_viewrefreshmode());
  ASSERT_TRUE(url_->has_viewrefreshmode());
  url_->set_viewrefreshtime(url_->get_viewrefreshtime());
  ASSERT_TRUE(url_->has_viewrefreshtime());
  url_->set_viewboundscale(url_->get_viewboundscale());
  ASSERT_TRUE(url_->has_viewboundscale());
  url_->set_viewformat(url_->get_viewformat());
  ASSERT_TRUE(url_->has_viewformat());
  url_->set_httpquery(url_->get_httpquery());
  ASSERT_TRUE(url_->has_httpquery());
}

// Verify set, get, has, clear:
TEST_F(UrlTest, TestSetGetHasClear) {
  // Non-default values:
  string href("href");
  RefreshModeEnum refreshmode = REFRESHMODE_ONINTERVAL;
  double refreshinterval = 1.0;
  ViewRefreshModeEnum viewrefreshmode = VIEWREFRESHMODE_ONREQUEST;
  double viewrefreshtime = 1.0;
  double viewboundscale = 0.5;
  string viewformat("viewformat");
  string httpquery("httpquery");

  // Set all fields:
  url_->set_href(href);
  url_->set_refreshmode(refreshmode);
  url_->set_refreshinterval(refreshinterval);
  url_->set_viewrefreshmode(viewrefreshmode);
  url_->set_viewrefreshtime(viewrefreshtime);
  url_->set_viewboundscale(viewboundscale);
  url_->set_viewformat(viewformat);
  url_->set_httpquery(httpquery);

  // Verify getter and has_xxx():
  ASSERT_TRUE(url_->has_href());
  ASSERT_TRUE(url_->has_refreshmode());
  ASSERT_TRUE(url_->has_refreshinterval());
  ASSERT_TRUE(url_->has_viewrefreshmode());
  ASSERT_TRUE(url_->has_viewrefreshtime());
  ASSERT_TRUE(url_->has_viewboundscale());
  ASSERT_TRUE(url_->has_viewformat());
  ASSERT_TRUE(url_->has_httpquery());

  ASSERT_EQ(href, url_->get_href());
  ASSERT_EQ(refreshmode, url_->get_refreshmode());
  ASSERT_EQ(refreshinterval, url_->get_refreshinterval());
  ASSERT_EQ(viewrefreshmode, url_->get_viewrefreshmode());
  ASSERT_EQ(viewrefreshtime, url_->get_viewrefreshtime());
  ASSERT_EQ(viewboundscale, url_->get_viewboundscale());
  ASSERT_EQ(viewformat, url_->get_viewformat());
  ASSERT_EQ(httpquery, url_->get_httpquery());

  // Clear all fields:
  url_->clear_href();
  url_->clear_refreshmode();
  url_->clear_refreshinterval();
  url_->clear_viewrefreshmode();
  url_->clear_viewrefreshtime();
  url_->clear_viewboundscale();
  url_->clear_viewformat();
  url_->clear_httpquery();
}

class IconTest : public testing::Test {
 protected:
  virtual void SetUp() {
    icon_ = KmlFactory::GetFactory()->CreateIcon();
  }

  IconPtr icon_;
};

TEST_F(IconTest, TestType) {
  ASSERT_EQ(Type_Icon, icon_->Type());
  ASSERT_TRUE(icon_->IsA(Type_Icon));
  ASSERT_TRUE(icon_->IsA(Type_BasicLink));
  ASSERT_TRUE(icon_->IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(IconTest, TestDefaults) {
  ASSERT_EQ(string(""), icon_->get_href());
  ASSERT_FALSE(icon_->has_href());
  ASSERT_EQ(REFRESHMODE_ONCHANGE, icon_->get_refreshmode());
  ASSERT_FALSE(icon_->has_refreshmode());
  ASSERT_DOUBLE_EQ(4.0, icon_->get_refreshinterval());
  ASSERT_FALSE(icon_->has_refreshinterval());
  ASSERT_EQ(VIEWREFRESHMODE_NEVER, icon_->get_viewrefreshmode());
  ASSERT_FALSE(icon_->has_viewrefreshmode());
  ASSERT_DOUBLE_EQ(4.0, icon_->get_viewrefreshtime());
  ASSERT_FALSE(icon_->has_viewrefreshtime());
  ASSERT_DOUBLE_EQ(1.0, icon_->get_viewboundscale());
  ASSERT_FALSE(icon_->has_viewboundscale());
  ASSERT_EQ(string(""), icon_->get_viewformat());
  ASSERT_FALSE(icon_->has_viewformat());
  ASSERT_EQ(string(""), icon_->get_httpquery());
  ASSERT_FALSE(icon_->has_httpquery());
}

// Verify setting default makes has_xxx() true:
TEST_F(IconTest, TestSetToDefaultValues) {
  // Verify default state:
  icon_->set_href(icon_->get_href());
  ASSERT_TRUE(icon_->has_href());
  icon_->set_refreshmode(icon_->get_refreshmode());
  ASSERT_TRUE(icon_->has_refreshmode());
  icon_->set_refreshinterval(icon_->get_refreshinterval());
  ASSERT_TRUE(icon_->has_refreshinterval());
  icon_->set_viewrefreshmode(icon_->get_viewrefreshmode());
  ASSERT_TRUE(icon_->has_viewrefreshmode());
  icon_->set_viewrefreshtime(icon_->get_viewrefreshtime());
  ASSERT_TRUE(icon_->has_viewrefreshtime());
  icon_->set_viewboundscale(icon_->get_viewboundscale());
  ASSERT_TRUE(icon_->has_viewboundscale());
  icon_->set_viewformat(icon_->get_viewformat());
  ASSERT_TRUE(icon_->has_viewformat());
  icon_->set_httpquery(icon_->get_httpquery());
  ASSERT_TRUE(icon_->has_httpquery());
}

// Verify set, get, has, clear:
TEST_F(IconTest, TestSetGetHasClear) {
  // Non-default values:
  string href("href");
  RefreshModeEnum refreshmode = REFRESHMODE_ONINTERVAL;
  double refreshinterval = 1.0;
  ViewRefreshModeEnum viewrefreshmode = VIEWREFRESHMODE_ONREQUEST;
  double viewrefreshtime = 1.0;
  double viewboundscale = 0.5;
  string viewformat("viewformat");
  string httpquery("httpquery");

  // Set all fields:
  icon_->set_href(href);
  icon_->set_refreshmode(refreshmode);
  icon_->set_refreshinterval(refreshinterval);
  icon_->set_viewrefreshmode(viewrefreshmode);
  icon_->set_viewrefreshtime(viewrefreshtime);
  icon_->set_viewboundscale(viewboundscale);
  icon_->set_viewformat(viewformat);
  icon_->set_httpquery(httpquery);

  // Verify getter and has_xxx():
  ASSERT_TRUE(icon_->has_href());
  ASSERT_TRUE(icon_->has_refreshmode());
  ASSERT_TRUE(icon_->has_refreshinterval());
  ASSERT_TRUE(icon_->has_viewrefreshmode());
  ASSERT_TRUE(icon_->has_viewrefreshtime());
  ASSERT_TRUE(icon_->has_viewboundscale());
  ASSERT_TRUE(icon_->has_viewformat());
  ASSERT_TRUE(icon_->has_httpquery());

  ASSERT_EQ(href, icon_->get_href());
  ASSERT_EQ(refreshmode, icon_->get_refreshmode());
  ASSERT_EQ(refreshinterval, icon_->get_refreshinterval());
  ASSERT_EQ(viewrefreshmode, icon_->get_viewrefreshmode());
  ASSERT_EQ(viewrefreshtime, icon_->get_viewrefreshtime());
  ASSERT_EQ(viewboundscale, icon_->get_viewboundscale());
  ASSERT_EQ(viewformat, icon_->get_viewformat());
  ASSERT_EQ(httpquery, icon_->get_httpquery());

  // Clear all fields:
  icon_->clear_href();
  icon_->clear_refreshmode();
  icon_->clear_refreshinterval();
  icon_->clear_viewrefreshmode();
  icon_->clear_viewrefreshtime();
  icon_->clear_viewboundscale();
  icon_->clear_viewformat();
  icon_->clear_httpquery();
}

class IconStyleIconTest : public testing::Test {
 protected:
  virtual void SetUp() {
    iconstyleicon_ = KmlFactory::GetFactory()->CreateIconStyleIcon();
  }

  IconStyleIconPtr iconstyleicon_;
};

TEST_F(IconStyleIconTest, TestType) {
  ASSERT_EQ(Type_IconStyleIcon, iconstyleicon_->Type());
  ASSERT_TRUE(iconstyleicon_->IsA(Type_IconStyleIcon));
  ASSERT_TRUE(iconstyleicon_->IsA(Type_BasicLink));
  ASSERT_TRUE(iconstyleicon_->IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(IconStyleIconTest, TestDefaults) {
  ASSERT_EQ(string(""), iconstyleicon_->get_href());
  ASSERT_FALSE(iconstyleicon_->has_href());
}

// Verify setting default makes has_xxx() true:
TEST_F(IconStyleIconTest, TestSetToDefaultValues) {
  // Verify default state:
  iconstyleicon_->set_href(iconstyleicon_->get_href());
  ASSERT_TRUE(iconstyleicon_->has_href());
}

// Verify set, get, has, clear:
TEST_F(IconStyleIconTest, TestSetGetHasClear) {
  // Non-default values:
  string href("href");

  // Set all fields:
  iconstyleicon_->set_href(href);

  // Verify getter and has_xxx():
  ASSERT_TRUE(iconstyleicon_->has_href());

  ASSERT_EQ(href, iconstyleicon_->get_href());

  // Clear all fields:
  iconstyleicon_->clear_href();
}

// Verify the Serialize method.
TEST_F(IconStyleIconTest, TestSerialize) {
  // Create a very special Serializer to assert that IconStyleIcon's Serialize
  // method is advertising itself as Type_IconStyleIcon.
  class VerifySerializer : public kmldom::Serializer {
   public:
    virtual void BeginById(int type_id, const Attributes& attributes) {
     ASSERT_EQ(Type_IconStyleIcon, type_id);
    }
  } test_serializer;
  // Serialize is public only on Element.
  ElementPtr e(iconstyleicon_);
  e->Serialize(test_serializer);
}

TEST_F(IconStyleIconTest, TestXmlSerialize) {
  // This is a special case in KML.
  // Verify that IconStyleIcon is serialized as "<Icon>".
  string xml_output = SerializeRaw(iconstyleicon_);
  // The following presumes the serializer does _not_ handle nil elements.
  ASSERT_EQ(0, xml_output.compare("<Icon/>"));
}

TEST_F(IconStyleIconTest, TestParseSerializeUnknown) {
  // <Icon> here is BasicLink which has no <refreshMode>.  However this should
  // be preserved as unknown xml.
  const string kIcon(
      "<Icon>"
      "<href>http://example.com/xyz.png</href>"
      "<refreshMode>onExpire</refreshMode>"
      "</Icon>");
  ElementPtr element(Parse(kIcon, NULL));
  ASSERT_TRUE(element);
  ASSERT_EQ(kIcon, SerializeRaw(element));
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
