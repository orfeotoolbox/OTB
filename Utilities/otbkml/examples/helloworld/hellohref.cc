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

// This program prints the hrefs in the supplied KML file.
// The following elements have hrefs:
//   NetworkLink/Link or Url
//   Overlay/Icon
//   Model/Link
//   IconStyle/Icon
//   ItemIcon/href
// These are also "hrefs", but are not printed by this program:
//   styleUrl, schemaUrl, targetHref, sourceHref

#include <iostream>
#include <string>
#include "kml/dom.h"
#include "kml/base/file.h"

using std::string;
using std::cout;
using std::endl;
using kmldom::BasicLinkPtr;
using kmldom::ContainerPtr;
using kmldom::DocumentPtr;
using kmldom::ElementPtr;
using kmldom::FeaturePtr;
using kmldom::GeometryPtr;
using kmldom::IconStylePtr;
using kmldom::ItemIconPtr;
using kmldom::KmlPtr;
using kmldom::ListStylePtr;
using kmldom::ModelPtr;
using kmldom::MultiGeometryPtr;
using kmldom::NetworkLinkPtr;
using kmldom::OverlayPtr;
using kmldom::PairPtr;
using kmldom::PlacemarkPtr;
using kmldom::StylePtr;
using kmldom::StyleMapPtr;
using kmldom::StyleSelectorPtr;

// Declare all functions used here.
static const FeaturePtr GetRootFeature(const ElementPtr& root);
static void PrintBasicLinkHref(const string what, const BasicLinkPtr& link);
static void PrintIconStyleIconHref(const IconStylePtr& iconstyle);
static void PrintModelLinkHref(const ModelPtr& model);
static void PrintNetworkLinkHref(const NetworkLinkPtr& networklink);
static void PrintOverlayIconHref(const OverlayPtr& overlay);
static void VisitContainer(const ContainerPtr& container);
static void VisitFeature(const FeaturePtr& feature);
static void VisitFeatureStyle(const FeaturePtr& feature);
static void VisitGeometry(const GeometryPtr& geometry);
static void VisitListStyle(const ListStylePtr& liststyle);
static void VisitPlacemark(const PlacemarkPtr& placemark);
static void VisitStyle(const StylePtr& style);
static void VisitStyleMap(const StyleMapPtr& stylemap);
static void VisitStyleSelector(const StyleSelectorPtr& styleselector);

static const FeaturePtr GetRootFeature(const ElementPtr& root) {
  const KmlPtr kml = kmldom::AsKml(root);
  if (kml && kml->has_feature()) {
    return kml->get_feature();
  }
  return kmldom::AsFeature(root);
}

// Link, Icon, Url are all BasicLink
static void PrintBasicLinkHref(const string what, const BasicLinkPtr& link) {
  if (link) {
    cout << what << " " << link->get_href() << endl;
  }
}

static void PrintNetworkLinkHref(const NetworkLinkPtr& networklink) {
  PrintBasicLinkHref("NetworkLink", networklink->get_link());
}

static void PrintOverlayIconHref(const OverlayPtr& overlay) {
  PrintBasicLinkHref("Overlay", overlay->get_icon());
}

static void PrintIconStyleIconHref(const IconStylePtr& iconstyle) {
  PrintBasicLinkHref("IconStyle", iconstyle->get_icon());
}

static void PrintModelLinkHref(const ModelPtr& model) {
  PrintBasicLinkHref("Model", model->get_link());
}

static void VisitGeometry(const GeometryPtr& geometry) {
  const MultiGeometryPtr multigeometry = kmldom::AsMultiGeometry(geometry);
  if (multigeometry) {
    for (size_t i = 0; i < multigeometry->get_geometry_array_size(); ++i) {
      VisitGeometry(multigeometry->get_geometry_array_at(i));
    }
  } else if (const ModelPtr model = kmldom::AsModel(geometry)) {
    PrintModelLinkHref(model);
  }
}

static void VisitPlacemark(const PlacemarkPtr& placemark) {
  if (placemark->has_geometry()) {
    VisitGeometry(placemark->get_geometry());
  }
}

static void VisitListStyle(const ListStylePtr& liststyle) {
  for (size_t i = 0; i < liststyle->get_itemicon_array_size(); ++i) {
    // ItemIcon is not a BasicLink.
    if (liststyle->get_itemicon_array_at(i)->has_href()) {
      cout << "ItemIcon " << liststyle->get_itemicon_array_at(i)->get_href()
           << endl;
    }
  }
}

static void VisitStyle(const StylePtr& style) {
  if (style->has_iconstyle()) {
    PrintIconStyleIconHref(style->get_iconstyle());
  }
  if (style->has_liststyle()) {
    VisitListStyle(style->get_liststyle());
  }
}

static void VisitStyleSelector(const StyleSelectorPtr& styleselector) {
  if (const StylePtr style = kmldom::AsStyle(styleselector)) {  // for IconStyle
    VisitStyle(style);
  } else if (const StyleMapPtr stylemap = kmldom::AsStyleMap(styleselector)) {
    VisitStyleMap(stylemap);
  }
}

static void VisitStyleMap(const StyleMapPtr& stylemap) {
  for (size_t i = 0; i < stylemap->get_pair_array_size(); ++i) {
    if (stylemap->get_pair_array_at(i)->has_styleselector()) {
      VisitStyleSelector(stylemap->get_pair_array_at(i)->get_styleselector());
    }
  }
}

static void VisitFeatureStyle(const FeaturePtr& feature) {
  if (feature->has_styleselector()) {
    VisitStyleSelector(feature->get_styleselector());
  }
  // visit list if Document
  if (const DocumentPtr document = kmldom::AsDocument(feature)) {
    for (size_t i = 0; i < document->get_styleselector_array_size(); ++i) {
      VisitStyleSelector(document->get_styleselector_array_at(i));
    }
  }
}

static void VisitContainer(const ContainerPtr& container) {
  for (size_t i = 0; i < container->get_feature_array_size(); ++i) {
    VisitFeature(container->get_feature_array_at(i));
  }
}

static void VisitFeature(const FeaturePtr& feature) {
  VisitFeatureStyle(feature);
  if (const ContainerPtr container = kmldom::AsContainer(feature)) {
    VisitContainer(container);
  } else if (const NetworkLinkPtr networklink =
             kmldom::AsNetworkLink(feature)) {
    PrintNetworkLinkHref(networklink);
  } else if (const OverlayPtr overlay = kmldom::AsOverlay(feature)) {
    PrintOverlayIconHref(overlay);
  } else if (const PlacemarkPtr placemark = kmldom::AsPlacemark(feature)) {
    VisitPlacemark(placemark);  // Model
  }
}

static void HandleFile(const char* kmlfile) {
  cout << kmlfile << endl;
  string kml;
  bool status = kmlbase::File::ReadFileToString(kmlfile, &kml);
  if (!status) {
    return;
  }
  string errors;
  ElementPtr root = kmldom::Parse(kml, &errors);
  if (!root) {
    return;
  }
  if (const FeaturePtr feature = GetRootFeature(root)) {
    VisitFeature(feature);
  }
}

int main(int argc, char** argv) {
  if (argc != 2) {
    cout << "usage: " << argv[0] << " file.kml" << endl;
    return 1;
  }
  const char* kmlfile = argv[1];
  HandleFile(kmlfile);
}
