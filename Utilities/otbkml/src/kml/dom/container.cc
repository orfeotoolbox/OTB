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

// This file contains the implementation of the abstract Container element.

#include "kml/dom/container.h"
#include "kml/dom/feature.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_ptr.h"
#include "kml/dom/serializer.h"

namespace kmldom {

Container::Container() {}

Container::~Container() {
  // feature_array_'s destructor calls the destructor of each FeaturePtr
  // releasing the reference and potentially freeing the Feature storage.
}

void Container::add_feature(const FeaturePtr& feature) {
  AddComplexChild(feature, &feature_array_);
}

void Container::AddElement(const ElementPtr& element) {
  if (FeaturePtr feature = AsFeature(element)) {
    add_feature(feature);
  } else {
    Feature::AddElement(element);
  }
}

void Container::Serialize(Serializer& serializer) const {
  Feature::Serialize(serializer);
  for (size_t i = 0; i < get_feature_array_size(); ++i) {
    serializer.SaveElement(get_feature_array_at(i));
  }
}

}  // end namespace kmldom
