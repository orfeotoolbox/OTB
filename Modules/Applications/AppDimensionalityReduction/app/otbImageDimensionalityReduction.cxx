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
#include "otbImageDimensionalityReductionFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "otbImageToVectorImageCastFilter.h"
#include "otbDimensionalityReductionModelFactory.h"

namespace otb
{
namespace Functor
{
/**
 * simple affine function : y = ax+b
 */
template<class TInput, class TOutput>
class AffineFunctor
{
public:
  typedef double InternalType;
  
  // constructor
  AffineFunctor() : m_A(1.0),m_B(0.0) {}
  
  // destructor
  virtual ~AffineFunctor() {}
  
  void SetA(InternalType a)
    {
    m_A = a;
    }
  
  void SetB(InternalType b)
    {
    m_B = b;
    }
  
  inline TOutput operator()(const TInput & x) const
    {
    return static_cast<TOutput>( static_cast<InternalType>(x)*m_A + m_B);
    }
private:
  InternalType m_A;
  InternalType m_B;
};
  
} // end of namespace Functor

namespace Wrapper
{
/**
 * \class ImageDimensionalityReduction
 *
 * Apply a dimensionality reduction model to an image
 */
class ImageDimensionalityReduction : public Application
{
public:
  /** Standard class typedefs. */
  typedef ImageDimensionalityReduction             Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ImageDimensionalityReduction, otb::Application);

  /** Filters typedef */
  typedef UInt8ImageType                                  MaskImageType;
  typedef itk::VariableLengthVector<
    FloatVectorImageType::InternalPixelType>              MeasurementType;
  typedef otb::StatisticsXMLFileReader<MeasurementType>   StatisticsReader;
  typedef otb::ShiftScaleVectorImageFilter<
    FloatVectorImageType, FloatVectorImageType>           RescalerType;
  typedef itk::UnaryFunctorImageFilter<
      FloatImageType,
      FloatImageType,
      otb::Functor::AffineFunctor<float,float> >          OutputRescalerType;
  typedef otb::ImageDimensionalityReductionFilter<
    FloatVectorImageType,
    FloatVectorImageType,
    MaskImageType>                                        DimensionalityReductionFilterType;
  typedef DimensionalityReductionFilterType::Pointer      DimensionalityReductionFilterPointerType;
  typedef DimensionalityReductionFilterType::ModelType    ModelType;
  typedef ModelType::Pointer                              ModelPointerType;
  typedef DimensionalityReductionFilterType::ValueType    ValueType;
  typedef DimensionalityReductionFilterType::LabelType    LabelType;
  typedef otb::DimensionalityReductionModelFactory<
    ValueType, LabelType>                                 DimensionalityReductionModelFactoryType;

protected:

