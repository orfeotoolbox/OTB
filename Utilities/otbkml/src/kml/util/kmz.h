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

// A collection of utility functions for reading KMZ files.

#ifndef KML_UTIL_KMZ_H__
#define KML_UTIL_KMZ_H__

#include <string>

// Returns true if the string is recognisably a KMZ archive. The checking
// is based solely on comparing the start of the data to the zipfile magic
// numbers and isn't a guarantee that it represents a complete zipfile.
bool DataIsKmz(const std::string& kmz_data);

// Read a specific file from a KMZ archive. Returns false if subfile was not
// found, or if subfile could not be read.
// Note: subfile must be a path relative to the archive root.
bool ReadFileFromKmz(const std::string& kmz_filename,
                     const std::string& subfile,
                     std::string* result);

// Read the default KML file from a KMZ archive if it can be found, otherwise
// read the first KML file. Return false if no KML file could be found.
// We define the default KML file as a file at the root level called "doc.kml".
// We define first to be coincident with the order in which the files were
// originally added to the archive.
bool ReadKmlFromKmz(const std::string& kmz_filename, std::string* result);

#endif  // KML_UTIL_KMZ_H__

