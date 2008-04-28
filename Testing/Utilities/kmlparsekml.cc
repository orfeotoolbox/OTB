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
REGISTER_TEST(kmlparsekml);
}


// This program parses a KML Placemark from a memory buffer and prints
// the value of the <name> element on standard output.

#include <iostream>
#include <string>
#include "kml/dom.h"  // The KML DOM header.
#include "kml/util/kmlprint.h" 

int kmlparsekml(int argc, char * argv[]) {
G_kmlprint.flux.open(argv[1]);
  // Parse KML from a memory buffer.
  std::string errors;
  kmldom::Element* element = kmldom::Parse(
    "<Placemark><name>hi</name></Placemark>",
    &errors);

  // Convert the type of the root element of the parse.
  const kmldom::Placemark* placemark = kmldom::AsPlacemark(element);

  // Access the value of the  element.
  G_kmlprint.flux << "The Placemark name is: " << placemark->name()
    << std::endl;

  // Release memory
  delete element;
G_kmlprint.flux.close();
  return EXIT_SUCCESS;
}
