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
#include "itkNeighborhood.h"
#include "otbImage.h"
//#include "itkVariableLengthVector.h"
//#include "itkConstNeighborhoodIterator.h"
#include "otbTextureImageFunction.h"
#include "otbFunctionWithNeighborhoodToImageFilter.h"
#include "itkOffset.h"
//#include "otbTextureFunctorBase.h"

template <class TInputScalarType, class TOutputScalarType> //IterInput1, class TIterInput2, class TOutput>
class TextureFunctorTest
{
public:
  /** Return the texture name */
  virtual std::string GetName() const
  {
    return "TextureFunctorTest";
  }

  TextureFunctorTest()
  {
    m_Offset.Fill(1);
  }
  virtual ~TextureFunctorTest() {}

  typedef itk::Offset<>                          OffsetType;
  typedef itk::Neighborhood<TInputScalarType, 2> NeighborhoodType;

  void SetOffset(OffsetType off){ m_Offset = off; }

  inline TOutputScalarType operator ()(const NeighborhoodType& neigh)
  {
    return static_cast<TOutputScalarType>(neigh.GetCenterValue());
  }

private:
  OffsetType m_Offset;
};

int otbFunctionWithNeighborhoodToImageFilterNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  const unsigned int Dimension = 2;
  typedef double                                    PixelType;
  typedef otb::Image<PixelType, Dimension>          ImageType;
  typedef TextureFunctorTest<PixelType, PixelType>                                       FunctorType;
  typedef otb::TextureImageFunction<ImageType, FunctorType>                              FunctionType;
  typedef otb::FunctionWithNeighborhoodToImageFilter<ImageType, ImageType, FunctionType> FilterType;

  // Instantiating object
  FilterType::Pointer filter = FilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
