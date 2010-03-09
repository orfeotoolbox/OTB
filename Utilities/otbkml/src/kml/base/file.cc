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

// The file contains the implementation of the File methods common across
// platforms.

#include "kml/base/file.h"
#include <fstream>

namespace kmlbase {

#ifdef WIN32
  const char kSeparator = '\\';
#else
  const char kSeparator = '/';
#endif  

bool File::ReadFileToString(const string& filename, string* output) {
  if (filename.empty() || !output) {
    return false;
  }
  std::ifstream input_file(filename.c_str(),
                           std::ios_base::in | std::ios_base::binary);
  if (!input_file.is_open() || !input_file.good()) {
    return false;
  }
  output->clear();
  const unsigned int kBufferSize = 1024;
  char buffer[kBufferSize];
  while (!input_file.eof() && input_file.good()) {
    input_file.read(buffer, kBufferSize);
    output->append(buffer, input_file.gcount());
  }
  return true;
}

bool File::WriteStringToFile(const string& data,
                             const string& filename) {
  if (filename.empty()) {
    return false;
  }
  std::ofstream output_file(filename.c_str(),
                            std::ios_base::out | std::ios_base::binary);
  if (!output_file.is_open() || !output_file.good()) {
    return false;
  }
  output_file.write(data.c_str(), static_cast<std::streamsize>(data.length()));
  output_file.close();
  return true;
}

string File::JoinPaths(const string& p1, const string& p2) {
  if (p1.empty()) return p2;
  if (p2.empty()) return p1;
  string temp_str(p1);
  if (p1[p1.length()-1] != kSeparator) {
    temp_str += kSeparator;
    return (temp_str + p2);
  } else {
    return (p1 + p2);
  }
}

void File::SplitFilePath(const string& filepath,
                         string* base_directory,
                         string* filename) {
  size_t found = filepath.find_last_of("/\\");
  if (base_directory) {
    if (found == string::npos) {
      *base_directory = ".";
    } else if (found == 0) {
      *base_directory = filepath.substr(0, found + 1);
    } else {
      *base_directory = filepath.substr(0, found);
    }
  }
  if (filename) {
    *filename = filepath.substr(found+1);
  }
}

}  // end namespace kmlbase
