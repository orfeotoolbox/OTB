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

#include "otbMultivariateAlterationDetectorImageFilter.h"

namespace otb
{
namespace Wrapper
{

class MultivariateAlterationDetector: public Application
{
public:
  /** Standard class typedefs. */
  typedef MultivariateAlterationDetector          Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(MultivariateAlterationDetector, otb::Wrapper::Application);

private:
  MultivariateAlterationDetector()
  {
    SetName("MultivariateAlterationDetector");
    SetDescription("Multivariate Alteration Detector");

    // Documentation
    SetDocName("Multivariate alteration detector");
    SetDocLongDescription("This application detects change between two given images.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
    SetDocCLExample("otbApplicationLauncherCommandLine MultivariateAlterationDetector ${OTB-BIN}/bin "
      "--in1 ${OTB-Data}/Input/Spot5-Gloucester-before.tif --in2 ${OTB-Data}/Input/Spot5-Gloucester-after.tif "
      "--out detectedChangeImage.tif ");
    AddDocTag("Feature extraction");
  }

  virtual ~MultivariateAlterationDetector()
  {
  }

  void DoCreateParameters()
  {
    AddParameter(ParameterType_InputImage,  "in1", "Input Image 1");
    AddParameter(ParameterType_InputImage,  "in2", "Input Image 2");
    AddParameter(ParameterType_OutputImage, "out", "Change Map");
    SetParameterDescription("out","Image of detected changes.");
  }

  void DoUpdateParameters()
  {
  }

  void DoExecute()
  {
    typedef otb::MultivariateAlterationDetectorImageFilter<
        FloatVectorImageType,
        FloatVectorImageType> ChangeFilterType;

    ChangeFilterType::Pointer changeFilter = ChangeFilterType::New();

    changeFilter->SetInput1(GetParameterImage("in1"));
    changeFilter->SetInput2(GetParameterImage("in2"));

    m_Ref = changeFilter;

    SetParameterOutputImage("out", changeFilter->GetOutput());
  }

  itk::LightObject::Pointer m_Ref;

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::MultivariateAlterationDetector)
