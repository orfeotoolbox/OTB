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

#include "otbImageFittingPolygonListFilter.h"
#include "otbPolygon.h"
#include "otbImage.h"
#include <fstream>
#include <cstdlib>

int otbImageFittingPolygonListFilter(int argc, char * argv[])
{
  const char * imageFileName = argv[1];
  const char * outfname = argv[1];
  const double threshold = atof(argv[2]);
  
  const unsigned int Dimension =2;
  typedef otb::Polygon<double>           PolygonType;
  typedef otb::Image<double,Dimension>   ImageType;
  
  typedef otb::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(imageFileName);
  
  typedef itk::CannyEdgeDetectionImageFilter<ImageType,ImageType> CannyFilterType;
  CannyFilterType::Pointer canny = CannyFilterType::New();
  
  canny->SetInput(reader->GetOutput());
      
  
  typedef otb::ImageFittingPolygonListFilter<PolygonType,ImageType> FittingPolygonType;
  
  FittingPolygonType::Pointer fittingPolygon = FittingPolygonType::New();
  fittingPolygon->SetInput(simplifyPolygonFilter->GetOutput());
  fittingPolygon->SetInputImage(canny->GetOutput());
  fittingPolygon->SetRadius(fittingRadius);
  fittingPolygon->SetNumberOfIterations(fittingIters);
  fittingPolygon->Update();

  
  return EXIT_SUCCESS;
}
