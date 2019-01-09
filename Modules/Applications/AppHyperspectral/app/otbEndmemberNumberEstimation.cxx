/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbStreamingStatisticsVectorImageFilter.h"
#include "otbEigenvalueLikelihoodMaximisation.h"
#include "otbVirtualDimensionality.h"

namespace otb
{
namespace Wrapper
{

class EndmemberNumberEstimation : public Application
{
public:
  /** Standard class typedefs. */
  typedef EndmemberNumberEstimation         Self;
  typedef Application                       Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(EndmemberNumberEstimation, otb::Application);

  typedef otb::StreamingStatisticsVectorImageFilter<FloatVectorImageType, float> 
    StreamingStatisticsVectorImageFilterType;
  typedef otb::VirtualDimensionality<float> VirtualDimensionalityType;
  typedef otb::EigenvalueLikelihoodMaximisation<float> EigenvalueLikelihoodMaximisationType;

private:
  void DoInit() override
  {
    SetName("EndmemberNumberEstimation");
    SetDescription(" ");

    // Documentation
    SetDocName("Endmember Number Estimation");
    SetDocLongDescription(" ");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("VertexComponentAnalysis, HyperspectralUnmixing");

    AddDocTag(Tags::Hyperspectral);

    AddParameter(ParameterType_InputImage,  "in",   "Input Image Filename");
    SetParameterDescription("in","The hyperspectral data cube input");

    AddParameter(ParameterType_Choice, "algo", "Unmixing algorithm");
    SetParameterDescription("algo", "The algorithm to use for the estimation");
    AddChoice("algo.elm", "elm");
    SetParameterDescription("algo.elm", "Eigenvalue Likelihood Maximization");
    AddChoice("algo.vd", "vd");
    SetParameterDescription("algo.vd", "virtual dimensionality");

    AddParameter(ParameterType_Int,"number","Number of endmembers");
    SetParameterDescription("number", "Estimated number of endmembers");
    SetParameterRole("number", Role_Output);

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "cupriteSubHsi.tif");
    SetDocExampleParameterValue("algo", "elm");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute() override
  {
    auto statisticsFilter = StreamingStatisticsVectorImageFilterType::New();
    statisticsFilter->SetInput(GetParameterImage("in"));

    statisticsFilter->Update();
    int numberOfEndmembers = 0;
    const std::string algorithm = GetParameterString("algo");
    if (algorithm=="elm")
      {
      auto elm = EigenvalueLikelihoodMaximisationType::New();
      elm->SetCovariance(statisticsFilter->GetCovariance().GetVnlMatrix());
      elm->SetCorrelation(statisticsFilter->GetCorrelation().GetVnlMatrix());
      elm->SetNumberOfPixels(GetParameterImage("in")->GetLargestPossibleRegion().GetNumberOfPixels());
      elm->Compute();
      numberOfEndmembers = elm->GetNumberOfEndmembers();
      }
    else if (algorithm=="vd")
      {
      auto vd = VirtualDimensionalityType::New();
      vd->SetCovariance(statisticsFilter->GetCovariance().GetVnlMatrix());
      vd->SetCorrelation(statisticsFilter->GetCorrelation().GetVnlMatrix());
      vd->SetNumberOfPixels(GetParameterImage("in")->GetLargestPossibleRegion().GetNumberOfPixels());
      vd->Compute();
      numberOfEndmembers = vd->GetNumberOfEndmembers();
      }
    SetParameterInt("number", numberOfEndmembers);
  }

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::EndmemberNumberEstimation)
