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
  typedef unsigned long                                                            ConfusionMatrixEltType;
  typedef itk::VariableSizeMatrix<ConfusionMatrixEltType>                          ConfusionMatrixType;
  typedef otb::ConfusionMatrixToMassOfBelief<ConfusionMatrixType, LabelPixelType>  ConfusionMatrixToMassOfBeliefType;
  typedef ConfusionMatrixToMassOfBeliefType::MapOfClassesType                      MapOfClassesType;

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
    SetDescription("Fuses several classifications maps of the same image on the basis of class labels.");
    SetDocName("Fusion of Classifications");
    SetDocLongDescription("This application allows you to fuse several classification maps and produces a single more robust classification map. "
        "Fusion is done either by mean of Majority Voting, or with the Dempster Shafer combination method on class labels.\n "
        "-MAJORITY VOTING: for each pixel, the class with the highest number of votes is selected.\n "
        "-DEMPSTER SHAFER: for each pixel, the class label for which the Belief Function is maximal is selected. This Belief Function is calculated "
        "by mean of the Dempster Shafer combination of Masses of Belief, and indicates the belief that each input classification map presents for each label "
        "value. Moreover, the Masses of Belief are based on the input confusion matrices of each classification map, either by using the PRECISION or RECALL "
        "rates, or the OVERALL ACCURACY, or the KAPPA coefficient. Thus, each input classification map needs to be associated with its corresponding input "
        "confusion matrix file for the Dempster Shafer fusion.\n"
        "-Input pixels with the NODATA label are not handled in the fusion of classification maps. Moreover, pixels for which all the input classifiers are set to NODATA "
        "keep this value in the output fused image.\n"
        "-In case of number of votes equality, the UNDECIDED label is attributed to the pixel.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("ImageClassifier application");

    AddDocTag(Tags::Learning);
    AddDocTag(Tags::Analysis);

    AddParameter(ParameterType_InputImageList, "il", "Input classifications");
    SetParameterDescription( "il", "List of input classification maps to fuse. Labels in each classification image must represent the same class." );


    /** GROUP FUSION METHOD */
    AddParameter(ParameterType_Choice, "method", "Fusion method");
    SetParameterDescription("method", "Selection of the fusion method and its parameters.");

    // Majority Voting
    AddChoice("method.majorityvoting","Majority Voting");
    SetParameterDescription("method.majorityvoting","Fusion of classification maps by majority voting for each output pixel.");

    // Dempster Shafer
    AddChoice("method.dempstershafer","Dempster Shafer combination");
    SetParameterDescription("method.dempstershafer","Fusion of classification maps by the Dempster Shafer combination method for each output pixel.");

    AddParameter(ParameterType_InputFilenameList, "method.dempstershafer.cmfl", "Confusion Matrices");
    SetParameterDescription("method.dempstershafer.cmfl", "A list of confusion matrix files (*.CSV format) to define the masses of belief and the class labels. Each file should be formatted the following way: "
        "the first line, beginning with a '#' symbol, should be a list of the class labels present in the corresponding input classification image, organized in the same order as the confusion matrix rows/columns.");

    AddParameter(ParameterType_Choice, "method.dempstershafer.mob", "Mass of belief measurement");
    SetParameterDescription("method.dempstershafer.mob","Type of confusion matrix measurement used to compute the masses of belief of each classifier.");
    AddChoice("method.dempstershafer.mob.precision","Precision");
    SetParameterDescription("method.dempstershafer.mob.precision","Masses of belief = Precision rates of each classifier (one rate per class label).");
    AddChoice("method.dempstershafer.mob.recall", "Recall");
    SetParameterDescription("method.dempstershafer.mob.recall", "Masses of belief = Recall rates of each classifier (one rate per class label).");
    AddChoice("method.dempstershafer.mob.accuracy", "Overall Accuracy");
    SetParameterDescription("method.dempstershafer.mob.accuracy", "Mass of belief = Overall Accuracy of each classifier (one unique value for all the class labels).");
    AddChoice("method.dempstershafer.mob.kappa", "Kappa");
    SetParameterDescription("method.dempstershafer.mob.kappa", "Mass of belief = Kappa coefficient of each classifier (one unique value for all the class labels).");

    AddParameter(ParameterType_Int, "nodatalabel", "Label for the NoData class");
    SetParameterDescription("nodatalabel", "Label for the NoData class. Such input pixels keep their NoData label in the output image and are not handled in the fusion process. By default, 'nodatalabel = 0'.");
    SetDefaultParameterInt("nodatalabel", 0);
    //MandatoryOff("nodatalabel");

    AddParameter(ParameterType_Int,"undecidedlabel","Label for the Undecided class");
    SetParameterDescription("undecidedlabel","Label for the Undecided class. Pixels with more than 1 fused class are marked as Undecided. Please note that the Undecided value must be different from existing "
        "labels in the input classifications. By default, 'undecidedlabel = 0'.");
    SetDefaultParameterInt("undecidedlabel",0);

    AddParameter(ParameterType_OutputImage,"out","The output classification image");
    SetParameterDescription("out","The output classification image resulting from the fusion of the input classification images.");
    SetDefaultOutputPixelType("out",ImagePixelType_uint8);

    // Doc example parameter settings
    SetDocExampleParameterValue("il", "classification1.tif classification2.tif classification3.tif");
    SetDocExampleParameterValue("method", "dempstershafer");
    SetDocExampleParameterValue("method.dempstershafer.cmfl", "classification1.csv classification2.csv classification3.csv");
    SetDocExampleParameterValue("method.dempstershafer.mob", "precision");
    SetDocExampleParameterValue("nodatalabel","0");
    SetDocExampleParameterValue("undecidedlabel","10");
    SetDocExampleParameterValue("out","classification_fused.tif");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }


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


  void DoExecute()
  {
    // Clear any previous filter
    m_Filters.clear();

    FloatVectorImageListType* imageList = GetParameterImageList("il");

    if (GetParameterInt("method") == Method_Majority_Voting)
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
        //setting default to suppress warning
        massOfBeliefDefMethod = ConfusionMatrixToMassOfBeliefType::PRECISION;
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

          MapOfClassesType mapOfClassesClk;
          ConfusionMatrixType confusionMatrixClk;
          CSVConfusionMatrixFileReader(confusionMatricesFilenameList[imageId], mapOfClassesClk, confusionMatrixClk);

          confusionMatrixToMassOfBeliefFilter->SetMapOfClasses(mapOfClassesClk);
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
          otbAppLogINFO("\tConfusion Matrix file_" << k << ": " << confusionMatricesFilenameList[k]);
          for (itMap = mapOfMassesOfBelief.begin(); itMap != mapOfMassesOfBelief.end(); ++itMap)
            {
            otbAppLogINFO("\t\tMassOfBelief_Cl_" << k << "[label_" << itMap->first << "] = " << itMap->second );
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
