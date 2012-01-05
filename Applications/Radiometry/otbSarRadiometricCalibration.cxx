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

#include "otbSarRadiometricCalibrationToImageFilter.h"

namespace otb
{
namespace Wrapper
{

class SarRadiometricCalibration : public Application
{
public:
  /** Standard class typedefs. */
  typedef SarRadiometricCalibration     Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(SarRadiometricCalibration, otb::Application);

  typedef otb::SarRadiometricCalibrationToImageFilter<ComplexFloatImageType,
                                                      ComplexFloatImageType>     CalibrationFilterType;

private:
  void DoInit()
  {
    SetName("SarRadiometricCalibration");
    SetDescription("Perform SAR calibration on input complex images");

    // Documentation
    SetDocName("SAR Radiometric calibration");
    SetDocLongDescription("This application performs SAR calibration on input complex images.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
    
    AddDocTag(Tags::Calibration);
    AddDocTag(Tags::SAR);

    AddParameter(ParameterType_ComplexInputImage,  "in", "Input Complex Image");
    AddParameter(ParameterType_ComplexOutputImage,  "out", "Output Image");
    AddParameter(ParameterType_RAM, "ram", "Available RAM");
    SetDefaultParameterInt("ram", 256);
    MandatoryOff("ram");
    AddParameter(ParameterType_Empty, "noise", "Disable Noise");
    MandatoryOff("noise");

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "RSAT_imagery_HH.tif");
    SetDocExampleParameterValue("out", "SarRadiometricCalibration.tif" );
  }

  void DoUpdateParameters()
  { }

  void DoExecute()
  {
    // Get the input complex image
    ComplexFloatImageType*  floatComplexImage = GetParameterComplexFloatImage("in");

    // Set the filer input
    m_CalibrationFilter = CalibrationFilterType::New();
    m_CalibrationFilter->SetInput(floatComplexImage);

    if (IsParameterEnabled("noise"))
      {
      m_CalibrationFilter->SetEnableNoise(false);
      }

    // Set the output image
    SetParameterComplexOutputImage("out", m_CalibrationFilter->GetOutput());
  }

  CalibrationFilterType::Pointer   m_CalibrationFilter;
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::SarRadiometricCalibration)
