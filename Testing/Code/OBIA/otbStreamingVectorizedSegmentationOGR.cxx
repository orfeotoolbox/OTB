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
#include "otbStreamingVectorizedSegmentationOGR.h"
#include "otbImageFileReader.h"

#include "otbMeanShiftVectorImageFilter.h"
#include "otbMeanShiftImageFilter.h"
#include "itkConnectedComponentFunctorImageFilter.h"
#include "otbConnectedComponentMuParserFunctor.h"

#include "otbPersistentImageToOGRDataFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"

#include "otbMeanShiftImageFilter2.h"
#include "otbMeanShiftConnectedComponentSegmentationFilter.h"

int otbStreamingVectorizedSegmentationOGRNew(int argc, char * argv[])
{
  typedef float InputPixelType;
  const unsigned int Dimension = 2;

  /** Typedefs */
  typedef otb::Image<InputPixelType,  Dimension>          ImageType;
  typedef otb::MeanShiftImageFilter<ImageType, ImageType> MeanShiftImageFilterType;
  typedef otb::StreamingVectorizedSegmentationOGR<ImageType, MeanShiftImageFilterType>::FilterType StreamingVectorizedSegmentationOGRType;

  StreamingVectorizedSegmentationOGRType::Pointer filter = StreamingVectorizedSegmentationOGRType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}

int otbStreamingVectorizedSegmentationOGR(int argc, char * argv[])
{
  const unsigned int spatialRadius = 9;
  const unsigned int spectralRadius = 9;
  const double spectralbandwidth = 50;
  const double spatialbandwidth = 10;
  const double threshold = 0.001;

  /* conencted component parameters */

  const char * maskexpression = "";
  const char * segmentationexpression = "distance<0.2";
  unsigned int minobjectsize = 20;


  typedef float InputPixelType;
  const unsigned int Dimension = 2;
  const std::string fieldName("DN");

  // Typedefs
  typedef otb::VectorImage<InputPixelType,  Dimension>    ImageType;
  //typedef otb::Image<InputPixelType,  Dimension>          ImageType;
  typedef otb::Image<unsigned int, Dimension>             LabelImageType;
  typedef double                                          KernelType;
  
  //old mean shift filter
  typedef otb::MeanShiftVectorImageFilter<ImageType, ImageType, LabelImageType> SegmentationFilterType;
  //typedef otb::MeanShiftImageFilter<ImageType, ImageType, LabelImageType> SegmentationFilterType;
  
  
  //connectedComponnent filter
  /*typedef otb::Functor::ConnectedComponentMuParserFunctor<ImageType::PixelType>  FunctorType;
  typedef itk::ConnectedComponentFunctorImageFilter<ImageType, LabelImageType, FunctorType, LabelImageType > SegmentationFilterType;
  */
  
  //New mean shift filter
  /*typedef otb::MeanShiftConnectedComponentSegmentationFilter
  < ImageType, LabelImageType, LabelImageType >  SegmentationFilterType; */
  typedef otb::MeanShiftImageFilter2<ImageType,ImageType,ImageType,KernelType> MeanShiftFilterType;
  
  
  typedef otb::StreamingVectorizedSegmentationOGR<ImageType, SegmentationFilterType> StreamingVectorizedSegmentationOGRType;
  typedef otb::ImageFileReader<ImageType>                      ReaderType;

  ReaderType::Pointer             reader = ReaderType::New();
  StreamingVectorizedSegmentationOGRType::Pointer filter = StreamingVectorizedSegmentationOGRType::New();
  MeanShiftFilterType::Pointer meanShiftFilter = MeanShiftFilterType::New();


  
  reader->SetFileName(argv[1]);
  reader->GenerateOutputInformation();
  
  
  //New Filter mean shift
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
  meanShiftFilter->UpdateOutputInformation();

  /*filter->GetSegmentationFilter()->SetMeanShiftFilter(meanShiftFilter);
  filter->GetSegmentationFilter()->SetMaskExpression(maskexpression);
  filter->GetSegmentationFilter()->SetConnectedComponentExpression(segmentationexpression);
  filter->GetSegmentationFilter()->SetMinimumObjectSize(minobjectsize); */
  
  filter->SetInput(reader->GetOutput());
  //filter->GetStreamer()->SetNumberOfLinesStrippedStreaming(atoi(argv[3]));
  filter->GetStreamer()->SetTileDimensionTiledStreaming(atoi(argv[3]));
  filter->SetFieldName(fieldName);
  filter->SetStartLabel(1);
  filter->SetUse8Connected(false);
  filter->GetSegmentationFilter()->SetSpatialRadius(5);
  filter->GetSegmentationFilter()->SetRangeRadius(15);
  filter->GetSegmentationFilter()->SetMinimumRegionSize(100);
  //filter->GetSegmentationFilter()->GetFunctor().SetExpression("distance<15");
  
  filter->SetFileName(argv[2]);
  filter->Initialize(); //must do this after SetFileName and SetInput...
  
  filter->Update();

  return EXIT_SUCCESS;
}
