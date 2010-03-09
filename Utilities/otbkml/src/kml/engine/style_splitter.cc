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

// This file contains the implementation of the StyleSplitter class and the
// SplitStyles function.

#include "kml/engine/style_splitter.h"
#include "kml/engine/style_splitter_internal.h"
#include <map>
#include "kml/base/string_util.h"
#include "kml/dom.h"
#include "kml/dom/parser_observer.h"
#include "kml/engine/engine_types.h"
#include "kml/engine/merge.h"

namespace kmlengine {

bool StyleSplitter::NewElement(const kmldom::ElementPtr& element) {
  // Use the first <Document> found as the home of all shared styles.
  if (!document_ && element->IsA(kmldom::Type_Document)) {
    document_ = AsDocument(element);
  }
  // Don't touch anything inside an <Update>.
  if (element->IsA(kmldom::Type_Update)) {
    in_update_ = true;
  }
  return true;
}

// static
kmldom::FeaturePtr StyleSplitter::AsNonDocumentFeature(
    const kmldom::ElementPtr& element) {
  if (kmldom::FeaturePtr feature = kmldom::AsFeature(element)) {
    return feature->IsA(kmldom::Type_Document) ? NULL : feature;
  }
  return NULL;
}

// static
kmldom::StyleSelectorPtr StyleSplitter::CreateStyleSelector(
    kmldom::KmlDomType type_id) {
  if (type_id == kmldom::Type_Style) {
    return kmldom::KmlFactory::GetFactory()->CreateStyle();
  } else if (type_id == kmldom::Type_StyleMap) {
    return kmldom::KmlFactory::GetFactory()->CreateStyleMap();
  } else {
    return NULL;
  }
}

bool StyleSplitter::EndElement(const kmldom::ElementPtr& parent,
                        const kmldom::ElementPtr& child) {
  // If we're not in <Update> and the child is a StyleSelector...
  if (!in_update_ && document_ && child->IsA(kmldom::Type_StyleSelector)) {
    // ...and the parent is a Feature, but not a <Document>...
    if (kmldom::FeaturePtr feature = AsNonDocumentFeature(parent)) {
      // ...and the feature does _not_ have a <styleUrl>...
      if (!feature->has_styleurl()) {
        const string style_id(
            CreateUniqueId(*shared_style_map_, id_counter_));
        // ...and this id does not collide:
        if (shared_style_map_->find(style_id) == shared_style_map_->end()) {
          ++id_counter_;  // Bump the id counter only if it was used.
          // Create an empty StyleSelector of the child's type and set the id.
          kmldom::StyleSelectorPtr shared =
              CreateStyleSelector(child->Type());
          shared->set_id(style_id);
          // Merge the children from child into the new StyleSelector.
          kmlengine::MergeElements(child, shared);
          // Add the StyleSelector to the Document and the shared style map.
          document_->add_styleselector(shared);
          (*shared_style_map_)[shared->get_id()] = shared;
          // Set the feature's <styleUrl> to this new shared style.
          feature->set_styleurl(string("#") + style_id);
          return false;  // Do _not_ add this child to this parent.
        }
      }
    }
  }
  // Falling through any of the conditions above means no action is taken
  // to interfere with this child being added to this parent.
  // Parsing of <Update> is complete so disable guard.
  if (child->IsA(kmldom::Type_Update)) {
    in_update_ = false;
  }
  return true;  // Proceed to add this child to this parent.
}

kmldom::ElementPtr SplitStyles(const string& input_kml,
                               string* errors) {
  SharedStyleMap shared_style_map;
  StyleSplitter style_splitter(&shared_style_map);
  kmldom::Parser parser;
  parser.AddObserver(&style_splitter);
  return parser.Parse(input_kml, errors);
}

}  // end namespace kmlengine
