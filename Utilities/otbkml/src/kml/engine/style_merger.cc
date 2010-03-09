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

// This file contains the implementation of the internal StyleMerger class
// used in style resolution.

#include "kml/engine/style_merger.h"
#include "kml/dom.h"
#include "kml/engine/engine_constants.h"
#include "kml/engine/kml_cache.h"
#include "kml/engine/kml_file.h"
#include "kml/engine/kml_uri.h"
#include "kml/engine/merge.h"

using kmldom::FeaturePtr;
using kmldom::KmlFactory;
using kmldom::PairPtr;
using kmldom::StylePtr;
using kmldom::StyleMapPtr;
using kmldom::StyleSelectorPtr;
using kmldom::StyleStateEnum;

namespace kmlengine {

// TODO: verify unsigned int to int init of nesting_depth_ ok on MSVC
StyleMerger::StyleMerger(const SharedStyleMap& shared_style_map,
                         KmlCache* kml_cache,
                         const string& base_url,
                         StyleStateEnum style_state)
    : shared_style_map_(shared_style_map),
      kml_cache_(kml_cache),
      base_url_(base_url),
      style_state_(style_state),
      resolved_style_(KmlFactory::GetFactory()->CreateStyle()),
      nesting_depth_(kDefaultMaxNestedStyleUrls) {
}

// TODO: verify unsigned int to int init of nesting_depth_ ok on MSVC
StyleMerger::StyleMerger(const SharedStyleMap& shared_style_map,
                         KmlCache* kml_cache,
                         const string& base_url,
                         StyleStateEnum style_state,
                         unsigned int nesting_depth)
    : shared_style_map_(shared_style_map),
      kml_cache_(kml_cache),
      base_url_(base_url),
      style_state_(style_state),
      resolved_style_(KmlFactory::GetFactory()->CreateStyle()),
      nesting_depth_(nesting_depth) {
}

// static
StyleMerger* StyleMerger::CreateFromKmlFile(
    const KmlFile& kml_file, kmldom::StyleStateEnum style_state) {
  return new StyleMerger(kml_file.get_shared_style_map(),
                         kml_file.get_kml_cache(),
                         kml_file.get_url(),
                         style_state);
}

void StyleMerger::MergeStyleUrl(const string& styleurl) {
  if (--nesting_depth_ < 0) {
    return;
  }
  string path;
  string style_id;  // fragment
  if (styleurl.empty() ||
      !SplitUri(styleurl, NULL, NULL, NULL, &path, NULL, &style_id) ||
      style_id.empty()) {
    return;  // Empty or bad styleurl or w/o fragment: ignore.
  }
  // If there's no path this is a StyleSelector within this file.
  if (path.empty()) {
    SharedStyleMap::const_iterator found = shared_style_map_.find(style_id);
    if (found != shared_style_map_.end()) {
      MergeStyleSelector(found->second);
    }
    return;
  }

  // No KmlCache provided for this KmlFile? Just return.
  if (!kml_cache_) {
    return;
  }

  // This fetches the given style KML from/into the KmlCache.
  // Note that KmlCache::FetchKml() understands any KML URL including those to
  // and into a KMZ (style.kmz#styld_id, style.kmz/style.kml#style_id).
  const KmlFilePtr kml_file = kml_cache_->FetchKmlRelative(base_url_, styleurl);
  if (!kml_file) {
    return;  // Fetch (and parse) failures are quietly ignored.
  }
  // Find the StyleSelector within the KmlFile.
  StyleSelectorPtr styleselector = kml_file->GetSharedStyleById(style_id);
  if (!styleselector) {
    return;  // No shared style by this id in this KML file: ignore.
  }
  // TODO: resolve relative hrefs in substyles of this styleselector w.r.t.
  // fetchable_url
  // Merge in this StyleSelector.
  MergeStyleSelector(styleselector);
}

// Both Feature and Pair have a styleUrl and/or StyleSelector.
void StyleMerger::MergeStyle(const string& styleurl,
                             const StyleSelectorPtr& styleselector) {
  // If there's a styleUrl to a shared style merge that in first.
  MergeStyleUrl(styleurl);

  // If there's an inline style that takes priority so merge that over.
  MergeStyleSelector(styleselector);
}

// Merge in the given StyleMap's Pair's whose key's match the style_state_.
void StyleMerger::MergeStyleMap(const StyleMapPtr& stylemap) {
  if (!stylemap) {
    return;
  }
  // TODO: KML standard language on multiple Pair of the same key.  This
  // implementation is a fine-grained "last one wins".
  for (size_t i = 0; i < stylemap->get_pair_array_size(); ++i) {
    // Lack of <key> returns <key>'s default.
    if (style_state_ == stylemap->get_pair_array_at(i)->get_key()) {
      PairPtr pair = stylemap->get_pair_array_at(i);
      // Recurse down this Pair's styleUrl and/or StyleSelector.
      MergeStyle(pair->get_styleurl(), pair->get_styleselector());
    }
  }
}

// Merge in the given StyleSelector.
void StyleMerger::MergeStyleSelector(const StyleSelectorPtr& styleselector) {
  // NULL ok with AsXXX()
  if (StylePtr style = AsStyle(styleselector)) {
    // All StyleMerger methods ultimately reduce to this call out to the
    // general purpose element merger.
    MergeElements(style, resolved_style_);
  } else if (StyleMapPtr stylemap = AsStyleMap(styleselector)) {
    MergeStyleMap(stylemap);
  }
}

}  // endnamespace kmlengine
