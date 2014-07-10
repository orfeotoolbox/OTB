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
#include "itkRGBPixel.h"
#include "otbImage.h"
#include "otbTileMapTransform.h"
#include "otbOrthoRectificationFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkPoint.h"
#include "otbImageRegionTileMapSplitter.h"
#include "otbTileMapImageIO.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbPerBandVectorImageFilter.h"
#include "otbPrintableImageFilter.h"
#include "otbMultiChannelExtractROI.h"

int otbTileMapWriter(int argc, char *argv[])
{
  if (argc != 4 && argc != 8)
    {
    std::cerr << "Usage: " << argv[0]
              << "<image> <SRTM directory> <output directory> <extract star index X (optional)> <extract star index Y(optional)> <extract star index X(optional)> <extract star index Y(optional)>" << std::endl;
    return EXIT_FAILURE;

    }

  int depth = 14;

  typedef otb::Image<unsigned int, 2>  ImageType;
  typedef otb::Image<double, 2>        DoubleImageType;

  typedef otb::VectorImage<unsigned int, 2>  VectorImageType;
  typedef otb::VectorImage<unsigned char, 2> CharVectorImageType;
  typedef otb::VectorImage<double, 2>        DoubleVectorImageType;

  typedef otb::ImageFileReader<VectorImageType> VectorReaderType;
  VectorReaderType::Pointer readerXS = VectorReaderType::New();
  readerXS->SetFileName(argv[1]);

  typedef otb::TileMapTransform<otb::TransformDirection::INVERSE> MapProjectionType;
  typedef otb::OrthoRectificationFilter
  <ImageType, DoubleImageType, MapProjectionType> OrthoRectifFilterType;
  typedef otb::ImageFileWriter<CharVectorImageType> WriterType;

  otb::DEMHandler::Instance()->OpenDEMDirectory(argv[2]);

  MapProjectionType::Pointer mapProjection = MapProjectionType::New();
  mapProjection->SetLevel(depth);

  ImageType::IndexType start;
  start[0] = 0;
  start[1] = 0;

  ImageType::SizeType size;
  size[0] = 2000;
  size[1] = 2000;

  ImageType::SpacingType spacing;
  spacing[0] = 1;
  spacing[1] = 1;

  typedef itk::Point <double, 2> PointType;
  PointType lonLatUL;
//   lonLatUL[0] = 103.843; //Sentosa
//   lonLatUL[1] = 1.23037;
//   lonLatUL[1] = 1.30164; //Tests pour matching google (Orchard Road)
//   lonLatUL[0] =   103.838;
//   lonLatUL[1] = 1.35610168284995; //Tests streaming (coin UL)
//   lonLatUL[0] = 103.760262770741;
//   lonLatUL[1] = 43.5596826111; //Tests Toulouse
//   lonLatUL[0] = 1.4835345;
//   lonLatUL[0] = -68.1; //Tests LaPaz
//   lonLatUL[1] = -16.5;
  lonLatUL[0] = 51.15; //Tests Iran spot5
  lonLatUL[1] = 35.9;

  typedef otb::TileMapTransform<otb::TransformDirection::FORWARD> MapProjectionForwardType;
  MapProjectionForwardType::Pointer mapProjectionForward = MapProjectionForwardType::New();
  mapProjectionForward->SetLevel(depth);
  PointType pointULexact;
  pointULexact = mapProjectionForward->TransformPoint(lonLatUL);

  PointType pointLRexact;
  pointLRexact[0] = pointULexact[0] + size[0] * spacing[0];
  pointLRexact[1] = pointULexact[1] + size[1] * spacing[1];

  PointType lonLatLR;
  lonLatLR = mapProjection->TransformPoint(pointLRexact);

  typedef otb::PerBandVectorImageFilter<VectorImageType,
      DoubleVectorImageType, OrthoRectifFilterType> VectorOrthoRectifFilterType;

  OrthoRectifFilterType::Pointer orthoRectifXS =
    OrthoRectifFilterType::New();
  VectorOrthoRectifFilterType::Pointer orthoRectifXSVector =
    VectorOrthoRectifFilterType::New();
  orthoRectifXSVector->SetFilter(orthoRectifXS);

  orthoRectifXS->SetMapProjection(mapProjection);

  orthoRectifXSVector->SetInput(readerXS->GetOutput());

  orthoRectifXS->SetOutputStartIndex(start);
  orthoRectifXS->SetOutputSize(size);
  orthoRectifXS->SetOutputSpacing(spacing);
  orthoRectifXS->SetOutputOrigin(pointULexact);

  typedef otb::MultiChannelExtractROI<DoubleVectorImageType::InternalPixelType, DoubleVectorImageType::InternalPixelType> ExtractorType;
  ExtractorType::Pointer extractor = ExtractorType::New();

  typedef otb::PrintableImageFilter<DoubleVectorImageType> PrintableImageFilterType;
  PrintableImageFilterType::Pointer printable = PrintableImageFilterType::New();
  printable->SetChannel(1);
  printable->SetChannel(2);
  printable->SetChannel(3);

  if(argc==8)
    {
      extractor->SetStartX(atoi(argv[4]));
      extractor->SetStartY(atoi(argv[5]));
      extractor->SetSizeX(atoi(argv[6]));
      extractor->SetSizeY(atoi(argv[7]));
      extractor->SetInput(orthoRectifXSVector->GetOutput());

      printable->SetInput(extractor->GetOutput());

    }
  else
    {
      printable->SetInput(orthoRectifXSVector->GetOutput());
    }


  typedef otb::ImageRegionTileMapSplitter<2> SplitterType;
  SplitterType::Pointer splitter = SplitterType::New();

  typedef otb::TileMapImageIO ImageIOType;
  ImageIOType::Pointer tileMapIO = ImageIOType::New();
  tileMapIO->SetDepth(depth);
  tileMapIO->SetCacheDirectory(argv[3]);
  tileMapIO->SetFileSuffix("jpg");

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("test.otb");
  writer->SetImageIO(tileMapIO);
  writer->SetNumberOfThreads(1);
  writer->SetInput(printable->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
