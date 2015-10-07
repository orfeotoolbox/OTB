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
                                                      FloatImageType>     CalibrationFilterType;

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
    SetParameterDescription("in", "Input complex image");

    AddParameter(ParameterType_OutputImage,  "out", "Output Image");
    SetParameterDescription("out", "Output calibrated complex image");

    AddRAMParameter();

    AddParameter(ParameterType_Empty, "noise", "Disable Noise");
    SetParameterDescription("noise", "Flag to disable noise");
    MandatoryOff("noise");

    AddParameter(ParameterType_Choice, "lut", "Lookup table sigma /gamma/ beta/ DN");
    AddChoice("lut.sigma", "Use sigma nought lookup");
    SetParameterDescription("lut.sigma","Use Sigma nought lookup value from product metadata");
    AddChoice("lut.gamma", "Use gamma nought lookup");
    SetParameterDescription("lut.gamma","Use Gamma nought lookup value from product metadata");
    AddChoice("lut.beta", "Use beta nought lookup");
    SetParameterDescription("lut.beta","Use Beta nought lookup value from product metadata");
    AddChoice("lut.dn", "Use DN value lookup");
    SetParameterDescription("lut.dn","Use DN value lookup value from product metadata");
    SetDefaultParameterInt("lut", 0);

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "RSAT_imagery_HH.tif");
    SetDocExampleParameterValue("out", "SarRadiometricCalibration.tif" );
  }

  void DoUpdateParameters()
  {

  }

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

    short lut = 0;

    lut = GetParameterInt("lut");

    m_CalibrationFilter->SetLookupSelected(lut);

    // Set the output image
    SetParameterOutputImage("out", m_CalibrationFilter->GetOutput());

  }

  CalibrationFilterType::Pointer   m_CalibrationFilter;

};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::SarRadiometricCalibration)
