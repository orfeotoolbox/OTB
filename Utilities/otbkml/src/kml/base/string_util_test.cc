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

#include "kml/base/string_util.h"
#include "kml/base/unit_test.h"

namespace kmlbase {

class StringUtilTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(StringUtilTest);
  CPPUNIT_TEST(TestBasicReplacements);
  CPPUNIT_TEST(TestSplitStringUsing);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestBasicReplacements();
  void TestSplitStringUsing();
};

CPPUNIT_TEST_SUITE_REGISTRATION(StringUtilTest);

void StringUtilTest::TestBasicReplacements() {
  StringMap sm;
  sm["old"] = "new";
  sm["one"] = "two";
  std::string in("old{old}old one{one}one");

  // Verify that supplying neither start nor end terminators produces a simple
  // key-value replacement.
  std::string expected("new{new}new two{two}two");
  CPPUNIT_ASSERT_EQUAL(expected, CreateExpandedStrings(in, sm, "", ""));

  // Verify that specifying either of start or end produces the expected
  // replacement.
  const std::string kStart("{");
  const std::string kEnd("}");
  expected = "oldnew}old onetwo}one";
  CPPUNIT_ASSERT_EQUAL(expected, CreateExpandedStrings(in, sm, kStart, ""));
  expected = "old{newold one{twoone";
  CPPUNIT_ASSERT_EQUAL(expected, CreateExpandedStrings(in, sm, "", kEnd));
  expected = "oldnewold onetwoone";
  CPPUNIT_ASSERT_EQUAL(expected, CreateExpandedStrings(in, sm, kStart, kEnd));

  // Verify that the replacement logic is well-behaved in the presence of
  // multiple start/end delimiters.
  in = "{{{old}}}}{}{one}";
  expected = "{{new}}}{}two";
  CPPUNIT_ASSERT_EQUAL(expected, CreateExpandedStrings(in, sm, kStart, kEnd));
}

void StringUtilTest::TestSplitStringUsing() {
  const std::string kHi("hi");
  const std::string kHow("how");
  const std::string kAre("are");
  const std::string kYou("you");
  const std::string kSep("|");
  const std::string kCsvLine(kHi + kSep + kHow + kSep + kAre + kSep + kYou);
  std::vector<std::string> parts;

  // A line with no separator gets one thing.  Special case of the "last" item
  // on a line.
  SplitStringUsing(kHi, "|", &parts);
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), parts.size());

  // A line with 3 separators gets you 4 things.
  parts.clear();
  SplitStringUsing(kCsvLine, "|", &parts);
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(4), parts.size());
  CPPUNIT_ASSERT_EQUAL(kHi, parts[0]);
  CPPUNIT_ASSERT_EQUAL(kHow, parts[1]);
  CPPUNIT_ASSERT_EQUAL(kAre, parts[2]);
  CPPUNIT_ASSERT_EQUAL(kYou, parts[3]);
}

}  // end namespace kmlbase

TEST_MAIN
