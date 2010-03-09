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

#include "kml/base/file.h"
#include "gtest/gtest.h"

// The following define is a convenience for testing inside Google.
#ifdef GOOGLE_INTERNAL
#include "kml/base/google_internal_test.h"
#endif

#ifndef DATADIR
#error DATADIR must be defined!
#endif

namespace kmlbase {

class FileTest : public testing::Test {
};

TEST_F(FileTest, TestReadFileToString) {
  const string kDoc = string(DATADIR) + "/kmz/doc.kmz";
  string file_data;
  ASSERT_TRUE(File::ReadFileToString(kDoc, &file_data));
  ASSERT_FALSE(file_data.empty());
  // doc.kmz is 332 bytes.
  ASSERT_EQ(static_cast<size_t>(332), file_data.size());
}

TEST_F(FileTest, TestWriteStringToFile) {
  // Create a temp file into which we'll write data.
  string tempfile;
  ASSERT_TRUE(File::CreateNewTempFile(&tempfile));
  ASSERT_FALSE(tempfile.empty());
  // Write some data.
  const string kText = "tom dick harry";
  ASSERT_TRUE(File::WriteStringToFile(kText, tempfile));
  // Read the file back into a different string and assert equality.
  string file_data;
  ASSERT_TRUE(File::ReadFileToString(tempfile, &file_data));
  ASSERT_FALSE(file_data.empty());
  ASSERT_EQ(kText, file_data);
  // Remove the temp file.
  ASSERT_TRUE(File::Delete(tempfile));
}

TEST_F(FileTest, TestExists) {
  const string kDoc = string(DATADIR) + "/kmz/doc.kmz";
  const string kNoSuchFile = string(DATADIR) + "/kmz/nosuchfile";
  ASSERT_TRUE(File::Exists(kDoc));
  ASSERT_FALSE(File::Exists(kNoSuchFile));
}

TEST_F(FileTest, TestDelete) {
  // Create a temp file.
  string tempfile;
  ASSERT_TRUE(File::CreateNewTempFile(&tempfile));
  // Assert it was created.
  ASSERT_FALSE(tempfile.empty());
  ASSERT_TRUE(File::Exists(tempfile));
  // Delete and confirm removal.
  ASSERT_TRUE(File::Delete(tempfile));
  ASSERT_FALSE(File::Exists(tempfile));
}

TEST_F(FileTest, TestCreateNewTempFile) {
  ASSERT_TRUE(false == File::CreateNewTempFile(NULL));
  string temp_filename;
  ASSERT_TRUE(File::CreateNewTempFile(&temp_filename));
  ASSERT_FALSE(temp_filename.empty());
  ASSERT_TRUE(File::Delete(temp_filename));
}

TEST_F(FileTest, TestJoinPaths) {
  // TODO: win32 separators for cross-platform testing.
  const string kPath1NoSep("/tom/dick");
  const string kPath1Sep("/tom/dick/");
  const string kPath2("harry");
  const string kExpected_posix("/tom/dick/harry");
  // The following two strings have no real meaning w.r.t. a windows file
  // system. They merely test correct handling of the underlying code.
  const string kExpected_win32_nosep("/tom/dick\\harry");
  const string kExpected_win32_sep("/tom/dick/\\harry");
  // Passing cases.
#ifdef WIN32
  ASSERT_EQ(kExpected_win32_nosep, File::JoinPaths(kPath1NoSep, kPath2));
  ASSERT_EQ(kExpected_win32_sep, File::JoinPaths(kPath1Sep, kPath2));
#else
  ASSERT_EQ(kExpected_posix, File::JoinPaths(kPath1NoSep, kPath2));
  ASSERT_EQ(kExpected_posix, File::JoinPaths(kPath1Sep, kPath2));

  const string kBaseDir("../../../../testdata/kmz/");
  const string kFilename("kmzfiles/dummy.kml");
  const string kExpected("../../../../testdata/kmz/kmzfiles/dummy.kml");
  ASSERT_EQ(kExpected, File::JoinPaths(kBaseDir, kFilename));

#endif
  // Pathological cases.
  // Joining with an empty string does not modify anything.
  ASSERT_EQ(kPath1NoSep, File::JoinPaths(kPath1NoSep, ""));
  ASSERT_EQ(kPath2, File::JoinPaths("", kPath2));
  ASSERT_EQ(string(""), File::JoinPaths("", ""));
}

const static struct {
  string filepath;
  string basedir;
  string filename;
} kFilePathTests[] = {
  {"/tom/dick/harry.txt", "/tom/dick", "harry.txt" },
  {"\\tom\\dick\\harry.txt", "\\tom\\dick", "harry.txt"},
  {"/harry.txt", "/", "harry.txt" },
  {"harry.txt", ".", "harry.txt" },
  {"", ".", "" }
};

TEST_F(FileTest, TestSplitFilePath) {
  // Test NULL handling.
  File::SplitFilePath(kFilePathTests[0].filepath, NULL, NULL);
  string basedir;
  File::SplitFilePath(kFilePathTests[0].filepath, &basedir, NULL);
  ASSERT_EQ(kFilePathTests[0].basedir, basedir);
  string filename;
  File::SplitFilePath(kFilePathTests[0].filepath, &basedir, &filename);
  ASSERT_EQ(kFilePathTests[0].basedir, basedir);
  ASSERT_EQ(kFilePathTests[0].filename, filename);

  // Test specific cases.
  const size_t kSize = sizeof(kFilePathTests)/sizeof(kFilePathTests[0]);
  for (size_t i = 0; i < kSize; ++i) {
    string basedir;
    string filename;
    File::SplitFilePath(kFilePathTests[i].filepath, &basedir, &filename);
    ASSERT_EQ(kFilePathTests[i].basedir, basedir);
    ASSERT_EQ(kFilePathTests[i].filename, filename);
  }
}

}  // end namespace kmlbase

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
