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

// This file contains the declaration of the CreateResolvedStyle() function.

#ifndef KML_ENGINE_STYLE_RESOLVER_H__
#define KML_ENGINE_STYLE_RESOLVER_H__

#include "kml/dom.h"
#include "kml/engine/kml_cache.h"
#include "kml/engine/kml_file.h"

namespace kmlengine {

// This creates a new <Style> representing the merge of the inline and/or
// shared StyleSelector(s) of the given Feature within the given KmlFile for
// the given style state (<key>).  All SubStyle simple and complex children
// set in inline/shared StyleSelectors are also set in the created Style.
kmldom::StylePtr CreateResolvedStyle(const kmldom::FeaturePtr& feature,
                                     const KmlFilePtr& kml_file,
                                     kmldom::StyleStateEnum style_state);

}  // end namespace kmlengine

#endif  // KML_ENGINE_STYLE_RESOLVER_H__
