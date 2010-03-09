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

// This file contains the unit tests for the ExpatHandlerNs class.

#include "kml/base/expat_handler_ns.h"
#include "boost/scoped_ptr.hpp"
#include "kml/base/attributes.h"
#include "kml/base/expat_parser.h"
#include "kml/base/xmlns.h"
#include "gtest/gtest.h"

using kmlbase::Attributes;
using kmlbase::Xmlns;

namespace kmlbase {

class ExpatHandlerNsTest : public testing::Test {
};

// Note: no namespace can use the kExpatNsSeparator (see base/expat_parser.h).
static const char kDefaultNamespace[] = "this:is:the:default:namespace";
static const char kDefaultPrefix[] = "default";
static const char kOtherNamespace[] = "this:is:another:namespace";
static const char kOtherPrefix[] = "other";

typedef std::vector<string> StringVector;

class TestHandler : public ExpatHandler {
 public:
  TestHandler(StringVector* log) : log_(log) {}
  virtual void StartElement(const string& name,
                            const StringVector& atts) {
    // TODO: handle attributes
    log_->push_back(name);
  }
  virtual void EndElement(const string& name) {
    log_->push_back(name);
  }
  virtual void CharData(const string& s) {
    // Nothing namespace/prefix related here.  Just ignore.
  }

 private:
  StringVector* log_;
};

// This verifies ExpatHandlerNs's StartElement() and EndElement() on XML with
// the default namespace.
TEST_F(ExpatHandlerNsTest, TestExpatParserDefault) {
  Attributes attributes;
  attributes.SetString("xmlns", kDefaultNamespace);
  boost::scoped_ptr<Xmlns> xmlns(Xmlns::Create(attributes));
  std::vector<string> log;
  TestHandler test_handler(&log);
  ExpatHandlerNs expat_handler_ns(&test_handler, xmlns.get());
  const string kXml(
      string("<hi xmlns=\"") + kDefaultNamespace + "\"/>");
  string errors;
  ASSERT_TRUE(ExpatParser::ParseString(kXml, &expat_handler_ns, &errors, true));
  ASSERT_EQ(static_cast<size_t>(2), log.size());
  ASSERT_EQ(string("hi"), log[0]);
  ASSERT_EQ(string("hi"), log[1]);
}

// This verifies ExpatHandlerNs's StartElement() and EndElement() on XML with
// no default namespace: all elements are prefixed to a namespace mapping
// known to the TestHandler.
TEST_F(ExpatHandlerNsTest, TestExpatParserPrefixed) {
  Attributes attributes;
  attributes.SetString(
      string("xmlns:") + kDefaultPrefix, kDefaultNamespace);
  boost::scoped_ptr<Xmlns> xmlns(Xmlns::Create(attributes));
  std::vector<string> log;
  TestHandler test_handler(&log);
  ExpatHandlerNs expat_handler_ns(&test_handler, xmlns.get());
  const string kTag("hi");
  const string kXml(
      string("<") + kDefaultPrefix + ":" + kTag + " " +
                  "xmlns:" + kDefaultPrefix + "=\"" + kDefaultNamespace +
                   "\"" + "/>");
  string errors;
  ASSERT_TRUE(ExpatParser::ParseString(kXml, &expat_handler_ns, &errors, true));
  ASSERT_EQ(static_cast<size_t>(2), log.size());
  ASSERT_EQ(string(kDefaultPrefix) + ":" + kTag, log[0]);
  ASSERT_EQ(string(kDefaultPrefix) + ":" + kTag, log[1]);
}

// This verifies ExpatHandlerNs's StartElement() and EndElement() on XML with
// elements in the default namespace and elements in another namespace.
TEST_F(ExpatHandlerNsTest, TestExpatParserDefaultAndPrefixed) {
  Attributes attributes;
  attributes.SetString("xmlns", kDefaultNamespace);
  attributes.SetString(
      string("xmlns:") + kOtherPrefix, kOtherNamespace);
  boost::scoped_ptr<Xmlns> xmlns(Xmlns::Create(attributes));
  std::vector<string> log;
  TestHandler test_handler(&log);
  ExpatHandlerNs expat_handler_ns(&test_handler, xmlns.get());
  const string kHi("hi");
  const string kThere("there");
  const string kXml(
      string("<") + kHi + " " +
                  "xmlns=\"" + kDefaultNamespace + "\" " +
                  "xmlns:" + kOtherPrefix + "=\"" + kOtherNamespace + "\">" +
                  "<" + kOtherPrefix + ":" + kThere + "/>" +
                  "</" + kHi + ">");
  string errors;
  ASSERT_TRUE(ExpatParser::ParseString(kXml, &expat_handler_ns, &errors, true));
  ASSERT_EQ(static_cast<size_t>(4), log.size());
  ASSERT_EQ(kHi, log[0]);
  ASSERT_EQ(string(kOtherPrefix) + ":" + kThere, log[1]);
  ASSERT_EQ(string(kOtherPrefix) + ":" + kThere, log[2]);
  ASSERT_EQ(kHi, log[3]);
}

}  // end namespace kmlbase

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
