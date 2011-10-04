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

#include "itkVariableLengthVector.h"
#include "otbChangeLabelImageFilter.h"
#include "otbStandardWriterWatcher.h"
#include "otbStatisticsXMLFileReader.h"
#include "otbShiftScaleVectorImageFilter.h"
#include "otbSVMImageClassificationFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "otbImageToVectorImageCastFilter.h"

namespace otb
{
namespace Wrapper
{

class ImageSVMClassifier : public Application
{
public:
  /** Standard class typedefs. */
  typedef ImageSVMClassifier            Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ImageSVMClassifier, otb::Application);

  /** Filters typedef */
  // Statistic XML file Reader
  typedef itk::VariableLengthVector<FloatVectorImageType::InternalPixelType>            MeasurementType;
  typedef otb::StatisticsXMLFileReader<MeasurementType>                                 StatisticsReader;
  typedef otb::ShiftScaleVectorImageFilter<FloatVectorImageType, FloatVectorImageType>  RescalerType;

  /// Classification typedefs
  typedef otb::SVMImageClassificationFilter<FloatVectorImageType, UInt8ImageType>   ClassificationFilterType;
  typedef ClassificationFilterType::Pointer                                      ClassificationFilterPointerType;
  typedef ClassificationFilterType::ModelType                                    ModelType;
  typedef ModelType::Pointer                                                     ModelPointerType;

  // Cast filter
  // TODO: supress that !!
  typedef MultiToMonoChannelExtractROI<FloatVectorImageType::InternalPixelType, 
                                       UInt8ImageType::PixelType>               ExtractImageFilterType;
  typedef ImageToVectorImageCastFilter<UInt8ImageType, FloatVectorImageType>    CastImageFilterType;

private:
  ImageSVMClassifier()
  {
    SetName("ImageSVMClassifier");
    SetDescription("Perform SVM classification based a previous computed svm model to an new input image.");
  }

  virtual ~ImageSVMClassifier()
  {
  }

  void DoCreateParameters()
  {
    AddParameter(ParameterType_InputImage, "in",  "Input Image to classify");
    SetParameterDescription( "in", "Input Image to classify");

    AddParameter(ParameterType_InputImage,  "mask",   "Input Mask to classify");
    SetParameterDescription( "mask", "A mask associated with the new image to classify");

    AddParameter(ParameterType_Filename, "imstat", "Image statistics file.");
    SetParameterDescription("imstat", "a XML file containing mean and standard deviation of input images used to train svm model.");
    MandatoryOff("instat");

    AddParameter(ParameterType_Filename, "svmmodel", "SVM Model.");
    SetParameterDescription("svmmodel", "An estimated svm model previously computed");
    
    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription( "out", "Output labeled image");

  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {
    otbAppLogDEBUG("Entering DoExecute");

    // Load input image
    FloatVectorImageType::Pointer inImage = GetParameterImage("in");
    inImage->UpdateOutputInformation();

    // Load svm model
    ModelPointerType modelSVM = ModelType::New();
    modelSVM->LoadModel(GetParameterString("svmmodel").c_str());


    // Normalize input image (optional)
    StatisticsReader::Pointer  statisticsReader = StatisticsReader::New();
    MeasurementType  meanMeasurementVector;
    MeasurementType  stddevMeasurementVector;
    RescalerType::Pointer rescaler = RescalerType::New();
    
    // Classify
    ClassificationFilterType::Pointer classificationFilter = ClassificationFilterType::New();
    classificationFilter->SetModel(modelSVM);


    // Normalize input image if asked
    if( HasValue("imstat")  )
      {
      otbAppLogDEBUG("Input image normalization activated.");
      // Load input image statistics
      statisticsReader->SetFileName(GetParameterString("imstat"));
      meanMeasurementVector   = statisticsReader->GetStatisticVectorByName("mean");
      stddevMeasurementVector = statisticsReader->GetStatisticVectorByName("stddev");
      otbAppLogDEBUG( "mean used: " << meanMeasurementVector );
      otbAppLogDEBUG( "standard deviation used: " << stddevMeasurementVector );
      // Rescale vector image
      rescaler->SetScale(stddevMeasurementVector);
      rescaler->SetShift(meanMeasurementVector);
      rescaler->SetInput(inImage);
      
      classificationFilter->SetInput(rescaler->GetOutput());
      }
    else
      {
      otbAppLogDEBUG("Input image normalization deactivated.");
      classificationFilter->SetInput(inImage);
      }
    
    if( HasValue("mask")  )
      {
      otbAppLogDEBUG("Use input mask.");
      // Load mask image and cast into LabeledImageType
      FloatVectorImageType::Pointer inMask = GetParameterImage("mask");
      ExtractImageFilterType::Pointer extract = ExtractImageFilterType::New();
      extract->SetInput( inMask );
      extract->SetChannel(0);
      extract->UpdateOutputInformation();
      
      classificationFilter->SetInputMask(extract->GetOutput());
      }
    
    
    CastImageFilterType::Pointer finalCast = CastImageFilterType::New();
    finalCast->SetInput( classificationFilter->GetOutput() );
    
    SetParameterOutputImage("out", finalCast->GetOutput());
  }

  //itk::LightObject::Pointer m_FilterRef;
};



}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ImageSVMClassifier)
