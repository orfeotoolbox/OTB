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

// This sample program creates shared style selectors.

#include <iostream>
#include "kml/dom.h"
#include "kml/convenience/convenience.h"

using kmldom::DocumentPtr;
using kmldom::IconStylePtr;
using kmldom::KmlFactory;
using kmldom::KmlPtr;
using kmldom::PairPtr;
using kmldom::PlacemarkPtr;
using kmldom::StylePtr;
using kmldom::StyleMapPtr;

int kmlsharedstyles(int argc, char* argv[]) {

  if(argc > 0)
    std::cerr << "Running " << argv[0] << std::endl;

  KmlFactory* kml_factory = KmlFactory::GetFactory();

  DocumentPtr document = kml_factory->CreateDocument();

  StylePtr normal = kml_factory->CreateStyle();
  normal->set_id("normal");
  IconStylePtr iconstyle = kml_factory->CreateIconStyle();
  iconstyle->set_scale(1.1);
  normal->set_iconstyle(iconstyle);
  document->add_styleselector(normal);

  StylePtr highlight = kml_factory->CreateStyle();
  highlight->set_id("highlight");
  iconstyle = kml_factory->CreateIconStyle();
  iconstyle->set_scale(2.3);
  highlight->set_iconstyle(iconstyle);
  document->add_styleselector(highlight);

  StyleMapPtr stylemap = kml_factory->CreateStyleMap();
  stylemap->set_id("stylemap");
  PairPtr pair = kml_factory->CreatePair();
  pair->set_key(kmldom::STYLESTATE_NORMAL);
  pair->set_styleurl("#normal");
  stylemap->add_pair(pair);

  pair = kml_factory->CreatePair();
  pair->set_key(kmldom::STYLESTATE_HIGHLIGHT);
  pair->set_styleurl("#highlight");
  stylemap->add_pair(pair);

  document->add_styleselector(stylemap);

  PlacemarkPtr placemark =
      kmlconvenience::CreatePointPlacemark("Roll", 32.751645, -113.987817);
  placemark->set_styleurl("#stylemap");

  document->add_feature(placemark);

  KmlPtr kml = kml_factory->CreateKml();
  kml->set_feature(document);

  std::cout << kmldom::SerializePretty(kml);

  return 0;
}
