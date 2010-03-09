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

#ifndef KML_DOM_CONTAINER_H__
#define KML_DOM_CONTAINER_H__

#include <vector>
#include "kml/dom/feature.h"
#include "kml/dom/kml22.h"
#include "kml/base/util.h"

namespace kmldom {

class Serializer;
class VisitorDriver;

// OGC KML 2.2 Standard: 9.6 kml:AbstractContainerGroup
// OGC KML 2.2 XSD: <element name="AbstractContainerGroup"...
class Container : public Feature {
 public:
  virtual ~Container();
  virtual KmlDomType Type() const { return Type_Container; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_Container || Feature::IsA(type);
  }

  void add_feature(const FeaturePtr& feature);

  size_t get_feature_array_size() const {
    return feature_array_.size();
  }

  const FeaturePtr& get_feature_array_at(size_t index) const {
    return feature_array_[index];
  }

  // If a Feature with the given id exists in this Container remove it from
  // the container and return it.  If no such Feature exists NULL is returned.
  // Note: This method is a special mostly for use with Update/Delete.
  FeaturePtr DeleteFeatureById(const string& id);

  // Visitor API methods, see visitor.h.
  virtual void AcceptChildren(VisitorDriver* driver);

 protected:
  // Container is abstract.
  Container();
  virtual void AddElement(const ElementPtr& element);
  void SerializeFeatureArray(Serializer& serializer) const;
  virtual void Serialize(Serializer& serializer) const;

 private:
  std::vector<FeaturePtr> feature_array_;
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(Container);
};

}  // end namespace kmldom

#endif // KML_CONTAINER_H_
