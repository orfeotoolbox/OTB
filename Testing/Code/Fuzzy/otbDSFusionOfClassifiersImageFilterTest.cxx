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

#include "otbDSFusionOfClassifiersImageFilter.h"
#include "otbImageListToVectorImageFilter.h"
#include "otbConfusionMatrixToMassOfBelief.h"

#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageList.h"

#include <otbImageFileReader.h>
#include "otbImageFileWriter.h"
#include <fstream>



typedef itk::VariableSizeMatrix<double>                                 ConfusionMatrixType;
typedef otb::ConfusionMatrixToMassOfBelief<ConfusionMatrixType>         ConfusionMatrixToMassOfBeliefType;
typedef ConfusionMatrixToMassOfBeliefType::MapOfIndicesType             MapOfIndicesType;


int CSVConfusionMatrixFileReader(const std::string fileName, MapOfIndicesType &mapOfIndicesClX, ConfusionMatrixType &confMatClX)
{
  std::ifstream inFile;
  inFile.open(fileName.c_str());

  if (!inFile)
    {
    std::cerr << "Confusion Matrix File opening problem with file:" << std::endl;
    std::cerr << fileName.c_str() << std::endl;
    return EXIT_FAILURE;
    }
  else
    {
    std::string currentLine, labelsLine, currentValue;
    const char commentChar = '#';
    const char separatorChar = ',';
    const char eolChar = '\n';
    std::getline(inFile, labelsLine, commentChar); // Skips the comment char
    std::getline(inFile, labelsLine, eolChar); // Gets the first line after the comment char until the End Of Line char

    std::istringstream issLabelsLine(labelsLine);
    mapOfIndicesClX.clear();
    int itLab = 0;
    while (issLabelsLine.good())
      {
      std::getline(issLabelsLine, currentValue, separatorChar);
      mapOfIndicesClX[itLab] = std::atoi(currentValue.c_str());
      ++itLab;
      }

    unsigned int nbLabelsClk = mapOfIndicesClX.size();
    confMatClX = ConfusionMatrixType(nbLabelsClk, nbLabelsClk);

    for (unsigned int itLin = 0; itLin < nbLabelsClk; ++itLin)
      {
      //Gets the itLin^th line after the first header line with the labels
      std::getline(inFile, currentLine, eolChar);
      std::istringstream issCurrentLine(currentLine);
      unsigned itCol = 0;
      while (issCurrentLine.good())
        {
        std::getline(issCurrentLine, currentValue, separatorChar);
        confMatClX(itLin, itCol) = std::atoi(currentValue.c_str());
        ++itCol;
        }
      }

    }

  inFile.close();
  return EXIT_SUCCESS;
}




int otbDSFusionOfClassifiersImageFilterNew(int argc, char* argv[])
{
  const  unsigned int Dimension = 2;
  typedef unsigned char PixelType;

  typedef otb::VectorImage<PixelType, Dimension>  VectorImageType;
  typedef otb::Image<PixelType, Dimension>        InputImageType;
  typedef otb::Image<PixelType, Dimension>        OutputImageType;

  typedef otb::DSFusionOfClassifiersImageFilter<VectorImageType, OutputImageType, OutputImageType>  DSFusionOfClassifiersImageFilterType;

  DSFusionOfClassifiersImageFilterType::Pointer dsFusionOfClassifiersImageFilter = DSFusionOfClassifiersImageFilterType::New();
  std::cout << "dsFusionOfClassifiersImageFilter:" << std::endl << dsFusionOfClassifiersImageFilter << std::endl;

  return EXIT_SUCCESS;
}



