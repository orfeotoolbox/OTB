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
#include <string>
#include "kml/dom/element.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/serializer.h"

namespace kmldom {
class Attributes;
}

using kmldom::Attributes;
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
  virtual ~ElementReplicator() {}

  // Serializer::BeginById() is called at the start of a complex element.
  virtual void BeginById(int type_id, const Attributes& attributes) {
    KmlDomType id = static_cast<KmlDomType>(type_id);
    ElementPtr clone = KmlFactory::GetFactory()->CreateElementById(id);
    clone->ParseAttributes(attributes);
    clone_stack_.push(clone);
  }

  // Serializer::End() is called at the end of a complex element.
  virtual void End() {
    // BeginById() always puts something on the stack so this is always safe.
    ElementPtr child = clone_stack_.top();
    // This mimics the behavior of KmlHandler::EndElement().
    if (!char_data_.empty()) {
      child->set_char_data(char_data_);
      char_data_.clear();
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
  virtual void SaveStringFieldById(int type_id, std::string value) {
    KmlDomType id = static_cast<KmlDomType>(type_id);
    ElementPtr clone = KmlFactory::GetFactory()->CreateFieldById(id);
    clone->set_char_data(value);
    clone_stack_.top()->AddElement(clone);
  }

  // Serializer::SaveContent() is called for arbitrary character data.
  virtual void SaveContent(const std::string& content, bool maybe_quote) {
    char_data_.append(content);
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
  std::string char_data_;
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
