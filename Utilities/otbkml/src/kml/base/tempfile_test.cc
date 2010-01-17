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

#include "kml/base/tempfile.h"
#include "kml/base/file.h"
#include "kml/base/unit_test.h"

namespace kmlbase {

class TempFileTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(TempFileTest);
  CPPUNIT_TEST(TestTempFile);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestTempFile();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TempFileTest);

void TempFileTest::TestTempFile() {
  std::string tempfile_name;  // To check successful deletion of tempfile.
  {
    TempFilePtr tempfile = TempFile::CreateTempFile();
    // The tempfile was created successfully.
    CPPUNIT_ASSERT(tempfile != 0);
    // The tempfile has a name.
    CPPUNIT_ASSERT(!tempfile->name().empty());
    tempfile_name = tempfile->name();
    // The tempfile is accessible.
    CPPUNIT_ASSERT(File::Exists(tempfile_name));
    // We can write and read data.
    const std::string s_written("some data");
    File::WriteStringToFile(s_written, tempfile_name);
    std::string s_read;
    File::ReadFileToString(tempfile_name, &s_read);
    CPPUNIT_ASSERT_EQUAL(s_written, s_read);
  }
  // Use of intrusive_ptr means TempFile's dtor is called at the end of the
  // block, which deletes the tempfile.
  CPPUNIT_ASSERT(!File::Exists(tempfile_name));
}

}  // end namespace kmlbase

TEST_MAIN
