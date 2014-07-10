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
#include "itkPolyLineParametricPath.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "otbImageToEdgePathFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int otbImageToEdgePathFilter(int itkNotUsed(argc), char * argv[])
{
  const char *       inputFilename  = argv[1];
  const char *       outputFilename = argv[2];
  const unsigned int foreground((const unsigned int) ::atoi(argv[3]));

  const unsigned int Dimension = 2;
  typedef unsigned char                    PixelType;
  typedef otb::Image<PixelType, Dimension> ImageType;
  typedef ImageType::IndexType             IndexType;

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

  std::cout << " pathFilter = " << pathFilter << std::endl;

  PathType * outputPath = pathFilter->GetOutput();

  typedef PathType::VertexType                    VertexType;
  typedef PathType::VertexListType                VertexListType;
  typedef PathType::VertexListType::ConstIterator VertexListConstIterator;
  typedef VertexListType::ConstPointer            VertexListTypePointer;

  VertexListTypePointer vertexList;
  VertexType            cindex;

  VertexListTypePointer ptrVertexList =  outputPath->GetVertexList();

  std::cout << "Size : " << ptrVertexList->Size() << std::endl;

  // Initialize Output Image
  ImageType::Pointer outputImage = ImageType::New();
  typedef itk::ImageRegionIteratorWithIndex<ImageType> IteratorType;
  outputImage->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
  outputImage->Allocate();
  outputImage->FillBuffer(255);
  // Create one iterator for the Input Image (this is a light object)
  IteratorType it(outputImage, outputImage->GetBufferedRegion());

  for (unsigned int cpt = 0; cpt <  ptrVertexList->Size(); ++cpt)
    {
    std::cout << " Point " << cpt << " : " << ptrVertexList->GetElement(cpt) << std::endl;
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
