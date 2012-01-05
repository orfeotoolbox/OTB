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

  typedef itk::TranslationTransform<double, FloatVectorImageType::ImageDimension> TransformType;
  typedef otb::StreamingResampleImageFilter<FloatVectorImageType, FloatVectorImageType, double>    ResampleFilterType;

  itkTypeMacro(RigidTransformResample, otb::Application);

private:

  void DoInit()
  {
    SetName("RigidTransformResample");
    SetDescription("Resample an image with a rigid transform");
    // Documentation
    SetDocName("Image resampling with a rigid transform");
    SetDocLongDescription("This application performs an translation on the input image.\n Parameters of the translation can be set with tx and ty options.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("Translation");
  
    AddDocTag("Conversion");
    AddDocTag(Tags::Geometry);

    AddParameter(ParameterType_InputImage,   "in",   "Input image");
    AddParameter(ParameterType_OutputImage,  "out",  "Output image");
    AddParameter(ParameterType_Float,        "tx",   "The X translation (in physical units)");
    AddParameter(ParameterType_Float,        "ty",   "The Y translation (in physical units)");

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "qb_toulouse_sub.tif");
    SetDocExampleParameterValue("out", "rigitTransformImage.tif");
    SetDocExampleParameterValue("tx", "5");
    SetDocExampleParameterValue("ty", "5");
  }
  
  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {
    
    FloatVectorImageType* inputImage = GetParameterImage("in");
    
    m_Transform = TransformType::New();
    TransformType::OutputVectorType offset;
    offset[0] = GetParameterFloat("tx");
    offset[1] = GetParameterFloat("ty");
    otbAppLogINFO( << "Offset : " << offset );
    m_Transform->SetOffset(offset);

    m_Resampler = ResampleFilterType::New();
    m_Resampler->SetInput(inputImage);
    m_Resampler->SetTransform(m_Transform);
    m_Resampler->SetOutputParametersFromImage(inputImage);
    FloatVectorImageType::PixelType defaultValue;
    itk::PixelBuilder<FloatVectorImageType::PixelType>::Zero(defaultValue,
                                                             inputImage->GetNumberOfComponentsPerPixel());
    m_Resampler->SetEdgePaddingValue(defaultValue);
    m_Resampler->UpdateOutputInformation();
    
    // Output Image
    SetParameterOutputImage("out", m_Resampler->GetOutput());
  }

  ResampleFilterType::Pointer m_Resampler;
  TransformType::Pointer m_Transform;
}; //class


} //namespace wrapper
} //namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::RigidTransformResample)
