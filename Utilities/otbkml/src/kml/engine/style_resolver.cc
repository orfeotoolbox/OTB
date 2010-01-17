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
#include "kml/engine/style_merger.h"
#include "kml/dom.h"
#include "kml/engine/href.h"
#include "kml/engine/kml_cache.h"
#include "kml/engine/kml_file.h"
#include "kml/engine/merge.h"

using kmldom::FeaturePtr;
using kmldom::StylePtr;

namespace kmlengine {

// This is the implementation of the public API function to compute the
// resolved style for a given Feature within a KML file.  See the header
// for the full description of usage.
StylePtr CreateResolvedStyle(const FeaturePtr& feature,
                             const KmlFilePtr& kml_file,
                             kmldom::StyleStateEnum style_state) {
  if (!feature) {
    return NULL;
  }
  StyleMerger style_merger(kml_file, style_state);
  style_merger.MergeStyle(feature->get_styleurl(),
                          feature->get_styleselector());
  return style_merger.GetResolvedStyle();
}

}  // endnamespace kmlengine
