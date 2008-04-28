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
// functions and the implementation of the Serialize class.

#include "kml/dom/serializer.h"
#include "kml/dom/kml_funcs.h"
#include <cstring>
#include <stack>
#include <string>
#include <sstream>
#include "kml/dom/attributes.h"
#include "kml/dom/element.h"
#include "kml/dom/kml22.h"
#include "kml/dom/xsd.h"

namespace kmldom {

// This function is in the public API for converting the given Element
// hierarchy to "pretty" xml.
std::string SerializePretty(const Element& root) {
  Serializer serializer("\n", "  ");
  root.Serialize(serializer);
  std::string xml;
  serializer.WriteString(&xml);
  return xml;
}

// This function is in the public API for converting the given Element
// hierarchy to xml with no additional whitespace for newlines or
// indentation.
std::string SerializeRaw(const Element& root) {
  Serializer serializer("", "");
  root.Serialize(serializer);
  std::string xml;
  serializer.WriteString(&xml);
  return xml;
}

// Construct the Serializer with the given strings for newline and indent.
// Use "" for no newline and/or indent.
Serializer::Serializer(const char* newline, const char* indent)
  : xsd_(Xsd::GetSchema()),
    newline_(newline),
    indent_(indent) {
}

// Study the incoming string for chars that are invalid to represent in XML.
// Wrap the whole thing in a CDATA if any found.  Doesn't try to '&amp;'-ify.
// Avoids CDATA-ing any string that's already a CDATA.
// Returns a string that's legal for an XML value.
const std::string Serializer::MaybeQuoteString(const std::string& value) {
  if ((value.find("<![CDATA[",0) != 0) &&
      (value.find_first_of("&'<>\"", 0) != std::string::npos))
    return "<![CDATA[" + value + "]]>";
  else
    return value;
}

// This emits the begin tag of the given element: "<Placemark id="foo">.
void Serializer::BeginById(int type_id, const Attributes& attributes) {
  const std::string& tag_name = xsd_->ElementName(type_id);
  Indent();
  std::string attrs;
  attributes.Serialize(&attrs);
  xml_.push_back("<" + tag_name + attrs + ">" + newline_);
  tag_stack_.push(tag_name);
}

// This emits the end tag of the current element: "</Placemark>".
// If there were no child elements nor any character data the begin tag
// is replaced with a nil tag: "<Placemark>" -> "<Placemark/>".
void Serializer::End() {
  std::string tag = tag_stack_.top();
  tag_stack_.pop();
  std::string& last_xml = xml_[xml_.size()-1];
  // Is the most recent item pushed out our begin tag?
  std::string::size_type tag_size = tag.size() + 1;  // "<" + tag
  // If there were attributes this counts on a space after the tag,
  // else a ">".
  if ((last_xml.compare(0, tag_size, "<" + tag) == 0) &&
     ((last_xml[tag_size] == '>') || (last_xml[tag_size] == ' '))) {
    // Yes, rewrite it to end with "/>"
    // Chop off the ">" and any newline_ (newline_ is always non-NULL).
    std::string::size_type length = last_xml.size() - 1 - strlen(newline_);
    // Re-assign with a "/>" + newline.
    last_xml.assign(last_xml.substr(0, length) + "/>" + newline_);
  } else {  // There's content after the begin tag so close as normal.
    Indent();
    xml_.push_back("</" + tag + ">" + newline_);
  }
}

// This emits a field.  All fields reduce to this method.
void Serializer::SaveStringFieldByName(std::string tagName, std::string value) {
  Indent();
  xml_.push_back("<" + tagName + ">" + MaybeQuoteString(value) +
                 "</" + tagName + ">" + newline_);
}

// This is a special case for Snippet/linkSnippet which has both
// character data and attributes.
void Serializer::SaveComplexStringFieldByName(std::string tagName,
                                              const Attributes& attributes,
                                              std::string value) {
  Indent();
  std::string attrs;
  attributes.Serialize(&attrs);
  xml_.push_back("<" + tagName + attrs + ">" + MaybeQuoteString(value) +
                 "</" + tagName + ">" + newline_);
}

// This is used to emit raw character data content.
void Serializer::SaveContent(std::string content) {
  xml_.push_back(content);
}

// This emits the white space specified by indent_.
void Serializer::Indent() {
  int depth = tag_stack_.size();
  while (depth--) {
    xml_.push_back(indent_);
  }
}

// This emits the state of the serializer to the given string.
void Serializer::WriteString(std::string* output) {
  if (output) {
    output->clear();
    for (size_t i = 0; i < xml_.size(); ++i) {
      *output += xml_[i];
    }
  }
}

// This emits the string for the given enum and enum value.
// For example, type_id=Type_altitudeMode, enum_value=ALTITUDEMODE_ABSOLUTE.
// If enum_value is not valid for the given type_id nothing is emitted.
void Serializer::SaveEnum(int type_id, int enum_value) {
  std::string enum_string = xsd_->EnumValue(type_id, enum_value);
  if (!enum_string.empty()) {
    SaveFieldById(type_id, enum_string);
  }
}

// This emits the given Element.  This is a method of Serialize such that
// an Element's Serialize method need only friend class Serialize.
void Serializer::SaveElement(const Element& element) {
  element.Serialize(*this);
}

}  // namespace kmldom
