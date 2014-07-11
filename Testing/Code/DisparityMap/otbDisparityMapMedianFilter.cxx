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
#include "otbDisparityMapMedianFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbStandardWriterWatcher.h"

 const unsigned int Dimension = 2;
 typedef float                                                         PixelType;
 typedef otb::Image<PixelType, Dimension>                              FloatImageType;
 typedef otb::ImageFileReader<FloatImageType>                          ReaderType;
 typedef otb::ImageFileWriter<FloatImageType>                 FloatWriterType;

 typedef otb::DisparityMapMedianFilter<FloatImageType,FloatImageType,FloatImageType>   DisparityMapMedianFilterType;


int otbDisparityMapMedianFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
   // Instantiation
  DisparityMapMedianFilterType::Pointer filter = DisparityMapMedianFilterType::New();

   return EXIT_SUCCESS;
}

int otbDisparityMapMedianFilter(int argc, char* argv[])
{

  if ((argc != 5) && (argc != 6))
    {
    std::cerr << "Usage: " << argv[0] << " hdispinput_fname output_fname radius incoherencethres (maskinput_fname) ";
    return EXIT_FAILURE;
    }

  // Instantiation
  DisparityMapMedianFilterType::Pointer filter = DisparityMapMedianFilterType::New();

  ReaderType::Pointer imgReader = ReaderType::New();
  imgReader->SetFileName(argv[1]);

  filter->SetInput(imgReader->GetOutput());
  filter->SetRadius(atoi(argv[3]));
  filter->SetIncoherenceThreshold(atof(argv[4]));

  ReaderType::Pointer maskReader = ReaderType::New();

  if (argc > 5)
    {
    maskReader->SetFileName(argv[5]);
    filter->SetMaskInput(maskReader->GetOutput());
    }

  FloatWriterType::Pointer writer = FloatWriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(argv[2]);

  otb::StandardWriterWatcher watcher1(writer, filter, "filtering");
  writer->Update();

  return EXIT_SUCCESS;
}
