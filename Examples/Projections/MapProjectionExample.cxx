/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */



//  Software Guide : BeginCommandLineArgs
//    OUTPUTS: {mapProjectionExample-output.txt}
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// Map projection is an important issue when working with satellite
// images. In the orthorectification process, converting between
// geographic and cartographic coordinates is a key step. In this
// process, everything is integrated and you don't need to know the
// details.
//
// However, sometimes, you need to go hands-on and find out the
// nitty-gritty details.  This example shows you how to play with map
// projections in OTB and how to convert coordinates. In most cases,
// the underlying work is done by OSSIM.
//
// First, we start by including the otbMapProjections header. In this
// file, over 30 projections are defined and ready to use. It is easy
// to add new one.
//
// The otbGenericMapProjection enables you to instantiate a map
// projection from a WKT (Well Known Text) string, which is popular
// with OGR for example.
//
// Software Guide : EndLatex

#include <fstream>
#include <iomanip>

// Software Guide : BeginCodeSnippet
#include "otbSpatialReference.h"
#include "otbGenericMapProjection.h"
// Software Guide : EndCodeSnippet

int main(int argc, char* argv[])
{
  if (argc < 2)
    {
    std::cout << argv[0] << " <outputfile> "  << std::endl;

    return EXIT_FAILURE;
    }

  // Software Guide : BeginLatex
  //
  // We retrieve the command line parameters and put them in the
  // correct variables. The transforms are going to work with an
  // \doxygen{itk}{Point}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  const char * outFileName = argv[1];

  itk::Point<double, 2> point;
  point[0] = 1.4835345;
  point[1] = 43.55968261;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The output of this program will be saved in a text file. We also want
  // to make sure that the precision of the digits will be enough.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  std::ofstream file;
  file.open(outFileName);
  file << std::setprecision(15);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We can now instantiate our first map projection. Here, it is a
  // UTM projection. We also need to provide the information about
  // the zone and the hemisphere for the projection. These are
  // specific to the UTM projection.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::GenericMapProjection<otb::TransformDirection::FORWARD> MapProjectionType;
  otb::SpatialReference utmSRS = otb::SpatialReference::FromUTM(31,otb::SpatialReference::hemisphere::north);
  MapProjectionType::Pointer utmProjection = MapProjectionType::New();
  utmProjection->SetWkt(utmSRS.ToWkt());
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The TransformPoint() method returns the coordinates of the point in the
  // new projection.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  file << "Forward UTM projection: " << std::endl;
  file << point << " -> ";
  file << utmProjection->TransformPoint(point);
  file << std::endl << std::endl;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We follow the same path for the Lambert93 projection:
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  // IGNF:LAMB93 is the IGNF code for lambert 93
  otb::SpatialReference lamb93SRS = otb::SpatialReference::FromDescription("IGNF:LAMB93");
  MapProjectionType::Pointer lambertProjection = MapProjectionType::New();
  lambertProjection->SetWkt(lamb93SRS.ToWkt());

  file << "Forward Lambert93 projection: " << std::endl;
  file << point << " -> ";
  file << lambertProjection->TransformPoint(point);
  file << std::endl << std::endl;

  // Software Guide : BeginLatex
  //
  // And of course, we don't forget to close the file:
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  file.close();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The final output of the program should be:
  //
  //  \begin{verbatim}
  //   Forward UTM projection:
  //       [1.4835345, 43.55968261] -> [377522.448427013, 4824086.71129131]
  //
  //   Forward Lambert93 projection:
  //      [1.4835345, 43.55968261] -> [577437.889798954, 6274578.791561]
  //
  //   \end{verbatim}
  //
  // Software Guide : EndLatex

  return EXIT_SUCCESS;
}
