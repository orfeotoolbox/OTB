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
#include "otbStreamingImageFileWriter.h"
#include "itkPoint.h"
#include "otbImageRegionTileMapSplitter.h"
#include "otbTileMapImageIO.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbPerBandVectorImageFilter.h"
#include "otbPrintableImageFilter.h"


int otbTileMapWriter(int argc, char *argv[])
{
  if (argc != 3)
  {
    std::cerr << "Usage: " << argv[0]
              << "<image> <output directory>" << std::endl;

  }

  int depth=14;
  
  typedef otb::Image<unsigned int, 2>     ImageType;
  typedef otb::Image<unsigned char, 2>     CharImageType;
  typedef otb::Image<double, 2>     DoubleImageType;
  
  typedef otb::VectorImage<unsigned int, 2>     VectorImageType;
  typedef otb::VectorImage<unsigned char, 2>     CharVectorImageType;
  typedef otb::VectorImage<double, 2>     DoubleVectorImageType;
  
  typedef itk::RGBPixel<unsigned char> RGBPixelType;
  typedef otb::Image<RGBPixelType, 2> RGBImageType;

  typedef otb::ImageFileReader<VectorImageType>  VectorReaderType;
  VectorReaderType::Pointer     readerXS=VectorReaderType::New();
  readerXS->SetFileName(argv[1]);
  
  typedef otb::TileMapTransform<otb::INVERSE> MapProjectionType;
  typedef otb::OrthoRectificationFilter
            <ImageType, DoubleImageType,MapProjectionType> OrthoRectifFilterType;
  typedef otb::StreamingImageFileWriter<CharVectorImageType>  WriterType;
  
//   OrthoRectifFilterType::Pointer  orthoRectifPAN =
//       OrthoRectifFilterType::New();
  
  MapProjectionType::Pointer mapProjection = MapProjectionType::New();
  mapProjection->SetLevel(depth);
//   orthoRectifPAN->SetMapProjection(mapProjection);
  
  ImageType::IndexType start;
  start[0]=0;
  start[1]=0;
//   orthoRectifPAN->SetOutputStartIndex(start);
                                    
  ImageType::SizeType size;
  size[0]=2000;
  size[1]=2000;
//   orthoRectifPAN->SetSize(size);
                                    
  ImageType::SpacingType spacing;
  spacing[0]=1;
  spacing[1]=1;
//   orthoRectifPAN->SetOutputSpacing(spacing);
                                    
//   orthoRectifPAN->SetInput(reader->GetOutput());
  
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
  
  typedef otb::TileMapTransform<otb::FORWARD> MapProjectionForwardType;
  MapProjectionForwardType::Pointer mapProjectionForward = MapProjectionForwardType::New();
  mapProjectionForward->SetLevel(depth);
  PointType pointULexact;
  pointULexact = mapProjectionForward->TransformPoint(lonLatUL);

  PointType pointLRexact;
  pointLRexact[0]=pointULexact[0]+size[0]*spacing[0];
  pointLRexact[1]=pointULexact[1]+size[1]*spacing[1];
      
  PointType lonLatLR;
  lonLatLR = mapProjection->TransformPoint(pointLRexact);

  typedef otb::PerBandVectorImageFilter<VectorImageType,
  DoubleVectorImageType, OrthoRectifFilterType> VectorOrthoRectifFilterType;

  OrthoRectifFilterType::Pointer  orthoRectifXS =
      OrthoRectifFilterType::New();
  VectorOrthoRectifFilterType::Pointer  orthoRectifXSVector =
      VectorOrthoRectifFilterType::New();
  orthoRectifXSVector->SetFilter(orthoRectifXS);

  orthoRectifXS->SetMapProjection(mapProjection);

  orthoRectifXSVector->SetInput(readerXS->GetOutput());

  orthoRectifXS->SetOutputStartIndex(start);
  orthoRectifXS->SetSize(size);
  orthoRectifXS->SetOutputSpacing(spacing);
  orthoRectifXS->SetOutputOrigin(pointULexact);
  orthoRectifXS->SetDEMDirectory("/home/christop/data/SRTM");
  
  typedef otb::PrintableImageFilter<DoubleVectorImageType> PrintableImageFilterType;
  PrintableImageFilterType::Pointer printable = PrintableImageFilterType::New();
  printable->SetInput(orthoRectifXSVector->GetOutput());
  printable->SetChannel(3);
  printable->SetChannel(2);
  printable->SetChannel(1); 
  
  
  typedef otb::ImageRegionTileMapSplitter<2> SplitterType;
  SplitterType::Pointer splitter = SplitterType::New();
  
  typedef otb::TileMapImageIO ImageIOType;
  ImageIOType::Pointer tileMapIO = ImageIOType::New();
  tileMapIO->SetDepth(depth);
  tileMapIO->SetCacheDirectory(argv[2]);
  
  
  WriterType::Pointer        writer=WriterType::New();
   writer->SetFileName("test.otb");
   writer->SetImageIO(tileMapIO);
  writer->SetNumberOfThreads(1);
  writer->SetInput(printable->GetOutput());
//   writer->SetRegionSplitter(splitter);
// // // //   writer->SetNumberOfStreamDivisions(2);
  writer->Update(); 
  
  std::cout << printable->GetOutput()->GetNumberOfComponentsPerPixel() << std::endl;
  
  
  return 0;
}
