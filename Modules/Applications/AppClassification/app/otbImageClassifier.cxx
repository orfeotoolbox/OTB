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

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbChangeLabelImageFilter.h"
#include "otbStandardWriterWatcher.h"
#include "otbStatisticsXMLFileReader.h"
#include "otbShiftScaleVectorImageFilter.h"
#include "otbImageClassificationFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "otbImageToVectorImageCastFilter.h"
#include "otbMachineLearningModelFactory.h"

namespace otb
{
namespace Wrapper
{

class ImageClassifier : public Application
{
public:
  /** Standard class typedefs. */
  typedef ImageClassifier               Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ImageClassifier, otb::Application);

  /** Filters typedef */
  // typedef UInt16ImageType                                                                    OutputImageType;
  typedef Int32ImageType                                                     OutputImageType;
  typedef UInt8ImageType                                                     MaskImageType;
  typedef itk::VariableLengthVector<FloatVectorImageType::InternalPixelType> MeasurementType;
  typedef otb::StatisticsXMLFileReader<MeasurementType>                      StatisticsReader;
  typedef otb::ShiftScaleVectorImageFilter<FloatVectorImageType, FloatVectorImageType> RescalerType;
  typedef otb::ImageClassificationFilter<FloatVectorImageType, OutputImageType, MaskImageType> ClassificationFilterType;
  typedef ClassificationFilterType::Pointer   ClassificationFilterPointerType;
  typedef ClassificationFilterType::ModelType ModelType;
  typedef ModelType::Pointer                  ModelPointerType;
  typedef ClassificationFilterType::ValueType ValueType;
  typedef ClassificationFilterType::LabelType LabelType;
  typedef otb::MachineLearningModelFactory<ValueType, LabelType> MachineLearningModelFactoryType;
  typedef ClassificationFilterType::ConfidenceImageType ConfidenceImageType;
  typedef ClassificationFilterType::ProbaImageType      ProbaImageType;

protected:
  ~ImageClassifier() override
  {
    MachineLearningModelFactoryType::CleanFactories();
  }

private:
  void DoInit() override
  {
    SetName("ImageClassifier");
    SetDescription("Performs a classification of the input image according to a model file.");

    // Documentation
    SetDocLongDescription(
        "This application performs an image classification based on a model file produced by the TrainImagesClassifier application. Pixels of the output image "
        "will contain the class labels decided by the classifier (maximal class label = 65535). The input pixels can be optionally centered and reduced "
        "according to the statistics file produced by the ComputeImagesStatistics application. An optional input mask can be provided, in which case only "
        "input image pixels whose corresponding mask value is greater than 0 will be classified. By default, the remaining pixels will be given the label 0 in "
        "the output image.");

    SetDocLimitations(
        "The input image must have the same type, order and number of bands as the images used to produce the statistics file and the SVM model file. If a "
        "statistics file was used during training by the TrainImagesClassifier, it is mandatory to use the same statistics file for classification. If an "
        "input mask is used, its size must match the input image size.");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("TrainImagesClassifier, ComputeImagesStatistics");

    AddDocTag(Tags::Learning);

    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in", "The input image to classify.");

    AddParameter(ParameterType_InputImage, "mask", "Input Mask");
    SetParameterDescription("mask", "The mask restricts the classification of the input image to the area where mask pixel values are greater than 0.");
    MandatoryOff("mask");

    AddParameter(ParameterType_InputFilename, "model", "Model file");
    SetParameterDescription("model", "A model file (produced by TrainImagesClassifier application, maximal class label = 65535).");

    AddParameter(ParameterType_InputFilename, "imstat", "Statistics file");
    SetParameterDescription("imstat",
                            "An XML file containing mean and standard deviation to center and reduce samples before classification (produced by "
                            "ComputeImagesStatistics application).");
    MandatoryOff("imstat");

    AddParameter(ParameterType_Int, "nodatalabel", "Label mask value");
    SetParameterDescription("nodatalabel",
                            "By default, "
                            "hidden pixels will have the assigned label 0 in the output image. "
                            "It is possible to define the label mask by another value, "
                            "but be careful not to use a label from another class (max. 65535).");

    SetDefaultParameterInt("nodatalabel", 0);
    MandatoryOff("nodatalabel");


    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out", "Output image containing class labels");
    SetDefaultOutputPixelType("out", ImagePixelType_uint8);

    AddParameter(ParameterType_OutputImage, "confmap", "Confidence map");
    SetParameterDescription("confmap",
                            "Confidence map of the produced classification. The confidence index depends on the model: \n\n"
                            "* LibSVM: difference between the two highest probabilities (needs a model with probability estimates, so that classes "
                            "probabilities can be computed for each sample)\n"
                            "* Boost: sum of votes\n"
                            "* DecisionTree: (not supported)\n"
                            "* KNearestNeighbors: number of neighbors with the same label\n"
                            "* NeuralNetwork: difference between the two highest responses\n"
                            "* NormalBayes: (not supported)\n"
                            "* RandomForest: Confidence (proportion of votes for the majority class). Margin (normalized difference of the votes of the 2 "
                            "majority classes) is not available for now.\n"
                            "* SVM: distance to margin (only works for 2-class models)\n");
    SetDefaultOutputPixelType("confmap", ImagePixelType_double);
    MandatoryOff("confmap");

    AddParameter(ParameterType_OutputImage, "probamap", "Probability map");
    SetParameterDescription("probamap",
                            "Probability of each class for each pixel. This is an image having a number of bands equal to the number of classes in the model. "
                            "This is only implemented for the Shark Random Forest classifier at this point.");
    SetDefaultOutputPixelType("probamap", ImagePixelType_uint16);
    MandatoryOff("probamap");

    AddRAMParameter();

    SetMultiWriting(true);

    AddParameter(ParameterType_Int, "nbclasses", "Number of classes in the model");
    SetDefaultParameterInt("nbclasses", 20);
    SetParameterDescription("nbclasses", "The number of classes is required by the output of the probability map in order to set the number of output bands.");

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "QB_1_ortho.tif");
    SetDocExampleParameterValue("imstat", "EstimateImageStatisticsQB1.xml");
    SetDocExampleParameterValue("model", "clsvmModelQB1.svm");
    SetDocExampleParameterValue("out", "clLabeledImageQB1.tif");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute() override
  {
    // Load input image
    FloatVectorImageType::Pointer inImage = GetParameterImage("in");
    inImage->UpdateOutputInformation();

    // Load svm model
    otbAppLogINFO("Loading model");
    m_Model = MachineLearningModelFactoryType::CreateMachineLearningModel(GetParameterString("model"), MachineLearningModelFactoryType::ReadMode);

    if (m_Model.IsNull())
    {
      otbAppLogFATAL(<< "Error when loading model " << GetParameterString("model") << " : unsupported model type");
    }

    m_Model->Load(GetParameterString("model"));
    otbAppLogINFO("Model loaded");

    // Normalize input image (optional)
    StatisticsReader::Pointer statisticsReader = StatisticsReader::New();
    MeasurementType           meanMeasurementVector;
    MeasurementType           stddevMeasurementVector;
    m_Rescaler = RescalerType::New();

    // Classify
    m_ClassificationFilter = ClassificationFilterType::New();
    m_ClassificationFilter->SetModel(m_Model);

    m_ClassificationFilter->SetDefaultLabel(GetParameterInt("nodatalabel"));

    // Normalize input image if asked
    if (IsParameterEnabled("imstat"))
    {
      otbAppLogINFO("Input image normalization activated.");
      // Load input image statistics
      statisticsReader->SetFileName(GetParameterString("imstat"));
      meanMeasurementVector   = statisticsReader->GetStatisticVectorByName("mean");
      stddevMeasurementVector = statisticsReader->GetStatisticVectorByName("stddev");
      otbAppLogINFO("mean used: " << meanMeasurementVector);
      otbAppLogINFO("standard deviation used: " << stddevMeasurementVector);
      // Rescale vector image
      m_Rescaler->SetScale(stddevMeasurementVector);
      m_Rescaler->SetShift(meanMeasurementVector);
      m_Rescaler->SetInput(inImage);

      m_ClassificationFilter->SetInput(m_Rescaler->GetOutput());
    }
    else
    {
      otbAppLogINFO("Input image normalization deactivated.");
      m_ClassificationFilter->SetInput(inImage);
    }


    if (IsParameterEnabled("mask"))
    {
      otbAppLogINFO("Using input mask");
      // Load mask image and cast into LabeledImageType
      MaskImageType::Pointer inMask = GetParameterUInt8Image("mask");

      m_ClassificationFilter->SetInputMask(inMask);
    }
    SetParameterOutputImage<OutputImageType>("out", m_ClassificationFilter->GetOutput());


    // output confidence map
    if (IsParameterEnabled("confmap") && HasValue("confmap"))
    {
      m_ClassificationFilter->SetUseConfidenceMap(true);
      if (m_Model->HasConfidenceIndex())
      {
        SetParameterOutputImage<ConfidenceImageType>("confmap", m_ClassificationFilter->GetOutputConfidence());
      }
      else
      {
        otbAppLogWARNING("Confidence map requested but the classifier doesn't support it!");
        this->DisableParameter("confmap");
      }
    }
    if (IsParameterEnabled("probamap") && HasValue("probamap"))
    {
      m_ClassificationFilter->SetUseProbaMap(true);
      if (m_Model->HasProbaIndex())
      {
        m_ClassificationFilter->SetNumberOfClasses(GetParameterInt("nbclasses"));
        SetParameterOutputImage<ProbaImageType>("probamap", m_ClassificationFilter->GetOutputProba());
      }
      else
      {
        otbAppLogWARNING("Probability map requested but the classifier doesn't support it!");
        this->DisableParameter("probamap");
      }
    }
  }

  ClassificationFilterType::Pointer m_ClassificationFilter;
  ModelPointerType                  m_Model;
  RescalerType::Pointer             m_Rescaler;
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ImageClassifier)
