// Copyright 2008, Google Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice,//     this list of conditions and the following disclaimer in the documentation//     and/or other materials provided with the distribution.
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

// This file contains the implementation of the ConvertOldSchema() and
// ParseOldSchema() functions.

#include "kml/engine/parse_old_schema.h"
#include <map>
#include "kml/dom.h"
#include "kml/engine/engine_types.h"

using kmldom::PlacemarkPtr;
using kmldom::SchemaPtr;

namespace kmlengine {

bool ConvertOldSchema(const string& input_xml,
                      const SchemaNameMap& schema_name_map,
                      string* output_xml) {
  if (!output_xml) {
    return false;
  }
  // Determine if this looks like "<tag>...</tag>".
  if (input_xml[0] != '<') {
    return false;
  }
  string::size_type gt = input_xml.find('>');
  if (gt == string::npos) {
    return false;
  }
  const string tag_name = input_xml.substr(1, gt-1);
  string::size_type end_tag =
      input_xml.find(string("</" + tag_name + ">"));
  if (end_tag == string::npos) {
    return false;
  }
  // See if there's a <Schema> by/for this name.
  SchemaNameMap::const_iterator iter = schema_name_map.find(tag_name);
  if (iter == schema_name_map.end()) {
    return false;
  }
  // Old <Schema> had a parent= attribute naming the element to extend, but
  // only <Placemark> was ever supported.  (And OGC KML 2.2 <Schema> has no
  // parent=.  Replace <tag>...</tag> with <Placemark>...</Placemark>).
  *output_xml = "<Placemark>" + input_xml.substr(gt+1, end_tag - gt - 1) +
      "</Placemark>";
  return true;
}

PlacemarkPtr ParseOldSchema(const string& xml,
                            const SchemaNameMap& schema_name_map,
                            string* errors) {
  string converted_xml;
  if (!ConvertOldSchema(xml, schema_name_map, &converted_xml)) {
    return NULL;
  }
  // TODO: translate each field to a <ExtendedData><Data> item, perhaps by
  // parsing here with a ParserObserver for this specific task.
  return kmldom::AsPlacemark(kmldom::Parse(converted_xml, errors));
}

}  // end namespace kmlengine
