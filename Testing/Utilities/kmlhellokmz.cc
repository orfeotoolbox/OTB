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
REGISTER_TEST(kmlhellokmz);
}


// An example of the KMZ API, showing DataIsKmz() and ReadKmlFromKmz().
// Prints the default KML file in a KMZ archive, if found, or the first KML
// file.

#include <iostream>
#include <string>
#include "kml/dom.h"
#include "kml/util/fileio.h"
#include "kml/util/kmz.h"
#include <fstream>
#include "kml/util/kmlprint.h"

using std::cout;
using std::endl;

int kmlhellokmz(int argc, char* argv[]) {
  if (argc != 3) {
    cout << "usage: " << argv[0] << " kmzfile outputfile" << endl;
    return EXIT_FAILURE;
  }
  const char* infile = argv[1];

  std::string data;
  ReadFileToString(infile, &data);
  if (data.empty()) {
    cout << "error: no data read from " << infile << endl;
    return EXIT_FAILURE;
  }

  bool is_kmz = DataIsKmz(data);
  if (!is_kmz) {
    cout << "error: " << infile << " is not a KMZ file" << endl;
    return EXIT_FAILURE;
  }

  std::string kml;
  ReadKmlFromKmz(infile, &kml);
  if (kml.empty()) {
    cout << "No data read from " << kml << endl;
    return EXIT_FAILURE;
  }

  G_kmlprint.flux.open(argv[2]);
  G_kmlprint.flux << kml << endl;
  G_kmlprint.flux.close();
  
  return EXIT_SUCCESS;
}
