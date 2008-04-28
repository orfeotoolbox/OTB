/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


    Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
    See OTBCopyright.txt for details.

    Some parts of this code are derived from kml library examples. See KMLCopyright.txt
    for details.

    This software is distributed WITHOUT ANY WARRANTY; without even
    the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// this file defines the otbMultiScaleTest for the test driver
// and all it expects is that you have a function called RegisterTests
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif
#include <iostream>
#include "otbTestMain.h"

void RegisterTests()
{
REGISTER_TEST(kmlhellohref);
}


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
#include "kml/util/fileio.h"

using std::string;
using std::cout;
using std::endl;
using kmldom::BasicLink;
using kmldom::Container;
using kmldom::Document;
using kmldom::Element;
using kmldom::Feature;
using kmldom::Geometry;
using kmldom::IconStyle;
using kmldom::ItemIcon;
using kmldom::Kml;
using kmldom::ListStyle;
using kmldom::Model;
using kmldom::MultiGeometry;
using kmldom::NetworkLink;
using kmldom::Overlay;
using kmldom::Pair;
using kmldom::Placemark;
using kmldom::Style;
using kmldom::StyleMap;
using kmldom::StyleSelector;

#include "kml/util/kmlprint.h"

// Declare all functions used here.
static const Feature* GetRootFeature(const Element* root);
static void PrintBasicLinkHref(const string what, const BasicLink* link);
static void PrintIconStyleIconHref(const IconStyle* iconstyle);
static void PrintModelLinkHref(const Model* model);
static void PrintNetworkLinkHref(const NetworkLink* networklink);
static void PrintOverlayIconHref(const Overlay* overlay);
static void VisitContainer(const Container* container);
static void VisitFeature(const Feature* feature);
static void VisitFeatureStyle(const Feature* feature);
static void VisitGeometry(const Geometry* geometry);
static void VisitListStyle(const ListStyle* liststyle);
static void VisitPlacemark(const Placemark* placemark);
static void VisitStyle(const Style* style);
static void VisitStyleMap(const StyleMap* stylemap);
static void VisitStyleSelector(const StyleSelector* styleselector);

static const Feature* GetRootFeature(const Element* root) {
  if (root) {
    const Kml* kml = kmldom::AsKml(root);
    if (kml && kml->has_feature()) {
      return kml->feature();
    }
    return kmldom::AsFeature(root);
  }
  return NULL;
}

// Link, Icon, Url are all BasicLink
static void PrintBasicLinkHref(const string what, const BasicLink* link) {
  if (link) {
    G_kmlprint.flux << what << " " << link->href() << endl;
  }
}

static void PrintNetworkLinkHref(const NetworkLink* networklink) {
  PrintBasicLinkHref("NetworkLink", networklink->link());
}

static void PrintOverlayIconHref(const Overlay* overlay) {
  PrintBasicLinkHref("Overlay", overlay->icon());
}

static void PrintIconStyleIconHref(const IconStyle* iconstyle) {
  PrintBasicLinkHref("IconStyle", iconstyle->icon());
}

static void PrintModelLinkHref(const Model* model) {
  PrintBasicLinkHref("Model", model->link());
}

static void VisitGeometry(const Geometry* geometry) {
  const MultiGeometry* multigeometry = kmldom::AsMultiGeometry(geometry);
  if (multigeometry) {
    for (size_t i = 0; i < multigeometry->geometry_array_size(); ++i) {
      VisitGeometry(multigeometry->geometry_array_at(i));
    }
  } else if (const Model* model = kmldom::AsModel(geometry)) {
    PrintModelLinkHref(model);
  }
}

static void VisitPlacemark(const Placemark* placemark) {
  if (placemark->has_geometry()) {
    VisitGeometry(placemark->geometry());
  }
}

static void VisitListStyle(const ListStyle* liststyle) {
  for (size_t i = 0; i < liststyle->itemicon_array_size(); ++i) {
    // ItemIcon is not a BasicLink.
    if (liststyle->itemicon_array_at(i)->has_href()) {
      G_kmlprint.flux << "ItemIcon " << liststyle->itemicon_array_at(i)->href() << endl;
    }
  }
}

static void VisitStyle(const Style* style) {
  if (style->has_iconstyle()) {
    PrintIconStyleIconHref(style->iconstyle());
  }
  if (style->has_liststyle()) {
    VisitListStyle(style->liststyle());
  }
}

static void VisitStyleSelector(const StyleSelector* styleselector) {
  if (const Style* style = kmldom::AsStyle(styleselector)) {  // for IconStyle
    VisitStyle(style);
  } else if (const StyleMap* stylemap = kmldom::AsStyleMap(styleselector)) {
    VisitStyleMap(stylemap);
  }
}

static void VisitStyleMap(const StyleMap* stylemap) {
  for (int i = 0; i < stylemap->pair_array_size(); ++i) {
    if (stylemap->pair_array_at(i)->has_styleselector()) {
      VisitStyleSelector(stylemap->pair_array_at(i)->styleselector());
    }
  }
}

static void VisitFeatureStyle(const Feature* feature) {
  if (feature->has_styleselector()) {
    VisitStyleSelector(feature->styleselector());
  }
  // visit list if Document
  if (const Document* document = kmldom::AsDocument(feature)) {
    for (size_t i = 0; i < document->styleselector_array_size(); ++i) {
      VisitStyleSelector(document->styleselector_array_at(i));
    }
  }
}

static void VisitContainer(const Container* container) {
  for (int i = 0; i < container->feature_array_size(); ++i) {
    VisitFeature(container->feature_array_at(i));
  }
}

static void VisitFeature(const Feature* feature) {
  VisitFeatureStyle(feature);
  if (const Container* container = kmldom::AsContainer(feature)) {
    VisitContainer(container);
  } else if (const NetworkLink* networklink = kmldom::AsNetworkLink(feature)) {
    PrintNetworkLinkHref(networklink);
  } else if (const Overlay* overlay = kmldom::AsOverlay(feature)) {
    PrintOverlayIconHref(overlay);
  } else if (const Placemark* placemark = kmldom::AsPlacemark(feature)) {
    VisitPlacemark(placemark);  // Model
  }
}

static void HandleFile(const char* kmlfile) {
  G_kmlprint.flux << kmlfile << endl;
  string kml;
  bool status = ReadFileToString(kmlfile, &kml);
  if (!status) {
    return;
  }
  string errors;
  Element* root = kmldom::Parse(kml, &errors);
  if (!root) {
    return;
  }
  if (const Feature* feature = GetRootFeature(root)) {
    VisitFeature(feature);
  }
  delete root;  // deletes everything including feature
}

int kmlhellohref(int argc, char* argv[]) {
  if (argc != 3) {
    cout << "usage: " << argv[0] << " file.kml outfile.txt" << endl;
    return EXIT_FAILURE;
  }
  const char* kmlfile = argv[1];
G_kmlprint.flux.open(argv[2]);
  HandleFile(kmlfile);
G_kmlprint.flux.close();
  return EXIT_SUCCESS;
}
