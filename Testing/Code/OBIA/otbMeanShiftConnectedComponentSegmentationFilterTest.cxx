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
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMeanShiftSmoothingImageFilter.h"
#include "otbMeanShiftConnectedComponentSegmentationFilter.h"

typedef float InputPixelType;
const unsigned int Dimension = 2;
typedef otb::Image<unsigned int, Dimension>             LabelImageType;
typedef otb::Image<unsigned int, Dimension>             MaskImageType;


typedef otb::VectorImage<InputPixelType, Dimension>     ImageType;
typedef otb::ImageFileReader<ImageType>                 ReaderType;
typedef otb::ImageFileWriter<LabelImageType>            WriterType;


typedef otb::MeanShiftConnectedComponentSegmentationFilter
  < ImageType, MaskImageType,
    LabelImageType >  MeanShiftConnectedComponentsegmentationFilterType;

typedef otb::MeanShiftSmoothingImageFilter<ImageType, ImageType> MeanShiftFilterType;


int otbMeanShiftConnectedComponentSegmentationFilter(int itkNotUsed(argc), char * argv[])
{

  /* mean shift parameters */

  const char * infname = argv[1];
  const char * outputFilename = argv[2];

  /* mean shift parameters */
  const double spatialBandwidth = atof(argv[3]);
  const double rangeBandwidth = atof(argv[4]);
  const double threshold = atof(argv[5]);

  /* conencted component parameters */

  const char * maskexpression = argv[6];
  const char * segmentationexpression = argv[7];
  unsigned int minobjectsize = atoi(argv[8]);

   // add meanshift options


  // Instantiating object
  MeanShiftFilterType::Pointer meanShiftFilter = MeanShiftFilterType::New();
  MeanShiftConnectedComponentsegmentationFilterType::Pointer segmentationFilter= MeanShiftConnectedComponentsegmentationFilterType::New();
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(infname);

  meanShiftFilter->SetSpatialBandwidth(spatialBandwidth);

  meanShiftFilter->SetRangeBandwidth(rangeBandwidth);
  meanShiftFilter->SetThreshold(threshold);

  meanShiftFilter->SetInput(reader->GetOutput());
  meanShiftFilter->GetRangeOutput();

  // set the mean shift filter
  segmentationFilter->SetMeanShiftFilter(meanShiftFilter);


  segmentationFilter->SetInput(meanShiftFilter->GetRangeOutput());

  segmentationFilter->SetMaskExpression(maskexpression);
  segmentationFilter->SetConnectedComponentExpression(segmentationexpression);
  segmentationFilter->SetMinimumObjectSize(minobjectsize);

  //segmentationFilter->Update();

 writer->SetInput(segmentationFilter->GetOutput());
 writer->SetFileName(outputFilename);
 writer->Update();

  return EXIT_SUCCESS;
}
