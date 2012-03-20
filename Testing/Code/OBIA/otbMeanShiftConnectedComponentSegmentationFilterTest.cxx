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
#include "otbMeanShiftImageFilter2.h"
#include "otbMeanShiftConnectedComponentSegmentationFilter.h"

typedef float InputPixelType;
const unsigned int Dimension = 2;
typedef double                                          KernelType;
typedef otb::Image<unsigned int, Dimension>             LabelImageType;
typedef otb::Image<unsigned int, Dimension>             MaskImageType;


typedef otb::VectorImage<InputPixelType, Dimension>     ImageType;
typedef otb::ImageFileReader<ImageType>                 ReaderType;
typedef otb::ImageFileWriter<LabelImageType>            WriterType;


typedef otb::MeanShiftConnectedComponentSegmentationFilter
  < ImageType, MaskImageType,
    LabelImageType >  MeanShiftConnectedComponentsegmentationFilterType;

typedef otb::MeanShiftImageFilter2<ImageType, ImageType,ImageType,KernelType> MeanShiftFilterType;


int otbMeanShiftConnectedComponentSegmentationFilter(int argc, char * argv[])
{

  /* mean shift parameters */

  const char * infname = argv[1];
  const char * outputFilename = argv[2];

  /* mean shift parameters */
  const unsigned int spatialRadius = atoi(argv[3]);
  const unsigned int spectralRadius = atoi(argv[4]);
  const double spectralbandwidth = atof(argv[5]);
  const double spatialbandwidth = atof(argv[6]);
  const double threshold = atof(argv[7]);

  /* conencted component parameters */

  const char * maskexpression = argv[8];
  const char * segmentationexpression = argv[9];
  unsigned int minobjectsize = atoi(argv[10]);

   // add meanshift options


  // Instantiating object
  MeanShiftFilterType::Pointer meanShiftFilter = MeanShiftFilterType::New();
  MeanShiftConnectedComponentsegmentationFilterType::Pointer segmentationFilter= MeanShiftConnectedComponentsegmentationFilterType::New();
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(infname);

  //define square radius
  ImageType::SizeType radius;
  radius[0] = spatialRadius;
  radius[1] = spatialRadius;

  meanShiftFilter->SetSpatialRadius(radius);

  radius[0] = spectralRadius;
  radius[1] = spectralRadius;
  meanShiftFilter->SetRangeRadius(radius);
  meanShiftFilter->SetSpectralBandwidth(spectralbandwidth);
  meanShiftFilter->SetSpatialBandwidth(spatialbandwidth);
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
