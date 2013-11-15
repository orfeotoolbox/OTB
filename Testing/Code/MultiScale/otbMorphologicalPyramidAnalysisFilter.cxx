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

#include "otbMorphologicalPyramidAnalysisFilter.h"
#include "otbOpeningClosingMorphologicalFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

#include "itkMacro.h"

int otbMorphologicalPyramidAnalysisFilter(int argc, char * argv[])
{
  const char *       inputFilename = argv[1];
  const char *       outputFilenamePrefix = argv[2];
  const char *       outputFilenameSuffix = argv[3];
  const unsigned int numberOfLevels = atoi(argv[4]);
  const float        decimationRatio = atof(argv[5]);

  const unsigned int Dimension = 2;
  typedef unsigned char InputPixelType;
  typedef unsigned char OutputPixelType;

  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  typedef itk::BinaryBallStructuringElement<InputPixelType, Dimension> StructuringElementType;
  typedef otb::OpeningClosingMorphologicalFilter<InputImageType, InputImageType, StructuringElementType>
  OpeningClosingFilterType;
  typedef otb::MorphologicalPyramidAnalysisFilter<InputImageType, OutputImageType, OpeningClosingFilterType>
  PyramidFilterType;
  typedef PyramidFilterType::OutputImageListType::Iterator ImageListIterator;

  // Reading input image
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);

  // Instantiation
  PyramidFilterType::Pointer pyramid = PyramidFilterType::New();
  pyramid->SetNumberOfLevels(numberOfLevels);
  pyramid->SetDecimationRatio(decimationRatio);
  pyramid->SetInput(reader->GetOutput());
  pyramid->Update();

  // Retrieving iterators on the results images
  ImageListIterator itAnalyse = pyramid->GetOutput()->Begin();
  ImageListIterator itSupFilter = pyramid->GetSupFilter()->Begin();
  ImageListIterator itInfFilter = pyramid->GetInfFilter()->Begin();
  ImageListIterator itInfDeci = pyramid->GetInfDeci()->Begin();
  ImageListIterator itSupDeci =  pyramid->GetSupDeci()->Begin();

  WriterType::Pointer writer =  WriterType::New();

  int i = 1;
  //      std::stringstream oss;
  std::ostringstream oss;
  // Writing the results images
  while ((itAnalyse != pyramid->GetOutput()->End())
         && (itSupFilter != pyramid->GetSupFilter()->End())
         && (itInfFilter != pyramid->GetInfFilter()->End())
         && (itInfDeci != pyramid->GetInfDeci()->End())
         && (itSupDeci != pyramid->GetSupDeci()->End())
         )
    {
    oss << outputFilenamePrefix << "_an_" << i << "." << outputFilenameSuffix;
    writer->SetInput(itAnalyse.Get());
    writer->SetFileName(oss.str().c_str());
    writer->Update();
    oss.str("");
    oss << outputFilenamePrefix << "_sf_" << i << "." << outputFilenameSuffix;
    writer->SetInput(itSupFilter.Get());
    writer->SetFileName(oss.str().c_str());
    writer->Update();
    oss.str("");
    oss << outputFilenamePrefix << "_if_" << i << "." << outputFilenameSuffix;
    writer->SetInput(itInfFilter.Get());
    writer->SetFileName(oss.str().c_str());
    writer->Update();
    oss.str("");
    oss << outputFilenamePrefix << "_id_" << i << "." << outputFilenameSuffix;
    writer->SetInput(itInfDeci.Get());
    writer->SetFileName(oss.str().c_str());
    writer->Update();
    std::cout << "File " << oss << " written" << std::endl;
    oss.str("");
    oss << outputFilenamePrefix << "_sd_" << i << "." << outputFilenameSuffix;
    writer->SetInput(itSupDeci.Get());
    writer->SetFileName(oss.str().c_str());
    writer->Update();
    oss.str("");
    ++itAnalyse;
    ++itSupFilter;
    ++itInfFilter;
    ++itInfDeci;
    ++itSupDeci;
    ++i;
    }

  return EXIT_SUCCESS;
}
