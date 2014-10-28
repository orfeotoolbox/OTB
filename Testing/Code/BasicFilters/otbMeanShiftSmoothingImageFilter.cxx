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
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMeanShiftSmoothingImageFilter.h"

int otbMeanShiftSmoothingImageFilter(int argc, char * argv[])
{
  if (argc != 10 && argc != 11)
    {
    std::cerr << "Usage: " << argv[0] <<
    " infname spatialfname spectralfname iterationfname labelfname spatialBandwidth rangeBandwidth threshold maxiterationnumber (usemodesearch)"
              << std::endl;
    return EXIT_FAILURE;
    }

  const char *       infname                   = argv[1];
  const char *       spatialfname              = argv[2];
  const char *       spectralfname             = argv[3];
  const char *       iterationfname            = argv[4];
  const char *       labelfname                = argv[5];
  const double       spatialBandwidth          = atof(argv[6]);
  const double       rangeBandwidth            = atof(argv[7]);
  const double       threshold                 = atof(argv[8]);
  const unsigned int maxiterationnumber        = atoi(argv[9]);
  bool               usemodesearch                 = true;
  if(argc==11)
    {
      usemodesearch        = atoi(argv[10])!=0;
    }

  /* maxit - threshold */

  const unsigned int Dimension = 2;
  typedef float                                            PixelType;
  typedef otb::VectorImage<PixelType, Dimension>           ImageType;
  typedef otb::ImageFileReader<ImageType>                  ReaderType;
  typedef otb::ImageFileWriter<ImageType>                  WriterType;
  typedef otb::MeanShiftSmoothingImageFilter<ImageType, ImageType> FilterType;
  typedef FilterType::OutputIterationImageType             IterationImageType;
  typedef otb::ImageFileWriter<IterationImageType>         IterationWriterType;
  typedef FilterType::OutputSpatialImageType               SpatialImageType;
  typedef otb::ImageFileWriter<SpatialImageType>           SpatialWriterType;
  typedef FilterType::OutputLabelImageType                 LabelImageType;
  typedef otb::ImageFileWriter<LabelImageType>             LabelWriterType;

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
  filter->SetModeSearch(usemodesearch);
  //filter->SetNumberOfThreads(1);
  SpatialWriterType::Pointer writer1 = SpatialWriterType::New();
  WriterType::Pointer writer2 = WriterType::New();
  IterationWriterType::Pointer writer4 = IterationWriterType::New();

  writer1->SetFileName(spatialfname);
  writer2->SetFileName(spectralfname);
  writer4->SetFileName(iterationfname);

  writer1->SetInput(filter->GetSpatialOutput());
  writer2->SetInput(filter->GetRangeOutput());
  writer4->SetInput(filter->GetIterationOutput());

 if (usemodesearch) //mode label image is only available with mode search optimization
    {
    LabelWriterType::Pointer writer5 = LabelWriterType::New();
    writer5->SetFileName(labelfname);
    writer5->SetInput(filter->GetLabelOutput());
    writer5->Update();
    }
  writer1->Update();
  writer2->Update();
  writer4->Update();

  return EXIT_SUCCESS;
}
