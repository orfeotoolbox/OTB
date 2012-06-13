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
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMeanShiftSmoothingImageFilter.h"

int otbMeanShiftSmoothingImageFilterThreading(int argc, char * argv[])
{
  if (argc != 7)
    {
    std::cerr << "Usage: " << argv[0] <<
    " inputFileName outputSingleThreadFileName outputMultiThreadFileName spatialBandwidth rangeBandwidth useModeSearch"
              << std::endl;
    return EXIT_FAILURE;
    }

  const char *       inputFileName              = argv[1];
  const char *       outputSingleThreadFileName = argv[2];
  const char *       outputMultiThreadFileName  = argv[3];
  const double       spatialBandwidth           = atof(argv[4]);
  const double       rangeBandwidth             = atof(argv[5]);
  bool               useModeSearch            = (atoi(argv[6])!=0);

  const unsigned int Dimension = 2;
  typedef float                                            PixelType;
  typedef double                                           KernelType;
  typedef otb::VectorImage<PixelType, Dimension>           ImageType;
  typedef otb::ImageFileReader<ImageType>                  ReaderType;
  typedef otb::ImageFileWriter<ImageType>                  WriterType;
  typedef otb::MeanShiftSmoothingImageFilter<ImageType, ImageType> FilterType;

  // Instantiating object
  FilterType::Pointer filterSingle = FilterType::New();
  FilterType::Pointer filterMulti = FilterType::New();
  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(inputFileName);

  // Set filter parameters
  filterSingle->SetSpatialBandwidth(spatialBandwidth);
  filterSingle->SetRangeBandwidth(rangeBandwidth);
  filterSingle->SetInput(reader->GetOutput());
  filterSingle->SetModeSearch(useModeSearch);
  filterSingle->SetNumberOfThreads(1);

  filterMulti->SetSpatialBandwidth(spatialBandwidth);
  filterMulti->SetRangeBandwidth(rangeBandwidth);
  filterMulti->SetInput(reader->GetOutput());
  filterMulti->SetModeSearch(useModeSearch);

  WriterType::Pointer writerSingle = WriterType::New();
  WriterType::Pointer writerMulti  = WriterType::New();

  writerSingle->SetFileName(outputSingleThreadFileName);
  writerMulti->SetFileName(outputMultiThreadFileName);

  writerSingle->SetInput(filterSingle->GetRangeOutput());
  writerMulti->SetInput(filterMulti->GetRangeOutput());

  writerSingle->Update();
  writerMulti->Update();

  return EXIT_SUCCESS;
}
