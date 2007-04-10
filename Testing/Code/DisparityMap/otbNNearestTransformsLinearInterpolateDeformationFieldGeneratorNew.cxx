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
#include "itkPointSet.h"
#include "otbVectorImage.h"
#include "otbNNearestTransformsLinearInterpolateDeformationFieldGenerator.h"

int otbNNearestTransformsLinearInterpolateDeformationFieldGeneratorNew(int argc, char * argv[])
{
  try
    {
      const unsigned int Dimension = 2;
      typedef double PixelType;
      typedef otb::VectorImage<PixelType,Dimension> ImageType;
      typedef ImageType::PointType PointType;
      typedef itk::PointSet<PointType,Dimension> PointSetType;
      typedef otb::NNearestTransformsLinearInterpolateDeformationFieldGenerator<PointSetType,ImageType> FilterType;

      // Instantiating object
      FilterType::Pointer filter = FilterType::New();
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
