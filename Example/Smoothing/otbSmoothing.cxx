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
#include "otbSmoothing.h"
#include "otbWrapperNumericalParameter.h"

namespace otb
{
namespace Wrapper
{

enum
{
  Smoothing_Mean,
  Smoothing_Gaussian,
  Smoothing_Anisotropic
};

Smoothing::Smoothing()
{
  this->SetName("Smoothing");
  this->SetDescription("Apply a smoothing filter to an image");
}

Smoothing::~Smoothing()
{
}

void Smoothing::DoCreateParameters()
{
  otb::Wrapper::InputImageParameter::Pointer  inImage  = otb::Wrapper::InputImageParameter::New();
  otb::Wrapper::OutputImageParameter::Pointer outImage = otb::Wrapper::OutputImageParameter::New();

  otb::Wrapper::ChoiceParameter::Pointer smoothingType  = otb::Wrapper::ChoiceParameter::New();
  smoothingType->SetName("Smoothing Type");
  smoothingType->SetKey("type");

  otb::Wrapper::RadiusParameter::Pointer meanSmoothingRadius  = otb::Wrapper::RadiusParameter::New();
  smoothingType->AddChoice("Mean", meanSmoothingRadius.GetPointer());

  otb::Wrapper::RadiusParameter::Pointer gaussianSmoothingRadius  = otb::Wrapper::RadiusParameter::New();
  smoothingType->AddChoice("Gaussian", gaussianSmoothingRadius.GetPointer());

  otb::Wrapper::FloatParameter::Pointer aniDifTimeStep  = otb::Wrapper::FloatParameter::New();
  aniDifTimeStep->SetName("Time Step");
  aniDifTimeStep->SetKey("TimeStep");
  otb::Wrapper::IntParameter::Pointer aniDifNbIter = otb::Wrapper::IntParameter::New();
  aniDifTimeStep->SetName("Nb Iterations");
  aniDifTimeStep->SetKey("NbIter");
  otb::Wrapper::ParameterGroup::Pointer aniDifGroup = otb::Wrapper::ParameterGroup::New();
  aniDifGroup->AddParameter(aniDifTimeStep.GetPointer());
  aniDifGroup->AddParameter(aniDifNbIter.GetPointer());
  smoothingType->AddChoice("Anisotropic Diffusion", aniDifGroup.GetPointer());

  ParameterGroup* params = GetParameterList();
  params->AddParameter(inImage.GetPointer());
  params->AddParameter(outImage.GetPointer());
  params->AddParameter(smoothingType.GetPointer());
}

void Smoothing::DoUpdateParameters()
{
}

void Smoothing::DoExecute()
{
  ParameterGroup* params = GetParameterList();

  otb::Wrapper::InputImageParameter* inImageParam = dynamic_cast<otb::Wrapper::InputImageParameter*>(params->GetParameter(0).GetPointer());
  typedef otb::Wrapper::InputImageParameter::VectorImageType::Pointer VectorImagePointerType;
  VectorImagePointerType inImage = boost::any_cast<VectorImagePointerType>(inImageParam->GetAnyValue());

//  otb::Wrapper::OutputImageParameter* outImageParam = dynamic_cast<otb::Wrapper::InputImageParameter*>(params->GetParameter(1).GetPointer());
//  otb::Wrapper::OutputImageParameter::VectorImageType outImage = boost::any_cast<float>(outImageParam->GetAnyValue());

  otb::Wrapper::ChoiceParameter* smoothingTypeParam = dynamic_cast<otb::Wrapper::ChoiceParameter*>(params->GetParameter(2).GetPointer());
  int smoothingType = smoothingTypeParam->GetValue();

  switch (smoothingType)
    {
    case Smoothing_Mean:
      {

      }
      break;
    case Smoothing_Gaussian:
      {

      }
      break;
    case Smoothing_Anisotropic:
      {

      }
      break;
    }
}

}

}
