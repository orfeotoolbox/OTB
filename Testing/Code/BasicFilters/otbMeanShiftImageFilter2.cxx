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
#include "otbMeanShiftImageFilter2.h"

int otbMeanShiftImageFilter2(int argc, char * argv[])
{
  if (argc != 10)
    {
    std::cerr << "Usage: " << argv[0] <<
    " infname spatialfname spectralfname metricfname iterationfname spatialBandwidth rangeBandwidth threshold maxiterationnumber"
              << std::endl;
    return EXIT_FAILURE;
    }

  const char *       infname                   = argv[1];
  const char *       spatialfname              = argv[2];
  const char *       spectralfname             = argv[3];
  const char *       metricfname               = argv[4];
  const char *       iterationfname            = argv[5];
  const double       spatialBandwidth          = atof(argv[6]);
  const double       rangeBandwidth            = atof(argv[7]);
  const double       threshold                 = atof(argv[8]);
  const unsigned int maxiterationnumber        = atoi(argv[9]);
  /* maxit - threshold */

  const unsigned int Dimension = 2;
  typedef float                                           PixelType;
  typedef double                                          KernelType;
  typedef otb::VectorImage<PixelType, Dimension>          ImageType;
  typedef otb::ImageFileReader<ImageType>                 ReaderType;
  typedef otb::ImageFileWriter<ImageType>                 WriterType;
  typedef otb::MeanShiftImageFilter2<ImageType, ImageType> FilterType;
  typedef FilterType::OutputIterationImageType IterationImageType;
  typedef otb::ImageFileWriter<IterationImageType> IterationWriterType;

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
  WriterType::Pointer writer1 = WriterType::New();
  WriterType::Pointer writer2 = WriterType::New();
  WriterType::Pointer writer3 = WriterType::New();
  IterationWriterType::Pointer writer4 = IterationWriterType::New();


  writer1->SetFileName(spatialfname);
  writer2->SetFileName(spectralfname);
  writer3->SetFileName(metricfname);
  writer4->SetFileName(iterationfname);

  writer1->SetInput(filter->GetSpatialOutput());
  writer2->SetInput(filter->GetRangeOutput());
  writer3->SetInput(filter->GetMetricOutput());
  writer4->SetInput(filter->GetIterationOutput());

  writer1->Update();
  writer2->Update();
  writer3->Update();
  writer4->Update();

  return EXIT_SUCCESS;
}
