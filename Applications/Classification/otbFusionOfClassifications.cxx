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
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "itkLabelVotingImageFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"

#include "otbDSFusionOfClassifiersImageFilter.h"
#include "otbImageListToVectorImageFilter.h"
#include "otbConfusionMatrixToMassOfBelief.h"

#include <iostream>
#include <fstream>

#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageList.h"




namespace otb
{

enum
{
  Method_Majority_Voting,
  Method_Dempster_Shafer
};

enum
{
  Mode_MOB_Precision,
  Mode_MOB_Recall,
  Mode_MOB_Accuracy,
  Mode_MOB_Kappa
};

namespace Wrapper
{

class FusionOfClassifications: public Application
{
public:
  /** Standard class typedefs. */
  typedef FusionOfClassifications Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef UInt16VectorImageType                VectorImageType;
  typedef UInt16ImageType                      IOLabelImageType;
  typedef IOLabelImageType::InternalPixelType  LabelPixelType;

  typedef otb::MultiToMonoChannelExtractROI<FloatVectorImageType::InternalPixelType, LabelPixelType> ConverterType;

  // Majority Voting
  typedef itk::LabelVotingImageFilter<IOLabelImageType, IOLabelImageType> LabelVotingFilterType;

  // Dempster Shafer
  typedef itk::VariableSizeMatrix<double>                                          ConfusionMatrixType;
  typedef otb::ConfusionMatrixToMassOfBelief<ConfusionMatrixType, LabelPixelType>       ConfusionMatrixToMassOfBeliefType;
  typedef ConfusionMatrixToMassOfBeliefType::MapOfIndicesType                      MapOfIndicesType;

  typedef otb::ImageList<IOLabelImageType>                                         ImageListType;
  typedef otb::ImageListToVectorImageFilter<ImageListType, VectorImageType>        ImageListToVectorImageFilterType;

  typedef ConfusionMatrixToMassOfBeliefType::MassOfBeliefDefinitionMethod          MassOfBeliefDefinitionMethod;

