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

#include "otbUnaryFunctorWithIndexImageFilter.h"
#include "otbImage.h"

namespace Functor
{
template <class TInput, class TOutput>
class UnaryFunctorWithIndexImageFilterFunctorNewTest
{
public:
  UnaryFunctorWithIndexImageFilterFunctorNewTest() {}
  ~UnaryFunctorWithIndexImageFilterFunctorNewTest() {}

  typedef itk::Index<2> IndexType;

  inline TOutput operator ()(const TInput& inPix, IndexType itkNotUsed(index))
  {
    return (static_cast<TOutput>(inPix));
  }
};
}

int otbUnaryFunctorWithIndexImageFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef double                                                                        InputPixelType;
  typedef otb::Image<InputPixelType, 2>                                                 ImageType;
  typedef ImageType::PixelType                                                          PixelType;
  typedef Functor::UnaryFunctorWithIndexImageFilterFunctorNewTest<PixelType, PixelType> FunctorType;
  typedef otb::UnaryFunctorWithIndexImageFilter<ImageType, ImageType,
      FunctorType>      UnaryFunctorWithIndexImageFilterType;

  // Instantiating object
  UnaryFunctorWithIndexImageFilterType::Pointer filter = UnaryFunctorWithIndexImageFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
