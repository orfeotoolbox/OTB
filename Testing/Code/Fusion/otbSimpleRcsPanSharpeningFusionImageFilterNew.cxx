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
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorImage.h"
#include "otbImage.h"

#include "otbSimpleRcsPanSharpeningFusionImageFilter.h"

int otbSimpleRcsPanSharpeningFusionImageFilterNew( int argc, char * argv[] )
{
  try 
    { 
     
      const unsigned int Dimension = 2;
      typedef double PixelType;
      
      
      typedef otb::VectorImage<PixelType,Dimension>  VectorImageType;
      typedef otb::Image<PixelType,Dimension>        PanchroImageType;
      typedef otb::ImageFileReader<VectorImageType>  VectorReaderType;
      typedef otb::ImageFileReader<PanchroImageType> ImageReaderType;
      typedef otb::SimpleRcsPanSharpeningFusionImageFilter
          <PanchroImageType, VectorImageType, VectorImageType> FilterType;
      
       FilterType::Pointer       filter = FilterType::New();
      
    } 
  
  catch( itk::ExceptionObject & err ) 
    { 
      std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
      std::cout << err << std::endl; 
      return EXIT_FAILURE;
    } 
  catch( ... ) 
    { 
      std::cout << "Exception levee inconnue !" << std::endl; 
      return EXIT_FAILURE;
    } 
  
  
  return EXIT_SUCCESS;
}


