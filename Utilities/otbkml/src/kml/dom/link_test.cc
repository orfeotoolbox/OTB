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
#include <string>
#include "kml/dom/kml22.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_funcs.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_ptr.h"
#include "kml/base/unit_test.h"

namespace kmldom {

class LinkTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(LinkTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST(TestParse);
  CPPUNIT_TEST(TestAcceptCdataInHref);
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp() {
    link_ = KmlFactory::GetFactory()->CreateLink();
  }

  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();
  void TestParse();
  void TestAcceptCdataInHref();

 private:
  LinkPtr link_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(LinkTest);

void LinkTest::TestType() {
  CPPUNIT_ASSERT(Type_Link == link_->Type());
  CPPUNIT_ASSERT(true == link_->IsA(Type_Link));
  CPPUNIT_ASSERT(true == link_->IsA(Type_BasicLink));
  CPPUNIT_ASSERT(true == link_->IsA(Type_Object));
}

// Verify proper defaults:
void LinkTest::TestDefaults() {
  CPPUNIT_ASSERT("" == link_->get_href());
  CPPUNIT_ASSERT(false == link_->has_href());
  CPPUNIT_ASSERT(REFRESHMODE_ONCHANGE == link_->get_refreshmode());
  CPPUNIT_ASSERT(false == link_->has_refreshmode());
  CPPUNIT_ASSERT(4.0 == link_->get_refreshinterval());
  CPPUNIT_ASSERT(false == link_->has_refreshinterval());
  CPPUNIT_ASSERT(VIEWREFRESHMODE_NEVER == link_->get_viewrefreshmode());
  CPPUNIT_ASSERT(false == link_->has_viewrefreshmode());
  CPPUNIT_ASSERT(4.0 == link_->get_viewrefreshtime());
  CPPUNIT_ASSERT(false == link_->has_viewrefreshtime());
  CPPUNIT_ASSERT(1.0 == link_->get_viewboundscale());
  CPPUNIT_ASSERT(false == link_->has_viewboundscale());
  CPPUNIT_ASSERT("" == link_->get_viewformat());
  CPPUNIT_ASSERT(false == link_->has_viewformat());
  CPPUNIT_ASSERT("" == link_->get_httpquery());
  CPPUNIT_ASSERT(false == link_->has_httpquery());
}

// Verify setting default makes has_xxx() true:
void LinkTest::TestSetToDefaultValues() {
  // Verify default state:
  TestDefaults();
  link_->set_href(link_->get_href());
  CPPUNIT_ASSERT(true == link_->has_href());
  link_->set_refreshmode(link_->get_refreshmode());
  CPPUNIT_ASSERT(true == link_->has_refreshmode());
  link_->set_refreshinterval(link_->get_refreshinterval());
  CPPUNIT_ASSERT(true == link_->has_refreshinterval());
  link_->set_viewrefreshmode(link_->get_viewrefreshmode());
  CPPUNIT_ASSERT(true == link_->has_viewrefreshmode());
  link_->set_viewrefreshtime(link_->get_viewrefreshtime());
  CPPUNIT_ASSERT(true == link_->has_viewrefreshtime());
  link_->set_viewboundscale(link_->get_viewboundscale());
  CPPUNIT_ASSERT(true == link_->has_viewboundscale());
  link_->set_viewformat(link_->get_viewformat());
  CPPUNIT_ASSERT(true == link_->has_viewformat());
  link_->set_httpquery(link_->get_httpquery());
  CPPUNIT_ASSERT(true == link_->has_httpquery());
}

// Verify set, get, has, clear:
void LinkTest::TestSetGetHasClear() {
  // Non-default values:
  std::string href("href");
  RefreshModeEnum refreshmode = REFRESHMODE_ONINTERVAL;
  double refreshinterval = 1.0;
  ViewRefreshModeEnum viewrefreshmode = VIEWREFRESHMODE_ONREQUEST;
  double viewrefreshtime = 1.0;
  double viewboundscale = 0.5;
  std::string viewformat("viewformat");
  std::string httpquery("httpquery");

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
  CPPUNIT_ASSERT(true == link_->has_href());
  CPPUNIT_ASSERT(true == link_->has_refreshmode());
  CPPUNIT_ASSERT(true == link_->has_refreshinterval());
  CPPUNIT_ASSERT(true == link_->has_viewrefreshmode());
  CPPUNIT_ASSERT(true == link_->has_viewrefreshtime());
  CPPUNIT_ASSERT(true == link_->has_viewboundscale());
  CPPUNIT_ASSERT(true == link_->has_viewformat());
  CPPUNIT_ASSERT(true == link_->has_httpquery());

  CPPUNIT_ASSERT(href == link_->get_href());
  CPPUNIT_ASSERT(refreshmode == link_->get_refreshmode());
  CPPUNIT_ASSERT(refreshinterval == link_->get_refreshinterval());
  CPPUNIT_ASSERT(viewrefreshmode == link_->get_viewrefreshmode());
  CPPUNIT_ASSERT(viewrefreshtime == link_->get_viewrefreshtime());
  CPPUNIT_ASSERT(viewboundscale == link_->get_viewboundscale());
  CPPUNIT_ASSERT(viewformat == link_->get_viewformat());
  CPPUNIT_ASSERT(httpquery == link_->get_httpquery());

  // Clear all fields:
  link_->clear_href();
  link_->clear_refreshmode();
  link_->clear_refreshinterval();
  link_->clear_viewrefreshmode();
  link_->clear_viewrefreshtime();
  link_->clear_viewboundscale();
  link_->clear_viewformat();
  link_->clear_httpquery();

  // Verify now in default state:
  TestDefaults();
}


void LinkTest::TestParse() {
  const std::string kContent = "foo.kml";
  const std::string kHref = "<href>" + kContent + "</href>";
  const std::string kLink = "<Link>" + kHref + "</Link>";
  std::string errors;
  ElementPtr root = Parse(kLink, &errors);
  CPPUNIT_ASSERT(root);
  CPPUNIT_ASSERT(errors.empty());
  const LinkPtr link = AsLink(root);
  CPPUNIT_ASSERT(link);
  CPPUNIT_ASSERT(link->has_href());
  CPPUNIT_ASSERT(kContent == link->get_href());
}

void LinkTest::TestAcceptCdataInHref() {
  const std::string kContent = "abl?output=kml&ab_cl=erth&fname=p7_8_9.kmz";
  const std::string kCdata = "<![CDATA[" + kContent + "]]>";
  const std::string kHref = "<href>" + kCdata + "</href>";
  const std::string kLink = "<Link>" + kHref + "</Link>";
  std::string errors;
  ElementPtr root = Parse(kLink, &errors);
  CPPUNIT_ASSERT(root);
  CPPUNIT_ASSERT(errors.empty());
  const LinkPtr link = AsLink(root);
  CPPUNIT_ASSERT(link);
  CPPUNIT_ASSERT(link->has_href());
  CPPUNIT_ASSERT(kContent == link->get_href());
}

class UrlTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(UrlTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp() {
    url_ = KmlFactory::GetFactory()->CreateUrl();
  }

  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();

 private:
  UrlPtr url_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(UrlTest);

void UrlTest::TestType() {
  CPPUNIT_ASSERT(Type_Url == url_->Type());
  CPPUNIT_ASSERT(true == url_->IsA(Type_Url));
  CPPUNIT_ASSERT(true == url_->IsA(Type_BasicLink));
  CPPUNIT_ASSERT(true == url_->IsA(Type_Object));
}

// Verify proper defaults:
void UrlTest::TestDefaults() {
  CPPUNIT_ASSERT("" == url_->get_href());
  CPPUNIT_ASSERT(false == url_->has_href());
  CPPUNIT_ASSERT(REFRESHMODE_ONCHANGE == url_->get_refreshmode());
  CPPUNIT_ASSERT(false == url_->has_refreshmode());
  CPPUNIT_ASSERT(4.0 == url_->get_refreshinterval());
  CPPUNIT_ASSERT(false == url_->has_refreshinterval());
  CPPUNIT_ASSERT(VIEWREFRESHMODE_NEVER == url_->get_viewrefreshmode());
  CPPUNIT_ASSERT(false == url_->has_viewrefreshmode());
  CPPUNIT_ASSERT(4.0 == url_->get_viewrefreshtime());
  CPPUNIT_ASSERT(false == url_->has_viewrefreshtime());
  CPPUNIT_ASSERT(1.0 == url_->get_viewboundscale());
  CPPUNIT_ASSERT(false == url_->has_viewboundscale());
  CPPUNIT_ASSERT("" == url_->get_viewformat());
  CPPUNIT_ASSERT(false == url_->has_viewformat());
  CPPUNIT_ASSERT("" == url_->get_httpquery());
  CPPUNIT_ASSERT(false == url_->has_httpquery());
}

// Verify setting default makes has_xxx() true:
void UrlTest::TestSetToDefaultValues() {
  // Verify default state:
  TestDefaults();
  url_->set_href(url_->get_href());
  CPPUNIT_ASSERT(true == url_->has_href());
  url_->set_refreshmode(url_->get_refreshmode());
  CPPUNIT_ASSERT(true == url_->has_refreshmode());
  url_->set_refreshinterval(url_->get_refreshinterval());
  CPPUNIT_ASSERT(true == url_->has_refreshinterval());
  url_->set_viewrefreshmode(url_->get_viewrefreshmode());
  CPPUNIT_ASSERT(true == url_->has_viewrefreshmode());
  url_->set_viewrefreshtime(url_->get_viewrefreshtime());
  CPPUNIT_ASSERT(true == url_->has_viewrefreshtime());
  url_->set_viewboundscale(url_->get_viewboundscale());
  CPPUNIT_ASSERT(true == url_->has_viewboundscale());
  url_->set_viewformat(url_->get_viewformat());
  CPPUNIT_ASSERT(true == url_->has_viewformat());
  url_->set_httpquery(url_->get_httpquery());
  CPPUNIT_ASSERT(true == url_->has_httpquery());
}

// Verify set, get, has, clear:
void UrlTest::TestSetGetHasClear() {
  // Non-default values:
  std::string href("href");
  RefreshModeEnum refreshmode = REFRESHMODE_ONINTERVAL;
  double refreshinterval = 1.0;
  ViewRefreshModeEnum viewrefreshmode = VIEWREFRESHMODE_ONREQUEST;
  double viewrefreshtime = 1.0;
  double viewboundscale = 0.5;
  std::string viewformat("viewformat");
  std::string httpquery("httpquery");

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
  CPPUNIT_ASSERT(true == url_->has_href());
  CPPUNIT_ASSERT(true == url_->has_refreshmode());
  CPPUNIT_ASSERT(true == url_->has_refreshinterval());
  CPPUNIT_ASSERT(true == url_->has_viewrefreshmode());
  CPPUNIT_ASSERT(true == url_->has_viewrefreshtime());
  CPPUNIT_ASSERT(true == url_->has_viewboundscale());
  CPPUNIT_ASSERT(true == url_->has_viewformat());
  CPPUNIT_ASSERT(true == url_->has_httpquery());

  CPPUNIT_ASSERT(href == url_->get_href());
  CPPUNIT_ASSERT(refreshmode == url_->get_refreshmode());
  CPPUNIT_ASSERT(refreshinterval == url_->get_refreshinterval());
  CPPUNIT_ASSERT(viewrefreshmode == url_->get_viewrefreshmode());
  CPPUNIT_ASSERT(viewrefreshtime == url_->get_viewrefreshtime());
  CPPUNIT_ASSERT(viewboundscale == url_->get_viewboundscale());
  CPPUNIT_ASSERT(viewformat == url_->get_viewformat());
  CPPUNIT_ASSERT(httpquery == url_->get_httpquery());

  // Clear all fields:
  url_->clear_href();
  url_->clear_refreshmode();
  url_->clear_refreshinterval();
  url_->clear_viewrefreshmode();
  url_->clear_viewrefreshtime();
  url_->clear_viewboundscale();
  url_->clear_viewformat();
  url_->clear_httpquery();

  // Verify now in default state:
  TestDefaults();
}

class IconTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(IconTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp() {
    icon_ = KmlFactory::GetFactory()->CreateIcon();
  }

  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();

 private:
  IconPtr icon_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(IconTest);

void IconTest::TestType() {
  CPPUNIT_ASSERT(Type_Icon == icon_->Type());
  CPPUNIT_ASSERT(true == icon_->IsA(Type_Icon));
  CPPUNIT_ASSERT(true == icon_->IsA(Type_BasicLink));
  CPPUNIT_ASSERT(true == icon_->IsA(Type_Object));
}

// Verify proper defaults:
void IconTest::TestDefaults() {
  CPPUNIT_ASSERT("" == icon_->get_href());
  CPPUNIT_ASSERT(false == icon_->has_href());
  CPPUNIT_ASSERT(REFRESHMODE_ONCHANGE == icon_->get_refreshmode());
  CPPUNIT_ASSERT(false == icon_->has_refreshmode());
  CPPUNIT_ASSERT(4.0 == icon_->get_refreshinterval());
  CPPUNIT_ASSERT(false == icon_->has_refreshinterval());
  CPPUNIT_ASSERT(VIEWREFRESHMODE_NEVER == icon_->get_viewrefreshmode());
  CPPUNIT_ASSERT(false == icon_->has_viewrefreshmode());
  CPPUNIT_ASSERT(4.0 == icon_->get_viewrefreshtime());
  CPPUNIT_ASSERT(false == icon_->has_viewrefreshtime());
  CPPUNIT_ASSERT(1.0 == icon_->get_viewboundscale());
  CPPUNIT_ASSERT(false == icon_->has_viewboundscale());
  CPPUNIT_ASSERT("" == icon_->get_viewformat());
  CPPUNIT_ASSERT(false == icon_->has_viewformat());
  CPPUNIT_ASSERT("" == icon_->get_httpquery());
  CPPUNIT_ASSERT(false == icon_->has_httpquery());
}

// Verify setting default makes has_xxx() true:
void IconTest::TestSetToDefaultValues() {
  // Verify default state:
  TestDefaults();
  icon_->set_href(icon_->get_href());
  CPPUNIT_ASSERT(true == icon_->has_href());
  icon_->set_refreshmode(icon_->get_refreshmode());
  CPPUNIT_ASSERT(true == icon_->has_refreshmode());
  icon_->set_refreshinterval(icon_->get_refreshinterval());
  CPPUNIT_ASSERT(true == icon_->has_refreshinterval());
  icon_->set_viewrefreshmode(icon_->get_viewrefreshmode());
  CPPUNIT_ASSERT(true == icon_->has_viewrefreshmode());
  icon_->set_viewrefreshtime(icon_->get_viewrefreshtime());
  CPPUNIT_ASSERT(true == icon_->has_viewrefreshtime());
  icon_->set_viewboundscale(icon_->get_viewboundscale());
  CPPUNIT_ASSERT(true == icon_->has_viewboundscale());
  icon_->set_viewformat(icon_->get_viewformat());
  CPPUNIT_ASSERT(true == icon_->has_viewformat());
  icon_->set_httpquery(icon_->get_httpquery());
  CPPUNIT_ASSERT(true == icon_->has_httpquery());
}

// Verify set, get, has, clear:
void IconTest::TestSetGetHasClear() {
  // Non-default values:
  std::string href("href");
  RefreshModeEnum refreshmode = REFRESHMODE_ONINTERVAL;
  double refreshinterval = 1.0;
  ViewRefreshModeEnum viewrefreshmode = VIEWREFRESHMODE_ONREQUEST;
  double viewrefreshtime = 1.0;
  double viewboundscale = 0.5;
  std::string viewformat("viewformat");
  std::string httpquery("httpquery");

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
  CPPUNIT_ASSERT(true == icon_->has_href());
  CPPUNIT_ASSERT(true == icon_->has_refreshmode());
  CPPUNIT_ASSERT(true == icon_->has_refreshinterval());
  CPPUNIT_ASSERT(true == icon_->has_viewrefreshmode());
  CPPUNIT_ASSERT(true == icon_->has_viewrefreshtime());
  CPPUNIT_ASSERT(true == icon_->has_viewboundscale());
  CPPUNIT_ASSERT(true == icon_->has_viewformat());
  CPPUNIT_ASSERT(true == icon_->has_httpquery());

  CPPUNIT_ASSERT(href == icon_->get_href());
  CPPUNIT_ASSERT(refreshmode == icon_->get_refreshmode());
  CPPUNIT_ASSERT(refreshinterval == icon_->get_refreshinterval());
  CPPUNIT_ASSERT(viewrefreshmode == icon_->get_viewrefreshmode());
  CPPUNIT_ASSERT(viewrefreshtime == icon_->get_viewrefreshtime());
  CPPUNIT_ASSERT(viewboundscale == icon_->get_viewboundscale());
  CPPUNIT_ASSERT(viewformat == icon_->get_viewformat());
  CPPUNIT_ASSERT(httpquery == icon_->get_httpquery());

  // Clear all fields:
  icon_->clear_href();
  icon_->clear_refreshmode();
  icon_->clear_refreshinterval();
  icon_->clear_viewrefreshmode();
  icon_->clear_viewrefreshtime();
  icon_->clear_viewboundscale();
  icon_->clear_viewformat();
  icon_->clear_httpquery();

  // Verify now in default state:
  TestDefaults();
}

class IconStyleIconTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(IconStyleIconTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST(TestSerialize);
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp() {
    iconstyleicon_ = KmlFactory::GetFactory()->CreateIconStyleIcon();
  }

  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();
  void TestSerialize();

 private:
  IconStyleIconPtr iconstyleicon_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(IconStyleIconTest);

void IconStyleIconTest::TestType() {
  CPPUNIT_ASSERT(Type_IconStyleIcon == iconstyleicon_->Type());
  CPPUNIT_ASSERT(true == iconstyleicon_->IsA(Type_IconStyleIcon));
  CPPUNIT_ASSERT(true == iconstyleicon_->IsA(Type_BasicLink));
  CPPUNIT_ASSERT(true == iconstyleicon_->IsA(Type_Object));
}

// Verify proper defaults:
void IconStyleIconTest::TestDefaults() {
  CPPUNIT_ASSERT("" == iconstyleicon_->get_href());
  CPPUNIT_ASSERT(false == iconstyleicon_->has_href());
}

// Verify setting default makes has_xxx() true:
void IconStyleIconTest::TestSetToDefaultValues() {
  // Verify default state:
  TestDefaults();
  iconstyleicon_->set_href(iconstyleicon_->get_href());
  CPPUNIT_ASSERT(true == iconstyleicon_->has_href());
}

// Verify set, get, has, clear:
void IconStyleIconTest::TestSetGetHasClear() {
  // Non-default values:
  std::string href("href");

  // Set all fields:
  iconstyleicon_->set_href(href);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(true == iconstyleicon_->has_href());

  CPPUNIT_ASSERT(href == iconstyleicon_->get_href());

  // Clear all fields:
  iconstyleicon_->clear_href();

  // Verify now in default state:
  TestDefaults();
}

// Verify the Serialize method.
void IconStyleIconTest::TestSerialize() {
  // This is a special case in KML.
  // Verify that IconStyleIcon is serialized as "<Icon>".
  std::string xml_output = SerializeRaw(iconstyleicon_);
  // The following presumes the serializer does _not_ handle nil elements.
  CPPUNIT_ASSERT_EQUAL(0, xml_output.compare("<Icon/>"));
}

}  // end namespace kmldom

TEST_MAIN
