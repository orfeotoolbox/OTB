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

// This file contains the implementation of the GetElementsById() function.

#include "kml/engine/find.h"
#include "kml/dom/serializer.h"

using kmldom::ElementPtr;
using kmldom::KmlDomType;
using kmldom::Serializer;

namespace kmlengine {

// The ElementFinder adds every complex element of the given type to the
// given vector.
class ElementTypeFinder : public Serializer {
 public:
  ElementTypeFinder(KmlDomType type_id, ElementVector* element_vector)
    : type_id_(type_id), element_vector_(element_vector) {
  }

  // This is the only Serializer virtual method implemented
  // in ElementFinder.  As such only complex elements are found.
  virtual void SaveElement(const ElementPtr& element) {
    // If this element is of the desired type save a pointer.
    if (element->IsA(type_id_)) {
      element_vector_->push_back(element);
    }
    // Call Serializer to recurse.
    Serializer::SaveElement(element);
  }

 private:
  const KmlDomType type_id_;
  ElementVector* element_vector_;
};

class AllElementFinder : public Serializer {
 public:
  AllElementFinder(bool recurse, ElementVector* element_vector)
    : recurse_(recurse), count_(0), element_vector_(element_vector) {
  }

  virtual void SaveElement(const ElementPtr& element) {
    ++count_;
    if (element_vector_) {
      element_vector_->push_back(element);
    }
    if (recurse_) {
      Serializer::SaveElement(element);
    }
  }

  int get_count() const {
    return count_;
  }

 private:
  bool recurse_;
  int count_;
  ElementVector* element_vector_;
};

// Append all elements of the given type id in the hierarchy
// root at element.
void GetElementsById(const ElementPtr& element, KmlDomType type_id,
                     ElementVector* element_vector) {
  if (!element || !element_vector) {
    return;
  }
  // The ElementTypeFinder derivation of Serializer does all the work.
  ElementTypeFinder element_type_finder(type_id, element_vector);
  element->Serialize(element_type_finder);
}

int GetChildElements(const ElementPtr& element, bool recurse,
                     ElementVector* element_vector) {
  if (!element) {
    return 0;
  }
  AllElementFinder all_element_finder(recurse, element_vector);
  element->Serialize(all_element_finder);
  return all_element_finder.get_count();
}

}  // end namespace kmlengine
