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

// TODO: file comment.

#include "kml/engine/feature_balloon.h"
#include "kml/engine/style_resolver.h"
#include "kml/base/string_util.h"
#include "kml/engine/engine_types.h"
#include "kml/engine/entity_mapper.h"
#include "kml/engine/style_merger.h"

using kmlbase::StringMap;
using kmldom::BalloonStylePtr;
using kmldom::FeaturePtr;
using kmldom::StylePtr;

namespace kmlengine {

string CreateBalloonText(const KmlFilePtr& kml_file,
                              const FeaturePtr& feature) {
  // First we merge the feature's styles so we have access to the BalloonStyle.
  // TODO: clarify if a highl;ight state can be used to define the balloon
  // style.
  StylePtr style = CreateResolvedStyle(feature, kml_file,
                                       kmldom::STYLESTATE_NORMAL);

  // Create a vector of strings and fill it with the Feature's entities and
  // their replacements.
  StringMap entity_map;
  EntityMapper entity_mapper(kml_file, &entity_map);
  entity_mapper.GetEntityFields(feature);

  // If we have BalloonStyle/text, expand its entities and return the result.
  if (style->has_balloonstyle()) {
    BalloonStylePtr bs = style->get_balloonstyle();
    if (bs->has_text()) {
      return CreateExpandedEntities(bs->get_text(), entity_map);
    }
  }

  // Otherwise, build a default description balloon for the feature.
  string text;

  // The feature's name, if given, appears at the top of the balloon as an
  // <h3> header with two consecutive line breaks.
  if (feature->has_name()) {
    text += "<h3>" + feature->get_name() + "</h3><br/><br/>";
  }

  // If we have a description, append it.
  if (feature->has_description()) {
    text += CreateExpandedEntities(feature->get_description(), entity_map);
  }

  // TODO: $[geDirections].

  // TODO:
  // If we have neither a BalloonStyle/text nor a description, we try to
  // build the balloon from any Data and/or SchemaData elements.

  return text;
}

}  // end namespace kmlengine
