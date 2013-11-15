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
#include "otbRenderingImageFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkRGBPixel.h"
#include "otbStandardRenderingFunction.h"
#include "otbChannelSelectorFunctor.h"
#include "itkExpNegativeImageFilter.h"

int otbRenderingImageFilterVectorWithExpNegativeTransfer(int argc, char * argv[])
{
  typedef double                                                 PixelType;
  typedef otb::VectorImage<PixelType, 2>                         ImageType;
  typedef ImageType::PixelType                                   VectorPixelType;
  typedef otb::Image<itk::RGBPixel<unsigned char>, 2>            RGBImageType;
  typedef otb::RenderingImageFilter<ImageType, RGBImageType>     RenderingFilterType;
  typedef otb::ImageFileReader<ImageType>                        ReaderType;
  typedef otb::ImageFileWriter<RGBImageType>            WriterType;
  typedef otb::Function::ChannelSelectorFunctor<VectorPixelType> PixelRepresentationFunctionType;
  typedef itk::Functor::ExpNegative<PixelType, PixelType>        ExpNegativeFunctionType;
  typedef otb::Function::StandardRenderingFunction<
      VectorPixelType,
      itk::RGBPixel<unsigned char>,
      PixelRepresentationFunctionType,
      ExpNegativeFunctionType>                               RenderingFunctionType;
  typedef RenderingFunctionType::ParametersType ParametersType;

  // Instantiation
  ReaderType::Pointer            reader    = ReaderType::New();
  RenderingFilterType::Pointer   rendering = RenderingFilterType::New();
  WriterType::Pointer            writer    = WriterType::New();
  RenderingFunctionType::Pointer function = RenderingFunctionType::New();

  // reading input image
  reader->SetFileName(argv[1]);
  reader->GenerateOutputInformation();
  unsigned int nbComponents = 3; //To be displayed

  // min & max
  ParametersType parameters(2 * nbComponents);

  unsigned int channelRed = atoi(argv[3]);
  unsigned int channelGreen = atoi(argv[4]);
  unsigned int channelBlue = atoi(argv[5]);
  for (unsigned int i = 0; i < parameters.Size(); ++i)
    {
    parameters[i] = atof(argv[7 + i]);
    ++i;
    parameters[i] = atof(argv[7 + i]);
    }

  // rendering
  rendering->SetInput(reader->GetOutput());
  rendering->SetRenderingFunction(function);
  function->SetParameters(parameters);
  function->GetPixelRepresentationFunction()->SetRedChannelIndex(channelRed);
  function->GetPixelRepresentationFunction()->SetGreenChannelIndex(channelGreen);
  function->GetPixelRepresentationFunction()->SetBlueChannelIndex(channelBlue);
  function->GetTransferFunction().SetFactor(atof(argv[6]));

  // writing
  writer->SetFileName(argv[2]);
  writer->SetInput(rendering->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
