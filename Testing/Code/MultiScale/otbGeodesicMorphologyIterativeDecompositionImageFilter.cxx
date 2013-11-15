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

#include "otbGeodesicMorphologyIterativeDecompositionImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

int otbGeodesicMorphologyIterativeDecompositionImageFilter(int argc, char * argv[])
{
  const char *       inputFilename = argv[1];
  const char *       outputFilenamePrefix = argv[2];
  const char *       outputFilenameSuffix = argv[3];
  const unsigned int numberOfLevels = atoi(argv[4]);
  const unsigned int step = atoi(argv[5]);
  const unsigned int initValue = atoi(argv[6]);

  const unsigned int Dimension = 2;
  typedef double InputPixelType;

  typedef otb::Image<InputPixelType, Dimension> InputImageType;
  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<InputImageType>  WriterType;

  typedef itk::BinaryBallStructuringElement<InputPixelType,
      Dimension>                                     StructuringElementType;
  typedef otb::GeodesicMorphologyIterativeDecompositionImageFilter<InputImageType,
      StructuringElementType> DecompositionImageFilterType;
  typedef DecompositionImageFilterType::OutputImageListType::Iterator
  ImageListIterator;

  // Reading input image
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);

  // Instantiation
  DecompositionImageFilterType::Pointer decomposition = DecompositionImageFilterType::New();
  decomposition->SetNumberOfIterations(numberOfLevels);
  decomposition->SetStep(step);
  decomposition->SetInitialValue(initValue);
  decomposition->SetInput(reader->GetOutput());
  decomposition->Update();

  // Retrieving iterators on the results images
  ImageListIterator itAnalyse = decomposition->GetOutput()->Begin();
  ImageListIterator itConvexMap = decomposition->GetConvexOutput()->Begin();
  ImageListIterator itConcaveMap = decomposition->GetConcaveOutput()->Begin();

  WriterType::Pointer writer;

  int                i = 1;
  std::ostringstream oss;
  // Writing the results images
  while ((itAnalyse != decomposition->GetOutput()->End())
         && (itConvexMap != decomposition->GetConvexOutput()->End())
         && (itConcaveMap != decomposition->GetConcaveOutput()->End())
         )
    {
    oss << outputFilenamePrefix << "_leveling_" << i << "." << outputFilenameSuffix;
    writer =  WriterType::New();
    writer->SetInput(itAnalyse.Get());
    writer->SetFileName(oss.str().c_str());
    writer->Update();
    oss.str("");
    oss << outputFilenamePrefix << "_convMap_" << i << "." << outputFilenameSuffix;
    writer =  WriterType::New();
    writer->SetInput(itConvexMap.Get());
    writer->SetFileName(oss.str().c_str());
    writer->Update();
    oss.str("");
    oss << outputFilenamePrefix << "_concMap_" << i << "." << outputFilenameSuffix;
    writer =  WriterType::New();
    writer->SetInput(itConcaveMap.Get());
    writer->SetFileName(oss.str().c_str());
    writer->Update();
    oss.str("");
    ++itAnalyse;
    ++itConvexMap;
    ++itConcaveMap;
    ++i;
    }
  return EXIT_SUCCESS;
}
