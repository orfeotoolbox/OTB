/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "itkMacro.h"
#include "itkPolyLineParametricPath.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "otbImageToEdgePathFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMacro.h"

int otbImageToEdgePathFilter(int itkNotUsed(argc), char* argv[])
{
  const char*        inputFilename  = argv[1];
  const char*        outputFilename = argv[2];
  const unsigned int foreground     = atoi(argv[3]);

  const unsigned int    Dimension = 2;
  typedef unsigned char PixelType;
  typedef otb::Image<PixelType, Dimension> ImageType;
  typedef ImageType::IndexType IndexType;

  typedef itk::PolyLineParametricPath<Dimension> PathType;
  typedef otb::ImageFileReader<ImageType>        ReaderType;
  typedef otb::ImageFileWriter<ImageType>        WriterType;

  typedef otb::ImageToEdgePathFilter<ImageType, PathType> ImageToEdgePathFilterType;

  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(inputFilename);

  ImageToEdgePathFilterType::Pointer pathFilter = ImageToEdgePathFilterType::New();
  PixelType                          foregroundValue(foreground);
  pathFilter->SetForegroundValue(foregroundValue);
  pathFilter->SetInput(reader->GetOutput());

  pathFilter->Update();

  otbLogMacro(Debug, << " pathFilter = " << pathFilter);

  PathType* outputPath = pathFilter->GetOutput();

  typedef PathType::VertexType         VertexType;
  typedef PathType::VertexListType     VertexListType;
  typedef VertexListType::ConstPointer VertexListTypePointer;

  VertexListTypePointer vertexList;
  VertexType            cindex;

  VertexListTypePointer ptrVertexList = outputPath->GetVertexList();

  otbLogMacro(Info, << "Size : " << ptrVertexList->Size());

  // Initialize Output Image
  ImageType::Pointer                                   outputImage = ImageType::New();
  typedef itk::ImageRegionIteratorWithIndex<ImageType> IteratorType;
  outputImage->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
  outputImage->Allocate();
  outputImage->FillBuffer(255);
  // Create one iterator for the Input Image (this is a light object)
  IteratorType it(outputImage, outputImage->GetBufferedRegion());

  for (unsigned int cpt = 0; cpt < ptrVertexList->Size(); ++cpt)
  {
    otbLogMacro(Debug, << " Point " << cpt << " : " << ptrVertexList->GetElement(cpt));
    IndexType pos;
    pos[0] = static_cast<unsigned long>(ptrVertexList->GetElement(cpt)[0]);
    pos[1] = static_cast<unsigned long>(ptrVertexList->GetElement(cpt)[1]);
    outputImage->SetPixel(pos, 0);
  }

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(outputImage);
  writer->SetFileName(outputFilename);
  writer->Update();

  /*        VertexListConstIterator iter=ptrVertexList->Begin();
      while( iter != ptrVertexList->End() )
            {
      //            it.Set( 255 );
      std::cout <<" VERTEX : "<<(*iter).GetElement()<<std::endl;
            ++iter;
            }
  */

  /*     x1 = cindex[0];
     y1 = cindex[1];
     cindex = vertexList->GetElement(1);
     x2 = cindex[0];
     y2 = cindex[1];
     fprintf(file,"%8.4f %8.3f\n", x1, y1);
  */

  return EXIT_SUCCESS;
}
