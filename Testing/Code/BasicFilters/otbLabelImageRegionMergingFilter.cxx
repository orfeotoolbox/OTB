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
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMeanShiftSmoothingImageFilter.h"
#include "otbLabelImageRegionMergingFilter.h"

int otbLabelImageRegionMergingFilter(int argc, char * argv[])
{
  if (argc != 10)
    {
    std::cerr << "Usage: " << argv[0] <<
    " infname spectralfname labelfname labelmergedfname clusteredfname spatialBandwidth rangeBandwidth threshold maxiterationnumber"
              << std::endl;
    return EXIT_FAILURE;
    }

  const char *       infname                   = argv[1];
  const char *       spectralfname             = argv[2];
  const char *       labelfname                = argv[3];
  const char *       labelmergedfname          = argv[4];
  const char *       clusteredfname            = argv[5];
  const double       spatialBandwidth          = atof(argv[6]);
  const double       rangeBandwidth            = atof(argv[7]);
  const double       threshold                 = atof(argv[8]);
  const unsigned int maxiterationnumber        = atoi(argv[9]);
  /* maxit - threshold */

  const unsigned int Dimension = 2;
  typedef float                                            PixelType;
  typedef otb::VectorImage<PixelType, Dimension>           ImageType;
  typedef otb::ImageFileReader<ImageType>                  ReaderType;
  typedef otb::ImageFileWriter<ImageType>                  WriterType;
  typedef otb::MeanShiftSmoothingImageFilter<ImageType, ImageType> FilterType;
  typedef FilterType::OutputLabelImageType                 LabelImageType;
  typedef otb::ImageFileWriter<LabelImageType>             LabelWriterType;

  typedef otb::LabelImageRegionMergingFilter<LabelImageType, ImageType> MergeFilterType;

  // Instantiating object
  FilterType::Pointer filter = FilterType::New();
  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(infname);

  // Set filter parameters
  filter->SetSpatialBandwidth(spatialBandwidth);
  filter->SetRangeBandwidth(rangeBandwidth);
  filter->SetThreshold(threshold);
  filter->SetMaxIterationNumber(maxiterationnumber);
  filter->SetInput(reader->GetOutput());
  //filter->SetNumberOfThreads(1);
  WriterType::Pointer writer2 = WriterType::New();
  LabelWriterType::Pointer writer5 = LabelWriterType::New();

  writer2->SetFileName(spectralfname);
  writer5->SetFileName(labelfname);

  writer2->SetInput(filter->GetRangeOutput());
  writer5->SetInput(filter->GetLabelOutput());

  MergeFilterType::Pointer mergeFilter = MergeFilterType::New();
  mergeFilter->SetInputLabelImage(filter->GetLabelOutput());
  mergeFilter->SetInputSpectralImage(filter->GetRangeOutput());
  mergeFilter->SetRangeBandwidth(rangeBandwidth);
  LabelWriterType::Pointer mergeWriter = LabelWriterType::New();
  mergeWriter->SetFileName(labelmergedfname);
  mergeWriter->SetInput(mergeFilter->GetOutput());

  WriterType::Pointer clusteredWriter = WriterType::New();
  clusteredWriter->SetFileName(clusteredfname);
  clusteredWriter->SetInput(mergeFilter->GetClusteredOutput());

  writer2->Update();
  writer5->Update();
  mergeWriter->Update();
  clusteredWriter->Update();

  return EXIT_SUCCESS;
}
