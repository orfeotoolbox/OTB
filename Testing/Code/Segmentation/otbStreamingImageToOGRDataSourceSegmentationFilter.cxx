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
#include "otbStreamingImageToOGRLayerSegmentationFilter.h"
#include "otbImageFileReader.h"

#include "otbMeanShiftVectorImageFilter.h"
#include "otbMeanShiftImageFilter.h"


#include "otbPersistentImageToOGRDataFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"

#include "otbOGRDataSourceWrapper.h"

int otbStreamingImageToOGRLayerSegmentationFilterNew(int argc, char * argv[])
{
  typedef float InputPixelType;
  const unsigned int Dimension = 2;

  /** Typedefs */
  typedef otb::Image<InputPixelType,  Dimension>          ImageType;
  typedef otb::MeanShiftImageFilter<ImageType, ImageType> MeanShiftImageFilterType;
  typedef otb::StreamingImageToOGRLayerSegmentationFilter<ImageType, MeanShiftImageFilterType>::FilterType StreamingImageToOGRLayerSegmentationFilterType;

  StreamingImageToOGRLayerSegmentationFilterType::Pointer filter = StreamingImageToOGRLayerSegmentationFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}

int otbStreamingImageToOGRLayerSegmentationFilter(int argc, char * argv[])
{

  if (argc != 13)
    {
      std::cerr << "Usage: " << argv[0];
      std::cerr << " inputImage maskImage outputVec layerName TileDimension"
               << "spatialRadius rangeRadius minObjectSize filterSmallObj minSize"
               << "SimplifyFlag Tolerance" << std::endl;
      return EXIT_FAILURE;
    }

  const char * imageName                    = argv[1];
  const char * maskName                     = argv[2];
  const char * dataSourceName               = argv[3];
  const char * layerName                    = argv[4];
  const unsigned int tileSize               = atoi(argv[5]);
  const unsigned int spatialRadiusOldMS     = atoi(argv[6]);
  const double rangeRadiusOldMS             = atof(argv[7]);
  const unsigned int minimumObjectSizeOldMS = atoi(argv[8]);
  const bool filterSmallObj                 = atoi(argv[9]);
  const unsigned int minSize                = atoi(argv[10]);
  const bool simplify                       = atoi(argv[11]);
  const double tolerance                    = atof(argv[12]);


  typedef float InputPixelType;
  const unsigned int Dimension = 2;
  const std::string fieldName("DN");

  // Typedefs
  typedef otb::VectorImage<InputPixelType,  Dimension>    ImageType;
  typedef otb::Image<unsigned int, Dimension>             LabelImageType;

  //old mean shift filter
  typedef otb::MeanShiftVectorImageFilter<ImageType, ImageType, LabelImageType> SegmentationFilterType;
  typedef otb::StreamingImageToOGRLayerSegmentationFilter<ImageType, SegmentationFilterType> StreamingImageToOGRLayerSegmentationFilterType;
  typedef otb::ImageFileReader<ImageType>                      ReaderType;
  typedef otb::ImageFileReader<LabelImageType>                 MaskReaderType;

  ReaderType::Pointer             reader = ReaderType::New();
  MaskReaderType::Pointer         maskReader = MaskReaderType::New();
  StreamingImageToOGRLayerSegmentationFilterType::Pointer filter = StreamingImageToOGRLayerSegmentationFilterType::New();

  reader->SetFileName(imageName);
  reader->UpdateOutputInformation();
  
  maskReader->SetFileName(maskName);
  maskReader->UpdateOutputInformation();
  
  otb::ogr::DataSource::Pointer ogrDS = otb::ogr::DataSource::New(dataSourceName, otb::ogr::DataSource::Modes::write);

  filter->SetInput(reader->GetOutput());
  filter->SetInputMask(maskReader->GetOutput());
//  filter->SetOGRLayer(ogrDS);
  //filter->GetStreamer()->SetNumberOfLinesStrippedStreaming(atoi(argv[3]));
  filter->GetStreamer()->SetTileDimensionTiledStreaming(tileSize);
  //filter->SetFieldName(fieldName);
  filter->SetStartLabel(1);
  filter->SetUse8Connected(false);
  filter->SetFilterSmallObject(filterSmallObj);
  filter->SetMinimumObjectSize(minSize);
  filter->SetSimplify(simplify);
  filter->SetSimplificationTolerance(tolerance);
  filter->GetSegmentationFilter()->SetSpatialRadius(spatialRadiusOldMS);
  filter->GetSegmentationFilter()->SetRangeRadius(rangeRadiusOldMS);
  filter->GetSegmentationFilter()->SetMinimumRegionSize(minimumObjectSizeOldMS);


  filter->Initialize(); //must be called !

  filter->Update();

  return EXIT_SUCCESS;
}
