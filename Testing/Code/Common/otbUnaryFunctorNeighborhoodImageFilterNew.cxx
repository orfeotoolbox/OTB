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
#include "itkMacro.h"

#include "otbUnaryFunctorNeighborhoodImageFilter.h"
#include "otbImage.h"

namespace Functor
{
template <class TIter, class TOutput>
class UnaryFunctorNeighborhoodImageFilterFunctorNewTest
{
public:
  UnaryFunctorNeighborhoodImageFilterFunctorNewTest() {}
  ~UnaryFunctorNeighborhoodImageFilterFunctorNewTest() {}

  inline TOutput operator ()(const TIter& it)
  {
    return (static_cast<TOutput>(it.GetCenterPixel()));

  }
};
}

int otbUnaryFunctorNeighborhoodImageFilterNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  typedef double InputPixelType;
  const int Dimension = 2;
  typedef otb::Image<InputPixelType, Dimension>                                           ImageType;
  typedef ImageType::PixelType                                                            PixelType;
  typedef itk::ConstNeighborhoodIterator<ImageType>                                       IterType;
  typedef Functor::UnaryFunctorNeighborhoodImageFilterFunctorNewTest<IterType, PixelType> FunctorType;
  typedef otb::UnaryFunctorNeighborhoodImageFilter<ImageType, ImageType,
      FunctorType>     UnaryFunctorNeighborhoodImageFilterType;

  // Instantiating object
  UnaryFunctorNeighborhoodImageFilterType::Pointer object = UnaryFunctorNeighborhoodImageFilterType::New();

  std::cout << object << std::endl;

  return EXIT_SUCCESS;
}
