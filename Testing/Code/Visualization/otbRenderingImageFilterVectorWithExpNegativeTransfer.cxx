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
#include "otbStreamingImageFileWriter.h"
#include "itkRGBPixel.h"
#include "otbStandardRenderingFunction.h"
#include "itkExpNegativeImageFilter.h"

int otbRenderingImageFilterVectorWithExpNegativeTransfer( int argc, char * argv[] )
{
  typedef double                                            PixelType;
  typedef otb::VectorImage<PixelType,2>                     ImageType;
  typedef ImageType::PixelType                              VectorPixelType;
  typedef otb::Image<itk::RGBPixel<unsigned char>, 2>       RGBImageType;
  typedef otb::RenderingImageFilter<ImageType,RGBImageType> RenderingFilterType;
  typedef otb::ImageFileReader<ImageType>                   ReaderType;
  typedef otb::StreamingImageFileWriter<RGBImageType>       WriterType;
  typedef itk::Function::ExpNegative<PixelType,PixelType>   ExpNegativeFunctionType;
  typedef otb::Function::StandardRenderingFunction<PixelType,
    itk::RGBPixel<unsigned char>,ExpNegativeFunctionType >  RenderingFunctionType;


  // Instanatiation
  ReaderType::Pointer          reader    = ReaderType::New();
  RenderingFilterType::Pointer rendering = RenderingFilterType::New();
  WriterType::Pointer          writer    = WriterType::New();
  RenderingFunctionType::Pointer function = RenderingFunctionType::New();


  // reading input image
  reader->SetFileName(argv[1]);
  reader->GenerateOutputInformation();
  unsigned int nbComponents = reader->GetOutput()->GetNumberOfComponentsPerPixel();

    // min & max
  VectorPixelType min(nbComponents);
  VectorPixelType max(nbComponents);

  unsigned int channel = atoi(argv[3]);

  for(unsigned int i = 0; i<nbComponents;++i)
    {
    min[i]=atof(argv[5+i]);
    max[i]=atof(argv[5+nbComponents+i]);
    }
 
  // rendering
  rendering->SetInput(reader->GetOutput());
  rendering->SetRenderingFunction(function);
  function->SetMinimum(min);
  function->SetMaximum(max);
  function->SetAllChannels(channel);
  function->GetTransferFunction().SetFactor(atof(argv[4]));

  // writing
  writer->SetFileName(argv[2]);
  writer->SetInput(rendering->GetOutput());
  writer->Update();


  return EXIT_SUCCESS;
}
