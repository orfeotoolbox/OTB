/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

  typedef otb::StreamingStatisticsVectorImageFilter<FloatVectorImageType, float> StreamingStatisticsVectorImageFilterType;
  typedef otb::VirtualDimensionality<float>                                      VirtualDimensionalityType;
  typedef otb::EigenvalueLikelihoodMaximisation<float>                           EigenvalueLikelihoodMaximisationType;

private:
  void DoInit() override
  {
    SetName("EndmemberNumberEstimation");
    SetDescription("Estimate the number of endmembers in a hyperspectral image");

    // Documentation
    SetDocName("Endmember Number Estimation");
    SetDocLongDescription("Estimate the number of endmembers "
    "in a hyperspectral image. First, compute statistics on the image and then "
    "apply an endmember number estimation algorithm using these statistics. Two "
    "algorithms are available:\n\n"

    "1. Virtual Dimensionality (VD) [1][2]\n"
    "2. Eigenvalue Likelihood Maximization (ELM) [3][4]\n\n"

    "The application then returns the estimated number of endmembers.\n\n"

    "[1] C.-I. Chang and Q. Du, Estimation of number of spectrally distinct signal "
    "sources in hyperspectral imagery, IEEE Transactions on Geoscience and Remote "
    "Sensing, vol. 43, no. 3, mar 2004.\n\n"

    "[2] J. Wang and C.-I. Chang, Applications of independent component analysis "
    "in endmember extraction and abundance quantification for hyperspectral imagery"
    ", IEEE Transactions on Geoscience and Remote Sensing, vol. 44, no. 9, pp. "
    "2601-1616, sep 2006.\n\n"

    "[3] Unsupervised Endmember Extraction of Martian Hyperspectral Images, B.Luo, "
    "J. Chanussot, S. Dout\'e and X. Ceamanos, IEEE Whispers 2009, Grenoble France, 2009\n\n"

    "[4] Unsupervised classification of hyperspectral images by using "
    "linear unmixing algorithm Luo, B. and Chanussot, J., IEEE Int. Conf. On Image"
    "Processing(ICIP) 2009, Cairo, Egypte, 2009"
    );

    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("VertexComponentAnalysis, HyperspectralUnmixing");

    AddDocTag(Tags::Hyperspectral);

    AddParameter(ParameterType_InputImage,  "in", "Input Image Filename");
    SetParameterDescription("in","The hyperspectral data cube input");

    AddParameter(ParameterType_Int,"number","Number of endmembers");
    SetParameterDescription("number", "The output estimated number of endmembers");
    SetParameterRole("number", Role_Output);

    AddParameter(ParameterType_Choice, "algo", "Unmixing algorithm");
    SetParameterDescription("algo", "The algorithm to use for the estimation");
    AddChoice("algo.elm", "Eigenvalue Likelihood Maximization");
    SetParameterDescription("algo.elm", "");
    AddChoice("algo.vd", "Virtual Dimensionality");
    SetParameterDescription("algo.vd", "");

    AddParameter( ParameterType_Float , "algo.vd.far" , "False alarm rate");
    SetMinimumParameterFloatValue("algo.vd.far", 0);
    SetMaximumParameterFloatValue("algo.vd.far", 1);
    SetDefaultParameterFloat( "algo.vd.far" , 1.0E-3 );
    SetParameterDescription( "algo.vd.far" , 
      "False alarm rate for the virtual dimensionality algorithm");

    AddRAMParameter();
    
    // Doc example parameter settings
    SetDocExampleParameterValue("in", "cupriteSubHsi.tif");
    SetDocExampleParameterValue("algo", "vd");
    SetDocExampleParameterValue("algo.vd.far", "1.0E-3");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute() override
  {
    // Load input image
    auto inputImage = GetParameterImage("in");

    otbAppLogINFO("Computing statistics on input image...");
    auto statisticsFilter = StreamingStatisticsVectorImageFilterType::New();
    statisticsFilter->SetInput(inputImage);
    AddProcess(statisticsFilter->GetStreamer(), "Statistic estimation step");

    statisticsFilter->Update();

    auto correlationMatrix = statisticsFilter->GetCorrelation().GetVnlMatrix();
    auto covarianceMatrix = statisticsFilter->GetCovariance().GetVnlMatrix();
    auto numberOfPixels = inputImage->GetLargestPossibleRegion().GetNumberOfPixels();

    int numberOfEndmembers = 0;
    const std::string algorithm = GetParameterString("algo");
    if (algorithm=="elm")
      {
      otbAppLogINFO("Estimation algorithm: Eigenvalue Likelihood Maximization.");
      auto elm = EigenvalueLikelihoodMaximisationType::New();
      elm->SetCovariance(covarianceMatrix);
      elm->SetCorrelation(correlationMatrix);
      elm->SetNumberOfPixels(numberOfPixels);
      elm->Compute();
      numberOfEndmembers = elm->GetNumberOfEndmembers();
      }
    else if (algorithm=="vd")
      {
      otbAppLogINFO("Estimation algorithm: Virtual Dimensionality.");
      auto vd = VirtualDimensionalityType::New();
      vd->SetCovariance(covarianceMatrix);
      vd->SetCorrelation(correlationMatrix);
      vd->SetNumberOfPixels(numberOfPixels);
      vd->SetFAR(GetParameterFloat("algo.vd.far"));
      vd->Compute();
      numberOfEndmembers = vd->GetNumberOfEndmembers();
      }
    SetParameterInt("number", numberOfEndmembers);
  }

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::EndmemberNumberEstimation)
