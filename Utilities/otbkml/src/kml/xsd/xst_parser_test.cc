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

// This file contains the unit tests for the XstParser class.

#include "kml/xsd/xst_parser.h"
#include <vector>
#include "boost/scoped_ptr.hpp"
#include "gtest/gtest.h"
#include "kml/xsd/xsd_file.h"

namespace kmlxsd {

// This class is the unit test fixture for the XsdHandler class.
class XstParserTest : public testing::Test {
 protected:
  virtual void SetUp() {
    xsd_file_.reset(new XsdFile);
    xst_parser_.reset(new XstParser(xsd_file_.get()));
  }

  boost::scoped_ptr<XsdFile> xsd_file_;
  boost::scoped_ptr<XstParser> xst_parser_;
};

// Verify the ParseXstAlias() method.
TEST_F(XstParserTest, TestParseXstAlias) {
  const string kRealName("real-name");
  const string kAlias("better-name");
  std::vector<string> alias;
  alias.push_back("alias");
  alias.push_back(kRealName);
  alias.push_back(kAlias);

  // Verify the XsdFile does not have this alias.
  ASSERT_EQ(string(""), xsd_file_->get_alias(kRealName));

  // Call the method under test.
  xst_parser_->ParseXstAlias(alias);

  // Read back the result in the XsdFile.
  ASSERT_EQ(kAlias, xsd_file_->get_alias(kRealName));
}

// Verify the ParseXst() method.
TEST_F(XstParserTest, TestParseXst) {
  const string kRealName("real-name");
  const string kAlias("better-name");
  const string kXstData(
      string("alias") + " " + kRealName + " " + kAlias);

  // Verify the XsdFile does not have this alias.
  ASSERT_EQ(string(""), xsd_file_->get_alias(kRealName));

  // Call the method under test.
  xst_parser_->ParseXst(kXstData);

  // Read back the result in the XsdFile.
  ASSERT_EQ(kAlias, xsd_file_->get_alias(kRealName));
}

}  // end namespace kmlxsd

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
