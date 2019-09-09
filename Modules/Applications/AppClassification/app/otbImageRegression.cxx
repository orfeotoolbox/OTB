/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#include "itkUnaryFunctorImageFilter.h"
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
namespace Functor
{
/**
 * simple affine function : y = ax+b
 */
template <class TInput, class TOutput>
class AffineFunctor
{
public:
  typedef double InternalType;

  void SetA(InternalType a)
  {
    m_A = a;
  }

  void SetB(InternalType b)
  {
    m_B = b;
  }

  inline TOutput operator()(const TInput& x) const
  {
    return static_cast<TOutput>(static_cast<InternalType>(x) * m_A + m_B);
  }

private:
  InternalType m_A = 1.0;
  InternalType m_B = 0.0;
};
}

namespace Wrapper
{

class ImageRegression : public Application
{
public:
  /** Standard class typedefs. */
  typedef ImageRegression               Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ImageRegression, otb::Application);

  /** Filters typedef */
  typedef UInt8ImageType                                                     MaskImageType;
  typedef itk::VariableLengthVector<FloatVectorImageType::InternalPixelType> MeasurementType;
  typedef otb::StatisticsXMLFileReader<MeasurementType>                      StatisticsReader;
  typedef otb::ShiftScaleVectorImageFilter<FloatVectorImageType, FloatVectorImageType> RescalerType;
  typedef itk::UnaryFunctorImageFilter<FloatImageType, FloatImageType, otb::Functor::AffineFunctor<float, float>> OutputRescalerType;
  typedef otb::ImageClassificationFilter<FloatVectorImageType, FloatImageType, MaskImageType> ClassificationFilterType;
  typedef ClassificationFilterType::Pointer   ClassificationFilterPointerType;
  typedef ClassificationFilterType::ModelType ModelType;
  typedef ModelType::Pointer                  ModelPointerType;
  typedef ClassificationFilterType::ValueType ValueType;
  typedef ClassificationFilterType::LabelType LabelType;
  typedef otb::MachineLearningModelFactory<ValueType, LabelType> MachineLearningModelFactoryType;

protected:
  ~ImageRegression() override
  {
    MachineLearningModelFactoryType::CleanFactories();
  }

private:
  void DoInit() override
  {
    SetName("ImageRegression");
    SetDescription("Performs a prediction of the input image according to a regression model file.");

    // Documentation
    SetDocLongDescription(
        "This application predict output values from an input "
        "image, based on a regression model file produced either by "
        "TrainVectorRegression or TrainImagesRegression. "
        "Pixels of the output image will contain the predicted values from "
        "the regression model (single band). The input pixels "
        "can be optionally centered and reduced according "
        "to the statistics file produced by the "
        "ComputeImagesStatistics application. An optional "
        "input mask can be provided, in which case only "
        "input image pixels whose corresponding mask value "
        "is greater than zero will be processed. The remaining "
        "of pixels will be given the value zero in the output "
        "image.");

    SetDocLimitations(
        "The input image must contain the feature bands used for "
        "the model training. "
        "If a statistics file was used during training by the "
        "TrainRegression, it is mandatory to use the same "
        "statistics file for prediction. If an input mask is "
        "used, its size must match the input image size.");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("TrainImagesRegression, TrainVectorRegression, VectorRegression, ComputeImagesStatistics");

    AddDocTag(Tags::Learning);

    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in", "The input image to predict.");

    AddParameter(ParameterType_InputImage, "mask", "Input Mask");
    SetParameterDescription("mask",
                            "The mask restrict the "
                            "classification of the input image to the area where mask pixel values "
                            "are greater than zero.");
    MandatoryOff("mask");

    AddParameter(ParameterType_InputFilename, "model", "Model file");
    SetParameterDescription("model",
                            "A regression model file (produced either by "
                            "TrainVectorRegression application or the TrainImagesRegression application).");

    AddParameter(ParameterType_InputFilename, "imstat", "Statistics file");
    SetParameterDescription("imstat",
                            "An XML file containing mean and standard"
                            " deviation to center and reduce samples before prediction "
                            "(produced by the ComputeImagesStatistics application). If this file contains "
                            "one more band than the sample size, the last stat of the last band will be"
                            "applied to expand the output predicted value.");
    MandatoryOff("imstat");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out", "Output image containing predicted values");

    AddRAMParameter();

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
    auto inImage = GetParameterImage("in");
    inImage->UpdateOutputInformation();
    unsigned int nbFeatures = inImage->GetNumberOfComponentsPerPixel();

    // Load svm model
    otbAppLogINFO("Loading model");
    auto model = MachineLearningModelFactoryType::CreateMachineLearningModel(GetParameterString("model"), MachineLearningModelFactoryType::ReadMode);

    if (model.IsNull())
    {
      otbAppLogFATAL(<< "Error when loading model " << GetParameterString("model") << " : unsupported model type");
    }

    model->Load(GetParameterString("model"));
    model->SetRegressionMode(true);
    otbAppLogINFO("Model loaded");

    // Classify
    auto classificationFilter = ClassificationFilterType::New();
    classificationFilter->SetModel(model);

    auto outputImage = classificationFilter->GetOutput();

    RescalerType::Pointer             rescaler;
    OutputRescalerType::Pointer       outRescaler;
    // Normalize input image if asked
    if (IsParameterEnabled("imstat"))
    {
      otbAppLogINFO("Input image normalization activated.");
      // Normalize input image (optional)
      auto statisticsReader = StatisticsReader::New();
      MeasurementType           meanMeasurementVector;
      MeasurementType           stddevMeasurementVector;
      rescaler = RescalerType::New();
      // Load input image statistics
      statisticsReader->SetFileName(GetParameterString("imstat"));
      meanMeasurementVector   = statisticsReader->GetStatisticVectorByName("mean");
      stddevMeasurementVector = statisticsReader->GetStatisticVectorByName("stddev");
      otbAppLogINFO("mean used: " << meanMeasurementVector);
      otbAppLogINFO("standard deviation used: " << stddevMeasurementVector);
      if (meanMeasurementVector.Size() == nbFeatures + 1)
      {
        double outMean   = meanMeasurementVector[nbFeatures];
        double outStdDev = stddevMeasurementVector[nbFeatures];
        meanMeasurementVector.SetSize(nbFeatures, false);
        stddevMeasurementVector.SetSize(nbFeatures, false);
        outRescaler = OutputRescalerType::New();
        outRescaler->SetInput(classificationFilter->GetOutput());
        outRescaler->GetFunctor().SetA(outStdDev);
        outRescaler->GetFunctor().SetB(outMean);
        outputImage = outRescaler->GetOutput();
      }
      else if (meanMeasurementVector.Size() != nbFeatures)
      {
        otbAppLogFATAL("Wrong number of components in statistics file : " << meanMeasurementVector.Size());
      }

      // Rescale vector image
      rescaler->SetScale(stddevMeasurementVector);
      rescaler->SetShift(meanMeasurementVector);
      rescaler->SetInput(inImage);

      classificationFilter->SetInput(rescaler->GetOutput());
    }
    else
    {
      otbAppLogINFO("Input image normalization deactivated.");
      classificationFilter->SetInput(inImage);
    }

    if (IsParameterEnabled("mask"))
    {
      otbAppLogINFO("Using input mask");
      // Load mask image and cast into LabeledImageType
      auto inMask = GetParameterUInt8Image("mask");
      classificationFilter->SetInputMask(inMask);
    }

    SetParameterOutputImage<FloatImageType>("out", outputImage);
    RegisterPipeline();
  }
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ImageRegression)
