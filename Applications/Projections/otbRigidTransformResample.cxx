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

#include "itkTranslationTransform.h"
#include "otbStreamingResampleImageFilter.h"

namespace otb
{
namespace Wrapper
{

class RigidTransformResample : public Application
{
public:
  /** Standard class typedefs. */
  typedef RigidTransformResample        Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(RigidTransformResample, otb::Application);

private:

  RigidTransformResample()
  {
    SetName("RigidTransformResample");
    SetDescription("Resample an image with a rigid transform");
    // Documentation
    SetDocName("Image resampling with a rigid transform");
    SetDocLongDescription("This application performs an translation on the input image.\n Parameters of the translation can be set with tx and ty options.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("Translation application");
    //SetDocCLExample("otbApplicationLauncherCommandLine RigidTransformResample ${OTB-BIN}/bin --in ${OTB-Data}/Input/QB_Toulouse_Ortho_XS.tif --out otbConvertWithScalingOutput.png uchar --type linear");
    AddDocTag("Conversion");
    AddDocTag("Image Projection");
  }

  virtual ~RigidTransformResample()
  {
  }

  void DoCreateParameters()
  {
    AddParameter(ParameterType_InputImage,   "in",   "Input image");
    AddParameter(ParameterType_OutputImage,  "out",  "Output image");
    AddParameter(ParameterType_Float,        "tx",   "The X translation (in physical units)");
    AddParameter(ParameterType_Float,        "ty",   "The Y translation (in physical units)");
  }
  
  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  { 
    typedef itk::TranslationTransform<double, FloatVectorImageType::ImageDimension> TransformType;
    typedef otb::StreamingResampleImageFilter<FloatVectorImageType,FloatVectorImageType,double>    ResampleFilterType;
    FloatVectorImageType* inputImage = GetParameterImage("in");
    
    TransformType::Pointer transform = TransformType::New();
    TransformType::OutputVectorType offset;
    offset[0] = GetParameterFloat("tx");
    offset[1] = GetParameterFloat("ty");
    std::cout << "Offset : " << offset << std::endl;
    transform->SetOffset(offset);

    ResampleFilterType::Pointer resampler = ResampleFilterType::New();
    resampler->SetInput(inputImage);
    resampler->SetTransform(transform);
    resampler->SetOutputParametersFromImage(inputImage);
    FloatVectorImageType::PixelType defaultValue;
    itk::PixelBuilder<FloatVectorImageType::PixelType>::Zero(defaultValue,
							     inputImage->GetNumberOfComponentsPerPixel());
    resampler->SetEdgePaddingValue(defaultValue);

    // Output Image 
    SetParameterOutputImage("out", resampler->GetOutput());
  }
}; //class


} //namespace wrapper
} //namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::RigidTransformResample)
