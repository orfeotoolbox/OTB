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


// Software Guide : BeginLatex
//
// The fusion filter \doxygen{otb}{DSFusionOfClassifiersImageFilter} is based on the Dempster
// Shafer (DS) fusion framework. For each pixel, it chooses the class label \emph{Ai} for which the
// belief function \emph{bel(Ai)} is maximal after the DS combination of all the available masses of
// belief of all the class labels. The masses of belief (MOBs) of all the labels present in each
// classification map are read from input *.CSV confusion matrix files.
// Moreover, the pixels into the input classification maps to be fused which are equal to the
// \emph{nodataLabel} value are ignored by the fusion process. In case of not unique class labels
// with the maximal belief function, the output pixels are set to the \emph{undecidedLabel} value.
// We start by including the appropriate header files.
//
// Software Guide : EndLatex


// Software Guide : BeginCodeSnippet
#include "otbImageListToVectorImageFilter.h"
#include "otbConfusionMatrixToMassOfBelief.h"
#include "otbDSFusionOfClassifiersImageFilter.h"

#include <fstream>
// Software Guide : EndCodeSnippet

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"


// Software Guide : BeginLatex
//
// We will assume unsigned short type input labeled images. We define a type for
// confusion matrices as \doxygen{itk}{VariableSizeMatrix} which will be used to estimate the masses of belief of all the
// class labels for each input classification map. For this purpose, the
// \doxygen{otb}{ConfusionMatrixToMassOfBelief} will be used to convert each input confusion matrix
// into masses of belief for each class label.
//
// Software Guide : EndLatex


// Software Guide : BeginCodeSnippet
  typedef unsigned short LabelPixelType;
  typedef unsigned long ConfusionMatrixEltType;
  typedef itk::VariableSizeMatrix<ConfusionMatrixEltType> ConfusionMatrixType;
  typedef otb::ConfusionMatrixToMassOfBelief
           <ConfusionMatrixType, LabelPixelType> ConfusionMatrixToMassOfBeliefType;
  typedef ConfusionMatrixToMassOfBeliefType::MapOfClassesType MapOfClassesType;
// Software Guide : EndCodeSnippet


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
      LabelPixelType labelRef = 0, labelProd = 0;
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
        labelRef = static_cast<LabelPixelType> (std::atoi(currentValue.c_str()));
        mapOfClassesRefClX[labelRef] = itLab;
        ++itLab;
        }

      itLab = 0;
      while (issProdLabelsLine.good())
        {
        std::getline(issProdLabelsLine, currentValue, separatorChar);
        labelProd = static_cast<LabelPixelType> (std::atoi(currentValue.c_str()));
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
      }
    inFile.close();
    return EXIT_SUCCESS;
  }


