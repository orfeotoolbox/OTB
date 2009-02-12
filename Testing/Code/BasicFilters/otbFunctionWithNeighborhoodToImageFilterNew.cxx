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
#include "itkNeighborhood.h"
#include "otbImage.h"
#include "itkVariableLengthVector.h"
#include "itkConstNeighborhoodIterator.h"
#include "otbTextureImageFunction.h"
#include "otbFunctionWithNeighborhoodToImageFilter.h"
#include "otbTextureFunctorBase.h"

template <class TIterInput1, class TIterInput2, class TOutput>
class TextureFunctorTest
{
public:
  TextureFunctorTest()
  {
    m_Offset.Fill(1);
  };
  ~TextureFunctorTest() {};

  typedef TIterInput1                    IterType1;
  typedef TIterInput2                    IterType2;
  typedef TOutput                        OutputType;
  typedef typename IterType1::OffsetType OffsetType;
  typedef typename IterType1::InternalPixelType InternalPixelType;
  typedef typename IterType1::ImageType  ImageType;
  typedef itk::Neighborhood<InternalPixelType, ::itk::GetImageDimension<ImageType>::ImageDimension>    NeighborhoodType;

  void SetOffset(OffsetType off){ m_Offset=off; };

  inline TOutput operator()(const IterType1 &it, const IterType2 &itOff)
    { 
      return static_cast<OutputType>(it.GetCenterPixel()[0]);
    }

  double ComputeOverSingleChannel(const NeighborhoodType &neigh, const NeighborhoodType &neighOff)
  {
    return 0.;
  }
 
 private:
  OffsetType m_Offset;
};


int otbFunctionWithNeighborhoodToImageFilterNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef double PixelType;
  typedef otb::Image<PixelType,Dimension>           ImageType;
  typedef itk::VariableLengthVector<double>         VectorType;
  typedef itk::ConstNeighborhoodIterator<ImageType> IteratorType;

  typedef TextureFunctorTest<IteratorType, IteratorType, VectorType>                     FunctorType;
  typedef otb::TextureImageFunction<ImageType, FunctorType>                              FunctionType;
  typedef otb::FunctionWithNeighborhoodToImageFilter<ImageType, ImageType, FunctionType> FilterType;

  // Instantiating object
  FilterType::Pointer object = FilterType::New();

  return EXIT_SUCCESS;
}
