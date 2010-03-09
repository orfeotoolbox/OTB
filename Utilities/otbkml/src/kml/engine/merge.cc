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

// This file contains the implementation of the MergeFields() and
// MergeElement() functions and their internal helper classes and functions.

#include "kml/engine/merge.h"
#include <vector>
#include "kml/dom.h"
#include "kml/dom/serializer.h"
#include "kml/engine/clone.h"
#include "kml/engine/engine_types.h"
#include "kml/engine/find.h"

using kmlbase::Attributes;
using kmldom::CoordinatesPtr;
using kmldom::ElementPtr;
using kmldom::KmlDomType;
using kmldom::KmlFactory;
using kmldom::Serializer;

namespace kmlengine {

// This Serializer-specialization finds all simple element children in the
// source and sets each of their values in the target.
class FieldMerger : public Serializer {
 public:
  // The target is expected to be a complex element.
  FieldMerger(ElementPtr target)
      : target_(target) {}

  virtual ~FieldMerger() {}

  // Set the attributes in the target.
  virtual void BeginById(int type_id, const Attributes& attributes) {
    // ParseAttributes reflects the state of the passed attributes, so we
    // preserve the state of the element's attributes here ourselves and into
    // our private copy of the state merge the passed attributes and then
    // pass the result to ParseAttributes which sets/clears each attribute to
    // exactly reflect the state we create here.
    Attributes target_attributes;
    target_->SerializeAttributes(&target_attributes);
    target_attributes.MergeAttributes(attributes);
    target_->ParseAttributes(target_attributes.Clone());
    // Merge on <coordinates> is consistent with setting any other simple
    // element: replace the content.  Since <coordinates> is not implemented
    // as a simple element and since the only "set" operations on <coordinates>
    // are add (append) we must first clear the <coordinates>.
    if (CoordinatesPtr coordinates = AsCoordinates(target_)) {
      coordinates->Clear();
    }
  }

  // The default implementation recurses on complex children.  FieldMerger is
  // only interested in the immediate simple children of the serialized element
  // so its implementation is empty to prevent recursing on complex children.
  virtual void SaveElement(const ElementPtr& element) {
  }

  // Save a lon,lat,alt tuple as appears within <coordinates>.
  virtual void SaveVec3(const kmlbase::Vec3& vec3) {
    if (CoordinatesPtr coordinates = AsCoordinates(target_)) {
      coordinates->add_vec3(vec3);
    }
  }

  // This sets the given field in the target.
  virtual void SaveStringFieldById(int type_id, string value) {
    KmlDomType id = static_cast<KmlDomType>(type_id);
    ElementPtr field = KmlFactory::GetFactory()->CreateFieldById(id);
    field->set_char_data(value);
    target_->AddElement(field);
  }

  // Serializer::SaveColor() is called to save all Color32 values.
  virtual void SaveColor(int type_id, const kmlbase::Color32& color) {
    SaveFieldById(type_id, color.to_string_abgr());
  }

 private:
  ElementPtr target_;
};

// This is the implementation of the public API function to merge the
// fields in one element into another.
void MergeFields(const ElementPtr& source, ElementPtr target) {
  // It's actually well behaved to copy each field from the element back on to
  // itself, but it's a bit silly so we detect that here and just return.
  // No action is performed if either source or target do not exist.
  if (!source || !target || source == target) {
    return;
  }
  FieldMerger field_merger(target);
  source->Serialize(field_merger);
}

// This function sets all target simple element ("fields") and all complex
// element children to values found in the source.  This form of merge
// behavior is central to "style merging".
// TODO: Update/Change behaves _slightly_ differently but may borrow from this.
void MergeElements(const ElementPtr& source, ElementPtr target) {
  if (!source || !target) {
    return;
  }
  // Get the pointers to the immediate complex element children of the source
  // and target element.
  ElementVector source_children;
  GetChildElements(source, false, &source_children);
  ElementVector target_children;
  GetChildElements(target, false, &target_children);

  // Iterate through the source children looking for a match in the target.
  // TODO: walk through both lists taking advantage of both being in
  // the same order given that Element::Serialize() is XSD order.
  for (size_t i = 0; i < source_children.size(); ++i) {
    size_t j;
    for (j = 0; j < target_children.size(); ++j) {
      // If the target has an element of this type...
      if (source_children[i]->Type() == target_children[j]->Type()) {
        // ... recurse and merge it.
        MergeElements(source_children[i], target_children[j]);
        break;
      }
    }
    // If we get here the target had no child of this type so we clone one.
    if (j == target_children.size()) {
      // This smashes anything in the same group.
      // LineString smashes Point in Placemark...
      target->AddElement(Clone(source_children[i]));
    }
  }

  // Merge in the immediately simple children of source to target.
  MergeFields(source, target);
}

}  // end namespace kmlengine
