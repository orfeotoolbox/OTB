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

// This file contains the unit tests for the KmzFile class.

#include "kml/engine/kmz_file.h"
#include <vector>
#include "boost/scoped_ptr.hpp"
#include "kml/base/file.h"
#include "kml/base/tempfile.h"
#include "kml/engine/get_links.h"
#include "gtest/gtest.h"

// The following define is a convenience for testing inside Google.
#ifdef GOOGLE_INTERNAL
#include "kml/base/google_internal_test.h"
#endif

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

using kmlbase::File;

namespace kmlengine {

class KmzTest : public testing::Test {
 protected:
  boost::scoped_ptr<KmzFile> kmz_file_;
};

TEST_F(KmzTest, TestOpenFromFile) {
  // doc.kmz contains a simple doc.kml and is a valid zip archive.
  const string kGoodKmz = string(DATADIR) + "/kmz/doc.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kGoodKmz.c_str()));
  ASSERT_TRUE(kmz_file_);
  string kml_data;
  // doc.kml can be read.
  ASSERT_TRUE(kmz_file_->ReadKml(&kml_data));
  ASSERT_FALSE(kml_data.empty());
  // nokml.kmz is a valid zip archive, but does not contain any KML files
  const string kBadKmz = string(DATADIR) + "/kmz/nokml.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kBadKmz.c_str()));
  ASSERT_TRUE(kmz_file_);
  kml_data.clear();
  // There is no KML file to read.
  ASSERT_FALSE(kmz_file_->ReadKml(&kml_data));
  ASSERT_TRUE(kml_data.empty());
}

TEST_F(KmzTest, TestOpenFromBadFile) {
  // Two kinds of bad file.
  // 1: a non-existant file:
  const string kNoSuchFile("nosuchfile.kmz");
  kmz_file_.reset(KmzFile::OpenFromFile(kNoSuchFile.c_str()));
  // The file cannot be opened.
  ASSERT_TRUE(kmz_file_ == NULL);
  // 2: a file that is not a valid KMZ archive.
  const string kBadKmz= string(DATADIR) + "/kmz/bad.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kBadKmz.c_str()));
  // The file could not be read.
  ASSERT_TRUE(kmz_file_ == NULL);
}

TEST_F(KmzTest, TestOpenFromString) {
  // doc.kmz contains a simple doc.kml and is a valid zip archive.
  const string kGoodKmz = string(DATADIR) + "/kmz/doc.kmz";
  string kmz_file_data;
  ASSERT_TRUE(File::ReadFileToString(kGoodKmz.c_str(), &kmz_file_data));
  ASSERT_FALSE(kmz_file_data.empty());
  kmz_file_.reset(KmzFile::OpenFromString(kmz_file_data));
  ASSERT_TRUE(kmz_file_);
  string kml_data;
  // doc.kml can be read.
  ASSERT_TRUE(kmz_file_->ReadKml(&kml_data));
  ASSERT_FALSE(kml_data.empty());
  // nokml.kmz is a valid zip archive, but does not contain any KML files
  const string kBadKmz = string(DATADIR) + "/kmz/nokml.kmz";
  kmz_file_data.clear();
  ASSERT_TRUE(File::ReadFileToString(kBadKmz.c_str(), &kmz_file_data));
  ASSERT_FALSE(kmz_file_data.empty());
  kmz_file_.reset(KmzFile::OpenFromString(kmz_file_data));
  ASSERT_TRUE(kmz_file_);
  kml_data.clear();
  // There is no KML file to read.
  ASSERT_FALSE(kmz_file_->ReadKml(&kml_data));
  ASSERT_TRUE(kml_data.empty());
}

TEST_F(KmzTest, TestReadKml) {
  // doc.kmz has two KML files at the root level, a.kml and doc.kml, which were
  // added to the archive in that order. Assert that a.kml is read instead
  // of doc.kml.
  const string kDoc = string(DATADIR) + "/kmz/doc.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kDoc.c_str()));
  ASSERT_TRUE(kmz_file_);
  string kml_data;
  ASSERT_TRUE(kmz_file_->ReadKml(&kml_data));
  ASSERT_FALSE(kml_data.empty());
  ASSERT_TRUE(string::npos != kml_data.find("a.kml"));
  // nokml.kmz is a valid zip archive, but does not contain any KML files
  const string kNokml = string(DATADIR) + "/kmz/nokml.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kNokml.c_str()));
  ASSERT_TRUE(kmz_file_);
  kml_data.clear();
  ASSERT_FALSE(kmz_file_->ReadKml(&kml_data));
  ASSERT_TRUE(kml_data.empty());
  // multikml-nodoc.kmz has three kml files added in the following order:
  // - z/c.kml
  // - b.kml
  // - a/a.kml
  // Each file has a placemark whose <name> is the archived filename.
  // Assert that z/c.kml is read first.
  const string kMulti1 = string(DATADIR) + "/kmz/multikml-nodoc.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kMulti1.c_str()));
  ASSERT_TRUE(kmz_file_);
  kml_data.clear();
  ASSERT_TRUE(kmz_file_->ReadKml(&kml_data));
  ASSERT_FALSE(kml_data.empty());
  ASSERT_TRUE(string::npos != kml_data.find("c.kml"));
  // Assert we handle a NULL output string.
  ASSERT_FALSE(kmz_file_->ReadKml(NULL));
}

// Verify the AndGetPath() part of ReadKmlAndGetPath().  Basic operation
// of ReadKml() is verified in TestReadKml() above.
TEST_F(KmzTest, TestReadKmlAndGetPath) {
  // doc.kmz has two KML files at the root level, a.kml and doc.kml, which were
  // added to the archive in that order. Assert that a.kml is read instead
  // of doc.kml.
  const string kDoc = string(DATADIR) + "/kmz/doc.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kDoc.c_str()));
  string kml_data;
  string kml_path;
  ASSERT_TRUE(kmz_file_->ReadKmlAndGetPath(&kml_data, &kml_path));
  ASSERT_EQ(string("a.kml"), kml_path);
  // Verify that a NULL output path arg is well behaved.
  ASSERT_TRUE(kmz_file_->ReadKmlAndGetPath(&kml_data, NULL));
  // multikml-doc.kmz has four kml files added in the following order:
  // - z/c.kml
  // - b.kml
  // - a/a.kml
  // - doc/doc.kml
  // Assert that z/c.kml is read because it is the first file in the TOC.
  const string kMulti2 = string(DATADIR) + "/kmz/multikml-doc.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kMulti2.c_str()));
  ASSERT_TRUE(kmz_file_->ReadKmlAndGetPath(&kml_data, &kml_path));
  ASSERT_EQ(string("z/c.kml"), kml_path);
  // nokml.kmz is a valid zip archive, but does not contain any KML files
  const string kBadKmz = string(DATADIR) + "/kmz/nokml.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kBadKmz.c_str()));
  kml_path.clear();
  ASSERT_FALSE(kmz_file_->ReadKmlAndGetPath(&kml_data, &kml_path));
  ASSERT_TRUE(kml_path.empty());
  // Verify that a NULL output path arg is well behaved in this circumstance.
  ASSERT_FALSE(kmz_file_->ReadKmlAndGetPath(&kml_data, NULL));
}

TEST_F(KmzTest, TestReadFile) {
  // nokml.kmz has a file called foo.txt in a folder called foo.
  const string kNokml = string(DATADIR) + "/kmz/nokml.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kNokml.c_str()));
  ASSERT_TRUE(kmz_file_);
  string file_data;
  ASSERT_TRUE(kmz_file_->ReadFile("foo/foo.txt", &file_data));
  ASSERT_FALSE(file_data.empty());
  string tmp = file_data;
  // But does not have a file called bar.txt in that folder
  ASSERT_FALSE(kmz_file_->ReadFile("foo/bar.txt", &file_data));
  // The original data was untouched by this failure.
  ASSERT_FALSE(file_data.empty());
  ASSERT_EQ(tmp, file_data);
  // Assert we handle a NULL output string.
  ASSERT_FALSE(kmz_file_->ReadFile("bar", NULL));
}

TEST_F(KmzTest, TestIsKmz) {
  // Verify that a valid KMZ archive passes IsKmz().
  const string kGoodKmz= string(DATADIR) + "/kmz/doc.kmz";
  string kmz_data;
  File::ReadFileToString(kGoodKmz, &kmz_data);
  ASSERT_FALSE(kmz_data.empty());
  ASSERT_TRUE(KmzFile::IsKmz(kmz_data));

  // Verify that an invalid KMZ archive fails IsKmz().
  const string kBadKmz= string(DATADIR) + "/kmz/bad.kmz";
  kmz_data.clear();
  File::ReadFileToString(kBadKmz, &kmz_data);
  ASSERT_FALSE(kmz_data.empty());
  ASSERT_FALSE(KmzFile::IsKmz(kmz_data));
}

TEST_F(KmzTest, TestList) {
  // multikml-nodoc.kmz has three kml files added in the following order:
  // - z/c.kml
  // - b.kml
  // - a/a.kml
  const string kMulti1 = string(DATADIR) + "/kmz/multikml-nodoc.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kMulti1.c_str()));
  ASSERT_TRUE(kmz_file_);
  std::vector<string> list;
  kmz_file_->List(&list);
  // 3 files were read into the vector.
  ASSERT_TRUE(3 == list.size());
  // They appear in the same order in which they were added.
  ASSERT_EQ(string("z/c.kml"), list[0]);
  ASSERT_EQ(string("b.kml"), list[1]);
  ASSERT_EQ(string("a/a.kml"), list[2]);
}

TEST_F(KmzTest, TestWriteKmz) {
  // Create a temp file into which we'll write our KMZ data.
  kmlbase::TempFilePtr tempfile = kmlbase::TempFile::CreateTempFile();
  ASSERT_TRUE(tempfile != NULL);
  // Create a KMZ file containing a KML file that is a simple placemark called
  // 'tmp kml'.
  const string kKml("<Placemark><name>tmp kml</name></Placemark>");
  ASSERT_TRUE(KmzFile::WriteKmz(tempfile->name().c_str(), kKml));
  // Now read the file, ensuring it was properly written.
  ASSERT_TRUE(File::Exists(tempfile->name()));
  string foo;
  ASSERT_TRUE(File::ReadFileToString(tempfile->name(), &foo));
  ASSERT_FALSE(foo.empty());
  // Open the file into our KmzFile instance and assert the KML was written
  // correctly.
  ASSERT_TRUE(File::Exists(tempfile->name().c_str()));
  kmz_file_.reset(KmzFile::OpenFromFile(tempfile->name().c_str()));
  ASSERT_TRUE(kmz_file_);
  string kml_data;
  ASSERT_TRUE(kmz_file_->ReadKml(&kml_data));
  ASSERT_FALSE(kml_data.empty());
  ASSERT_EQ(kKml, kml_data);
}

// This verifies that a const KmzFile compiles and runs with read methods.
TEST_F(KmzTest, TestConstKmzFile) {
  const string kDoc = string(DATADIR) + "/kmz/doc.kmz";
  const KmzFile* kmz_file = KmzFile::OpenFromFile(kDoc.c_str());
  string kml_data;
  ASSERT_TRUE(kmz_file->ReadKml(&kml_data));
  ASSERT_FALSE(kml_data.empty());
  delete kmz_file;
}

TEST_F(KmzTest, TestCreate) {
  // Verify that a file cannot be created from an unwritable path.
  boost::scoped_ptr<KmzFile> kmz(KmzFile::Create("/nosuchpath/here.kmz"));
  ASSERT_TRUE(NULL == kmz);
  // A file is created against a writable path.
  kmlbase::TempFilePtr tempfile = kmlbase::TempFile::CreateTempFile();
  ASSERT_TRUE(tempfile != NULL);
  kmz.reset(KmzFile::Create(tempfile->name().c_str()));
  ASSERT_TRUE(kmz);
}

TEST_F(KmzTest, TestAddFile) {
  kmlbase::TempFilePtr tempfile = kmlbase::TempFile::CreateTempFile();
  ASSERT_TRUE(tempfile != NULL);
  {
    // Create an empty KmzFile.
    KmzFilePtr kmz = KmzFile::Create(tempfile->name().c_str());
    ASSERT_TRUE(kmz);
    // Add three files to the archive.
    const string kNewKml = "<Placemark><name/></Placemark>";
    ASSERT_TRUE(kmz->AddFile(kNewKml, "doc.kml"));
    ASSERT_TRUE(kmz->AddFile(kNewKml, "files/new.kml"));
    ASSERT_TRUE(kmz->AddFile(kNewKml, "other/blah.kml"));
    // Fails because it points above the archive.
    ASSERT_FALSE(kmz->AddFile(kNewKml, "../invalid.kml"));
    // Fails because the path is absolute.
    ASSERT_FALSE(kmz->AddFile(kNewKml, "/also/invalid.kml"));
  }
  // KmzFile's destructor closes the file handle and cleans up.
  ASSERT_TRUE(File::Exists(tempfile->name().c_str()));

  // Verify that the archive we created contains the files in order.
  KmzFilePtr created(KmzFile::OpenFromFile(tempfile->name().c_str()));
  ASSERT_TRUE(created);
  std::vector<string> list;
  created->List(&list);
  ASSERT_EQ(static_cast<size_t>(3), list.size());
  ASSERT_EQ(string("doc.kml"), list[0]);
  ASSERT_EQ(string("files/new.kml"), list[1]);
  ASSERT_EQ(string("other/blah.kml"), list[2]);
}

TEST_F(KmzTest, TestAddFileList) {
  kmlbase::TempFilePtr tempfile = kmlbase::TempFile::CreateTempFile();
  size_t errs = 0;
  ASSERT_TRUE(tempfile != NULL);
  {
    // Create an empty KmzFile.
    KmzFilePtr kmz_file = KmzFile::Create(tempfile->name().c_str());
    ASSERT_TRUE(kmz_file);

    // Create a KmlFile from the testdata file.
    const string kBaseDir = File::JoinPaths(string(DATADIR), "kmz");
    const string kTestKml = File::JoinPaths(kBaseDir, "doc.kml");
    string kml_data;
    ASSERT_TRUE(File::ReadFileToString(kTestKml, &kml_data));

    // Gather the local resources used in the KML.
    kmlbase::StringVector file_paths;
    ASSERT_TRUE(GetRelativeLinks(kml_data, &file_paths));

    // We know there are four resources in the file.
    ASSERT_EQ(static_cast<size_t>(5), file_paths.size());

    // Add its resources.
    errs = kmz_file->AddFileList(kBaseDir, file_paths);
  }
  // KmzFile's destructor closes the file handle and cleans up.
  ASSERT_TRUE(File::Exists(tempfile->name().c_str()));

  // Although doc.kml has 5 href fields, two of them are outright
  // duplicates, another resolves to the same path as the dupes, and one is
  // invalid. Verify that only two resources were added by AddFileList.
  ASSERT_EQ(static_cast<size_t>(1), errs);
  KmzFilePtr created(KmzFile::OpenFromFile(tempfile->name().c_str()));
  ASSERT_TRUE(created);
  kmlbase::StringVector list;
  created->List(&list);
  ASSERT_EQ(static_cast<size_t>(2), list.size());
  ASSERT_EQ(string("dummy.png"), list[0]);
  ASSERT_EQ(string("kmzfiles/dummy.kml"), list[1]);
}

TEST_F(KmzTest, TestCreateFromElement) {
  kmlbase::TempFilePtr tempfile = kmlbase::TempFile::CreateTempFile();
  ASSERT_TRUE(tempfile != NULL);
  const string kBaseDir = File::JoinPaths(string(DATADIR), "kmz");
  const string kTestKml = File::JoinPaths(kBaseDir, "doc.kml");
  string kml_data;
  ASSERT_TRUE(File::ReadFileToString(kTestKml, &kml_data));
  KmlFilePtr kml_file =
    KmlFile::CreateFromStringWithUrl(kml_data, kBaseDir, NULL);
  {
  ASSERT_TRUE(KmzFile::CreateFromElement(
        kml_file->get_root(), kml_file->get_url(), tempfile->name()));
  }
  KmzFilePtr created(KmzFile::OpenFromFile(tempfile->name().c_str()));
  ASSERT_TRUE(created);
  std::vector<string> list;
  created->List(&list);
  ASSERT_EQ(static_cast<size_t>(3), list.size());
  ASSERT_EQ(string("doc.kml"), list[0]);
  ASSERT_EQ(string("dummy.png"), list[1]);
  ASSERT_EQ(string("kmzfiles/dummy.kml"), list[2]);
}

TEST_F(KmzTest, TestCreateFromKmlFilePath) {
  kmlbase::TempFilePtr tempfile = kmlbase::TempFile::CreateTempFile();
  ASSERT_TRUE(tempfile != NULL);
  const string kBaseDir = File::JoinPaths(string(DATADIR), "kmz");
  const string kTestKml = File::JoinPaths(kBaseDir, "doc.kml");
  {
  ASSERT_TRUE(KmzFile::CreateFromKmlFilepath(kTestKml, tempfile->name()));
  }
  KmzFilePtr created(KmzFile::OpenFromFile(tempfile->name().c_str()));
  ASSERT_TRUE(created);
  std::vector<string> list;
  created->List(&list);
  ASSERT_EQ(static_cast<size_t>(3), list.size());
  ASSERT_EQ(string("doc.kml"), list[0]);
  ASSERT_EQ(string("dummy.png"), list[1]);
  ASSERT_EQ(string("kmzfiles/dummy.kml"), list[2]);
}

TEST_F(KmzTest, TestCreateFromKmlFile) {
  kmlbase::TempFilePtr tempfile = kmlbase::TempFile::CreateTempFile();
  ASSERT_TRUE(tempfile != NULL);
  const string kBaseDir = File::JoinPaths(string(DATADIR), "kmz");
  const string kTestKml = File::JoinPaths(kBaseDir, "doc.kml");
  string kml_data;
  ASSERT_TRUE(File::ReadFileToString(kTestKml, &kml_data));
  KmlFilePtr kml_file =
    KmlFile::CreateFromStringWithUrl(kml_data, kBaseDir, NULL);
  {
  ASSERT_TRUE(KmzFile::CreateFromKmlFilepath(kTestKml, tempfile->name()));
  }
  KmzFilePtr created(KmzFile::OpenFromFile(tempfile->name().c_str()));
  ASSERT_TRUE(created);
  std::vector<string> list;
  created->List(&list);
  ASSERT_EQ(static_cast<size_t>(3), list.size());
  ASSERT_EQ(string("doc.kml"), list[0]);
  ASSERT_EQ(string("dummy.png"), list[1]);
  ASSERT_EQ(string("kmzfiles/dummy.kml"), list[2]);
}

TEST_F(KmzTest, TestCreateFromGoogleEarthFile) {
  kmlbase::TempFilePtr tempfile = kmlbase::TempFile::CreateTempFile();
  ASSERT_TRUE(tempfile != NULL);
  const string kBaseDir = File::JoinPaths(string(DATADIR), "kmz");
  const string kTestKml = File::JoinPaths(kBaseDir, "camels.kml");
  string kml_data;
  ASSERT_TRUE(File::ReadFileToString(kTestKml, &kml_data));
  KmlFilePtr kml_file =
    KmlFile::CreateFromStringWithUrl(kml_data, kBaseDir, NULL);
  {
  ASSERT_TRUE(KmzFile::CreateFromKmlFilepath(kTestKml, tempfile->name()));
  }
  KmzFilePtr created(KmzFile::OpenFromFile(tempfile->name().c_str()));
  ASSERT_TRUE(created);
  std::vector<string> list;
  created->List(&list);
  ASSERT_EQ(static_cast<size_t>(5), list.size());
  ASSERT_EQ(string("doc.kml"), list[0]);
  ASSERT_EQ(string("files/camelbrown200.png"), list[1]);
  ASSERT_EQ(string("files/camelblack200.png"), list[2]);
  ASSERT_EQ(string("files/camera_mode.png"), list[3]);
  ASSERT_EQ(string("files/camelcolor200.png"), list[4]);
}

TEST_F(KmzTest, TestSaveToString) {
  const string kGoodKmz = string(DATADIR) + "/kmz/doc.kmz";

  kmz_file_.reset(KmzFile::OpenFromFile(kGoodKmz.c_str()));
  ASSERT_TRUE(kmz_file_);

  string read_kmz_data;
  ASSERT_TRUE(kmlbase::File::ReadFileToString(kGoodKmz, &read_kmz_data));
  ASSERT_FALSE(read_kmz_data.empty());

  ASSERT_FALSE(kmz_file_->SaveToString(NULL));
  string saved_kmz_data;
  ASSERT_TRUE(kmz_file_->SaveToString(&saved_kmz_data));
  ASSERT_EQ(read_kmz_data, saved_kmz_data);
}

TEST_F(KmzTest, TestSetGetMaxUncompressedFileSize) {
  const unsigned int kDefaultSize = 2147483647;  // 2 GB.
  const unsigned int kNewSize = 209715200;  // 200 MB.
  const string kGoodKmz = string(DATADIR) + "/kmz/doc.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kGoodKmz.c_str()));
  ASSERT_TRUE(kmz_file_);
  ASSERT_EQ(kDefaultSize, kmz_file_->get_max_uncompressed_file_size());
  kmz_file_->set_max_uncompressed_file_size(kNewSize);
  ASSERT_EQ(kNewSize, kmz_file_->get_max_uncompressed_file_size());
}

TEST_F(KmzTest, TestMaxUnCompressedSizeExceeded) {
  const string kGoodKmz = string(DATADIR) + "/kmz/doc.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kGoodKmz.c_str()));
  ASSERT_TRUE(kmz_file_);
  const unsigned int kMaxSize = 43;
  kmz_file_->set_max_uncompressed_file_size(kMaxSize);  // 43 bytes.
  ASSERT_EQ(kMaxSize, kmz_file_->get_max_uncompressed_file_size());
  // ReadFile fails on a file that is 44 bytes.
  ASSERT_FALSE(kmz_file_->ReadFile("doc.kml", NULL));
}

}  // end namespace kmlengine

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
