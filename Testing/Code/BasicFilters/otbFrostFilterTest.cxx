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

#include "otbImageFileReader.h"
#include "otbFrostImageFilter.h"

#include "itkExceptionObject.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"
#include "itkRandomImageSource.h"
#include "itkMeanImageFilter.h"


int otbFrostFilterTest( int argc, char * argv[] )
{

  typedef itk::Image<float,2> FloatImage2DType;

  itk::RandomImageSource<FloatImage2DType>::Pointer random;
  random = itk::RandomImageSource<FloatImage2DType>::New();
  random->SetMin(0.0);
  random->SetMax(1000.0);
  random->SetNumberOfThreads(1);

  unsigned long randomSize[2];
  randomSize[0] = randomSize[1] = 8;
  random->SetSize(randomSize);
  
  float spacing[2] = {0.7, 2.1};
  random->SetSpacing( spacing );
  float origin[2] = {15, 400};
  random->SetOrigin( origin );
    
  // Create a mean image
  otb::FrostImageFilter<FloatImage2DType, FloatImage2DType>::Pointer frost;
  frost = otb::FrostImageFilter<FloatImage2DType,FloatImage2DType>::New();
  frost->SetInput(random->GetOutput());

  // define the neighborhood size used for the mean filter (5x5)
  FloatImage2DType::SizeType neighRadius;
  neighRadius[0] = 1;
  neighRadius[1] = 1;
  frost->SetRadius(neighRadius);

  // run the algorithm
  frost->Update();

  itk::ImageRegionIterator<FloatImage2DType> it;
  it = itk::ImageRegionIterator<FloatImage2DType>(random->GetOutput(),
                               random->GetOutput()->GetBufferedRegion());
  std::cout << "Input image" << std::endl;
  unsigned int i;
  for (i=1; !it.IsAtEnd(); ++i, ++it)
    {
    std::cout << "\t" << it.Get();
    if ((i % 8) == 0)
      {
      std::cout << std::endl;
      }
    }

  std::cout << "Output image" << std::endl;
  it = itk::ImageRegionIterator<FloatImage2DType>(frost->GetOutput(),
                               frost->GetOutput()->GetBufferedRegion());
  for (i=1; !it.IsAtEnd(); ++i, ++it)
    {
    std::cout << "\t" << it.Get();
    if ((i % 8) == 0)
      {
      std::cout << std::endl;
      }
    }
  
  // Test the itkGetConstReferenceMacro
  const FloatImage2DType::SizeType & radius = frost->GetRadius();
  std::cout << "frost->GetRadius():" << radius << std::endl;

  return EXIT_SUCCESS;

}

