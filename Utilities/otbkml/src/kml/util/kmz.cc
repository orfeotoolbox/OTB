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

#include "kml/util/kmz.h"
#include <cstring>
#include "kml/util/fileio.h"
#include "minizip/unzip.h"

// Internal helper function to read the file currently pointed to by the
// zipfile cursor. 
static bool ReadCurrentFile(unzFile zipfile, std::string* result) {
  if (unzOpenCurrentFile(zipfile) != UNZ_OK) {
    return false;
  }
  unz_file_info file_info;
  if (unzGetCurrentFileInfo(zipfile, &file_info, 0, 0, 0, 0, 0, 0) != UNZ_OK) {
    unzCloseCurrentFile(zipfile);
    return false;
  }
  int nbytes = file_info.uncompressed_size;
  char* filedata = new char[nbytes];
  if (unzReadCurrentFile(zipfile, filedata, nbytes) == nbytes) {
    result->assign(filedata, nbytes);
    unzCloseCurrentFile(zipfile);
    delete [] filedata;
    return true;
  }
  unzCloseCurrentFile(zipfile);
  delete [] filedata;
  return false;
}

// Internal helper function to read one named file from an archive. The caller
// is responsible for ensuring that result is not NULL.
static bool ReadOne(unzFile zipfile,
                    const std::string& subfile,
                    std::string* result) {
  if (!zipfile) {
    return false;
  }
  result->clear();  // Because API defines a false return == result->empty().
  if (unzLocateFile(zipfile, subfile.c_str(), 0) == UNZ_OK
      && ReadCurrentFile(zipfile, result)) {
    return true;
  }
  return false;
}

// Internal helper function to setup a unzFile instance.
unzFile CreateUnzFile(const std::string& kmz_filename) {
  unzFile zipfile = NULL;
  zipfile = unzOpen(kmz_filename.c_str());
  if (zipfile == NULL) {
    return NULL;
  }
  if (unzGoToFirstFile(zipfile) != UNZ_OK) {
    unzClose(zipfile);
    return NULL;
  }
  return zipfile;
}

bool DataIsKmz(const std::string& kmz_data) {
  return kmz_data.substr(0, 4) == "PK\003\004" ? true : false;
}

bool ReadFileFromKmz(const std::string& kmz_filename,
                     const std::string& subfile,
                     std::string* result) {
  if (!result) {
    return false;
  }
  unzFile zipfile = CreateUnzFile(kmz_filename);
  if (ReadOne(zipfile, subfile, result)) {
    unzClose(zipfile);
    return true;
  }
  unzClose(zipfile);
  return false;
}

bool ReadKmlFromKmz(const std::string& kmz_filename, std::string* result) {
  if (!result) {
    return false;
  }
  unzFile zipfile = CreateUnzFile(kmz_filename);
  // Try to read doc.kml first:
  if (ReadOne(zipfile, "doc.kml", result)) {
    unzClose(zipfile);
    return true;
  }
  // If no doc.kml, try to read first KML file:
  unz_file_info file_info;
  do {
    static char buf[1024];
    // 1024 is rather arbitrary, but is what almost all other implemenations
    // use, including Google Earth. minizip should not overrun the buffer
    // no matter what.
    if (unzGetCurrentFileInfo(zipfile, &file_info, buf, sizeof(buf),
                              0, 0, 0, 0) == UNZ_OK) {
      if (strlen(buf) >= 4 && strcmp(buf + strlen(buf)-4, ".kml") == 0 &&
          ReadCurrentFile(zipfile, result)) {
        unzClose(zipfile);
        return true;
      }
    }
  } while (unzGoToNextFile(zipfile) == UNZ_OK);
  unzClose(zipfile);
  return false;
}
