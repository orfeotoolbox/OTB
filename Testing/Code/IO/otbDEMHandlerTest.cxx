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
#include "itkExceptionObject.h"

#include "itkExceptionObject.h"
#include "otbDEMHandler.h"
#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "otbMapProjections.h"

int otbDEMHandlerTest(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  char * srtm_directory(argv[1]);
  char * geoidFile(argv[2]);
  const char * outputfilename(argv[3]);
  double height = 0.0;
  double height2 = 0.0;

  typedef otb::Image<float,Dimension> ImageType;
  typedef otb::DEMHandler              DEMHandlerType;

  // Instantiating object
  DEMHandlerType::Pointer demHandler = DEMHandlerType::New();
  demHandler->OpenDEMDirectory(srtm_directory);
  demHandler->OpenGeoidFile(geoidFile);

  typedef otb::UtmInverseProjection                      utmProjection;
  typedef utmProjection::InputPointType          InputPoint;
  InputPoint                                      geoPoint;
  geoPoint[0] = atof(argv[4]);//3.6999;
  geoPoint[1] = atof(argv[5]);//44.08;

  height = demHandler->GetHeightAboveMSL(geoPoint);
  height2 = demHandler->GetHeightAboveEllipsoid(geoPoint);

  std::ofstream file;
  file.open(outputfilename);
  file << "--- HEIGHT ABOVE MSL TEST ---" << std::endl;
  file << " geoPoint: " << geoPoint[1] << " ; " << geoPoint[0] << std::endl;
  file << " -> Height above MSL: " << height << std::endl;
  std::cout << "Height above MSL: " << height << std::endl;

  file << " -> Height above Ellipsoid: " << height2 << std::endl;
  std::cout << "Height above Ellipsoid: " << height2  << std::endl;


  file.close();

  


  return EXIT_SUCCESS;
}
