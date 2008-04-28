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

#ifndef KML_DOM_DOCUMENT_H__
#define KML_DOM_DOCUMENT_H__

#include <vector>
#include "kml/dom/container.h"
#include "kml/dom/schema.h"
#include "kml/dom/styleselector.h"
#include "kml/dom/kml22.h"

namespace kmldom {

class Document : public Container {
 public:
  virtual ~Document();
  virtual KmlDomType Type() const { return Type_Document; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_Document || Container::IsA(type);
  }

  // <Schema>
  void add_schema(Schema* schema) {
    if (schema && schema->set_parent(this)) {
      schema_array_.push_back(schema);
    }
  }

  const size_t schema_array_size() const {
    return schema_array_.size();
  }

  const Schema* schema_array_at(unsigned int index) const {
    return schema_array_[index];
  }

  // <Style> and <StyleMap>
  void add_styleselector(StyleSelector* styleselector) {
    if (styleselector && styleselector->set_parent(this)) {
      styleselector_array_.push_back(styleselector);
    }
  }

  const size_t styleselector_array_size() const {
    return styleselector_array_.size();
  }

  const StyleSelector* styleselector_array_at(unsigned int index) const {
    return styleselector_array_[index];
  }

  // Note: If Document contains a StyleSelector, it is appended to Document's
  // array of StyleSelectors and is NOT handed up to Feature. The current
  // KML Spec/XSD is incorrect in that it gives any Feature this array
  // behaviour. Any Feature other than Document may have only ONE StyleSelector.
 private:
  friend class KmlFactory;
  Document();
  friend class KmlHandler;
  virtual void AddElement(Element* element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  std::vector<Schema*> schema_array_;
  std::vector<StyleSelector*> styleselector_array_;
  DISALLOW_EVIL_CONSTRUCTORS(Document);
};

}  // end namespace kmldom

#endif  // KML_DOM_DOCUMENT_H__