  typedef otb::DSFusionOfClassifiersImageFilter<VectorImageType, IOLabelImageType> DSFusionOfClassifiersImageFilterType;
  typedef DSFusionOfClassifiersImageFilterType::VectorOfMapOfMassesOfBeliefType    VectorOfMapOfMassesOfBeliefType;
  typedef DSFusionOfClassifiersImageFilterType::LabelMassMapType                   LabelMassMapType;


  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(FusionOfClassifications, otb::Application);

private:
  void DoInit()
  {
    SetName("FusionOfClassifications");
    SetDescription("Fuses several classifications maps of the same image by majority voting on class labels.");
    SetDocName("Fusion of Classifications");
    SetDocLongDescription("This application allows to fuse several classifications maps and produce a single more robust classification map. Fusion is done by mean of majority voting on class labels: for each pixel, the class with the highest number of votes is selected. In case of number of votes equality, the undecided label is attributed to the pixel.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("SVMImagesClassifier application");
 
    AddDocTag(Tags::Learning);
    AddDocTag(Tags::Analysis);

    AddParameter(ParameterType_InputImageList, "il", "Input classifications");
    SetParameterDescription( "il", "List of input classification to fuse. Labels in each classification image must represent the same class." );


    /** GROUP FUSION METHOD */
    AddParameter(ParameterType_Choice, "method", "Fusion method");
    SetParameterDescription("method", "Selection of fusion methods and their parameters.");

    // Majority Voting
    AddChoice("method.majorityvoting","Fusion from majority voting");
    SetParameterDescription("method.majorityvoting","Fusion of classification maps from majority voting for each output pixel.");

    // Dempster Shafer
    AddChoice("method.dempstershafer","Fusion with Dempster Shafer");
    SetParameterDescription("method.dempstershafer","Fusion of classification maps with the Dempster Shafer method.");

    AddParameter(ParameterType_InputFilenameList, "method.dempstershafer.cmfl", "Confusion Matrices");
    SetParameterDescription("method.dempstershafer.cmfl", "A list of confusion matrix files (csv format) to define the masses of belief and the class labels.");

    AddParameter(ParameterType_Choice, "method.dempstershafer.mob", "Mass of belief measurement");
    SetParameterDescription("method.dempstershafer.mob","Confusion matrix measurement standing for the masses of belief of each classifier.");
    AddChoice("method.dempstershafer.mob.precision","Precision");
    SetParameterDescription("method.dempstershafer.mob.precision","Masses of belief = Precision rates of each classifier (one rate per class label).");
    AddChoice("method.dempstershafer.mob.recall", "Recall");
    SetParameterDescription("method.dempstershafer.mob.recall", "Masses of belief = Recall rates of each classifier (one rate per class label).");
    AddChoice("method.dempstershafer.mob.accuracy", "Overall Accuracy");
    SetParameterDescription("method.dempstershafer.mob.accuracy", "Mass of belief = Overall Accuracy of each classifier (one unique rate for all the class labels).");
    AddChoice("method.dempstershafer.mob.kappa", "Kappa");
    SetParameterDescription("method.dempstershafer.mob.kappa", "Mass of belief = Kappa coefficient of each classifier (one unique rate for all the class labels).");

    AddParameter(ParameterType_Int, "nodatalabel", "Label for the NoData class");
    SetParameterDescription("nodatalabel", "Label for the NoData class. Such input pixels keep their NoData label in the output image. By default, 'nodatalabel = 0'.");
    SetDefaultParameterInt("nodatalabel", 0);
    MandatoryOff("nodatalabel");

    AddParameter(ParameterType_Int,"undecidedlabel","Label for the Undecided class");
    SetParameterDescription("undecidedlabel","Label for the Undecided class. Pixels with more than 1 fused class are marked as Undecided. Please note that the Undecided value must be different from existing labels in the input classifications. By default, 'undecidedlabel = 0'.");
    SetDefaultParameterInt("undecidedlabel",0);

    AddParameter(ParameterType_OutputImage,"out","The output classification image");
    SetParameterDescription("out","The output classification image resulting from the fusion of the input classification images");

    // Doc example parameter settings
    SetDocExampleParameterValue("il", "classification1.tif classification2.tif");
    SetDocExampleParameterValue("out","classification_fused.tif");
    SetDocExampleParameterValue("undecidedlabel","10");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }


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


  void DoExecute()
  {
    // Clear any previous filter
    m_Filters.clear();

    FloatVectorImageListType* imageList = GetParameterImageList("il");

    if (GetParameterInt("method") == Method_Majority_Voting)
    //if (IsParameterEnabled( "method.majorityvoting" ) == true)
      {
      otbAppLogINFO("Fusion by Majority Voting");

      LabelVotingFilterType::Pointer labelVotingFilter = LabelVotingFilterType::New();
      labelVotingFilter->SetLabelForUndecidedPixels(GetParameterInt("undecidedlabel"));

      m_Filters.push_back(labelVotingFilter.GetPointer());

      //Iterate over all input images
      for (unsigned int imageId = 0; imageId < imageList->Size(); ++imageId)
        {
        FloatVectorImageType* image = imageList->GetNthElement(imageId);

        ConverterType::Pointer converter = ConverterType::New();
        converter->SetInput(image);
        converter->SetChannel(1);

        labelVotingFilter->SetInput(imageId, converter->GetOutput());

        // Register filter
        m_Filters.push_back(converter.GetPointer());
        }

      SetParameterOutputImage("out", labelVotingFilter->GetOutput());
      }// END Fusion by Majority Voting

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    else
      {
      if (GetParameterInt("method") == Method_Dempster_Shafer)
        {
        otbAppLogINFO("Fusion with the Dempster Shafer method");

        ImageListType::Pointer imageListTemp = ImageListType::New();
        ConfusionMatrixToMassOfBeliefType::Pointer confusionMatrixToMassOfBeliefFilter = ConfusionMatrixToMassOfBeliefType::New();

        DSFusionOfClassifiersImageFilterType::Pointer dsFusionOfClassifiersImageFilter = DSFusionOfClassifiersImageFilterType::New();
        dsFusionOfClassifiersImageFilter->SetLabelForUndecidedPixels(GetParameterInt("undecidedlabel"));

        m_Filters.push_back(confusionMatrixToMassOfBeliefFilter.GetPointer());
        m_Filters.push_back(dsFusionOfClassifiersImageFilter.GetPointer());

        std::vector<std::string> confusionMatricesFilenameList = GetParameterStringList("method.dempstershafer.cmfl");

        MassOfBeliefDefinitionMethod massOfBeliefDefMethod;
        switch (GetParameterInt("method.dempstershafer.mob"))
          {
          case Mode_MOB_Precision:
            {
            otbAppLogINFO("Masses of belief = PRECISION rates of each classifier (one rate per class label)");
            massOfBeliefDefMethod = ConfusionMatrixToMassOfBeliefType::PRECISION;
            }
            break;
          case Mode_MOB_Recall:
            {
            otbAppLogINFO("Masses of belief = RECALL rates of each classifier (one rate per class label)");
            massOfBeliefDefMethod = ConfusionMatrixToMassOfBeliefType::RECALL;
            }
            break;
          case Mode_MOB_Accuracy:
            {
            otbAppLogINFO("Mass of belief = OVERALL ACCURACY of each classifier (one unique rate for all the class labels)");
            massOfBeliefDefMethod = ConfusionMatrixToMassOfBeliefType::ACCURACY;
            }
            break;
          case Mode_MOB_Kappa:
            {
            otbAppLogINFO("Mass of belief = KAPPA coefficient of each classifier (one unique rate for all the class labels)");
            massOfBeliefDefMethod = ConfusionMatrixToMassOfBeliefType::KAPPA;
            }
            break;
          }

        //Iterate over all input images
        VectorOfMapOfMassesOfBeliefType vectorOfMapOfMassesOfBelief;
        for (unsigned int imageId = 0; imageId < imageList->Size(); ++imageId)
          {
          FloatVectorImageType* image = imageList->GetNthElement(imageId);

          ConverterType::Pointer converter = ConverterType::New();
          converter->SetInput(image);
          converter->SetChannel(1);

          imageListTemp->PushBack(converter->GetOutput());

          MapOfIndicesType mapOfIndicesClk;
          ConfusionMatrixType confusionMatrixClk;
          CSVConfusionMatrixFileReader(confusionMatricesFilenameList[imageId], mapOfIndicesClk, confusionMatrixClk);

          confusionMatrixToMassOfBeliefFilter->SetMapOfIndices(mapOfIndicesClk);
          confusionMatrixToMassOfBeliefFilter->SetConfusionMatrix(confusionMatrixClk);
          confusionMatrixToMassOfBeliefFilter->SetDefinitionMethod(massOfBeliefDefMethod);
          confusionMatrixToMassOfBeliefFilter->Update();

          // Vector containing ALL the K (= nbClassifiers) std::map<Label, MOB> of Masses of Belief
          vectorOfMapOfMassesOfBelief.push_back(confusionMatrixToMassOfBeliefFilter->GetMapMassOfBelief());
          // Register filter
          m_Filters.push_back(converter.GetPointer());
          }

        // **********************************************
        // Image List To VectorImage
        // **********************************************
        ImageListToVectorImageFilterType::Pointer imageListToVectorImageFilter = ImageListToVectorImageFilterType::New();
        imageListToVectorImageFilter->SetInput(imageListTemp);
        imageListToVectorImageFilter->Update();

        otbAppLogINFO("Number of input classification maps: " << imageListToVectorImageFilter->GetOutput()->GetNumberOfComponentsPerPixel());
        otbAppLogINFO("Number of input confusion matrix files: " << vectorOfMapOfMassesOfBelief.size());
        LabelMassMapType::iterator itMap;
        for (unsigned int k = 0; k < vectorOfMapOfMassesOfBelief.size(); ++k)
          {
          LabelMassMapType mapOfMassesOfBelief = vectorOfMapOfMassesOfBelief[k];
          otbAppLogINFO("Classifier[" << k << "]: ");
          otbAppLogINFO("    Confusion Matrix file_" << k << ": " << confusionMatricesFilenameList[k]);
          for (itMap = mapOfMassesOfBelief.begin(); itMap != mapOfMassesOfBelief.end(); ++itMap)
            {
            otbAppLogINFO("       MassOfBelief_Cl_" << k << "[label_" << itMap->first << "] = " << itMap->second );
            }
          otbAppLogINFO("");
          }

        // DSFusionOfClassifiersImageFilter Inputs
        dsFusionOfClassifiersImageFilter->SetInput(imageListToVectorImageFilter->GetOutput());
        dsFusionOfClassifiersImageFilter->SetInputMapsOfMassesOfBelief(&vectorOfMapOfMassesOfBelief);
        dsFusionOfClassifiersImageFilter->SetLabelForNoDataPixels(GetParameterInt("nodatalabel"));
        dsFusionOfClassifiersImageFilter->SetLabelForUndecidedPixels(GetParameterInt("undecidedlabel"));
        //dsFusionOfClassifiersImageFilter->Update();

        SetParameterOutputImage("out", dsFusionOfClassifiersImageFilter->GetOutput());
        }// END Fusion with the Dempster Shafer method

      }
  }
  
  std::vector<itk::LightObject::Pointer> m_Filters;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::FusionOfClassifications)
