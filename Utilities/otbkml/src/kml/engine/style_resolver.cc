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

// This file contains the implementation of the CreateResolvedStyle() function.

#include "kml/engine/style_resolver.h"
#include "kml/dom.h"
#include "kml/engine/id_mapper.h"
#include "kml/engine/kml_file.h"
#include "kml/engine/style_merger.h"

using kmldom::FeaturePtr;
using kmldom::KmlFactory;
using kmldom::PairPtr;
using kmldom::StylePtr;
using kmldom::StyleMapPtr;
using kmldom::StyleSelectorPtr;

namespace kmlengine {

// This is the implementation of the public API function to compute the
// resolved style for a given Feature within a KML file.  See the header
// for the full description of usage.
StylePtr CreateResolvedStyle(const FeaturePtr& feature,
                             const KmlFilePtr& kml_file,
                             kmldom::StyleStateEnum style_state) {
  return StyleResolver::CreateResolvedStyle(feature->get_styleurl(),
                                            feature->get_styleselector(),
                                            kml_file->get_shared_style_map(),
                                            kml_file->get_url(),
                                            kml_file->get_kml_cache(),
                                            style_state);
}

// static
StylePtr StyleResolver::CreateResolvedStyle(
    const string& styleurl,
    const StyleSelectorPtr& styleselector,
    const SharedStyleMap& shared_style_map,
    const string& base_url,
    KmlCache* kml_cache,
    kmldom::StyleStateEnum style_state) {
  StyleMerger style_merger(shared_style_map, kml_cache, base_url, style_state);
  style_merger.MergeStyle(styleurl, styleselector);
  // This always returns the thus-far resolved <Style> even if the nesting
  // level was reached.
  return style_merger.GetResolvedStyle();
}

// static
kmldom::StyleSelectorPtr StyleResolver::CreateResolvedStyleSelector(
    const string& styleurl, const SharedStyleMap& shared_style_map) {
  const string empty;
  StyleMapPtr stylemap = KmlFactory::GetFactory()->CreateStyleMap();
  PairPtr normal = KmlFactory::GetFactory()->CreatePair();
  normal->set_key(kmldom::STYLESTATE_NORMAL);
  StylePtr style = CreateResolvedStyle(styleurl, NULL, shared_style_map, empty,
                                       NULL, kmldom::STYLESTATE_NORMAL);
  normal->set_styleselector(AsStyleSelector(ClearIds(style)));
  stylemap->add_pair(normal);
  PairPtr highlight = KmlFactory::GetFactory()->CreatePair();
  highlight->set_key(kmldom::STYLESTATE_HIGHLIGHT);
  style = CreateResolvedStyle(styleurl, NULL, shared_style_map, empty,
                              NULL, kmldom::STYLESTATE_HIGHLIGHT);
  highlight->set_styleselector(AsStyleSelector(ClearIds(style)));
  stylemap->add_pair(highlight);
  return stylemap;
}

}  // endnamespace kmlengine
