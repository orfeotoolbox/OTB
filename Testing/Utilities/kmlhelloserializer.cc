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

#include <iostream>
#include <string>
#include "kml/dom.h"
#include "kml/util/kmlprint.h"

using std::cout;
using std::endl;
using kmldom::Folder;
using kmldom::KmlFactory;
using kmldom::Placemark;

int kmlhelloserializer(int argc, char * argv[]) {
  KmlFactory* factory(KmlFactory::GetFactory());
  Folder* folder = factory->CreateFolder();
  folder->set_name("folder name");
  folder->set_open(true);

  Placemark* pm0 = factory->CreatePlacemark();
  pm0->set_name("pm0");
  folder->add_feature(pm0);

  Placemark* pm1 = factory->CreatePlacemark();
  pm1->set_name("pm1");
  pm0->set_visibility(false);
  folder->add_feature(pm1);

  Folder* empty = factory->CreateFolder();
  empty->set_name("empty folder");
  // Since visibility, open, etc are not explicitely set Serialize()
  // does not emit KML for these.
  folder->add_feature(empty);

G_kmlprint.flux.open(argv[1]);
  // SerializeRaw() adds no extraneous ("pretty") white space.
  G_kmlprint.flux << kmldom::SerializeRaw(*folder) << endl;
G_kmlprint.flux.close();

  // Cleans up folder and everything below it.
  delete folder;
  return EXIT_SUCCESS;
}
