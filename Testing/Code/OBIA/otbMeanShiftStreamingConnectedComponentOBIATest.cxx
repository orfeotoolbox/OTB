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
#include "otbVectorImage.h"
#include "otbVectorData.h"
#include "otbImageFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbMeanShiftSmoothingImageFilter.h"
#include "otbStreamingConnectedComponentSegmentationOBIAToVectorDataFilter.h"

typedef float InputPixelType;
const unsigned int Dimension = 2;

typedef otb::Image<unsigned int, Dimension>             LabelImageType;
typedef otb::Image<unsigned int, Dimension>             MaskImageType;

typedef otb::VectorData<double, Dimension>          VectorDataType;
typedef VectorDataType::Pointer                     VectorDataPointerType;
typedef otb::VectorDataFileWriter<VectorDataType>   VectorDataFileWriterType;
typedef VectorDataFileWriterType::Pointer           VectorDataFileWriterPointerType;

typedef otb::VectorImage<InputPixelType, Dimension>     ImageType;
typedef otb::ImageFileReader<ImageType>                 ReaderType;





typedef otb::StreamingConnectedComponentSegmentationOBIAToVectorDataFilter
  < ImageType,
    LabelImageType,
    MaskImageType,
    VectorDataType >  ConnectedComponentSegmentationOBIAToVectorDataFilterType;

typedef otb::MeanShiftSmoothingImageFilter<ImageType, ImageType> MeanShiftFilterType;


int otbMeanShiftStreamingConnectedComponentSegmentationOBIAToVectorDataFilter(int itkNotUsed(argc), char * argv[])
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
  const char * obiaexpression = argv[9];
  unsigned int nbstreams = atoi(argv[10]);

  // add meanshift options


  // Instantiating object
  MeanShiftFilterType::Pointer meanShiftFilter = MeanShiftFilterType::New();
  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(infname);

  meanShiftFilter->SetSpatialBandwidth(spatialBandwidth);
  meanShiftFilter->SetRangeBandwidth(rangeBandwidth);

  meanShiftFilter->SetThreshold(threshold);

  meanShiftFilter->SetInput(reader->GetOutput());
  meanShiftFilter->GetRangeOutput();

  ConnectedComponentSegmentationOBIAToVectorDataFilterType::FilterType::Pointer
      connected = ConnectedComponentSegmentationOBIAToVectorDataFilterType::FilterType::New();
  connected->GetFilter()->SetInput(meanShiftFilter->GetRangeOutput());

  connected->GetFilter()->SetMaskExpression(maskexpression);
  connected->GetFilter()->SetConnectedComponentExpression(segmentationexpression);
  connected->GetFilter()->SetMinimumObjectSize(minobjectsize);
  connected->GetFilter()->SetOBIAExpression(obiaexpression);

  connected->GetStreamer()->SetNumberOfDivisionsStrippedStreaming(nbstreams);
  connected->Update();

  VectorDataFileWriterPointerType vdwriter = VectorDataFileWriterType::New();
  vdwriter->SetInput(connected->GetFilter()->GetOutputVectorData());
  vdwriter->SetFileName(outputFilename);
  vdwriter->Update();

  return EXIT_SUCCESS;
}
