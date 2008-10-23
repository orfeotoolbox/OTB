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

// This sample program reads and parses one KML or KMZ file and emits the
// serialization to standard output.  Since all recognized elements
// are saved in the KML DOM with white space discarded and Serialize()
// formats nicely this is overall a pretty printer.

#include <iostream>
#include <string>
#include "boost/scoped_ptr.hpp"
#include "kml/dom.h"
#include "kml/engine.h"
#include "kml/base/file.h"

using kmldom::ElementPtr;
using kmlengine::KmlFile;
using kmlengine::KmlFilePtr;
using std::cout;
using std::endl;

int main(int argc, char** argv) {
  if (argc != 2) {
    cout << "usage: " << argv[0] << " kmlfile" << endl;
    return 1;
  }

  // Read the file content.
  std::string file_data;
  if (!kmlbase::File::ReadFileToString(argv[1], &file_data)) {
    cout << argv[1] << " read failed" << endl;
    return 1;
  }

  // Parse it.
  std::string errors;
  KmlFilePtr kml_file = KmlFile::CreateFromParse(file_data, &errors);
  if (!kml_file) {
    cout << errors;
    return 1;
  }

  // Serialize it and output to stdout.
  std::string output;
  kml_file->SerializeToString(&output);
  cout << output;

  return 0;
}
