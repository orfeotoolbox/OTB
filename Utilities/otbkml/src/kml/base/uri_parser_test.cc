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

// This file contains the unit tests for the internal UriParser class.
// This is more to test the UriParser interface as the uriparser library has
// its own internal unit test suite for its functionality (including example
// cases from RFC 3986 itself).

// TODO photooverlay $[x]
// TODO ;flyTo ,etc

#include "kml/base/uri_parser.h"
#include "kml/base/unit_test.h"

namespace kmlbase {

// This test fixture is for the unit test cases of the UriParser class.
class UriParserTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(UriParserTest);
  CPPUNIT_TEST(TestBasicCreateFromParse);
  CPPUNIT_TEST(TestBasicCreateResolvedUri);
  CPPUNIT_TEST(TestBasicParse);
  CPPUNIT_TEST(TestBasicNormalize);
  CPPUNIT_TEST(TestBasicResolve);
  CPPUNIT_TEST(TestBasicToString);
  CPPUNIT_TEST(TestBasicGetComponents);
  CPPUNIT_TEST(TestBasicUriResolutionTestCases);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestBasicCreateFromParse();
  void TestBasicCreateResolvedUri();
  void TestBasicParse();
  void TestBasicNormalize();
  void TestBasicResolve();
  void TestBasicToString();
  void TestBasicGetComponents();
  void TestBasicUriResolutionTestCases();

 private:
  boost::scoped_ptr<UriParser> uri_parser_;
  void VerifyUriResolution(const char* base, const char* relative,
                           const char* want_result);
};

CPPUNIT_TEST_SUITE_REGISTRATION(UriParserTest);

// Verify basic usage of the CreateFromParse() static method.
void UriParserTest::TestBasicCreateFromParse() {
  // Note: since UriParse does not actually generate any networking traffic
  // the hostnames and TLDs used here generally do not conform to RFC 2606.
  const std::string kUrl("http://host.com/path/to/some.kml#id");
  uri_parser_.reset(UriParser::CreateFromParse(kUrl.c_str()));
  CPPUNIT_ASSERT(uri_parser_.get());
  std::string url;
  CPPUNIT_ASSERT(uri_parser_->ToString(&url));
  CPPUNIT_ASSERT_EQUAL(kUrl, url);
}

// Verify basic usage of the CreateResolvedUri() static method.
void UriParserTest::TestBasicCreateResolvedUri() {
  const std::string kHost("http://hi.there.com/");
  const std::string kPath("blah.kml");
  const std::string kBase = kHost + kPath;
  const std::string kRelative("images/pretty.jpg");
  uri_parser_.reset(UriParser::CreateResolvedUri(kBase.c_str(),
                                                 kRelative.c_str()));
  CPPUNIT_ASSERT(uri_parser_.get());
  std::string url;
  CPPUNIT_ASSERT(uri_parser_->ToString(&url));
  CPPUNIT_ASSERT_EQUAL(kHost + kRelative, url);
}

// Verify basic usage of the Parse() method.
void UriParserTest::TestBasicParse() {
  const std::string kUrl("this/is/a/relative/url.kmz/some/file.kml#id");
  uri_parser_.reset(new UriParser);
  CPPUNIT_ASSERT(uri_parser_->Parse(kUrl.c_str()));
  std::string url;
  CPPUNIT_ASSERT(uri_parser_->ToString(&url));
  CPPUNIT_ASSERT_EQUAL(kUrl, url);
}

// Verify basic usage of the Normalize() method.
void UriParserTest::TestBasicNormalize() {
  const std::string kUrl("this/../a/relative/url.kmz/../file.kml#id");
  const std::string kNormalized("a/relative/file.kml#id");
  uri_parser_.reset(new UriParser);
  // Parse() does not normalize.
  CPPUNIT_ASSERT(uri_parser_->Parse(kUrl.c_str()));
  std::string url;
  CPPUNIT_ASSERT(uri_parser_->ToString(&url));
  CPPUNIT_ASSERT_EQUAL(kUrl, url);
  // Normalize() normalizes.
  CPPUNIT_ASSERT(uri_parser_->Normalize());
  CPPUNIT_ASSERT(uri_parser_->ToString(&url));
  CPPUNIT_ASSERT_EQUAL(kNormalized, url);
}