  ~ImageDimensionalityReduction() override
  {
    DimensionalityReductionModelFactoryType::CleanFactories();
  }

private:
  void DoInit() override
  {
    SetName("ImageDimensionalityReduction");
    SetDescription("Performs dimensionality reduction of the input image "
      "according to a dimensionality reduction model file.");

    // Documentation
    SetDocName("Image Dimensionality Reduction");
    SetDocLongDescription("This application reduces the dimension of an input"
                          " image, based on a machine learning model file produced by"
                          " the TrainDimensionalityReduction application. Pixels of the "
                          "output image will contain the reduced values from"
                          "the model. The input pixels"
                          " can be optionally centered and reduced according "
                          "to the statistics file produced by the "
                          "ComputeImagesStatistics application.");

    SetDocLimitations("The input image must contain the feature bands used for"
                      " the model training. "
                      "If a statistics file was used during training by the "
                      "Training application, it is mandatory to use the same "
                      "statistics file for reduction.");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("TrainDimensionalityReduction, ComputeImagesStatistics");

    AddDocTag(Tags::Learning);

    AddParameter(ParameterType_InputImage, "in",  "Input Image");
    SetParameterDescription( "in", "The input image to predict.");

    AddParameter(ParameterType_InputImage,  "mask",   "Input Mask");
    SetParameterDescription( "mask", "The mask allow restricting "
      "classification of the input image to the area where mask pixel values "
      "are greater than 0.");
    MandatoryOff("mask");

    AddParameter(ParameterType_InputFilename, "model", "Model file");
    SetParameterDescription("model", "A dimensionality reduction model file (produced by "
                            "TrainRegression application).");

    AddParameter(ParameterType_InputFilename, "imstat", "Statistics file");
    SetParameterDescription("imstat", "A XML file containing mean and standard"
      " deviation to center and reduce samples before prediction "
      "(produced by ComputeImagesStatistics application). If this file contains"
                            "one more bands than the sample size, the last stat of last band will be"
                            "applied to expand the output predicted value");
    MandatoryOff("imstat");

    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription( "out", "Output image containing reduced values");

    AddRAMParameter();

   // Doc example parameter settings
    SetDocExampleParameterValue("in", "QB_1_ortho.tif");
    SetDocExampleParameterValue("imstat", "EstimateImageStatisticsQB1.xml");
    SetDocExampleParameterValue("model", "clsvmModelQB1.model");
    SetDocExampleParameterValue("out", "ReducedImageQB1.tif");
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
    unsigned int nbFeatures = inImage->GetNumberOfComponentsPerPixel();

    // Load DR model using a factory
    otbAppLogINFO("Loading model");
    m_Model = DimensionalityReductionModelFactoryType::CreateDimensionalityReductionModel(
      GetParameterString("model"),
      DimensionalityReductionModelFactoryType::ReadMode);

    if (m_Model.IsNull())
      {
      otbAppLogFATAL(<< "Error when loading model " << GetParameterString("model")
        << " : unsupported model type");
      }

    m_Model->Load(GetParameterString("model"));
    otbAppLogINFO("Model loaded, dimension = "<< m_Model->GetDimension());

    // Classify
    m_ClassificationFilter = DimensionalityReductionFilterType::New();
    m_ClassificationFilter->SetModel(m_Model);
    
    FloatVectorImageType::Pointer outputImage = m_ClassificationFilter->GetOutput();

    // Normalize input image if asked
    if( IsParameterEnabled("imstat") && HasValue("imstat") )
      {
      otbAppLogINFO("Input image normalization activated.");
      // Normalize input image (optional)
      StatisticsReader::Pointer  statisticsReader = StatisticsReader::New();
      MeasurementType  meanMeasurementVector;
      MeasurementType  stddevMeasurementVector;
      m_Rescaler = RescalerType::New();
      
      // Load input image statistics
      statisticsReader->SetFileName(GetParameterString("imstat"));
      meanMeasurementVector   = statisticsReader->GetStatisticVectorByName("mean");
      stddevMeasurementVector = statisticsReader->GetStatisticVectorByName("stddev");
      otbAppLogINFO( "mean used: " << meanMeasurementVector );
      otbAppLogINFO( "standard deviation used: " << stddevMeasurementVector );
      if (meanMeasurementVector.Size() != nbFeatures)
        {
        otbAppLogFATAL("Wrong number of components in statistics file : "<<meanMeasurementVector.Size());
        }
        
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

    if(IsParameterEnabled("mask"))
      {
      otbAppLogINFO("Using input mask");
      // Load mask image and cast into LabeledImageType
      MaskImageType::Pointer inMask = GetParameterUInt8Image("mask");

      m_ClassificationFilter->SetInputMask(inMask);
      }

    SetParameterOutputImage<FloatVectorImageType>("out", outputImage);
  }

  DimensionalityReductionFilterType::Pointer m_ClassificationFilter;
  ModelPointerType m_Model;
  RescalerType::Pointer m_Rescaler;
  OutputRescalerType::Pointer m_OutRescaler;
};

} // end of namespace Wrapper
} // end of namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::ImageDimensionalityReduction)
