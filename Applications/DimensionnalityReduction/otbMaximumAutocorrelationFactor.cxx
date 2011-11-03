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

#include "otbMaximumAutocorrelationFactorImageFilter.h"

namespace otb
{
namespace Wrapper
{

class MaximumAutocorrelationFactor: public Application
{
public:
  /** Standard class typedefs. */
  typedef MaximumAutocorrelationFactor          Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(MaximumAutocorrelationFactor, otb::Wrapper::Application);

private:
  MaximumAutocorrelationFactor()
  {
    SetName("MaximumAutocorrelationFactor");
    SetDescription("Maximum autocorrelation factor");
  }

  virtual ~MaximumAutocorrelationFactor()
  {
  }

  void DoCreateParameters()
  {
    AddParameter(ParameterType_InputImage,  "in", "Input Image 1");
    AddParameter(ParameterType_OutputImage, "out", "Change Map");
  }

  void DoUpdateParameters()
  {
  }

  void DoExecute()
  {
    typedef otb::MaximumAutocorrelationFactorImageFilter<
        FloatVectorImageType,
        FloatVectorImageType> MAFFilterType;

    MAFFilterType::Pointer changeFilter = MAFFilterType::New();

    changeFilter->SetInput(GetParameterImage("in"));
    m_Ref = changeFilter;

    SetParameterOutputImage("out", changeFilter->GetOutput());
  }

  itk::LightObject::Pointer m_Ref;

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::MaximumAutocorrelationFactor)
