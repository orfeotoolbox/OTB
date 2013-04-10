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


typedef unsigned char                                                       PixelType;
typedef itk::VariableSizeMatrix<double>                                     ConfusionMatrixType;
typedef otb::ConfusionMatrixToMassOfBelief<ConfusionMatrixType, PixelType>  ConfusionMatrixToMassOfBeliefType;
typedef ConfusionMatrixToMassOfBeliefType::MapOfIndicesType                 MapOfIndicesType;


int CSVConfusionMatrixFileReader(const std::string fileName, MapOfIndicesType &mapOfIndicesClX, ConfusionMatrixType &confusionMatrixClX)
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
    confusionMatrixClX = ConfusionMatrixType(nbLabelsClk, nbLabelsClk);

    for (unsigned int itLin = 0; itLin < nbLabelsClk; ++itLin)
      {
      //Gets the itLin^th line after the first header line with the labels
      std::getline(inFile, currentLine, eolChar);
      std::istringstream issCurrentLine(currentLine);
      unsigned int itCol = 0;
      while (issCurrentLine.good())
        {
        std::getline(issCurrentLine, currentValue, separatorChar);
        confusionMatrixClX(itLin, itCol) = std::atoi(currentValue.c_str());
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

  typedef otb::VectorImage<PixelType, Dimension>  VectorImageType;
  typedef otb::Image<PixelType, Dimension>        OutputImageType;
  typedef otb::Image<unsigned int, Dimension>     MaskType;

  // filter types
  typedef otb::DSFusionOfClassifiersImageFilter<VectorImageType, OutputImageType, MaskType>
      DSFusionOfClassifiersImageFilter3TemplatesType;
  typedef otb::DSFusionOfClassifiersImageFilter<VectorImageType, OutputImageType>
      DSFusionOfClassifiersImageFilter2TemplatesType;

  // filters
  DSFusionOfClassifiersImageFilter3TemplatesType::Pointer
      dsFusionOfClassifiersImageFilter3Templates = DSFusionOfClassifiersImageFilter3TemplatesType::New();
  DSFusionOfClassifiersImageFilter2TemplatesType::Pointer
      dsFusionOfClassifiersImageFilter2Templates = DSFusionOfClassifiersImageFilter2TemplatesType::New();

  std::cout << dsFusionOfClassifiersImageFilter3Templates << std::endl;
  std::cout << std::endl;
  std::cout << dsFusionOfClassifiersImageFilter2Templates << std::endl;
  std::cout << std::endl;

  return EXIT_SUCCESS;
}



int otbDSFusionOfClassifiersImageFilterTest(int argc, char* argv[])
{
  const  unsigned int Dimension = 2;

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

  typedef ConfusionMatrixToMassOfBeliefType::MassOfBeliefDefinitionMethod MassOfBeliefDefinitionMethod;

  typedef otb::DSFusionOfClassifiersImageFilter<VectorImageType, OutputImageType, OutputImageType>  DSFusionOfClassifiersImageFilterType;
  typedef DSFusionOfClassifiersImageFilterType::VectorOfMapOfMassesOfBeliefType  VectorOfMapOfMassesOfBeliefType;

  DSFusionOfClassifiersImageFilterType::Pointer dsFusionOfClassifiersImageFilter = DSFusionOfClassifiersImageFilterType::New();

  unsigned int nbParameters = 4;
  unsigned int nbClassifiers = (argc - 1 - nbParameters) / 2;


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


  ConfusionMatrixToMassOfBeliefType::Pointer confusionMatrixToMassOfBeliefFilter = ConfusionMatrixToMassOfBeliefType::New();
  VectorOfMapOfMassesOfBeliefType vectorOfMapOfMassesOfBelief;
  for (unsigned int itCM = 0; itCM < nbClassifiers; ++itCM)
    {
    std::string fileNameClassifiedImage = argv[itCM + 1];
    std::string fileNameConfMat = argv[itCM + 1 + nbClassifiers];

    reader = ReaderType::New();
    reader->SetFileName(fileNameClassifiedImage);
    reader->Update();

    imageList->PushBack(reader->GetOutput());

    MapOfIndicesType mapOfIndicesClk;
    ConfusionMatrixType confusionMatrixClk;
    CSVConfusionMatrixFileReader(fileNameConfMat, mapOfIndicesClk, confusionMatrixClk);

    confusionMatrixToMassOfBeliefFilter->SetMapOfIndices(mapOfIndicesClk);
    confusionMatrixToMassOfBeliefFilter->SetConfusionMatrix(confusionMatrixClk);
    confusionMatrixToMassOfBeliefFilter->SetDefinitionMethod(massOfBeliefDefMethod);
    confusionMatrixToMassOfBeliefFilter->Update();

    // Vector containing ALL the K (= nbClassifiers) std::map<Label, MOB> of Masses of Belief
    vectorOfMapOfMassesOfBelief.push_back(confusionMatrixToMassOfBeliefFilter->GetMapMassOfBelief());
    }



  // **********************************************
  // Image List To VectorImage
  // **********************************************
  ImageListToVectorImageFilterType::Pointer imageListToVectorImageFilter = ImageListToVectorImageFilterType::New();
  imageListToVectorImageFilter->SetInput(imageList);
  //imageListToVectorImageFilter->Update();


  // DSFusionOfClassifiersImageFilter Inputs
  dsFusionOfClassifiersImageFilter->SetInput(imageListToVectorImageFilter->GetOutput());
  dsFusionOfClassifiersImageFilter->SetInputMapsOfMassesOfBelief(&vectorOfMapOfMassesOfBelief);
  dsFusionOfClassifiersImageFilter->SetLabelForNoDataPixels(atoi(argv[argc - 3]));
  dsFusionOfClassifiersImageFilter->SetLabelForUndecidedPixels(atoi(argv[argc - 2]));
  //dsFusionOfClassifiersImageFilter->Update();

  std::string fileNameFusedImage = argv[argc - 1];
  writer->SetFileName(fileNameFusedImage);
  writer->SetInput(dsFusionOfClassifiersImageFilter->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
