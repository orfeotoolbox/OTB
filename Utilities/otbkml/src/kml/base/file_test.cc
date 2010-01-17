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
#include "kml/base/unit_test.h"

#ifndef DATADIR
#error DATADIR must be defined!
#else
static const std::string kDataDir = DATADIR;
#endif

namespace kmlbase {

class FileTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(FileTest);
  CPPUNIT_TEST(TestReadFileToString);
  CPPUNIT_TEST(TestWriteStringToFile);
  CPPUNIT_TEST(TestExists);
  CPPUNIT_TEST(TestDelete);
  CPPUNIT_TEST(TestCreateNewTempFile);
  CPPUNIT_TEST(TestJoinPaths);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestReadFileToString();
  void TestWriteStringToFile();
  void TestExists();
  void TestDelete();
  void TestCreateNewTempFile();
  void TestJoinPaths();
};

CPPUNIT_TEST_SUITE_REGISTRATION(FileTest);

void FileTest::TestReadFileToString() {
  const std::string kDoc = kDataDir + "/kmz/doc.kmz";
  std::string file_data;
  CPPUNIT_ASSERT(File::ReadFileToString(kDoc, &file_data));
  CPPUNIT_ASSERT(!file_data.empty());
  // doc.kmz is 180 bytes.
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(180), file_data.size());
}

void FileTest::TestWriteStringToFile() {
  // Create a temp file into which we'll write data.
  std::string tempfile;
  CPPUNIT_ASSERT(File::CreateNewTempFile(&tempfile));
  CPPUNIT_ASSERT(!tempfile.empty());
  // Write some data.
  const std::string kText = "tom dick harry";
  CPPUNIT_ASSERT(File::WriteStringToFile(kText, tempfile));
  // Read the file back into a different string and assert equality.
  std::string file_data;
  CPPUNIT_ASSERT(File::ReadFileToString(tempfile, &file_data));
  CPPUNIT_ASSERT(!file_data.empty());
  CPPUNIT_ASSERT_EQUAL(kText, file_data);
  // Remove the temp file.
  CPPUNIT_ASSERT(File::Delete(tempfile));
}

void FileTest::TestExists() {
  const std::string kDoc = kDataDir + "/kmz/doc.kmz";
  const std::string kNoSuchFile = kDataDir + "/kmz/nosuchfile";
  CPPUNIT_ASSERT(File::Exists(kDoc));
  CPPUNIT_ASSERT(!File::Exists(kNoSuchFile));
}

void FileTest::TestDelete() {
  // Create a temp file.
  std::string tempfile;
  CPPUNIT_ASSERT(File::CreateNewTempFile(&tempfile));
  // Assert it was created.
  CPPUNIT_ASSERT(!tempfile.empty());
  CPPUNIT_ASSERT(File::Exists(tempfile));
  // Delete and confirm removal.
  CPPUNIT_ASSERT(File::Delete(tempfile));
  CPPUNIT_ASSERT(!File::Exists(tempfile));
}

void FileTest::TestCreateNewTempFile() {
  CPPUNIT_ASSERT(false == File::CreateNewTempFile(NULL));
  std::string temp_filename;
  CPPUNIT_ASSERT(File::CreateNewTempFile(&temp_filename));
  CPPUNIT_ASSERT(!temp_filename.empty());
  CPPUNIT_ASSERT(File::Delete(temp_filename));
}

void FileTest::TestJoinPaths() {
  // TODO: win32 separators for cross-platform testing.
  const std::string kPath1NoSep("/tom/dick");
  const std::string kPath1Sep("/tom/dick/");
  const std::string kPath2("harry");
  const std::string kExpected("/tom/dick/harry");
  // Passing cases.
  CPPUNIT_ASSERT_EQUAL(kExpected, File::JoinPaths(kPath1NoSep, kPath2));
  CPPUNIT_ASSERT_EQUAL(kExpected, File::JoinPaths(kPath1Sep, kPath2));
  // Pathological cases.
  // Joining with an empty string does not modify anything.
  CPPUNIT_ASSERT_EQUAL(kPath1NoSep, File::JoinPaths(kPath1NoSep, ""));
  CPPUNIT_ASSERT_EQUAL(kPath2, File::JoinPaths("", kPath2));
  CPPUNIT_ASSERT_EQUAL(std::string(""), File::JoinPaths("", ""));
}

}  // end namespace kmlbase

TEST_MAIN