int otbDSFusionOfClassifiersImageFilterTest(int argc, char* argv[])
{
  const  unsigned int Dimension = 2;
  typedef unsigned char PixelType;

  typedef otb::VectorImage<PixelType, Dimension>  VectorImageType;
  typedef otb::Image<PixelType, Dimension>        InputImageType;
  typedef otb::Image<PixelType, Dimension>        OutputImageType;

  typedef otb::ImageList<InputImageType> ImageListType;
  ImageListType::Pointer imageList = ImageListType::New();

  typedef otb::ImageListToVectorImageFilter<ImageListType, VectorImageType> ImageListToVectorImageFilterType;

  typedef otb::ImageFileReader <InputImageType> ReaderType;
  ReaderType::Pointer reader;

  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();

  typedef otb::DSFusionOfClassifiersImageFilter<VectorImageType, OutputImageType, OutputImageType>  DSFusionOfClassifiersImageFilterType;
  typedef DSFusionOfClassifiersImageFilterType::VectorOfMapOfIndicesType         VectorOfMapOfIndicesType;
  typedef DSFusionOfClassifiersImageFilterType::VectorOfConfusionMatricesType    VectorOfConfusionMatricesType;
  typedef DSFusionOfClassifiersImageFilterType::VectorOfMapOfMassesOfBeliefType  VectorOfMapOfMassesOfBeliefType;
  typedef DSFusionOfClassifiersImageFilterType::MassOfBeliefDefinitionMethod     MassOfBeliefDefinitionMethod;

  DSFusionOfClassifiersImageFilterType::Pointer dsFusionOfClassifiersImageFilter = DSFusionOfClassifiersImageFilterType::New();

  unsigned int nbParameters = 4;
  unsigned int nbClassifiers = (argc - 1 - nbParameters) / 2;

  VectorOfMapOfIndicesType vectorOfMapOfIndices;
  VectorOfConfusionMatricesType vectorOfConfMatrices;
  for (unsigned int itCM = 0; itCM < nbClassifiers; ++itCM)
    {
    std::string fileNameClassifiedImage = argv[itCM + 1];
    std::string fileNameConfMat = argv[itCM + 1 + nbClassifiers];

    reader = ReaderType::New();
    reader->SetFileName(fileNameClassifiedImage);
    reader->Update();

    MapOfIndicesType mapOfIndicesClk;
    ConfusionMatrixType confMatClk;
    CSVConfusionMatrixFileReader(fileNameConfMat, mapOfIndicesClk, confMatClk);

    imageList->PushBack(reader->GetOutput());
    vectorOfMapOfIndices.push_back(mapOfIndicesClk);
    vectorOfConfMatrices.push_back(confMatClk);
    }

  // **********************************************
  // Image List To VectorImage
  // **********************************************
  ImageListToVectorImageFilterType::Pointer imageListToVectorImageFilter = ImageListToVectorImageFilterType::New();
  imageListToVectorImageFilter->SetInput(imageList);
  //imageListToVectorImageFilter->Update();

  std::string massOfBeliefDefMethodStr = argv[argc - 4];
  MassOfBeliefDefinitionMethod massOfBeliefDefMethod;
  if (massOfBeliefDefMethodStr.compare("PRECISION") == 0)
    {
    massOfBeliefDefMethod = ConfusionMatrixToMassOfBeliefType::PRECISION;
    }
  else
    {
    if (massOfBeliefDefMethodStr.compare("RECALL") == 0)
      {
      massOfBeliefDefMethod = ConfusionMatrixToMassOfBeliefType::RECALL;
      }
    else
      {
      if (massOfBeliefDefMethodStr.compare("ACCURACY") == 0)
        {
        massOfBeliefDefMethod = ConfusionMatrixToMassOfBeliefType::ACCURACY;
        }
      else
        {
        if (massOfBeliefDefMethodStr.compare("KAPPA") == 0)
          {
          massOfBeliefDefMethod = ConfusionMatrixToMassOfBeliefType::KAPPA;
          }
        }
      }
    }

  // Filter Inputs
  dsFusionOfClassifiersImageFilter->SetInput(imageListToVectorImageFilter->GetOutput());
  dsFusionOfClassifiersImageFilter->SetInputMapsOfIndices(&vectorOfMapOfIndices);
  dsFusionOfClassifiersImageFilter->SetInputConfusionMatrices(&vectorOfConfMatrices);
  dsFusionOfClassifiersImageFilter->SetDefinitionMethod(massOfBeliefDefMethod);
  dsFusionOfClassifiersImageFilter->SetLabelForNoDataPixels(atoi(argv[argc - 3]));
  dsFusionOfClassifiersImageFilter->SetLabelForUndecidedPixels(atoi(argv[argc - 2]));
  //dsFusionOfClassifiersImageFilter->Update();

  std::string fileNameFusedImage = argv[argc - 1];
  writer->SetFileName(fileNameFusedImage);
  writer->SetInput(dsFusionOfClassifiersImageFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
