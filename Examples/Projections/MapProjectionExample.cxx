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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

//  Software Guide : BeginCommandLineArgs
//    OUTPUTS: {mapProjectionExample.tex}
//    1.4835345  43.55968261
//  Software Guide : EndCommandLineArgs


// Software Guide : BeginLatex
//
// Map projection is an important issue when working with satellite images. In the
// orthorectification process, converting between geographic and cartographic
// coordinate is a key step. In this process, everything is integrated and you
// don't need to know the details.
//
// However, sometimes, you need to go hands-on and find out the nitty-gritty details.
// This example show you how to play with map projection in OTB and how to convert
// coordinates. In most cases, the underlying work is done by ossim.
//
// First, we start by including the otbMapProjections header. In this file, over 30
// projections are defined and ready to use. It is easy to add new one. The
// otbGenericMapProjection enable you to instanciate a map projection from a
// WKT (well know text) string, which is popular with ORG for example.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbMapProjections.h"
#include "otbGenericMapProjection.h"
// Software Guide : EndCodeSnippet

int main( int argc, char* argv[] )
{
  if(argc < 3  )
  {
    std::cout << argv[0] <<" <outputfile> <lon> <lat> "  << std::endl;

    return EXIT_FAILURE;
  }

  // Software Guide : BeginLatex
  //
  // We retrieve the command line parameters and put them in the correct variables. The
  // transforms are going to work with an \doxygen{itk}{Point}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  const char * outFileName = argv[1];

  itk::Point<double,2> point;
  point[0]=atof(argv[2]);
  point[1]=atof(argv[3]);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The output of this program will be save in a text file. We also want
  // to make sure the precision of the digit will be enough.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  std::ofstream file;
  file.open(outFileName);
  file << std::setprecision(15);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We can now instanciate our first map projection. Here it is a UTM projection
  // we also need to provide the information concerning the zone and the hemisphere
  // for the projection. These are specific to UTM projection.
  //
  // Software Guide : EndLatex


  // Software Guide : BeginCodeSnippet
  otb::UtmForwardProjection::Pointer utmProjection = otb::UtmForwardProjection::New();
  utmProjection->SetZone(31);
  utmProjection->SetHemisphere('N');
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The TransformPoint() method return the coordinate of the point is the
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
  otb::Lambert93ForwardProjection::Pointer lambertProjection = otb::Lambert93ForwardProjection::New();

  file << "Forward Lambert93 projection: " << std::endl;
  file << point << " -> ";
  file << lambertProjection->TransformPoint(point);
  file << std::endl << std::endl;
  // Software Guide : EndCodeSnippet


  // Software Guide : BeginLatex
  //
  // If you follow carefully the previous examples, you've noticed that the target
  // projection have been directly coded, which mean that they can't be changed at
  // run-time. What happens if you don't know the target projection when you're writing
  // the program? It can depends on some input provided by the user (image, shapefile).
  //
  // In this situation, you can use the \doxygen{otb}{GenericMapProjection}. It will
  // accept a string to set the projection. This string should be in the WKT format
  // widely accepted by OGR.
  //
  // For example:
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  std::string projectionRefWkt ="PROJCS[\"UTM Zone 31, Northern Hemisphere\","
      "GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],"
      "TOWGS84[0,0,0,0,0,0,0],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]],"
      "UNIT[\"degree\",0.0174532925199433,AUTHORITY[\"EPSG\",\"9108\"]],AXIS[\"Lat\",NORTH],AXIS[\"Long\",EAST],"
      "AUTHORITY[\"EPSG\",\"4326\"]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",0],"
      "PARAMETER[\"central_meridian\",3],PARAMETER[\"scale_factor\",0.9996],PARAMETER[\"false_easting\",500000],"
      "PARAMETER[\"false_northing\",0],UNIT[\"Meter\",1]]";
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // This string is then passed to the projection using the SetWkt() method.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::GenericMapProjection<otb::FORWARD> GenericMapProjection;
  GenericMapProjection::Pointer genericMapProjection = GenericMapProjection::New();
  genericMapProjection->SetWkt(projectionRefWkt);

  file << "Forward gerenic projection: " << std::endl;
  file << point << " -> ";
  file << genericMapProjection ->TransformPoint(point);
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
  // \input{mapProjectionExample.tex}
  //
  // Software Guide : EndLatex

  return EXIT_SUCCESS;
}
