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

// This file contains the implementation of the Clone() function.

#include "kml/engine/clone.h"
#include <stack>
#include "kml/dom/element.h"
#include "kml/dom/geometry.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/serializer.h"

namespace kmlbase {
class Attributes;
}

using kmlbase::Attributes;
using kmldom::CoordinatesPtr;
using kmldom::ElementPtr;
using kmldom::KmlFactory;
using kmldom::KmlDomType;

namespace kmlengine {

// The ElementReplicator is a Serializer used by the Clone() function to walk
// the entire hierarchy of the target element.  Each field and child element
// of each element is "serialized" to a new parallel instance using the same
// methods used in the parser.  This technique essentially mates the output
// of the serializer to the input of the parser.
class ElementReplicator : public kmldom::Serializer {
 public:
  ElementReplicator()
    : serializing_unknown_(false) {
  }

  virtual ~ElementReplicator() {}

  // Serializer::BeginById() is called at the start of a complex element.
  virtual void BeginById(int type_id, const Attributes& attributes) {
    KmlDomType id = static_cast<KmlDomType>(type_id);
    ElementPtr clone = KmlFactory::GetFactory()->CreateElementById(id);
    clone->ParseAttributes(attributes.Clone());
    clone_stack_.push(clone);
  }

  // Serializer::End() is called at the end of a complex element.
  virtual void End() {
    // BeginById() always puts something on the stack so this is always safe.
    ElementPtr child = clone_stack_.top();
    // This mimics the part of KmlHandler::EndElement() which special cases
    // those complex elements which have character data.
    // TODO: refactor the special-casing in KmlHandler::EndElement() such that
    // it can be used from here.  Using child->AddElement(child) is dangerous.
    if (!char_data_.empty()) {
      child->set_char_data(char_data_);
      char_data_.clear();
      // NOTE: This very much expects this to mean "parse yourself".  If
      // this falls through to Element::AddChild() this will make the element
      // a child of itself by putting itself in the misplaced elements array.
      // TODO: see above TODO
      child->AddElement(child);
    }
    // Two or more items on the stack implies the top is a child to be added
    // to the parent above it on the stack.
    if (clone_stack_.size() > 1) {
      // Pop off the child.
      clone_stack_.pop();
      // Parent is now the top item.
      clone_stack_.top()->AddElement(child);
    }
  }

  // Serializer::SaveStringFieldById() is called for each field.
  virtual void SaveStringFieldById(int type_id, string value) {
    KmlDomType id = static_cast<KmlDomType>(type_id);
    ElementPtr clone = KmlFactory::GetFactory()->CreateFieldById(id);
    clone->set_char_data(value);
    clone_stack_.top()->AddElement(clone);
  }

  // Detects if we're serializing the unknown elements array.  We don't use
  // element_count here and instead rely on EndElementArray() to indicate the
  // end of the array.  Begin/End are always paired and never nested.
  virtual void BeginElementArray(int type_id, size_t element_count) {
    if (type_id == kmldom::Type_Unknown) {
      serializing_unknown_ = true;
    }
  }

  // This is called after the last element of the given array.  This brackets
  // the BeginElementArray() above.
  virtual void EndElementArray(int type_id) {
    if (type_id == kmldom::Type_Unknown) {
      serializing_unknown_ = false;
    }
  }

  // Serializer::SaveContent() is called for arbitrary character data.
  virtual void SaveContent(const string& content, bool maybe_quote) {
    // If this is an item in the unknown elements array do _not_ add it to
    // this element's raw char_data, add it correctly to the unknown element
    // array directly.
    if (serializing_unknown_) {
      if (clone_stack_.size() > 0) {
        clone_stack_.top()->AddUnknownElement(content);
      }
    } else {
      char_data_.append(content);
    }
  }

  // Serializer::SaveVec3() is called to save each <coordinates> tuple.
  virtual void SaveVec3(const kmlbase::Vec3& vec3) {
    if (CoordinatesPtr coordinates = AsCoordinates(clone_stack_.top())) {
      coordinates->add_vec3(vec3);
    }  // else something is very wrong.
  }

  // Serializer::SaveColor() is called to save all Color32 values.
  virtual void SaveColor(int type_id, const kmlbase::Color32& color) {
    SaveFieldById(type_id, color.to_string_abgr());
  }

  // Return the top of the stack which holds the root element.
  ElementPtr root() {
    if (clone_stack_.empty()) {
      return NULL;
    }
    return clone_stack_.top();
  }

 private:
  // This stack operates akin to the stack in the parser.
  std::stack<ElementPtr> clone_stack_;
  string char_data_;
  // This flag indicates that we're serializing an array of unknown elements.
  // See BeginElementArray(), EndElementArray(), and SaveContent() above.
  bool serializing_unknown_;
};

// Clone operates by "Serializing" the given element.  The ElementReplicator
// operates akin to the parser in that it maintains a stack of complex
// elements created from the factory and sets fields and child elements
// to the parent as they are completed.
ElementPtr Clone(const ElementPtr& element) {
  if (!element) {
    return NULL;
  }
  ElementReplicator serializer;
  element->Serialize(serializer);
  return serializer.root();
}

}  // end namespace kmlengine
