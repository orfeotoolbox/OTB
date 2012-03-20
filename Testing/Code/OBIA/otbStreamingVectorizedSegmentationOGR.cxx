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

  typedef float InputPixelType;
  const unsigned int Dimension = 2;
  const std::string fieldName("DN");

  // Typedefs
  typedef otb::VectorImage<InputPixelType,  Dimension>    ImageType;
  //typedef otb::Image<InputPixelType,  Dimension>          ImageType;
  typedef otb::Image<unsigned int, Dimension>             LabelImageType;
  typedef otb::MeanShiftVectorImageFilter<ImageType, ImageType, LabelImageType> MeanShiftImageFilterType;
  //typedef otb::MeanShiftImageFilter<ImageType, ImageType, LabelImageType> MeanShiftImageFilterType;
  
  typedef otb::Functor::ConnectedComponentMuParserFunctor<ImageType::PixelType>  FunctorType;
  typedef itk::ConnectedComponentFunctorImageFilter<ImageType, LabelImageType, FunctorType, LabelImageType > SegmentationFilterType;
  //typedef otb::StreamingVectorizedSegmentationOGR<ImageType, SegmentationFilterType> StreamingVectorizedSegmentationOGRType;
  typedef otb::StreamingVectorizedSegmentationOGR<ImageType, MeanShiftImageFilterType> StreamingVectorizedSegmentationOGRType;

  typedef otb::ImageFileReader<ImageType>                      ReaderType;


  ReaderType::Pointer             reader = ReaderType::New();
  StreamingVectorizedSegmentationOGRType::Pointer filter = StreamingVectorizedSegmentationOGRType::New();


  reader->SetFileName(argv[1]);
  reader->GenerateOutputInformation();
  filter->SetInput(reader->GetOutput());
  //filter->GetStreamer()->SetNumberOfLinesStrippedStreaming(atoi(argv[3]));
  filter->GetStreamer()->SetTileDimensionTiledStreaming(atoi(argv[3]));
  filter->SetFieldName(fieldName);
  filter->SetStartLabel(1);
  filter->GetSegmentationFilter()->SetSpatialRadius(10);
  filter->GetSegmentationFilter()->SetRangeRadius(15);
  filter->GetSegmentationFilter()->SetMinimumRegionSize(400);
  //filter->GetSegmentationFilter()->GetFunctor().SetExpression("distance<15");
  
  filter->SetFileName(argv[2]);
  filter->Initialize(); //must do this after SetFileName ... 
  
  filter->Update();

  return EXIT_SUCCESS;
}
