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
#include "otbROIdataConversion.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include <iostream>

int otbROIdataConversion(int itkNotUsed(argc), char* argv[])
{
  typedef double PixelType;
  const unsigned int Dimension = 2;
  typedef otb::Image<PixelType,  Dimension>                      InputImageType;
  typedef otb::Image<PixelType,  Dimension>                      ROIImageType;
  typedef otb::ROIdataConversion<InputImageType, ROIImageType> ConverterType;
  typedef ConverterType::OutputImageType                         OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileReader<ROIImageType>    ROIReaderType;

  ConverterType::Pointer converter = ConverterType::New();
  ReaderType::Pointer    readerIm = ReaderType::New();
  ReaderType::Pointer    readerTr = ROIReaderType::New();

  readerIm->SetFileName(argv[1]);
  readerTr->SetFileName(argv[2]);
  readerIm->Update();
  readerTr->Update();

  converter->SetInput(readerIm->GetOutput());
  converter->SetROIImage(readerTr->GetOutput());
  converter->Update();

  std::ofstream file;
  file.open(argv[3]);
  itk::ImageRegionIterator<OutputImageType> outputIter(converter->GetOutput(),
                                                       converter->GetOutput()->GetLargestPossibleRegion());
  outputIter.GoToBegin();

  while (!outputIter.IsAtEnd())
    {
    file << outputIter.Get() << "  ";
    ++outputIter;
    }
  file << std::endl;
  file.close();

  return EXIT_SUCCESS;
}
