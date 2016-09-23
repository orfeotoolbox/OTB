/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


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
#include "otbMapProjections.h"
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
  otb::UtmForwardProjection::Pointer utmProjection
    = otb::UtmForwardProjection::New();
  utmProjection->SetZone(31);
  utmProjection->SetHemisphere('N');
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
  otb::Lambert93ForwardProjection::Pointer lambertProjection
    = otb::Lambert93ForwardProjection::New();

  file << "Forward Lambert93 projection: " << std::endl;
  file << point << " -> ";
  file << lambertProjection->TransformPoint(point);
  file << std::endl << std::endl;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // If you followed carefully the previous examples, you've noticed
  // that the target projections have been directly coded, which means
  // that they can't be changed at run-time. What happens if you don't
  // know the target projection when you're writing the program? It
  // can depend on some input provided by the user (image,
  // shapefile).
  //
  // In this situation, you can use the
  // \doxygen{otb}{GenericMapProjection}. It will accept a string to
  // set the projection. This string should be in the WKT format.
  //
  // For example:
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  std::string projectionRefWkt = "PROJCS[\"UTM Zone 31, Northern Hemisphere\","
                                 "GEOGCS[\"WGS 84\", DATUM[\"WGS_1984\", SPHEROID[\"WGS 84\", 6378137, 298.257223563,"
                                 "AUTHORITY[\"EPSG\",\"7030\"]], TOWGS84[0, 0, 0, 0, 0, 0, 0],"
                                 "AUTHORITY[\"EPSG\",\"6326\"]], PRIMEM[\"Greenwich\", 0, AUTHORITY[\"EPSG\",\"8901\"]],"
                                 "UNIT[\"degree\", 0.0174532925199433, AUTHORITY[\"EPSG\",\"9108\"]],"
                                 "AXIS[\"Lat\", NORTH], AXIS[\"Long\", EAST],"
                                 "AUTHORITY[\"EPSG\",\"4326\"]], PROJECTION[\"Transverse_Mercator\"],"
                                 "PARAMETER[\"latitude_of_origin\", 0], PARAMETER[\"central_meridian\", 3],"
                                 "PARAMETER[\"scale_factor\", 0.9996], PARAMETER[\"false_easting\", 500000],"
                                 "PARAMETER[\"false_northing\", 0], UNIT[\"Meter\", 1]]";
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // This string is then passed to the projection using the
  // \code{SetWkt()} method.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::GenericMapProjection<otb::TransformDirection::FORWARD> GenericMapProjection;
  GenericMapProjection::Pointer genericMapProjection =
    GenericMapProjection::New();
  genericMapProjection->SetWkt(projectionRefWkt);

  file << "Forward generic projection: " << std::endl;
  file << point << " -> ";
  file << genericMapProjection->TransformPoint(point);
  file << std::endl << std::endl;
  // Software Guide : EndCodeSnippet

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
  //   Forward generic projection:
  //      [1.4835345, 43.55968261] -> [377522.448427013, 4824086.71129131]
  //   \end{verbatim}
  //
  // Software Guide : EndLatex

  return EXIT_SUCCESS;
}
