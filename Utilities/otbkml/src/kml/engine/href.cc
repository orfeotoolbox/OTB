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

#include "kml/engine/href.h"

namespace kmlengine {

// private
size_t Href::ParseScheme(const string& href) {
  // Look for <scheme>://...
  size_t css = href.find("://");
  if (css == string::npos || css == 0) {
    return 0;
  }
  scheme_ = href.substr(0, css);
  return css + 3;  // First char after ://
}

// private
size_t Href::ParseNetLoc(const string& href) {
  // Look for ...://<net_loc>/...
  size_t slash = href.find('/');
  if (slash == string::npos) {
    return 0;
  }
  net_loc_ = href.substr(0, slash);
  return slash + 1;  // First char after /
}

// private
void Href::Parse(const string& href) {
  size_t net_loc_start = ParseScheme(href);
  size_t path_start;
  if (net_loc_start > 0) {
    path_start = ParseNetLoc(href.substr(net_loc_start)) + net_loc_start;
  } else {
    path_start = 0;
  }
  size_t path_end = href.size();
  size_t pound = href.find_first_of('#');
  if (pound != string::npos) {
    fragment_ = href.substr(pound + 1);
    path_end = pound;
  }
  path_ = href.substr(path_start, path_end);
}

}  // end namespace kmlengine

