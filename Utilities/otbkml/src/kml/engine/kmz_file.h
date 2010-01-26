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

// This file contains the declaration of the KmzFile class.

#ifndef KML_ENGINE_KMZ_FILE_H__
#define KML_ENGINE_KMZ_FILE_H__

#include <string>
#include <vector>

#include "boost/scoped_ptr.hpp"
#include "kml/base/referent.h"
#include "kml/base/tempfile.h"
#include "kml/base/util.h"

namespace kmlengine {

// ZlibImpl hides the implementation details of Zlib's Minizip from our
// interface.
class ZlibImpl;

// The Kmz class represents an instance of a KMZ file. It contains methods
// for reading and writing KMZ files.
class KmzFile : public kmlbase::Referent {
 public:
  ~KmzFile();

  // Open a KMZ file from a file path. Returns a pointer to a KmzFile object
  // if the file could be opened and read, and the data was recognizably KMZ.
  // Otherwise returns NULL.
  static KmzFile* OpenFromFile(const char* kmz_filepath);

  // Open a KMZ file from a string. Returns a pointer to a KmzFile object if a
  // temporary file could be created, the data was recognizably KMZ. Otherwise
  // returns NULL.
  static KmzFile* OpenFromString(const std::string& kmz_data);

  static KmzFile* CreateFromString(const std::string& kmz_data) {
    return OpenFromString(kmz_data);
  }

  // Checks to see if kmz_data looks like a PK ZIP file.
  static bool IsKmz(const std::string& kmz_data);

  // Read the default KML file from a KMZ archive if it can be found, otherwise
  // read the first KML file. Return false if no KML file could be found.
  // We define the default KML file as a file at the root level called
  // "doc.kml". We define first to be coincident with the order in which the
  // files were originally added to the archive. The output string is not
  // cleared before being written to.
  bool ReadKml(std::string* output) const;

  // Read a specific file from a KMZ archive. Returns false if subfile was not
  // found, or if subfile could not be read. Note: subfile must be a full path
  // from the archive root. Relative references of "../../foo" are not handled.
  // The output string is not cleared before being written to.
  bool ReadFile(const char* subfile, std::string* output) const;

  // Fills a vector of strings of the files contained in the KMZ archive.
  // The vector is not cleared, only appended to. The string is the full path
  // name of the KML file from the archive root, with '/' as the separator.
  // Returns false upon error.
  bool List(std::vector<std::string>* subfiles);

  // Creates a KMZ file from a string of KML data. Returns true if
  // kmz_filepath could be successfully created and written.
  // TODO: Permit adding resources (images, models, etc.) to the KMZ archive.
  static bool WriteKmz(const char* kmz_filepath, const std::string& kml);

 private:
  // Class can only be created from static methods.
  KmzFile(ZlibImpl* zlibimpl_, const kmlbase::TempFilePtr& tempfile);
  ZlibImpl* zlibimpl_;
  kmlbase::TempFilePtr tempfile_;
  // Internal helper function to read the file currently pointed to by the
  // zipfile cursor.
  bool ReadCurrentFile(std::string* result) const;
  // Internal helper function to read one named file from an archive.
  bool ReadOne(const char* subfile, std::string* output) const;
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(KmzFile);
};

typedef boost::intrusive_ptr<KmzFile> KmzFilePtr;

}  // end namespace kmlengine

#endif  // KML_ENGINE_KMZ_FILE_H__
