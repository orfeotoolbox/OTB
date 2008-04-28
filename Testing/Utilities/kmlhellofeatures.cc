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
REGISTER_TEST(kmlhellofeatures);
}


// Walks the container hierarchy of the KML file printing about each Feature.

#include <iostream>
#include <string>
#include "kml/dom.h"
#include "kml/util/fileio.h"

using std::cout;
using std::endl;
using kmldom::Element;
using kmldom::Feature;
using kmldom::Kml;

#include "kml/util/kmlprint.h"

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

int kmlhellofeatures(int argc, char* argv[]) {
  if (argc != 3) {
    cout << "usage: " << argv[0] << " kmlfile outfile" << endl;
    return EXIT_FAILURE;
  }

  // Read it.
  std::string kml;
  if (!ReadFileToString(argv[1], &kml)) {
    cout << argv[1] << " read failed" << endl;
    return EXIT_FAILURE;
  }

  // Parse it.
  std::string errors;
  Element* root = kmldom::Parse(kml, &errors);
  if (!root) {
    cout << errors;
    return EXIT_FAILURE;
  }
G_kmlprint.flux.open(argv[2]);
  // Print it.
  const Feature* feature = GetRootFeature(root);
  if (feature) {
    PrintFeature(feature, 0);
  } else {
    cout << "No root feature" << endl;
  }
G_kmlprint.flux.close();

  return EXIT_SUCCESS;
}