// Verify basic usage of the Resolve() method.
void UriParserTest::TestBasicResolve() {
  const std::string kBase("http://foo.com/hello/");
  const std::string kRelative("../hi");
  const std::string kResult("http://foo.com/hi");
  boost::scoped_ptr<UriParser> base_uri(
      UriParser::CreateFromParse(kBase.c_str()));
  boost::scoped_ptr<UriParser> relative_uri(
      UriParser::CreateFromParse(kRelative.c_str()));
  uri_parser_.reset(new UriParser);
  CPPUNIT_ASSERT(uri_parser_->Resolve(*base_uri.get(), *relative_uri.get()));
  std::string url;
  CPPUNIT_ASSERT(uri_parser_->ToString(&url));
  CPPUNIT_ASSERT_EQUAL(kResult, url);
}

// Verify basic usage of the ToString() method.
void UriParserTest::TestBasicToString() {
  uri_parser_.reset(new UriParser);
  CPPUNIT_ASSERT(!uri_parser_->ToString(NULL));
  const std::string kUrl("a/b/c/d");
  CPPUNIT_ASSERT(uri_parser_->Parse(kUrl.c_str()));
  std::string url;
  CPPUNIT_ASSERT(uri_parser_->ToString(&url));
  CPPUNIT_ASSERT_EQUAL(kUrl, url);
}

// Verify basic usage of the GetScheme(), GetHost(), GetPort(), GetPath(),
// GetQuery(), and GetFragment() methods.
void UriParserTest::TestBasicGetComponents() {
  uri_parser_.reset(new UriParser);
  // Verify NULL uri returns false for all components.
  CPPUNIT_ASSERT(!uri_parser_->GetScheme(NULL));
  CPPUNIT_ASSERT(!uri_parser_->GetHost(NULL));
  CPPUNIT_ASSERT(!uri_parser_->GetPort(NULL));
  CPPUNIT_ASSERT(!uri_parser_->GetPath(NULL));
  CPPUNIT_ASSERT(!uri_parser_->GetQuery(NULL));
  CPPUNIT_ASSERT(!uri_parser_->GetFragment(NULL));
  // Verify initial state returns false with non-NULL string output arg.
  std::string output;
  CPPUNIT_ASSERT(!uri_parser_->GetScheme(&output));
  CPPUNIT_ASSERT(output.empty());
  CPPUNIT_ASSERT(!uri_parser_->GetHost(&output));
  CPPUNIT_ASSERT(output.empty());
  CPPUNIT_ASSERT(!uri_parser_->GetPort(&output));
  CPPUNIT_ASSERT(output.empty());
  CPPUNIT_ASSERT(!uri_parser_->GetPath(&output));
  CPPUNIT_ASSERT(output.empty());
  CPPUNIT_ASSERT(!uri_parser_->GetQuery(&output));
  CPPUNIT_ASSERT(output.empty());
  CPPUNIT_ASSERT(!uri_parser_->GetFragment(&output));
  CPPUNIT_ASSERT(output.empty());

  // Verify a typical URI. 
  const std::string kScheme("http");
  const std::string kHost("www.somehost.com");
  const std::string kPath("path/to/some.kml");
  const std::string kUrlNoFragment(kScheme + "://" + kHost + "/" + kPath);
  const std::string kFragment("id");
  uri_parser_.reset(UriParser::CreateFromParse(kUrlNoFragment.c_str()));
  // Verify NULL output string returns proper status of component's existence.
  CPPUNIT_ASSERT(uri_parser_->GetScheme(NULL));
  CPPUNIT_ASSERT(uri_parser_->GetHost(NULL));
  CPPUNIT_ASSERT(!uri_parser_->GetPort(NULL));
  CPPUNIT_ASSERT(uri_parser_->GetPath(NULL));
  CPPUNIT_ASSERT(!uri_parser_->GetQuery(NULL));
  CPPUNIT_ASSERT(!uri_parser_->GetFragment(NULL));
  // Verify output string gets proper result.
  CPPUNIT_ASSERT(uri_parser_->GetScheme(&output));
  CPPUNIT_ASSERT_EQUAL(kScheme, output);
  CPPUNIT_ASSERT(uri_parser_->GetHost(&output));
  CPPUNIT_ASSERT_EQUAL(kHost, output);
  CPPUNIT_ASSERT(uri_parser_->GetPath(&output));
  CPPUNIT_ASSERT_EQUAL(kPath, output);
  output.clear();
  CPPUNIT_ASSERT(!uri_parser_->GetFragment(&output));
  CPPUNIT_ASSERT(output.empty());

  // Verify a URI with fragment.
  const std::string kUrlWithFragment(kUrlNoFragment + "#" + kFragment);
  uri_parser_.reset(UriParser::CreateFromParse(kUrlWithFragment.c_str()));
  // Verify NULL output string returns proper status of component's existence.
  CPPUNIT_ASSERT(uri_parser_->GetScheme(NULL));
  CPPUNIT_ASSERT(uri_parser_->GetHost(NULL));
  CPPUNIT_ASSERT(!uri_parser_->GetPort(NULL));
  CPPUNIT_ASSERT(uri_parser_->GetPath(NULL));
  CPPUNIT_ASSERT(!uri_parser_->GetQuery(NULL));
  CPPUNIT_ASSERT(uri_parser_->GetFragment(NULL));
  // Verify output string gets proper result.
  CPPUNIT_ASSERT(uri_parser_->GetScheme(&output));
  CPPUNIT_ASSERT_EQUAL(kScheme, output);
  CPPUNIT_ASSERT(uri_parser_->GetHost(&output));
  CPPUNIT_ASSERT_EQUAL(kHost, output);
  CPPUNIT_ASSERT(uri_parser_->GetPath(&output));
  CPPUNIT_ASSERT_EQUAL(kPath, output);
  CPPUNIT_ASSERT(uri_parser_->GetFragment(&output));
  CPPUNIT_ASSERT_EQUAL(kFragment, output);
}

