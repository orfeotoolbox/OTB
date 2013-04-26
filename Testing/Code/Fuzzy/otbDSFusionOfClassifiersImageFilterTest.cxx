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
typedef unsigned long                                                       ConfusionMatrixEltType;
typedef itk::VariableSizeMatrix<ConfusionMatrixEltType>                     ConfusionMatrixType;
typedef otb::ConfusionMatrixToMassOfBelief<ConfusionMatrixType, PixelType>  ConfusionMatrixToMassOfBeliefType;
typedef ConfusionMatrixToMassOfBeliefType::MapOfClassesType                 MapOfClassesType;


int CSVConfusionMatrixFileReader(const std::string fileName, MapOfClassesType &mapOfClassesRefClX, ConfusionMatrixType &confusionMatrixClX)
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
      PixelType labelRef = 0, labelProd = 0;
      std::string currentLine, refLabelsLine, prodLabelsLine, currentValue;
      const char endCommentChar = ':';
      const char separatorChar = ',';
      const char eolChar = '\n';
      std::getline(inFile, refLabelsLine, endCommentChar); // Skips the comments
      std::getline(inFile, refLabelsLine, eolChar); // Gets the first line after the comment char until the End Of Line char
      std::getline(inFile, prodLabelsLine, endCommentChar); // Skips the comments
      std::getline(inFile, prodLabelsLine, eolChar); // Gets the second line after the comment char until the End Of Line char

      std::istringstream issRefLabelsLine(refLabelsLine);
      std::istringstream issProdLabelsLine(prodLabelsLine);

      MapOfClassesType mapOfClassesProdClX;

      mapOfClassesRefClX.clear();
      mapOfClassesProdClX.clear();
      int itLab = 0;
      while (issRefLabelsLine.good())
        {
        std::getline(issRefLabelsLine, currentValue, separatorChar);
        labelRef = static_cast<PixelType> (std::atoi(currentValue.c_str()));
        mapOfClassesRefClX[labelRef] = itLab;
        ++itLab;
        }

      itLab = 0;
      while (issProdLabelsLine.good())
        {
        std::getline(issProdLabelsLine, currentValue, separatorChar);
        labelProd = static_cast<PixelType> (std::atoi(currentValue.c_str()));
        mapOfClassesProdClX[labelProd] = itLab;
        ++itLab;
        }

      unsigned int nbRefLabelsClk = mapOfClassesRefClX.size();
      unsigned int nbProdLabelsClk = mapOfClassesProdClX.size();
      ConfusionMatrixType confusionMatrixClXTemp;
      confusionMatrixClXTemp = ConfusionMatrixType(nbRefLabelsClk, nbProdLabelsClk);
      confusionMatrixClXTemp.Fill(0);

      // Reading the confusion matrix confusionMatrixClXTemp from the file
      for (unsigned int itRow = 0; itRow < nbRefLabelsClk; ++itRow)
        {
        //Gets the itRow^th line after the header lines with the labels
        std::getline(inFile, currentLine, eolChar);
        std::istringstream issCurrentLine(currentLine);
        unsigned int itCol = 0;
        while (issCurrentLine.good())
          {
          std::getline(issCurrentLine, currentValue, separatorChar);
          confusionMatrixClXTemp(itRow, itCol) = static_cast<ConfusionMatrixEltType> (std::atoi(currentValue.c_str()));
          ++itCol;
          }
        }

      MapOfClassesType::iterator  itMapOfClassesRef, itMapOfClassesProd;

      /*for (itMapOfClassesRef = mapOfClassesRefClX.begin(); itMapOfClassesRef != mapOfClassesRefClX.end(); ++itMapOfClassesRef)
        {
        std::cout << "mapOfClassesRefClX[" << itMapOfClassesRef->first << "] = " << itMapOfClassesRef->second << std::endl;
        }
      std::cout << std::endl;
      for (itMapOfClassesProd = mapOfClassesProdClX.begin(); itMapOfClassesProd != mapOfClassesProdClX.end(); ++itMapOfClassesProd)
        {
        std::cout << "mapOfClassesProdClX[" << itMapOfClassesProd->first << "] = " << itMapOfClassesProd->second << std::endl;
        }*/

      // Formatting confusionMatrixClX from confusionMatrixClXTemp in order to make confusionMatrixClX a square matrix
      // from the reference labels in mapOfClassesRefClX
      int indiceLabelRef = 0, indiceLabelProd = 0;
      int indiceLabelRefTemp = 0, indiceLabelProdTemp = 0;
      // Initialization of confusionMatrixClX
      confusionMatrixClX = ConfusionMatrixType(nbRefLabelsClk, nbRefLabelsClk);
      confusionMatrixClX.Fill(0);
      for (itMapOfClassesRef = mapOfClassesRefClX.begin(); itMapOfClassesRef != mapOfClassesRefClX.end(); ++itMapOfClassesRef)
        {
        // labels labelRef of mapOfClassesRefClX are already sorted
        labelRef = itMapOfClassesRef->first;
        indiceLabelRefTemp = itMapOfClassesRef->second;

        for (itMapOfClassesProd = mapOfClassesProdClX.begin(); itMapOfClassesProd != mapOfClassesProdClX.end(); ++itMapOfClassesProd)
          {
          // labels labelProd of mapOfClassesProdClX are already sorted
          labelProd = itMapOfClassesProd->first;
          indiceLabelProdTemp = itMapOfClassesProd->second;

          // If labelProd is present in mapOfClassesRefClX
          if (mapOfClassesRefClX.count(labelProd) != 0)
            {
            // Indice of labelProd in mapOfClassesRefClX; itMapOfClassesRef->second elements are already SORTED
            indiceLabelProd = mapOfClassesRefClX[labelProd];
            confusionMatrixClX(indiceLabelRef, indiceLabelProd) = confusionMatrixClXTemp(indiceLabelRefTemp, indiceLabelProdTemp);
            }
          }
        ++indiceLabelRef;
        }

      //std::cout << "confusionMatrixClXTemp:" << std::endl << confusionMatrixClXTemp << std::endl << std::endl;
      //std::cout << "confusionMatrixClX:" << std::endl << confusionMatrixClX << std::endl;
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

    MapOfClassesType mapOfClassesClk;
    ConfusionMatrixType confusionMatrixClk;
    CSVConfusionMatrixFileReader(fileNameConfMat, mapOfClassesClk, confusionMatrixClk);

    confusionMatrixToMassOfBeliefFilter->SetMapOfClasses(mapOfClassesClk);
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
