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
#include "otbWrapperApplicationFactory.h"
#include "otbWrapperCompositeApplication.h"


namespace otb
{
namespace Wrapper
{
class BundleToPerfectSensor : public CompositeApplication
{
public:
  /** Standard class typedefs. */
  typedef BundleToPerfectSensor         Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(BundleToPerfectSensor, otb::Wrapper::CompositeApplication);

private:

  void DoInit() ITK_OVERRIDE
  {
    SetName("BundleToPerfectSensor");
    SetDescription("Perform P+XS pansharpening");

    // Documentation
    SetDocName("Bundle to perfect sensor");
    SetDocLongDescription("This application performs P+XS pansharpening. The default mode use Pan and XS sensor models to estimate the transformation to superimpose XS over Pan before the fusion (\"default mode\"). The application provides also a PHR mode for Pleiades images which does not use sensor models as Pan and XS products are already coregistered but only estimate an affine transformation to superimpose XS over the Pan.Note that this option is automatically activated in case Pleiades images are detected as input.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Geometry);
    AddDocTag(Tags::Pansharpening);

    AddApplication("Superimpose", "superimpose", "Reproject XS onto Pan");
    AddApplication("Pansharpening", "pansharp", "Fusion of XS and Pan");

    ShareParameter("inp","superimpose.inr","Input PAN Image","Input panchromatic image.");
    ShareParameter("inxs","superimpose.inm","Input XS Image","Input XS image.");
    ShareParameter("out","pansharp.out");
    ShareParameter("elev","superimpose.elev");
    ShareParameter("mode","superimpose.mode");
    ShareParameter("lms","superimpose.lms",
      "Spacing of the deformation field",
      "Spacing of the deformation field. Default is 10 times the PAN image spacing.");
    ShareParameter("ram","superimpose.ram");

    Connect("pansharp.inp","superimpose.inr");
    Connect("pansharp.ram","superimpose.ram");

    GetInternalApplication("superimpose")->SetParameterString("interpolator","bco");
    GetInternalApplication("pansharp")->SetParameterString("method","rcs");
    
    // Doc example parameter settings
    SetDocExampleParameterValue("inp", "QB_Toulouse_Ortho_PAN.tif");
    SetDocExampleParameterValue("inxs", "QB_Toulouse_Ortho_XS.tif");
    SetDocExampleParameterValue("out", "BundleToPerfectSensor.png uchar");

  }

  void DoUpdateParameters() ITK_OVERRIDE
  {
    UpdateInternalParameters("superimpose");
  }

  void DoExecute() ITK_OVERRIDE
  {
    ExecuteInternal("superimpose");

    GetInternalApplication("pansharp")->SetParameterInputImage("inxs",
      GetInternalApplication("superimpose")->GetParameterOutputImage("out"));

    ExecuteInternal("pansharp");
  }

};


}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::BundleToPerfectSensor)