int main(int argc, char * argv[])
{
// Software Guide : BeginLatex
//
// The input labeled images to be fused are expected to be scalar images.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
  const unsigned int     Dimension = 2;
  typedef otb::Image<LabelPixelType, Dimension> LabelImageType;
  typedef otb::VectorImage<LabelPixelType, Dimension> VectorImageType;
// Software Guide : EndCodeSnippet

  LabelPixelType nodataLabel = atoi(argv[argc - 3]);
  LabelPixelType undecidedLabel = atoi(argv[argc - 2]);
  const char * outfname = argv[argc - 1];

  unsigned int nbParameters = 3;
  unsigned int nbClassificationMaps = (argc - 1 - nbParameters) / 2;

  // Software Guide : BeginLatex
  //
  // We declare an \doxygen{otb}{ImageListToVectorImageFilter} which will stack all the
  // input classification maps to be fused as a single VectorImage for which each
  // band is a classification map. This VectorImage will then be the input of the
  // Dempster Shafer fusion filter \doxygen{otb}{DSFusionOfClassifiersImageFilter}.
  //
  // Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  typedef otb::ImageList<LabelImageType> LabelImageListType;
  typedef otb::ImageListToVectorImageFilter
            <LabelImageListType, VectorImageType> ImageListToVectorImageFilterType;
  // Software Guide : EndCodeSnippet

  typedef ConfusionMatrixToMassOfBeliefType::MassOfBeliefDefinitionMethod MassOfBeliefDefinitionMethod;


// Software Guide : BeginLatex
//
// The Dempster Shafer fusion filter \doxygen{otb}{DSFusionOfClassifiersImageFilter} is declared.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
  // Dempster Shafer
  typedef otb::DSFusionOfClassifiersImageFilter
        <VectorImageType, LabelImageType> DSFusionOfClassifiersImageFilterType;
// Software Guide : EndCodeSnippet

  typedef DSFusionOfClassifiersImageFilterType::VectorOfMapOfMassesOfBeliefType VectorOfMapOfMassesOfBeliefType;

// Software Guide : BeginLatex
//
// Both reader and writer are defined. Since the images
// to classify can be very big, we will use a streamed writer which
// will trigger the streaming ability of the fusion filter.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<LabelImageType> ReaderType;
  typedef otb::ImageFileWriter<LabelImageType> WriterType;
// Software Guide : EndCodeSnippet


// Software Guide : BeginLatex
//
// The image list of input classification maps is filled. Moreover, the input
// confusion matrix files are converted into masses of belief.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
  ReaderType::Pointer reader;
  LabelImageListType::Pointer imageList = LabelImageListType::New();
  ConfusionMatrixToMassOfBeliefType::Pointer confusionMatrixToMassOfBeliefFilter;
  confusionMatrixToMassOfBeliefFilter = ConfusionMatrixToMassOfBeliefType::New();

  MassOfBeliefDefinitionMethod massOfBeliefDef;

  // Several parameters are available to estimate the masses of belief
  // from the confusion matrices: PRECISION, RECALL, ACCURACY and KAPPA
  massOfBeliefDef = ConfusionMatrixToMassOfBeliefType::PRECISION;

  VectorOfMapOfMassesOfBeliefType vectorOfMapOfMassesOfBelief;
  for (unsigned int itCM = 0; itCM < nbClassificationMaps; ++itCM)
    {
    std::string fileNameClassifiedImage = argv[itCM + 1];
    std::string fileNameConfMat = argv[itCM + 1 + nbClassificationMaps];

    reader = ReaderType::New();
    reader->SetFileName(fileNameClassifiedImage);
    reader->Update();

    imageList->PushBack(reader->GetOutput());

    MapOfClassesType mapOfClassesClk;
    ConfusionMatrixType confusionMatrixClk;

    // The data (class labels and confusion matrix values) are read and
    // extracted from the *.CSV file with an ad-hoc file parser
    CSVConfusionMatrixFileReader(
        fileNameConfMat, mapOfClassesClk, confusionMatrixClk);

    // The parameters of the ConfusionMatrixToMassOfBelief filter are set
    confusionMatrixToMassOfBeliefFilter->SetMapOfClasses(mapOfClassesClk);
    confusionMatrixToMassOfBeliefFilter->SetConfusionMatrix(confusionMatrixClk);
    confusionMatrixToMassOfBeliefFilter->SetDefinitionMethod(massOfBeliefDef);
    confusionMatrixToMassOfBeliefFilter->Update();

    // Vector containing ALL the K (= nbClassificationMaps) std::map<Label, MOB>
    // of Masses of Belief
    vectorOfMapOfMassesOfBelief.push_back(
        confusionMatrixToMassOfBeliefFilter->GetMapMassOfBelief());
    }
// Software Guide : EndCodeSnippet


// Software Guide : BeginLatex
//
// The image list of input classification maps is converted into a VectorImage to
// be used as input of the \doxygen{otb}{DSFusionOfClassifiersImageFilter}.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
  // Image List To VectorImage
  ImageListToVectorImageFilterType::Pointer imageListToVectorImageFilter;
  imageListToVectorImageFilter = ImageListToVectorImageFilterType::New();
  imageListToVectorImageFilter->SetInput(imageList);

  DSFusionOfClassifiersImageFilterType::Pointer dsFusionFilter;
  dsFusionFilter = DSFusionOfClassifiersImageFilterType::New();

  // The parameters of the DSFusionOfClassifiersImageFilter are set
  dsFusionFilter->SetInput(imageListToVectorImageFilter->GetOutput());
  dsFusionFilter->SetInputMapsOfMassesOfBelief(&vectorOfMapOfMassesOfBelief);
  dsFusionFilter->SetLabelForNoDataPixels(nodataLabel);
  dsFusionFilter->SetLabelForUndecidedPixels(undecidedLabel);
// Software Guide : EndCodeSnippet


// Software Guide : BeginLatex
//
// Once it is plugged the pipeline triggers its execution by updating
// the output of the writer.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(dsFusionFilter->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();
// Software Guide : EndCodeSnippet
  return EXIT_SUCCESS;
}
