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
  template <class TIter1, class TIter2, class TOutput>
  class UnaryFunctorNeighborhoodWithOffsetImageFilterFunctorNewTest
  {
  public:
    UnaryFunctorNeighborhoodWithOffsetImageFilterFunctorNewTest() {};
    ~UnaryFunctorNeighborhoodWithOffsetImageFilterFunctorNewTest() {};

    typedef TIter1 IterType1;
    typedef typename IterType1::OffsetType OffsetType;
    typedef TIter2 IterType2;
    

    void SetOffset(OffsetType off){ m_Offset=off; };
    OffsetType GetOffset(){ return m_Offset; };

    inline TOutput operator() (const TIter1 & it1, const TIter2 & it2)
    {
      return(static_cast<TOutput>(it1.GetCenterPixel()));

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
  typedef Functor::UnaryFunctorNeighborhoodWithOffsetImageFilterFunctorNewTest<IterType, IterType, PixelType>  FunctorType;
  typedef otb::UnaryFunctorNeighborhoodWithOffsetImageFilter<ImageType, ImageType, FunctorType> UnaryFunctorNeighborhoodWithOffsetImageFilterType;

  // Instantiating object
  UnaryFunctorNeighborhoodWithOffsetImageFilterType::Pointer object = UnaryFunctorNeighborhoodWithOffsetImageFilterType::New();

  return EXIT_SUCCESS;
}

