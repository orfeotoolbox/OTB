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
#include "boost/scoped_ptr.hpp"
#include "gtest/gtest.h"

namespace kmlbase {

// This test fixture is for the unit test cases of the UriParser class.
class UriParserTest : public testing::Test {
 protected:
  boost::scoped_ptr<UriParser> uri_parser_;
  void VerifyUriResolution(const char* base, const char* relative,
                           const char* want_result);
};

// Verify basic usage of the CreateFromParse() static method.
TEST_F(UriParserTest, TestBasicCreateFromParse) {
  // Note: since UriParse does not actually generate any networking traffic
  // the hostnames and TLDs used here generally do not conform to RFC 2606.
  const string kUrl("http://host.com/path/to/some.kml#id");
  uri_parser_.reset(UriParser::CreateFromParse(kUrl.c_str()));
  ASSERT_TRUE(uri_parser_.get());
  string url;
  ASSERT_TRUE(uri_parser_->ToString(&url));
  ASSERT_EQ(kUrl, url);
}

// Verify basic usage of the CreateResolvedUri() static method.
TEST_F(UriParserTest, TestBasicCreateResolvedUri) {
  const string kHost("http://hi.there.com/");
  const string kPath("blah.kml");
  const string kBase = kHost + kPath;
  const string kRelative("images/pretty.jpg");
  uri_parser_.reset(UriParser::CreateResolvedUri(kBase.c_str(),
                                                 kRelative.c_str()));
  ASSERT_TRUE(uri_parser_.get());
  string url;
  ASSERT_TRUE(uri_parser_->ToString(&url));
  ASSERT_EQ(kHost + kRelative, url);
}

// Verify basic usage of the Parse() method.
TEST_F(UriParserTest, TestBasicParse) {
  const string kUrl("this/is/a/relative/url.kmz/some/file.kml#id");
  uri_parser_.reset(new UriParser);
  ASSERT_TRUE(uri_parser_->Parse(kUrl.c_str()));
  string url;
  ASSERT_TRUE(uri_parser_->ToString(&url));
  ASSERT_EQ(kUrl, url);
}

// Verify basic usage of the Normalize() method.
TEST_F(UriParserTest, TestBasicNormalize) {
  const string kUrl("this/../a/relative/url.kmz/../file.kml#id");
  const string kNormalized("a/relative/file.kml#id");
  uri_parser_.reset(new UriParser);
  // Parse() does not normalize.
  ASSERT_TRUE(uri_parser_->Parse(kUrl.c_str()));
  string url;
  ASSERT_TRUE(uri_parser_->ToString(&url));
  ASSERT_EQ(kUrl, url);
  // Normalize() normalizes.
  ASSERT_TRUE(uri_parser_->Normalize());
  ASSERT_TRUE(uri_parser_->ToString(&url));
  ASSERT_EQ(kNormalized, url);
}

// Verify basic usage of the Resolve() method.
TEST_F(UriParserTest, TestBasicResolve) {
  const string kBase("http://foo.com/hello/");
  const string kRelative("../hi");
  const string kResult("http://foo.com/hi");
  boost::scoped_ptr<UriParser> base_uri(
      UriParser::CreateFromParse(kBase.c_str()));
  boost::scoped_ptr<UriParser> relative_uri(
      UriParser::CreateFromParse(kRelative.c_str()));
  uri_parser_.reset(new UriParser);
  ASSERT_TRUE(uri_parser_->Resolve(*base_uri.get(), *relative_uri.get()));
  string url;
  ASSERT_TRUE(uri_parser_->ToString(&url));
  ASSERT_EQ(kResult, url);
}

// Verify basic usage of the ToString() method.
TEST_F(UriParserTest, TestBasicToString) {
  uri_parser_.reset(new UriParser);
  ASSERT_FALSE(uri_parser_->ToString(NULL));
  const string kUrl("a/b/c/d");
  ASSERT_TRUE(uri_parser_->Parse(kUrl.c_str()));
  string url;
  ASSERT_TRUE(uri_parser_->ToString(&url));
  ASSERT_EQ(kUrl, url);
}

// Verify basic usage of the GetScheme(), GetHost(), GetPort(), GetPath(),
// GetQuery(), and GetFragment() methods.
TEST_F(UriParserTest, TestBasicGetComponents) {
  uri_parser_.reset(new UriParser);
  // Verify NULL uri returns false for all components.
  ASSERT_FALSE(uri_parser_->GetScheme(NULL));
  ASSERT_FALSE(uri_parser_->GetHost(NULL));
  ASSERT_FALSE(uri_parser_->GetPort(NULL));
  ASSERT_FALSE(uri_parser_->GetPath(NULL));
  ASSERT_FALSE(uri_parser_->GetQuery(NULL));
  ASSERT_FALSE(uri_parser_->GetFragment(NULL));
  // Verify initial state returns false with non-NULL string output arg.
  string output;
  ASSERT_FALSE(uri_parser_->GetScheme(&output));
  ASSERT_TRUE(output.empty());
  ASSERT_FALSE(uri_parser_->GetHost(&output));
  ASSERT_TRUE(output.empty());
  ASSERT_FALSE(uri_parser_->GetPort(&output));
  ASSERT_TRUE(output.empty());
  ASSERT_FALSE(uri_parser_->GetPath(&output));
  ASSERT_TRUE(output.empty());
  ASSERT_FALSE(uri_parser_->GetQuery(&output));
  ASSERT_TRUE(output.empty());
  ASSERT_FALSE(uri_parser_->GetFragment(&output));
  ASSERT_TRUE(output.empty());

  // Verify a typical URI. 
  const string kScheme("http");
  const string kHost("www.somehost.com");
  const string kPath("path/to/some.kml");
  const string kUrlNoFragment(kScheme + "://" + kHost + "/" + kPath);
  const string kFragment("id");
  uri_parser_.reset(UriParser::CreateFromParse(kUrlNoFragment.c_str()));
  // Verify NULL output string returns proper status of component's existence.
  ASSERT_TRUE(uri_parser_->GetScheme(NULL));
  ASSERT_TRUE(uri_parser_->GetHost(NULL));
  ASSERT_FALSE(uri_parser_->GetPort(NULL));
  ASSERT_TRUE(uri_parser_->GetPath(NULL));
  ASSERT_FALSE(uri_parser_->GetQuery(NULL));
  ASSERT_FALSE(uri_parser_->GetFragment(NULL));
  // Verify output string gets proper result.
  ASSERT_TRUE(uri_parser_->GetScheme(&output));
  ASSERT_EQ(kScheme, output);
  ASSERT_TRUE(uri_parser_->GetHost(&output));
  ASSERT_EQ(kHost, output);
  ASSERT_TRUE(uri_parser_->GetPath(&output));
  ASSERT_EQ(kPath, output);
  output.clear();
  ASSERT_FALSE(uri_parser_->GetFragment(&output));
  ASSERT_TRUE(output.empty());

  // Verify a URI with fragment.
  const string kUrlWithFragment(kUrlNoFragment + "#" + kFragment);
  uri_parser_.reset(UriParser::CreateFromParse(kUrlWithFragment.c_str()));
  // Verify NULL output string returns proper status of component's existence.
  ASSERT_TRUE(uri_parser_->GetScheme(NULL));
  ASSERT_TRUE(uri_parser_->GetHost(NULL));
  ASSERT_FALSE(uri_parser_->GetPort(NULL));
  ASSERT_TRUE(uri_parser_->GetPath(NULL));
  ASSERT_FALSE(uri_parser_->GetQuery(NULL));
  ASSERT_TRUE(uri_parser_->GetFragment(NULL));
  // Verify output string gets proper result.
  ASSERT_TRUE(uri_parser_->GetScheme(&output));
  ASSERT_EQ(kScheme, output);
  ASSERT_TRUE(uri_parser_->GetHost(&output));
  ASSERT_EQ(kHost, output);
  ASSERT_TRUE(uri_parser_->GetPath(&output));
  ASSERT_EQ(kPath, output);
  ASSERT_TRUE(uri_parser_->GetFragment(&output));
  ASSERT_EQ(kFragment, output);
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
static const struct {
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
  string got_result;
  if (want_result == NULL) {  // We're expecting resolution to fail.
    ASSERT_FALSE(UriParser::CreateResolvedUri(base, relative));
    return;
  }
  uri_parser_.reset(UriParser::CreateResolvedUri(base, relative));
  uri_parser_->ToString(&got_result);
  ASSERT_EQ(string(want_result), got_result);
}

// Verify all expected URI resolution test cases.
TEST_F(UriParserTest, TestBasicUriResolutionTestCases) {
  const size_t size = sizeof(kUriTestCases)/sizeof(kUriTestCases[0]);
  for (size_t i = 0; i < size; ++i) {
    VerifyUriResolution(kUriTestCases[i].base,
                             kUriTestCases[i].relative,
                             kUriTestCases[i].result);
  }
}

static const struct {
  const char* unix_filename;
  const char* unix_uri;
  const char* windows_filename;
  const char* windows_uri;
} kUriFilenameCases[] = {
  {
  "/home/libkml/foo.bar",
  "file:///home/libkml/foo.bar",
  "C:\\home\\libkml\\foo.bar",
  "file:///C:/home/libkml/foo.bar"
  },
  {
  "/this/path has/some spaces in/it",
  "file:///this/path%20has/some%20spaces%20in/it",
  "C:\\this\\path has\\some spaces in\\it",
  "file:///C:/this/path%20has/some%20spaces%20in/it"
  },
  {
  "some/relative path/to a.file",
  "some/relative%20path/to%20a.file",
  "some\\relative path\\to a.file",
  "some/relative%20path/to%20a.file"
  }
};

TEST_F(UriParserTest, TestUriFilenameConversions) {
  const size_t size = sizeof(kUriFilenameCases)/sizeof(kUriFilenameCases[0]);
  for (size_t i = 0; i < size; ++i) {
    // Unix filename to URI.
    string unix_uri;
    ASSERT_TRUE(
        UriParser::UnixFilenameToUri(kUriFilenameCases[i].unix_filename,
                                     &unix_uri));
    ASSERT_EQ(string(kUriFilenameCases[i].unix_uri), unix_uri);
    // Windows filename to URI.
    string windows_uri;
    ASSERT_TRUE(
        UriParser::WindowsFilenameToUri(kUriFilenameCases[i].windows_filename,
                                        &windows_uri));
    ASSERT_EQ(string(kUriFilenameCases[i].windows_uri), windows_uri);
    // URI to unix filename.
    string unix_filename;
    ASSERT_TRUE(UriParser::UriToUnixFilename(kUriFilenameCases[i].unix_uri,
                                             &unix_filename));
    ASSERT_EQ(string(kUriFilenameCases[i].unix_filename), unix_filename);
    // URI to windows filename.
    string windows_filename;
    ASSERT_TRUE(
        UriParser::UriToWindowsFilename(kUriFilenameCases[i].windows_uri,
                                        &windows_filename));
    ASSERT_EQ(string(kUriFilenameCases[i].windows_filename),
              windows_filename);
  }
}

TEST_F(UriParserTest, TestUriToFilename) {
  // This simply tests that the ifdef works as expected.
  string uri;
  string expected_filename;
#ifdef WIN32
  uri = "file:///C:/home/libkml/foo.bar";
  expected_filename = "C:\\home\\libkml\\foo.bar";
#else
  uri = "file:///home/libkml/foo.bar";
  expected_filename = "/home/libkml/foo.bar";
#endif
  string filename;
  ASSERT_TRUE(UriParser::UriToFilename(uri, &filename));
  ASSERT_EQ(expected_filename, filename);
}

TEST_F(UriParserTest, TestFilenameToUri) {
  // This simply tests that the ifdef works as expected.
  string filename;
  string expected_uri;
#ifdef WIN32
  filename = "C:\\home\\libkml\\foo.bar";
  expected_uri = "file:///C:/home/libkml/foo.bar";
#else
  filename = "/home/libkml/foo.bar";
  expected_uri = "file:///home/libkml/foo.bar";
#endif
  string uri;
  ASSERT_TRUE(UriParser::FilenameToUri(filename, &uri));
  ASSERT_EQ(expected_uri, uri);
}

}  // end namespace kmlbase

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
