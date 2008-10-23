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

// This file contains the declaration of the internal StyleMerger class.

#ifndef KML_ENGINE_STYLE_MERGER_H__
#define KML_ENGINE_STYLE_MERGER_H__

#include <string>
#include "kml/dom.h"
#include "kml/engine/kml_cache.h"
#include "kml/engine/kml_file.h"

namespace kmlengine {

// This class computes a resolved style for a Feature in a KML file.
// Usage is as follows:
//  // Create a KmlFileNetCache to fetch and parse a KML file:
//  KmlFileNetCache kml_file_net_cache(&your_net_fetcher, max_cache_items);
//  KmlFilePtr = kml_file_net_cache.Fetch(kml_url);
//  // (Parse into a KmlFile makes use of its GetSharedStyleById())
//  // Create a style merger instance for the given style state.
//  StyleMerger style_merger(kmlfile, STYLESTATE_NORMAL|STYLESTATE_HIGHLIGHT,
//                           &kml_file_net_cache);
//  // Merge in the Feature's styleUrl and StyleSelector (both may be empty).
//  style_merger.MergeStyle(styleurl, styleselector);
//  // MergeStyle() recurses down the styleurl as necessary.
//  // The caller retrieves the shared style:
//  StylePtr style = style_merger.ResolvedStyle()
//  // The style itself is non-null, but only those SubStyles with values
//  // found in the resolution process are set.
//  The CreateResolvedStyle() function is the preferred API to use in
//  application code.
class StyleMerger {
 public:
  StyleMerger(const KmlFilePtr& kml_file, kmldom::StyleStateEnum style_state);

  const kmldom::StylePtr& GetResolvedStyle() const {
    return resolved_style_;
  }

  // Both Feature and Pair have a styleUrl and/or StyleSelector.
  void MergeStyle(const std::string& styleurl,
                  const kmldom::StyleSelectorPtr& styleselector);

  // Merge in the StyleSelector this styleurl references.  Remote fetches are
  // performed through the KmlFileNetCache if one is supplied otherwise
  // remote fetches are quietly ignored.  An empty styleurl is quietly ignored.
  void MergeStyleUrl(const std::string& styleurl);

  // Merge in the given StyleMap's Pair's whose key's match the style_state_.
  void MergeStyleMap(const kmldom::StyleMapPtr& stylemap);

  // Merge in the given StyleSelector.
  void MergeStyleSelector(const kmldom::StyleSelectorPtr& styleselector);

 private:
  const KmlFilePtr kml_file_;
  const kmldom::StyleStateEnum style_state_;
  kmldom::StylePtr resolved_style_;
};

}  // endnamespace kmlengine

#endif  // KML_ENGINE_STYLE_MERGER_H__
