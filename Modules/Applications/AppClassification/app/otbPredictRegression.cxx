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
  
}

namespace Wrapper
{

class PredictRegression : public Application
{
public:
  /** Standard class typedefs. */
  typedef PredictRegression             Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(PredictRegression, otb::Application);

  /** Filters typedef */
  typedef UInt8ImageType                                                                       MaskImageType;
  typedef itk::VariableLengthVector<FloatVectorImageType::InternalPixelType>                   MeasurementType;
  typedef otb::StatisticsXMLFileReader<MeasurementType>                                        StatisticsReader;
  typedef otb::ShiftScaleVectorImageFilter<FloatVectorImageType, FloatVectorImageType>         RescalerType;
  typedef itk::UnaryFunctorImageFilter<
      FloatImageType,
      FloatImageType,
      otb::Functor::AffineFunctor<float,float> >                                               OutputRescalerType;
  typedef otb::ImageClassificationFilter<FloatVectorImageType, FloatImageType, MaskImageType>  ClassificationFilterType;
  typedef ClassificationFilterType::Pointer                                                    ClassificationFilterPointerType;
  typedef ClassificationFilterType::ModelType                                                  ModelType;
  typedef ModelType::Pointer                                                                   ModelPointerType;
  typedef ClassificationFilterType::ValueType                                                  ValueType;
  typedef ClassificationFilterType::LabelType                                                  LabelType;
  typedef otb::MachineLearningModelFactory<ValueType, LabelType>                               MachineLearningModelFactoryType;

private:
  void DoInit()
  {
    SetName("PredictRegression");
    SetDescription("Performs a prediction of the input image according to a regression model file.");

    // Documentation
    SetDocName("Predict Regression");
    SetDocLongDescription("This application predict output values from an input"
                          " image, based on a regression model file produced by"
                          " the TrainRegression application. Pixels of the "
                          "output image will contain the predicted values from"
                          "the regression model (single band). The input pixels"
                          " can be optionally centered and reduced according "
                          "to the statistics file produced by the "
                          "ComputeImagesStatistics application. An optional "
                          "input mask can be provided, in which case only "
                          "input image pixels whose corresponding mask value "
                          "is greater than 0 will be processed. The remaining"
                          " of pixels will be given the value 0 in the output"
                          " image.");

    SetDocLimitations("The input image must contain the feature bands used for"
                      " the model training (without the predicted value). "
                      "If a statistics file was used during training by the "
                      "TrainRegression, it is mandatory to use the same "
                      "statistics file for prediction. If an input mask is "
                      "used, its size must match the input image size.");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("TrainRegression, ComputeImagesStatistics");

    AddDocTag(Tags::Learning);

    AddParameter(ParameterType_InputImage, "in",  "Input Image");
    SetParameterDescription( "in", "The input image to predict.");

    // TODO : use CSV input/output ?

    AddParameter(ParameterType_InputImage,  "mask",   "Input Mask");
    SetParameterDescription( "mask", "The mask allow restricting "
      "classification of the input image to the area where mask pixel values "
      "are greater than 0.");
    MandatoryOff("mask");

    AddParameter(ParameterType_InputFilename, "model", "Model file");
    SetParameterDescription("model", "A regression model file (produced by "
      "TrainRegression application).");

    AddParameter(ParameterType_InputFilename, "imstat", "Statistics file");
    SetParameterDescription("imstat", "A XML file containing mean and standard"
      " deviation to center and reduce samples before prediction "
      "(produced by ComputeImagesStatistics application). If this file contains"
      "one more band than the sample size, the last stat of last band will be"
      "applied to expand the output predicted value");
    MandatoryOff("imstat");

    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription( "out", "Output image containing predicted values");

    AddRAMParameter();

   // Doc example parameter settings
    SetDocExampleParameterValue("in", "QB_1_ortho.tif");
    SetDocExampleParameterValue("imstat", "EstimateImageStatisticsQB1.xml");
    SetDocExampleParameterValue("model", "clsvmModelQB1.svm");
    SetDocExampleParameterValue("out", "clLabeledImageQB1.tif");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {
    // Load input image
    FloatVectorImageType::Pointer inImage = GetParameterImage("in");
    inImage->UpdateOutputInformation();
    unsigned int nbFeatures = inImage->GetNumberOfComponentsPerPixel();

    // Load svm model
    otbAppLogINFO("Loading model");
    m_Model = MachineLearningModelFactoryType::CreateMachineLearningModel(GetParameterString("model"),
                                                                          MachineLearningModelFactoryType::ReadMode);

    if (m_Model.IsNull())
      {
      otbAppLogFATAL(<< "Error when loading model " << GetParameterString("model") << " : unsupported model type");
      }

    m_Model->Load(GetParameterString("model"));
    m_Model->SetRegressionMode(true);
    otbAppLogINFO("Model loaded");

    // Classify
    m_ClassificationFilter = ClassificationFilterType::New();
    m_ClassificationFilter->SetModel(m_Model);
    
    FloatImageType::Pointer outputImage = m_ClassificationFilter->GetOutput();

    // Normalize input image if asked
    if(IsParameterEnabled("imstat")  )
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
      if (meanMeasurementVector.Size() == nbFeatures + 1)
        {
        double outMean = meanMeasurementVector[nbFeatures];
        double outStdDev = stddevMeasurementVector[nbFeatures];
        meanMeasurementVector.SetSize(nbFeatures,false);
        stddevMeasurementVector.SetSize(nbFeatures,false);
        m_OutRescaler = OutputRescalerType::New();
        m_OutRescaler->SetInput(m_ClassificationFilter->GetOutput());
        m_OutRescaler->GetFunctor().SetA(outStdDev);
        m_OutRescaler->GetFunctor().SetB(outMean);
        outputImage = m_OutRescaler->GetOutput();
        }
      else if (meanMeasurementVector.Size() != nbFeatures)
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

    SetParameterOutputImage<FloatImageType>("out", outputImage);

  }

  ClassificationFilterType::Pointer m_ClassificationFilter;
  ModelPointerType m_Model;
  RescalerType::Pointer m_Rescaler;
  OutputRescalerType::Pointer m_OutRescaler;

};


}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::PredictRegression)
