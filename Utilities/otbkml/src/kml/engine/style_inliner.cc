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

// This file contains the implementation of the internal StyleInliner.

#include "kml/engine/style_inliner.h"
#include "kml/engine/style_inliner_internal.h"
#include "kml/engine/id_mapper.h"
#include "kml/engine/kml_uri.h"
#include "kml/engine/style_resolver.h"

namespace kmlengine {

StyleInliner::StyleInliner()
  : in_update_(false) {
}

// ParserObserver::NewElement()
bool StyleInliner::NewElement(const kmldom::ElementPtr& element) {
  // Grab a pointer to what we will consider the root <Document>.
  if (!document_ && element->IsA(kmldom::Type_Document)) {
    document_ = AsDocument(element);
  }
  if (element->IsA(kmldom::Type_Update)) {
    in_update_ = true;
  }
  return true;  // Always continue parsing.
}

// TODO: create a kmldom::AsNonDocumentFeature()
// static
kmldom::FeaturePtr StyleInliner::AsNonDocumentFeature(
    const kmldom::ElementPtr& element) {
  if (kmldom::FeaturePtr feature = kmldom::AsFeature(element)) {
    return feature->IsA(kmldom::Type_Document) ? NULL : feature;
  }
  return NULL;
}

// ParserObserver::EndElement()
bool StyleInliner::EndElement(const kmldom::ElementPtr& parent,
                              const kmldom::ElementPtr& child) {
  if (in_update_) {
    return true;  // Leave everything under <Update> alone.
  }
  if (parent->IsA(kmldom::Type_Document)) {
    // Capture shared styles.
    if (kmldom::StyleSelectorPtr styleselector =
        kmldom::AsStyleSelector(child)) {
      if (styleselector->has_id()) {
        shared_styles_[styleselector->get_id()] = styleselector;
        return false;  // Do not add the shared style to the <Document>.
      }
    }
  }
  // Do a "mini-resolve" of each Feature's styleUrl.
  if (kmldom::FeaturePtr feature = AsNonDocumentFeature(parent)) {
    if (child->Type() == kmldom::Type_styleUrl) {
      // If it's a local reference and we've captured this shared style
      // give a copy of that to the Feature instead of this styleUrl.
      string fragment;
      string path;
      if (SplitUri(child->get_char_data(), NULL, NULL, NULL, &path, NULL,
                   &fragment) && path.empty()) {
        SharedStyleMap::const_iterator iter = shared_styles_.find(fragment);
        if (iter != shared_styles_.end()) {
          feature->set_styleselector(
              kmlengine::StyleResolver::CreateResolvedStyleSelector(
                  child->get_char_data(), shared_styles_));
          return false;  // Don't add the <styleUrl> to the feature.
        }
      }
      // We fall to here if the styleUrl is a remote reference or if the
      // styleUrl's local reference is not found.
    }
  }
  // Everything else.
  return true;
}

bool StyleInliner::AddChild(const kmldom::ElementPtr& parent,
                            const kmldom::ElementPtr& child) {
  if (child->IsA(kmldom::Type_Update)) {
    in_update_ = false;
  }
  return true;  // Always continue parsing.
}

kmldom::ElementPtr InlineStyles(const string& input_kml,
                               string* errors) {
  StyleInliner style_inliner;
  kmldom::Parser parser;
  parser.AddObserver(&style_inliner);
  return parser.Parse(input_kml, errors);
}

}  // end namespace kmlengine