// This is a table of URI resolution test cases.  The given result is the
// proper resolution of the given base with the given relative URI.  A "base"
// in the context of KML is the URI of the KML file and the "relative" is
// something that might appear in an <href>.  The unit test suite within
// the uriparser library is intended to cover the broad functionality of
// that library against the RFC 3986 standard.  These unit test cases are
// essentially KML-specific examples which all still follow RFC 3986
// (including the ones using KMZ pathnames which is opaque to the RFC 3986
// standard).
// TODO: This table does NOT contain some special KMZ handling performed by GE
//       (which may potentially be left out of libkml).
const struct {
  const char* base;
  const char* relative;
  const char* result;
} kUriTestCases[] = {
  // TODO these first NULL result test cases indicate a need for KML-specific
  //      handling likely elsewhere in libkml.  The intention is to keep
  //      UriParser true to the uriparser library.  That is, while it is not
  //      directly RFC 3986 valid to resolve against a non-absolute path this
  //      _is_ commonplace and valid in KML.
  {
  "x/a.kml",
  "b.kml",
  NULL  // NOT: "x/b.kml"
  },
  {
  "a.kml",
  "b.kml",
  NULL  // NOT: "b.kml"
  },
  {  // Note that uriparser requires the base to be absolute (has a scheme).
  "file://x",
  "y",
  "file://x/y"
  },
  {
  "file:///a.kml",
  "b.kml",
  "file:///b.kml"
  },
  {
  "file://dir/a.kml",
  "b.kml",
  "file://dir/b.kml"
  },
  {
  "http://somehost.net/DIR/path.kml",
  "/path/starts/with/slash",
  "http://somehost.net/path/starts/with/slash"
  },
  {
  "http://somehost.net/DIR/path.kml",
  "path/does/not/with/slash",
  "http://somehost.net/DIR/path/does/not/with/slash"
  },
  {
  "http://web.gaggle.com/cine/me3b/me3b.kml",
  "../me3b_daily/00.kmz",
  "http://web.gaggle.com/cine/me3b_daily/00.kmz"
  },
  {  // An absolute relative path will just be the result.
  "http://web.gaggle.com/veho/philly/root/en/philly.kml",
  "http://web.gaggle.com/veho/philly/philly0/en/mapfinder.kml",
  "http://web.gaggle.com/veho/philly/philly0/en/mapfinder.kml"
  },
  {
  "http://web.gaggle.com/veho/philly/root/en/philly.kml",
  "http://web.gaggle.com/veho/philly/philly0/kart/sat/kml/0.kmz",
  "http://web.gaggle.com/veho/philly/philly0/kart/sat/kml/0.kmz"
  },
  {
  "http://web.gaggle.com/vaer/gnrl/loopy/loopster.kml",
  "loopster/root.kmz",
  "http://web.gaggle.com/vaer/gnrl/loopy/loopster/root.kmz"
  },
  {
  "http://web.gaggle.com/cine/me3b_daily/00.kmz",
  "05.kmz",
  "http://web.gaggle.com/cine/me3b_daily/05.kmz"
  },
  {  // Up outside the KMZ to a normal file.
  "http://web.gaggle.com/veho/philly/philly0/kart/sat/kml/0.kmz",
  "../imagery/0_10.jpg",
  "http://web.gaggle.com/veho/philly/philly0/kart/sat/imagery/0_10.jpg"
  },
  {
  "http://web.gaggle.com/veho/philly/philly0/kart/sat/kml/0.kmz",
  "00.kmz",
  "http://web.gaggle.com/veho/philly/philly0/kart/sat/kml/00.kmz"
  },
  {
  "http://web.gaggle.com/veho/philly/philly0/kart/sat/kml/0.kmz",
  "../imagery/0_10.jpg",
  "http://web.gaggle.com/veho/philly/philly0/kart/sat/imagery/0_10.jpg"
  },
  {
  "http://web.gaggle.com/vaer/gnrl/loopy/loopster/root.kmz/doc.kml",
  "level00/0.kml",
  "http://web.gaggle.com/vaer/gnrl/loopy/loopster/root.kmz/level00/0.kml"
  },
  {
  "http://web.gaggle.com/vaer/gnrl/loopy/loopster/root.kmz/doc.kml",
  "level00/0.kml",
  "http://web.gaggle.com/vaer/gnrl/loopy/loopster/root.kmz/level00/0.kml"
  },
  {
  "http://web.gaggle.com/vaer/gnrl/loopy/loopster/root.kmz/level00/0.kml",
  "../level01/02.kml",
  "http://web.gaggle.com/vaer/gnrl/loopy/loopster/root.kmz/level01/02.kml"
  },
  {
  "http://web.gaggle.com/vaer/gnrl/loopy/loopster/root.kmz/level01/02.kml",
  "../level02/020.kml",
  "http://web.gaggle.com/vaer/gnrl/loopy/loopster/root.kmz/level02/020.kml"
  },
  {  // Relative within the same KMZ.
  "http://web.gaggle.com/vaer/nimbus-now/nrl/loopy/loopster/root.kmz/level02/"
      "020.kml",
  "../level03/0202.kml",
  "http://web.gaggle.com/vaer/nimbus-now/nrl/loopy/loopster/root.kmz/level03/"
      "0202.kml"
  },
  {  // Relative up and over into another KMZ.
  "http://web.gaggle.com/vaer/nimbus-now/nrl/loopy/loopster/root.kmz/level03/"
      "0202.kml",
  "../../images0018.kmz/images/0202.png",
  "http://web.gaggle.com/vaer/nimbus-now/nrl/loopy/loopster/images0018.kmz/"
      "images/0202.png"
  }
};

// This is an internal utility to use UriParser::CreateResolvedUri()
// to resolve and verify a given test case.
void UriParserTest::VerifyUriResolution(const char* base, const char* relative,
                                        const char* want_result) {
  std::string got_result;
  if (want_result == NULL) {  // We're expecting resolution to fail.
    CPPUNIT_ASSERT(!UriParser::CreateResolvedUri(base, relative));
    return;
  }
  uri_parser_.reset(UriParser::CreateResolvedUri(base, relative));
  uri_parser_->ToString(&got_result);
  CPPUNIT_ASSERT_EQUAL(std::string(want_result), got_result);
}

// Verify all expected URI resolution test cases.
void UriParserTest::TestBasicUriResolutionTestCases() {
  const size_t size = sizeof(kUriTestCases)/sizeof(kUriTestCases[0]);
  for (size_t i = 0; i < size; ++i) {
    VerifyUriResolution(kUriTestCases[i].base,
                             kUriTestCases[i].relative,
                             kUriTestCases[i].result);
  }
}

}  // end namespace kmlbase

TEST_MAIN
