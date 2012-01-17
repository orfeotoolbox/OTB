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
  typedef itk::VariableLengthVector<FloatVectorImageType::InternalPixelType>            MeasurementType;
  typedef otb::StatisticsXMLFileReader<MeasurementType>                                 StatisticsReader;
  typedef otb::ShiftScaleVectorImageFilter<FloatVectorImageType, FloatVectorImageType>  RescalerType;
  typedef otb::SVMImageClassificationFilter<FloatVectorImageType, UInt8ImageType>       ClassificationFilterType;
  typedef ClassificationFilterType::Pointer                                             ClassificationFilterPointerType;
  typedef ClassificationFilterType::ModelType                                           ModelType;
  typedef ModelType::Pointer                                                            ModelPointerType;

private:
  void DoInit()
  {
    SetName("ImageSVMClassifier");
    SetDescription("Performs a SVM classification of the input image according to a SVM model file.");
    
    // Documentation
    SetDocName("Image SVM Classification");
    SetDocLongDescription("This application performs a SVM image classification based on a SVM model file (*.svm extension) produced by the TrainSVMImagesClassifier application. Pixels of the output image will contain the class label decided by the SVM classifier. The input pixels can be optionnaly centered and reduced according to the statistics file produced by the ComputeImagesStatistics application. An optional input mask can be provided, in which case only input image pixels whose corresponding mask value is greater than 0 will be classified. The remaining of pixels will be given the label 0 in the output image.");

    SetDocLimitations("The input image must have the same type, order and number of bands than the images used to produce the statistics file and the SVM model file. If a statistics file was used during training by the TrainSVMImagesClassifier, it is mandatory to use the same statistics file for classification. If an input mask is used, its size must match the input image size.");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("TrainSVMImagesClassifier, ValidateSVMImagesClassifier, ComputeImagesStatistics");
 
    AddDocTag(Tags::Learning);

    AddParameter(ParameterType_InputImage, "in",  "Input Image");
    SetParameterDescription( "in", "The input image to classify.");

    AddParameter(ParameterType_InputImage,  "mask",   "Input Mask");
    SetParameterDescription( "mask", "The mask allows to restrict classification of the input image to the area where mask pixel values are greater than 0.");
    MandatoryOff("mask");

    AddParameter(ParameterType_Filename, "svm", "SVM Model file");
    SetParameterDescription("svm", "A SVM model file.");

    AddParameter(ParameterType_Filename, "imstat", "Statistics file");
    SetParameterDescription("imstat", "A XML file containing mean and standard deviation to center and reduce samples before classification.");
    MandatoryOff("imstat");
    
    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription( "out", "Output image labeled with class labels");
    SetParameterOutputImagePixelType( "out", ImagePixelType_uint8);

    AddParameter(ParameterType_RAM, "ram", "Available RAM");
    SetDefaultParameterInt("ram", 256);
    MandatoryOff("ram");

   // Doc example parameter settings
    SetDocExampleParameterValue("in", "QB_1_ortho.tif");
    SetDocExampleParameterValue("imstat", "EstimateImageStatisticsQB1.xml");
    SetDocExampleParameterValue("svm", "clsvmModelQB1.svm");
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

    // Load svm model
    otbAppLogINFO("Loading SVM model");
    m_ModelSVM = ModelType::New();
    m_ModelSVM->LoadModel(GetParameterString("svm").c_str());
    otbAppLogINFO("SVM model loaded");

    // Normalize input image (optional)
    StatisticsReader::Pointer  statisticsReader = StatisticsReader::New();
    MeasurementType  meanMeasurementVector;
    MeasurementType  stddevMeasurementVector;
    m_Rescaler = RescalerType::New();
    
    // Classify
    m_ClassificationFilter = ClassificationFilterType::New();
    m_ClassificationFilter->SetModel(m_ModelSVM);
  
    // Normalize input image if asked
    if(IsParameterEnabled("imstat")  )
      {
      otbAppLogINFO("Input image normalization activated.");
      // Load input image statistics
      statisticsReader->SetFileName(GetParameterString("imstat"));
      meanMeasurementVector   = statisticsReader->GetStatisticVectorByName("mean");
      stddevMeasurementVector = statisticsReader->GetStatisticVectorByName("stddev");
      otbAppLogINFO( "mean used: " << meanMeasurementVector );
      otbAppLogINFO( "standard deviation used: " << stddevMeasurementVector );
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
      UInt8ImageType::Pointer inMask = GetParameterUInt8Image("mask");
      
      m_ClassificationFilter->SetInputMask(inMask);
      }

    SetParameterOutputImage<UInt8ImageType>("out", m_ClassificationFilter->GetOutput());
  }

  ClassificationFilterType::Pointer m_ClassificationFilter;
  ModelPointerType m_ModelSVM;
  RescalerType::Pointer m_Rescaler;
};


}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ImageSVMClassifier)
