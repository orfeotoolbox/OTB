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

#include "otbSpatialObjectToImageDrawingFilter.h"
#include "itkEllipseSpatialObject.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

int otbSpatialObjectToImageDrawingFilter(int argc, char * argv[])
{
  try
    {
      const char * outputFilename = argv[1];
      const double radiusx = atof(argv[2]);
      const double radiusy = atof(argv[3]);
      const unsigned int Dimension = 2;
      typedef unsigned char PixelType;
      typedef otb::Image<PixelType,Dimension> ImageType;
      typedef otb::ImageFileWriter<ImageType> WriterType;
      typedef itk::EllipseSpatialObject<Dimension> SpatialObjectType;
      typedef otb::SpatialObjectToImageDrawingFilter<SpatialObjectType,ImageType> SpatialObjectToImageDrawingFilterType;
      // Instantiating object
      SpatialObjectToImageDrawingFilterType::Pointer filter = SpatialObjectToImageDrawingFilterType::New();
      WriterType::Pointer writer = WriterType::New();

      SpatialObjectType::Pointer ellipse = SpatialObjectType::New();
      SpatialObjectType::ArrayType radius;
      radius[0]=radiusx;
      radius[1]=radiusy;
      
      ellipse->SetRadius(radius);

      ImageType::PointType origin;
      origin[0]=-radiusx;
      origin[1]=-radiusy;
      filter->SetInput(ellipse);
      filter->SetOrigin(origin);
      filter->SetInsideValue(255);
      filter->SetOutsideValue(0);

      writer->SetInput(filter->GetOutput());
      writer->SetFileName(outputFilename);
      writer->Update();
    }
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject thrown !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 

  catch( ... ) 
    { 
    std::cout << "Unknown exception thrown !" << std::endl; 
    return EXIT_FAILURE;
    } 
  return EXIT_SUCCESS;
}
