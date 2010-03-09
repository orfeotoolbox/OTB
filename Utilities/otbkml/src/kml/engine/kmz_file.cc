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

// The file contains the implementation of the KmzFile class methods.

#include "kml/engine/kmz_file.h"
#include <cstring>
#include <set>
#include "boost/scoped_ptr.hpp"
#include "kml/base/file.h"
#include "kml/base/string_util.h"
#include "kml/base/zip_file.h"
#include "kml/engine/get_links.h"
#include "kml/engine/href.h"
#include "kml/engine/kml_uri.h"

using kmlbase::File;
using kmlbase::StringVector;
using kmlbase::ZipFile;

namespace kmlengine {

// This is the default name for writing a KML file to a new archive. The
// default file for reading is simply the first file in the table of contents
// that ends with ".kml".
const char kDefaultKmlFilename[] = "doc.kml";

KmzFile::KmzFile(ZipFile* zip_file) : zip_file_(zip_file) {}

KmzFile::~KmzFile() {}

// Static.
KmzFile* KmzFile::OpenFromFile(const char* kmz_filename) {
  if (ZipFile* zipfile = ZipFile::OpenFromFile(kmz_filename)) {
    return new KmzFile(zipfile);
  }
  return NULL;
}

// Static.
KmzFile* KmzFile::OpenFromString(const string& kmz_data) {
  if (ZipFile* zipfile = ZipFile::OpenFromString(kmz_data)) {
    return new KmzFile(zipfile);
  }
  return NULL;
}

// Static.
bool KmzFile::IsKmz(const string& kmz_data) {
  return ZipFile::IsZipData(kmz_data);
}


void KmzFile::set_max_uncompressed_file_size(unsigned int i) {
  zip_file_->set_max_uncompressed_file_size(i);
}

unsigned int KmzFile::get_max_uncompressed_file_size() {
  return zip_file_->get_max_uncompressed_file_size();
}


bool KmzFile::ReadKmlAndGetPath(string* output,
                                string* kml_name) const {
  if (!output) {
    return false;
  }
  string default_kml;
  if (!zip_file_->FindFirstOf(".kml", &default_kml)) {
    return false;
  }
  if (!zip_file_->GetEntry(default_kml, output)) {
    return false;
  }
  if (kml_name) {
    *kml_name = default_kml;
  }
  return true;
}

bool KmzFile::ReadKml(string* output) const {
  return ReadKmlAndGetPath(output, NULL);
}

bool KmzFile::ReadFile(const char* path_in_kmz, string* output) const {
  return zip_file_->GetEntry(path_in_kmz, output);
}

bool KmzFile::List(std::vector<string>* subfiles) {
  return zip_file_->GetToc(subfiles);
}

bool KmzFile::SaveToString(string* kmz_bytes) {
  if (!kmz_bytes) {
    return false;
  }
  *kmz_bytes = zip_file_->get_data();
  return true;
}

// Static.
KmzFile* KmzFile::Create(const char* kmz_filepath) {
  ZipFile* zipfile = ZipFile::Create(kmz_filepath);
  if (!zipfile) {
    return NULL;
  }
  return new KmzFile(zipfile);
}

bool KmzFile::AddFile(const string& data, const string& path_in_kmz) {
  return zip_file_->AddEntry(data, path_in_kmz);
}

// TODO: the implementation of this function really belongs in base/zip_file.
size_t KmzFile::AddFileList(const string& base_url,
                            const StringVector& file_paths) {
  size_t error_count = 0;
  // We remember all stored resources so we can eliminate duplicates.
  std::set<string> stored_hrefs;

  StringVector::const_iterator itr;
  for (itr = file_paths.begin(); itr != file_paths.end(); ++itr) {
    // Drop the fragment if any to get the stem of the filename.
    Href href(*itr);
    if (href.has_fragment()) {
      href.clear_fragment();
    }

    // Normalize the href.
    string normalized_href;
    if (!NormalizeHref(href.get_path(), &normalized_href)) {
      error_count++;
      continue;
    }

    // If the normalized_href points above the base_url, we consider it
    // invalid.
    if (normalized_href.substr(0, 2) == "..") {
      error_count++;
      continue;
    }

    // Detect duplicate resources and skip if found.
    if (stored_hrefs.find(normalized_href) != stored_hrefs.end()) {
      continue;  // Not an error.
    }
    stored_hrefs.insert(normalized_href);

    // Try to read the file pointed to by base_url and the normalized href.
    string relative_path = File::JoinPaths(base_url, normalized_href);
    string file_data;
    if (!kmlbase::File::ReadFileToString(relative_path, &file_data)) {
      error_count++;
      continue;
    }

    // Add the file to the KMZ archive.
    if (!AddFile(file_data, normalized_href)) {
      error_count++;
      continue;
    }
  }
  return error_count;
}

// Static.
bool KmzFile::WriteKmz(const char* kmz_filepath, const string& kml) {
  boost::scoped_ptr<KmzFile> kmz(KmzFile::Create(kmz_filepath));
  if (!kmz.get()) {
    return false;
  }
  if (!kmz->AddFile(kml, kDefaultKmlFilename)) {
    return false;
  }
  return kmlbase::File::Exists(kmz_filepath);
}

// Static.
bool KmzFile::CreateFromKmlFilepath(const string& kml_filepath,
                                    const string& kmz_filepath) {
  if (kmz_filepath.empty() || kml_filepath.empty()) {
    return false;
  }
  string kml_data;
  if (!kmlbase::File::ReadFileToString(kml_filepath, &kml_data)) {
    return false;
  }

  string base_dir;
  kmlbase::File::SplitFilePath(kml_filepath, &base_dir, NULL);

  KmlFilePtr kml_file =
    KmlFile::CreateFromStringWithUrl(kml_data, base_dir, NULL);

  return CreateFromKmlFile(kml_file, kmz_filepath);
}

// Static.
bool KmzFile::CreateFromElement(const kmldom::ElementPtr& element,
                                const string& base_url,
                                const string& kmz_filepath) {
  if (kmz_filepath.empty()) {
    return false;
  }
  KmzFilePtr kmz_file = Create(kmz_filepath.c_str());
  if (!kmz_file) {
    return false;
  }
  const string kml_data = kmldom::SerializePretty(element);
  // First add the KML file. This is the file opened by default by a client
  // from a KMZ archive.
  kmz_file->AddFile(kml_data, kDefaultKmlFilename);

  // Next gather the local references and add them.
  StringVector file_paths;
  if (GetRelativeLinks(kml_data, &file_paths)) {
    kmz_file->AddFileList(base_url, file_paths);
  }

  return kmlbase::File::Exists(kmz_filepath);
}

// Static.
bool KmzFile::CreateFromKmlFile(const KmlFilePtr& kml_file,
                                const string& kmz_filepath) {
  return KmzFile::CreateFromElement(
      kml_file->get_root(), kml_file->get_url(), kmz_filepath);
}

}  // end namespace kmlengine
