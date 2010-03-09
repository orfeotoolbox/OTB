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

#include "kml/xsd/xsd_file.h"
#include <algorithm>
#include <vector>
#include "kml/base/expat_handler.h"
#include "kml/base/expat_parser.h"
#include "kml/base/string_util.h"
#include "kml/xsd/xsd_handler.h"

using std::vector;
using kmlbase::ExpatParser;

namespace kmlxsd {

// static
XsdFile* XsdFile::CreateFromParse(const string& xsd_data,
                                        string* errors) {
  XsdFile* xsd_file = new XsdFile;
  XsdHandler xsd_handler(xsd_file);
  if (ExpatParser::ParseString(xsd_data, &xsd_handler, errors, false)) {
    return xsd_file;
  }
  delete xsd_file;
  return NULL;
}

// TODO: mem_fun might help avoid this functor
typedef std::pair<string, XsdElementPtr> NameElementPair;
struct GetElement : public std::unary_function<NameElementPair, void> {
  GetElement(XsdElementVector* elements)
    : elements_(elements) {
  }

  void operator()(NameElementPair name_element_pair) {
    elements_->push_back(name_element_pair.second);
  }

 private:
  XsdElementVector* elements_;
};

void XsdFile::GetAllElements(XsdElementVector* elements) const {
  // TODO: a normal for loop would be simpler...
  std::for_each(element_map_.begin(), element_map_.end(), GetElement(elements));
}

void XsdFile::GetAllTypes(XsdTypeVector* types) const {
  XsdTypeMap::const_iterator iter = type_map_.begin();
  XsdTypeMap::const_iterator type_map_end = type_map_.end();
  for(;iter != type_map_end; ++iter) {
    XsdTypePtr xsd_type = iter->second;
    types->push_back(xsd_type);
  }
}

const XsdTypePtr XsdFile::FindType(const string& type_name) const {
  XsdTypeMap::const_iterator iter = type_map_.find(type_name);
  return iter == type_map_.end() ? NULL : iter->second;
}

const XsdElementPtr XsdFile::FindElement(
    const string& element_name) const {
  XsdElementMap::const_iterator iter = element_map_.find(element_name);
  return iter == element_map_.end() ? NULL : iter->second;
}

const XsdTypePtr XsdFile::FindElementType(const XsdElementPtr& element) const {
  // Not much to do if the <xs:element> has no type="..." attribute.
  if (!element || element->get_type().empty()) {
    return NULL;
  }
  // If this is primitive synthesize an XsdType.
  if (element->is_primitive()) {
    return XsdPrimitiveType::Create(element->get_type());
  }
  // Chop off the namespace prefix of this element's type IFF this type is in
  // the <xs:schema>'s targetNamespace.
  string type_name;
  if (!xsd_schema_ ||
      !xsd_schema_->SplitNsName(element->get_type(), &type_name)) {
    return NULL;
  }
  return FindType(type_name);
}

void XsdFile::FindChildElements(const XsdComplexTypePtr& complex_type,
                                XsdElementVector* elements) const {
  if (!complex_type) {
    return;
  }
  size_t sequence_size = complex_type->get_sequence_size();
  for (size_t i = 0; i < sequence_size; ++i) {
    const XsdElementPtr element = complex_type->get_sequence_at(i);
    if (element->is_ref()) {
       elements->push_back(ResolveRef(element->get_name()));
    } else {
      elements->push_back(element);
    }
  }
}

void XsdFile::GetChildElements(const string& complex_element_name,
                               XsdElementVector* elements) const {
  const XsdTypePtr xsd_type =
      FindElementType(FindElement(complex_element_name));
  if (XsdComplexTypePtr complex_type =
          XsdComplexType::AsComplexType(xsd_type)) {
    FindChildElements(complex_type, elements);
  }
}

const XsdElementPtr XsdFile::ResolveRef(const string& element_ref) const {
  // Proceed only if there is an <xs:schema> set and if the given element_ref
  // is for this XSD file's target namespace.
  string element_name;
  if (!xsd_schema_ || !xsd_schema_->SplitNsName(element_ref, &element_name)) {
    return NULL;
  }
  return FindElement(element_name);
}

XsdComplexTypePtr XsdFile::GetBaseType(
    const XsdComplexTypePtr& complex_type) const {
  string type_name;
  if (!xsd_schema_->SplitNsName(complex_type->get_extension_base(),
                                &type_name)) {
    return NULL;  // XSD is incomplete.
  }
  return XsdComplexType::AsComplexType(FindType(type_name));
}

bool XsdFile::GetTypeHierarchy(const XsdComplexTypePtr& complex_type,
                               vector<XsdComplexTypePtr>* type_hier) const {
  if (!xsd_schema_) {  // Not much we can do w/o a <xs:schema ... >!
    return false;
  }
  XsdComplexTypePtr current = complex_type;
  while (current->has_extension_base()) {
    if (XsdComplexTypePtr base = GetBaseType(current)) {
      if (type_hier) {
        type_hier->push_back(base);
      }
      current = base;
    } else {
      return false;
    }
  }
  return true;  // Walked type hierarchy successfully.
}

// TODO: XsdFile::ElementIterator w/ some filter or a back_inserter-alike for
// these three...
void XsdFile::GetAbstractElements(XsdElementVector* elements) const {
  XsdElementMap::const_iterator iter = element_map_.begin();
  for (; iter != element_map_.end(); ++iter) {
    XsdElementPtr element = iter->second;
    // TODO: filter out extension elements, e.g., only referenced substi-groups
    if (element->is_abstract()) {
      elements->push_back(element);
    }
  }
}

void XsdFile::GetComplexElements(XsdElementVector* elements) const {
  XsdElementMap::const_iterator iter = element_map_.begin();
  for (; iter != element_map_.end(); ++iter) {
    XsdElementPtr element = iter->second;
    if (!element->is_abstract()) {
      // TODO: FindElementType a bit inefficient... make a map<element,type>
      if (XsdComplexType::AsComplexType(FindElementType(element))) {
        elements->push_back(element);
      }
    }
  }
}

void XsdFile::GetSimpleElements(XsdElementVector* elements) const {
  XsdElementMap::const_iterator iter = element_map_.begin();
  for (; iter != element_map_.end(); ++iter) {
    XsdElementPtr element = iter->second;
    if (!element->is_abstract()) {
      if (element->is_primitive() ||
          (XsdSimpleType::AsSimpleType(FindElementType(element)))) {
        elements->push_back(element);
      }
    }
  }
}

// TODO: this isn't particularily XsdFile-specific
void XsdFile::GenerateElementIdVector(XsdElementVector* elements,
                                      size_t* begin_complex,
                                      size_t *begin_simple) const {
  // TODO: make a map<XsdElement, XsdType> first and avoid FindElementType
  // in the GetXxxElements() above.
  if (!elements) {
    return;
  }
  GetAbstractElements(elements);
  if (begin_complex) {
    *begin_complex = elements->size();
  }
  GetComplexElements(elements);
  if (begin_simple) {
    *begin_simple = elements->size();
  }
  GetSimpleElements(elements);
}

bool XsdFile::SearchTypeHierarchy(const XsdComplexTypePtr& complex_type,
                                  const XsdComplexTypePtr& find_type) const {
  if (!xsd_schema_) {  // Not much we can do w/o a <xs:schema ... >!
    return false;
  }
  if (find_type == complex_type) {
    return true;
  }
  if (XsdComplexTypePtr base_type = GetBaseType(complex_type)) {
    return SearchTypeHierarchy(base_type, find_type);
  }
  return false;
}

void XsdFile::GetElementsOfType(const XsdComplexTypePtr& type,
                                XsdElementVector* elements) const {
  if (!elements) {
    return;
  }
  XsdElementMap::const_iterator iter = element_map_.begin();
  for (; iter != element_map_.end(); ++iter) {
    XsdElementPtr element = iter->second;
    if (XsdComplexTypePtr complex_type =
            XsdComplexType::AsComplexType(FindElementType(element))) {
      if (SearchTypeHierarchy(complex_type, type)) {
        elements->push_back(element);
      }
    }
  }
}

void XsdFile::GetElementsOfTypeByName(const string& type_name,
                                      XsdElementVector* elements) const {
  if (!elements) {
    return;
  }
  if (const XsdComplexTypePtr complex_type =
      XsdComplexType::AsComplexType(FindType(type_name))) {
    GetElementsOfType(complex_type, elements);
  }
}

}  // end namespace kmlxsd
