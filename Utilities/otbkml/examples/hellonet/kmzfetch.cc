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

#include <iostream>
#include <map>
#include <string>
#include "kml/base/file.h"
#include "kml/dom.h"
#include "kml/engine.h"
#include "curlfetch.h"

using std::cout;
using std::endl;
using kmlbase::File;
using kmlengine::KmzCache;
using kmlengine::KmlFile;
using kmlengine::KmlFilePtr;

int main(int argc, char** argv) {
  if (argc != 2 && argc != 3) {
    cout << "usage: " << argv[0] << " url.kmz[/file/in/kmz] [output_file]"
      << endl;
    return 1;
  }
  const char* url = argv[1];
  const char* output = argc == 3 ? argv[2] : NULL;

  CurlNetFetcher curl_net_fetcher;
  KmzCache kmz_cache(&curl_net_fetcher, 2);

  std::string data;
  if (!kmz_cache.FetchUrl(url, &data)) {
    cout << "fetch failed " << url << endl;
    return 1;
  }

  if (output) {
    if (!File::WriteStringToFile(data, output)) {
      cout << "write failed " << output << endl;
      return 1;
    }
    cout << "wrote " << data.size() << " bytes to " << output << endl;
    return 0;
  }

  // Try to parse it as KML.
  KmlFilePtr kml_file = KmlFile::CreateFromParse(data, NULL);
  if (kml_file) {
    std::string output;
    if (kml_file->SerializeToString(&output)) {
      cout << output;
    }
  }

  return 0;
}
