// Copyright 2010, Google Inc. All rights reserved.
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

// This file contains the unit tests for the CsvSplitter class.

#include "kml/base/csv_splitter.h"
#include "kml/base/file.h"
#include "gtest/gtest.h"

// The following define is a convenience for testing inside Google.
#ifdef GOOGLE_INTERNAL
#include "kml/base/google_internal_test.h"
#endif

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

namespace kmlbase {

TEST(CsvSplitterTest, TestSimpleFindNextLine) {
  CsvSplitter csv_splitter("hi,there\nhow,are,you?\n");
  size_t this_end;
  size_t next_begin = csv_splitter.FindNextLine(&this_end);
  ASSERT_EQ(static_cast<size_t>(8), this_end);
  ASSERT_EQ(static_cast<size_t>(9), next_begin);
}

TEST(CsvSplitterTest, TestSimpleSplitCurrentLine) {
  CsvSplitter csv_splitter("hi,there\nhow,are,you?\n");
  StringVector cols;
  ASSERT_TRUE(csv_splitter.SplitCurrentLine(&cols));
  ASSERT_EQ(static_cast<size_t>(2), cols.size());
  ASSERT_EQ("hi", cols[0]);
  ASSERT_EQ("there", cols[1]);
  cols.clear();
  ASSERT_TRUE(csv_splitter.SplitCurrentLine(&cols));
  ASSERT_EQ(static_cast<size_t>(3), cols.size());
  ASSERT_EQ("how", cols[0]);
  ASSERT_EQ("are", cols[1]);
  ASSERT_EQ("you?", cols[2]);
  cols.clear();
  ASSERT_FALSE(csv_splitter.SplitCurrentLine(&cols));
  ASSERT_TRUE(cols.empty());
}

TEST(CsvSplitterTest, TestFindNextLineNoLastNewline) {
  CsvSplitter csv_splitter("hi,there\nhow,are,you?");
  size_t this_end;
  size_t next_begin = csv_splitter.FindNextLine(&this_end);
  ASSERT_EQ(static_cast<size_t>(8), this_end);
  ASSERT_EQ(static_cast<size_t>(9), next_begin);
  StringVector cols;
  ASSERT_TRUE(csv_splitter.SplitCurrentLine(&cols));
  ASSERT_EQ(static_cast<size_t>(2), cols.size());
}

TEST(CsvSplitterTest, TestSplitCurrentLineNoLastNewLine) {
  CsvSplitter csv_splitter("hi,there\nhow,are,you?");
  StringVector cols;
  ASSERT_TRUE(csv_splitter.SplitCurrentLine(&cols));
  ASSERT_EQ(static_cast<size_t>(2), cols.size());
  cols.clear();
  ASSERT_TRUE(csv_splitter.SplitCurrentLine(&cols));
  ASSERT_EQ(static_cast<size_t>(3), cols.size());
}

// Test line FindNextLine on a file with \r\n line endings.
TEST(CsvSplitterTest, TestCrNlFile) {
  const string kCrNlCsv =
      kmlbase::File::JoinPaths(DATADIR,
                               kmlbase::File::JoinPaths("csv", "crnl.csv"));
  string cr_nl_csv;
  ASSERT_TRUE(File::ReadFileToString(kCrNlCsv.c_str(), &cr_nl_csv));
  CsvSplitter csv_splitter(cr_nl_csv);
  size_t this_end;
  size_t next_begin = csv_splitter.FindNextLine(&this_end);
  ASSERT_EQ(static_cast<size_t>(5), this_end);
  ASSERT_EQ(static_cast<size_t>(7), next_begin);
  StringVector cols;
  ASSERT_TRUE(csv_splitter.SplitCurrentLine(&cols));
  ASSERT_EQ(static_cast<size_t>(3), cols.size());
  ASSERT_EQ(string("a"), cols[0]);
  ASSERT_EQ(string("b"), cols[1]);
  ASSERT_EQ(string("c"), cols[2]);
  cols.clear();
  ASSERT_TRUE(csv_splitter.SplitCurrentLine(&cols));
  ASSERT_EQ(static_cast<size_t>(3), cols.size());
  ASSERT_EQ(string("1"), cols[0]);
  ASSERT_EQ(string("2"), cols[1]);
  ASSERT_EQ(string("3"), cols[2]);
  ASSERT_FALSE(csv_splitter.SplitCurrentLine(&cols));
}

// Test line FindNextLine on a file with \r line endings.
TEST(CsvSplitterTest, TestCrFile) {
  const string kColleges =
      kmlbase::File::JoinPaths(DATADIR,
                               kmlbase::File::JoinPaths("csv", "colleges.csv"));
  string colleges;
  ASSERT_TRUE(File::ReadFileToString(kColleges.c_str(), &colleges));
  CsvSplitter csv_splitter(colleges);
  int count = 0;
  StringVector cols;
  ASSERT_TRUE(csv_splitter.SplitCurrentLine(&cols));
  ASSERT_EQ(string("name"), cols[0]);
  ASSERT_EQ(string("address"), cols[1]);
  ASSERT_EQ(string("Website"), cols[2]);
  ASSERT_EQ(string("Type"), cols[3]);
  ASSERT_EQ(string("Size"), cols[4]);
  cols.clear();
  while (csv_splitter.SplitCurrentLine(&cols)) {
    ASSERT_EQ(static_cast<size_t>(5), cols.size());
    if (++count != 2966) {
      cols.clear();
    }
  }
  ASSERT_EQ(string("Zion Bible College"), cols[0]);
  ASSERT_EQ(string("320 South Main Street, Haverhill, Massachusetts 01835"),
            cols[1]);
  ASSERT_EQ(string("www.zbc.edu"), cols[2]);
  ASSERT_EQ(string("4-year, Private not-for-profit"), cols[3]);
  ASSERT_EQ(string("247"), cols[4]);
  ASSERT_EQ(2966, count);
}

}  // end namespace kmlbase

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
