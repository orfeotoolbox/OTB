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
#include "otbImage.h"
#include "otbFunctionWithNeighborhoodToImageFilter.h"
#include "itkVarianceImageFunction.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbTextureFunctorBase.h"
#include "otbTextureImageFunction.h"

template <class TScalarInput, class TScalarOutput>
class ITK_EXPORT FunctorTest :
  public otb::Functor::TextureFunctorBase<TScalarInput, TScalarOutput>
{
public:
  FunctorTest(){};
  ~FunctorTest(){}

  typedef itk::Neighborhood<TScalarInput, 2> NeighborhoodType;

  virtual double ComputeOverSingleChannel(const NeighborhoodType& neigh, const NeighborhoodType& neighOff)
  {
    double out = neigh.GetCenterValue();

    return out;
  }
};

int otbFunctionWithNeighborhoodToImageFilter(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef double                                                                                    PixelType;
  typedef otb::Image<PixelType, Dimension>                                                          InputImageType;
  typedef InputImageType::SizeType                                                                  SizeType;
  typedef InputImageType::OffsetType                                                                OffsetType;
  typedef otb::Image<PixelType, Dimension>                                                          OutputImageType;
  typedef otb::ImageFileReader<InputImageType>                                                      ReaderType;
  typedef otb::StreamingImageFileWriter<OutputImageType>                                            WriterType;
  typedef FunctorTest<PixelType, PixelType>                                                         FunctorType;
  typedef otb::TextureImageFunction<InputImageType, FunctorType>                                    FunctionType;
  typedef otb::FunctionWithNeighborhoodToImageFilter<InputImageType, OutputImageType, FunctionType> FilterType;

  // Instantiating object
  FilterType::Pointer filter = FilterType::New();
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  // jsut for accessors, the functor doesn't care
  SizeType radius;
  radius.Fill(2);
  OffsetType offset;
  offset.Fill(2);

  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);
  filter->SetInput(reader->GetOutput());
  filter->SetRadius(radius);
  filter->SetOffset(offset);

  writer->SetInput(filter->GetOutput());
  //writer->SetNumberOfStreamDivisions(1);
  writer->Update();

  return EXIT_SUCCESS;
}
