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

#include "otbUnaryFunctorNeighborhoodWithOffsetImageFilter.h"
#include "otbVectorImage.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkOffset.h"

namespace Functor
{
template <class TInput, class TOutput>
class UnaryFunctorNeighborhoodWithOffsetImageFilterFunctorNewTest
{
public:
  UnaryFunctorNeighborhoodWithOffsetImageFilterFunctorNewTest() {}
  ~UnaryFunctorNeighborhoodWithOffsetImageFilterFunctorNewTest() {}

  typedef TInput                                      InputScalarType;
  typedef TOutput                                     OutputScalarType;
  typedef itk::VariableLengthVector<InputScalarType>  InputVectorType;
  typedef itk::VariableLengthVector<OutputScalarType> OutputVectorType;
  typedef itk::Offset<>                               OffsetType;
  typedef itk::Neighborhood<InputScalarType, 2>       NeighborhoodType;
  typedef itk::Neighborhood<InputVectorType, 2>       NeighborhoodVectorType;

  void SetOffset(OffsetType off)
  {
    m_Offset = off;
  }
  OffsetType GetOffset()
  {
    return m_Offset;
  }

  inline OutputScalarType operator ()(const NeighborhoodType& neigh)
  {
    return (static_cast<OutputScalarType>(neigh.GetCenterValue()));
  }
  inline OutputVectorType operator ()(const NeighborhoodVectorType& neigh)
  {
    return (static_cast<OutputVectorType>(neigh.GetCenterValue()));

  }

private:
  OffsetType m_Offset;
};
}

int otbUnaryFunctorNeighborhoodWithOffsetImageFilterNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  typedef double InputPixelType;
  const int Dimension = 2;
  typedef otb::VectorImage<InputPixelType,
      Dimension>               ImageType;
  typedef ImageType::PixelType
  PixelType;
  typedef itk::ConstNeighborhoodIterator<ImageType> IterType;
  typedef Functor::UnaryFunctorNeighborhoodWithOffsetImageFilterFunctorNewTest<InputPixelType,
      InputPixelType> FunctorType;
  typedef otb::UnaryFunctorNeighborhoodWithOffsetImageFilter<ImageType, ImageType,
      FunctorType>
  UnaryFunctorNeighborhoodWithOffsetImageFilterType;

  // Instantiating object
  UnaryFunctorNeighborhoodWithOffsetImageFilterType::Pointer filter =
    UnaryFunctorNeighborhoodWithOffsetImageFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
