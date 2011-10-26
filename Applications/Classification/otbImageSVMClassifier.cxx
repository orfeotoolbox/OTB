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
  ImageSVMClassifier()
  {
    SetName("ImageSVMClassifier");
    SetDescription("Perform SVM classification based on a previous computed SVM model");
    
    // Documentation
    SetDocName("Image SVM Classifier Application");
    SetDocLongDescription("This application performs an image classification based on the SVM classifier."
        "The image to classify and the SVM model are given in input, the application will generate the classified output image. "
        " Optionally, the user can give an image statistics file (that contains min, max) to normalize the input image before the classification."
        " Furthemore, the user can give a mask to define area of work (only pixels with values greater to 0 will be proceed), "
        "these no classify pixels will appear in the output image with the value 0.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("TrainSVMImagesClassifier, ValidateSVMImagesClassifier, EstimatesImagesStatistics");
    SetDocCLExample("otbApplicationLauncherCommandLine ImageSVMClassifier ${OTB-BIN}/bin --in ${OTB-DATA}/Classification/QB_1_ortho.tif  --imstat ${OTB-DATA}/Baseline/OTB-Applications/Files/clImageStatisticsQB1.xml --svn ${OTB-DATA}/Baseline/OTB-Applications/Files/clsvmModelQB1.svm --out otbConcatenateImages.png uchar");
    AddDocTag("Classification");
    AddDocTag("SVM");

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
    MandatoryOff("mask");

    AddParameter(ParameterType_Filename, "imstat", "Image statistics file.");
    SetParameterDescription("imstat", "a XML file containing mean and standard deviation of input images used to train svm model");
    MandatoryOff("imstat");

    AddParameter(ParameterType_Filename, "svm", "SVM Model.");
    SetParameterDescription("svm", "An estimated SVM model previously computed");
    
    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription( "out", "Output labeled image");
    SetParameterOutputImagePixelType( "out", ImagePixelType_uint8);
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
    if( HasValue("imstat")  )
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
    
  
    if( HasValue("mask")  )
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
