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

#include "otbUnaryFunctorNeighborhoodWithOffsetImageFilter.h"
#include "otbVectorImage.h"
#include "itkConstNeighborhoodIterator.h"

namespace Functor
{
template <class TIter, class TOutput>
class UnaryFunctorNeighborhoodWithOffsetImageFilterFunctorNewTest
{
public:
  UnaryFunctorNeighborhoodWithOffsetImageFilterFunctorNewTest() {};
  ~UnaryFunctorNeighborhoodWithOffsetImageFilterFunctorNewTest() {};

  typedef TIter IterType;
  typedef typename IterType::OffsetType OffsetType;

  void SetOffset(OffsetType off)
  {
    m_Offset=off;
  };
  OffsetType GetOffset()
  {
    return m_Offset;
  };

  inline TOutput operator() (const TIter & it)
  {
    return(static_cast<TOutput>(it.GetCenterPixel()));

  }
private:
  OffsetType m_Offset;
};
}


int otbUnaryFunctorNeighborhoodWithOffsetImageFilterNew(int argc, char * argv[])
{
  typedef double InputPixelType;
  const int Dimension = 2;
  typedef otb::VectorImage<InputPixelType,Dimension> ImageType;
  typedef ImageType::PixelType PixelType;
  typedef itk::ConstNeighborhoodIterator<ImageType>   IterType;;
  typedef Functor::UnaryFunctorNeighborhoodWithOffsetImageFilterFunctorNewTest<IterType, PixelType>  FunctorType;
  typedef otb::UnaryFunctorNeighborhoodWithOffsetImageFilter<ImageType, ImageType, FunctorType> UnaryFunctorNeighborhoodWithOffsetImageFilterType;

  // Instantiating object
  UnaryFunctorNeighborhoodWithOffsetImageFilterType::Pointer object = UnaryFunctorNeighborhoodWithOffsetImageFilterType::New();

  return EXIT_SUCCESS;
}

