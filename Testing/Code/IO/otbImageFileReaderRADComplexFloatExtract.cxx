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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"
#include <iostream>
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbExtractROI.h"
#include "itkComplexToRealImageFilter.h"
#include "itkComplexToImaginaryImageFilter.h"
#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"


int otbImageFileReaderRADComplexFloatExtract(int argc, char* argv[])
{
  // Verify the number of parameters in the command line
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  typedef std::complex<float> InputPixelType;
  typedef std::complex<float> OutputPixelType;
  
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputCplxImageType;
  typedef otb::Image<int, Dimension>             OutputScalarImageType;
  typedef otb::VectorImage<int, Dimension>       OutputImageType;
  typedef otb::ImageList<OutputScalarImageType>  ImageListType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  typedef itk::ComplexToRealImageFilter<OutputCplxImageType, OutputScalarImageType> RealExtractorType;
  typedef itk::ComplexToImaginaryImageFilter<OutputCplxImageType
, OutputScalarImageType> ImaginaryExtractorType;
  typedef otb::ImageListToVectorImageFilter<ImageListType, OutputImageType>           ListToVectorImageFilterType;


  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  typedef otb::ExtractROI<InputPixelType,
      OutputPixelType>  ExtractROIFilterType;

  ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();

  extractROIFilter->SetInput(reader->GetOutput());

  extractROIFilter->SetStartX(10);
  extractROIFilter->SetStartY(10);
  extractROIFilter->SetSizeX(100);
  extractROIFilter->SetSizeY(100);

  // The rest of the code is here to translate the image complexe into
  // a VectorImage of int which each channel is the real and imagynary part

  RealExtractorType::Pointer realExt = RealExtractorType::New();
  ImaginaryExtractorType::Pointer imgExt = ImaginaryExtractorType::New();

  realExt->SetInput(extractROIFilter->GetOutput());
  imgExt->SetInput(extractROIFilter->GetOutput());

  ImageListType::Pointer imList = ImageListType::New();
  imList->PushBack(realExt->GetOutput());
  imList->PushBack(imgExt->GetOutput());
  ListToVectorImageFilterType::Pointer listCaster = ListToVectorImageFilterType::New();
  listCaster->SetInput(imList);

  writer->SetInput(listCaster->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
