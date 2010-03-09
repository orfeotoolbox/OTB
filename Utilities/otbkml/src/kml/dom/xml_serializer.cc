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

// This file contains the implementation of the public XML serializing
// functions and the implementation of the XmlSerialize class.

#include "kml/dom/xml_serializer.h"
#include "kml/dom/kml_funcs.h"
#include <cstring>
#include <stack>
#include <sstream>
#include "kml/base/attributes.h"
#include "kml/dom/element.h"
#include "kml/dom/kml22.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/xsd.h"

using kmlbase::Attributes;

namespace kmldom {

// This function is in the public API for converting the given Element
// hierarchy to "pretty" xml.
string SerializePretty(const ElementPtr& root) {
  if (!root) {
    return string("");
  }
  XmlSerializer serializer("\n", "  ");
  root->Serialize(serializer);
  string xml;
  serializer.WriteString(&xml);
  return xml;
}

// This function is in the public API for converting the given Element
// hierarchy to xml with no additional whitespace for newlines or
// indentation.
string SerializeRaw(const ElementPtr& root) {
  if (!root) {
    return string("");
  }
  XmlSerializer serializer("", "");
  root->Serialize(serializer);
  string xml;
  serializer.WriteString(&xml);
  return xml;
}

// Construct the XmlSerializer with the given strings for newline and indent.
// Use "" for no newline and/or indent.
XmlSerializer::XmlSerializer(const char* newline, const char* indent)
  : newline_(newline),
    indent_(indent) {
}

// This emits the begin tag of the given element: "<Placemark id="foo">.
void XmlSerializer::BeginById(int type_id, const Attributes& attributes) {
  const string& tag_name = xsd_.ElementName(type_id);
  Indent();
  string attrs;
  attributes.Serialize(&attrs);
  xml_.push_back("<" + tag_name + attrs + ">" + newline_);
  tag_stack_.push(tag_name);
}

// This emits the end tag of the current element: "</Placemark>".
// If there were no child elements nor any character data the begin tag
// is replaced with a nil tag: "<Placemark>" -> "<Placemark/>".
void XmlSerializer::End() {
  string tag = tag_stack_.top();
  tag_stack_.pop();
  string& last_xml = xml_[xml_.size()-1];
  // Is the most recent item pushed out our begin tag?
  string::size_type tag_size = tag.size() + 1;  // "<" + tag
  // If there were attributes this counts on a space after the tag,
  // else a ">".
  if ((last_xml.compare(0, tag_size, "<" + tag) == 0) &&
     ((last_xml[tag_size] == '>') || (last_xml[tag_size] == ' '))) {
    // Yes, rewrite it to end with "/>"
    // Chop off the ">" and any newline_ (newline_ is always non-NULL).
    string::size_type length = last_xml.size() - 1 - strlen(newline_);
    // Re-assign with a "/>" + newline.
    last_xml.assign(last_xml.substr(0, length) + "/>" + newline_);
  } else {  // There's content after the begin tag so close as normal.
    Indent();
    xml_.push_back("</" + tag + ">" + newline_);
  }
}

// This emits a field.  All fields reduce to this method.
void XmlSerializer::SaveStringFieldById(int type_id, string value) {
  string tagName = Xsd::GetSchema()->ElementName(type_id);
  Indent();
  if (value.empty()) {
    xml_.push_back("<" + tagName + "/>" + newline_);
  } else {
    xml_.push_back("<" + tagName + ">" + MaybeQuoteString(value) +
                   "</" + tagName + ">" + newline_);
  }
}

// This is used to emit raw character data content.  Honor request to emit
// content unescaped if maybe_quote requests.
void XmlSerializer::SaveContent(const string& content, bool maybe_quote) {
  if (maybe_quote) {
    xml_.push_back(MaybeQuoteString(content));
  } else {
    xml_.push_back(content);
  }
}

void XmlSerializer::SaveColor(int type_id, const kmlbase::Color32& color) {
  SaveFieldById(type_id, color.to_string_abgr());
}

// This emits the white space specified by indent_.
void XmlSerializer::Indent() {
  size_t depth = tag_stack_.size();
  while (depth--) {
    xml_.push_back(indent_);
  }
}

// This emits the state of the serializer to the given string.
void XmlSerializer::WriteString(string* output) {
  if (output) {
    output->clear();
    for (size_t i = 0; i < xml_.size(); ++i) {
      *output += xml_[i];
    }
  }
}

}  // namespace kmldom
