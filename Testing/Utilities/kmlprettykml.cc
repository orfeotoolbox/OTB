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
REGISTER_TEST(kmlprettykml);
}


// This sample program reads and parses one KML or KMZ file and emits the
// serialization to standard output.  Since all recognized elements
// are saved in the KML DOM with white space discarded and Serialize()
// formats nicely this is overall a pretty printer.

#include <iostream>
#include <string>
#include "kml/dom.h"
#include "kml/util/fileio.h"
#include "kml/util/kmz.h"
#include "kml/util/kmlprint.h"

using std::cout;
using std::endl;
using kmldom::Element;

int kmlprettykml(int argc, char* argv []) {
  if (argc != 3) {
    cout << "usage: " << argv[0] << " kmlfile outputfile" << endl;
    return EXIT_FAILURE;
  }

G_kmlprint.flux.open(argv[2]);
  // Read the file.
  std::string file_data;
  if (!ReadFileToString(argv[1], &file_data)) {
    cout << argv[1] << " read failed" << endl;
    return EXIT_FAILURE;
  }

  // If the file was KMZ, extract the KML file.
  std::string kml;
  if (DataIsKmz(file_data)) {
    if (!ReadKmlFromKmz(argv[1], &kml)) {
      G_kmlprint.flux << "Failed reading KMZ file" << endl;
      return EXIT_FAILURE;
    }
  } else {
    kml = file_data;
  }

  // Parse it.
  std::string errors;
  Element* root = kmldom::Parse(kml, &errors);
  if (!root) {
    G_kmlprint.flux << errors << endl;
    return EXIT_FAILURE;
  }

  // Serialize it and output to stdout.
  G_kmlprint.flux << kmldom::SerializePretty(*root);

  // Free the storage created by Parse().
  delete root;
G_kmlprint.flux.close();
  return EXIT_SUCCESS;
}
