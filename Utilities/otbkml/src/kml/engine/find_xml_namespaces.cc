// Copyright 2009, Google Inc. All rights reserved.
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

#include "kml/engine/find_xml_namespaces.h"
#include <set>
#include "kml/base/attributes.h"
#include "kml/base/xml_namespaces.h"
#include "kml/dom/serializer.h"

using kmlbase::Attributes;
using kmldom::ElementPtr;
using kmldom::Serializer;

namespace kmlengine {

typedef std::set<kmlbase::XmlnsId> XmlnsIdSet;

class XmlNamespaceFinder : public Serializer {
 public:
  XmlNamespaceFinder(XmlnsIdSet* xmlns_id_set)
    : xmlns_id_set_(xmlns_id_set) {
  }

  // This is the only Serializer virtual method implemented
  // in ElementFinder.  As such only complex elements are found.
  virtual void SaveElement(const ElementPtr& element) {
    xmlns_id_set_->insert(element->get_xmlns());

    // Call Serializer to recurse.
    Serializer::SaveElement(element);
  }

 private:
  XmlnsIdSet* xmlns_id_set_;
};

void FindXmlNamespaces(const ElementPtr& element,
                       Attributes* xmlns_attributes) {
  if (element && xmlns_attributes) {
    XmlnsIdSet xmlns_id_set;
    XmlNamespaceFinder xml_namespace_finder(&xmlns_id_set);
    xmlns_id_set.insert(element->get_xmlns());
    element->Serialize(xml_namespace_finder);
    XmlnsIdSet::const_iterator iter = xmlns_id_set.begin();
    for (; iter != xmlns_id_set.end(); ++iter) {
      string prefix;
      string xml_namespace;
      if (kmlbase::FindXmlNamespaceAndPrefix(*iter, &prefix, &xml_namespace)) {
        xmlns_attributes->SetValue(prefix, xml_namespace);
      }
    }
  }
}

void FindAndInsertXmlNamespaces(ElementPtr element) {
  if (element) {
    Attributes xmlns;
    FindXmlNamespaces(element, &xmlns);
    // We (kmlengine in libkml) never prefix KML 2.2 elements.
    string kml_namespace;
    if (xmlns.CutValue("kml", &kml_namespace)) {
      // This makes KML the default namespace
      xmlns.SetValue("xmlns", kml_namespace);
    }
    element->MergeXmlns(xmlns);
  }
}

}  // end namespace kmlengine
