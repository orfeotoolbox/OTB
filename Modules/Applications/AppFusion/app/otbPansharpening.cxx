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

#include "otbWrapperApplicationFactory.h"

#include "itkUnaryFunctorImageFilter.h"
#include "itkVectorIndexSelectionCastImageFilter.h"
#include "otbGenericRSResampleImageFilter.h"
#include "otbBCOInterpolateImageFunction.h"

#include "otbSimpleRcsPanSharpeningFusionImageFilter.h"
#include "otbBayesianFusionFilter.h"

#include "otbGenericRSResampleImageFilter.h"
#include "otbBCOInterpolateImageFunction.h"

#include "otbLmvmPanSharpeningFusionImageFilter.h"

#include "itkFixedArray.h"

// Elevation handler
#include "otbWrapperElevationParametersHandler.h"

namespace otb
{
namespace Wrapper
{


class Pansharpening : public Application
{
public:
  /** Standard class typedefs. */
  typedef Pansharpening                 Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef otb::SimpleRcsPanSharpeningFusionImageFilter<FloatImageType, FloatVectorImageType, FloatVectorImageType> SimpleRCSFilterType;

  typedef otb::LmvmPanSharpeningFusionImageFilter
    <FloatImageType, FloatVectorImageType, FloatVectorImageType, double> LmvmFilterType;

  typedef otb::BayesianFusionFilter<FloatVectorImageType, FloatVectorImageType, FloatImageType, FloatVectorImageType> BayesianFilterType;

  typedef otb::BCOInterpolateImageFunction<FloatVectorImageType>   InterpolatorType;
  typedef otb::GenericRSResampleImageFilter<FloatVectorImageType,
                                            FloatVectorImageType>  ResamplerType;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(Pansharpening, otb::Application);

private:

  void DoInit() override
  {
    SetName("Pansharpening");
    SetDescription("Perform P+XS pansharpening");

    // Documentation
    SetDocLongDescription("This application performs P+XS pansharpening. Pansharpening is a process of merging high-resolution panchromatic and lower resolution multispectral imagery to create a single high-resolution color image. Algorithms available in the applications are: RCS, bayesian fusion and Local Mean and Variance Matching(LMVM).");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Geometry);
    AddDocTag(Tags::Pansharpening);

    AddParameter(ParameterType_InputImage,   "inp",   "Input PAN Image");
    SetParameterDescription("inp"," Input panchromatic image.");
    AddParameter(ParameterType_InputImage,   "inxs",  "Input XS Image");
    SetParameterDescription("inxs"," Input XS image.");

    AddParameter(ParameterType_OutputImage,  "out",   "Output image");
    SetParameterDescription("out", "Output image.");

    AddParameter(ParameterType_Choice, "method", "Algorithm");
    SetParameterDescription("method", "Selection of the pan-sharpening method.");

    AddChoice("method.rcs", "RCS");
    SetParameterDescription("method.rcs", "Simple RCS Pan sharpening operation.");

    AddChoice("method.lmvm", "LMVM");
    SetParameterDescription("method.lmvm", "Local Mean and Variance Matching (LMVM) Pan sharpening.");

    AddParameter(ParameterType_Int, "method.lmvm.radiusx", "X radius" );
    SetParameterDescription("method.lmvm.radiusx","Set the x radius of the sliding window." );
    SetMinimumParameterIntValue("method.lmvm.radiusx", 1);
    SetDefaultParameterInt("method.lmvm.radiusx", 3);

    AddParameter(ParameterType_Int, "method.lmvm.radiusy", "Y radius");
    SetParameterDescription("method.lmvm.radiusy", "Set the y radius of the sliding window.");
    SetMinimumParameterIntValue("method.lmvm.radiusy", 1);
    SetDefaultParameterInt("method.lmvm.radiusy", 3);

    AddChoice("method.bayes", "Bayesian");
    SetParameterDescription("method.bayes", "Bayesian fusion.");

    AddParameter(ParameterType_Float, "method.bayes.lambda", "Weight");
    SetParameterDescription("method.bayes.lambda", "Set the weighting value.");
    SetMinimumParameterFloatValue("method.bayes.lambda", 0);
    SetDefaultParameterFloat("method.bayes.lambda", 0.9999);

    AddParameter(ParameterType_Float, "method.bayes.s", "S coefficient");
    SetParameterDescription("method.bayes.s", "Set the S coefficient.");
    SetMinimumParameterFloatValue("method.bayes.s", 1);
    SetDefaultParameterFloat("method.bayes.s", 1);

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("inp", "QB_Toulouse_Ortho_PAN.tif");
    SetDocExampleParameterValue("inxs", "QB_Toulouse_Ortho_XS.tif");
    SetDocExampleParameterValue("out", "Pansharpening.tif uint16");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute() override
  {
    FloatVectorImageType* panchroV = GetParameterImage("inp");
    if ( panchroV->GetNumberOfComponentsPerPixel() != 1 )
      {
      itkExceptionMacro(<< "The panchromatic image must be a single channel image")
      }

    // Transform the PAN image to otb::Image
    typedef otb::Image<FloatVectorImageType::InternalPixelType> InternalFloatImageType;
    typedef itk::VectorIndexSelectionCastImageFilter<FloatVectorImageType, InternalFloatImageType> VectorIndexSelectionCastImageFilterType;

    VectorIndexSelectionCastImageFilterType::Pointer channelSelect = VectorIndexSelectionCastImageFilterType::New();
    m_Ref.push_back(channelSelect.GetPointer());
    channelSelect->SetIndex(0);
    channelSelect->SetInput(panchroV);
    channelSelect->UpdateOutputInformation();

    InternalFloatImageType::Pointer panchro = channelSelect->GetOutput();
    FloatVectorImageType* xs = GetParameterImage("inxs");

    switch (GetParameterInt("method"))
      {
      case 0:
      {
      SimpleRCSFilterType::Pointer  filter = SimpleRCSFilterType::New();
      m_Ref.push_back(filter.GetPointer());

      filter->SetPanInput(panchro);
      filter->SetXsInput(xs);

      filter->UpdateOutputInformation();
      otbAppLogINFO( << "Simple RCS algorithm" );
      m_Ref.push_back(filter.GetPointer());
      SetParameterOutputImage("out", filter->GetOutput());

      break;
      }
      case 1:
      {
      LmvmFilterType::Pointer filter = LmvmFilterType::New();

      filter->SetXsInput(xs);
      filter->SetPanInput(panchro);

      double radiusx = static_cast<unsigned int> (GetParameterInt("method.lmvm.radiusx"));
      double radiusy = static_cast<unsigned int> (GetParameterInt("method.lmvm.radiusy"));

      InternalFloatImageType::SizeType radius;
      radius[0] = radiusx;
      radius[1] = radiusy;

      filter->SetRadius(radius);

      itk::Array<double> filterCoeffs;
      filterCoeffs.SetSize((2 * radius[0] + 1) * (2 * radius[1] + 1));
      filterCoeffs.Fill(1);
      filter->SetFilter(filterCoeffs);

      filter->UpdateOutputInformation();

      otbAppLogINFO( << "Lmvm algorithm" );
      m_Ref.push_back(filter.GetPointer());
      SetParameterOutputImage("out", filter->GetOutput());

      break;
      }
      case 2:
      {
      BayesianFilterType::Pointer filter = BayesianFilterType::New();

      double lambda = static_cast<double> (GetParameterFloat("method.bayes.lambda"));
      double s = static_cast<double> (GetParameterFloat("method.bayes.s"));

      filter->SetS(s);
      filter->SetLambda(lambda);

      filter->SetMultiSpect(xs);

      filter->SetMultiSpectInterp(xs);
      filter->SetPanchro(panchro);

      filter->UpdateOutputInformation();
      otbAppLogINFO( << "Bayesian fusion algorithm" );

      m_Ref.push_back(filter.GetPointer());
      SetParameterOutputImage("out", filter->GetOutput());

      break;
      }

      default:
      {
      otbAppLogFATAL(<<"non defined method "<<GetParameterInt("method")<<std::endl);
      break;
      }
      return;
      }
  }

  std::vector<itk::ProcessObject::Pointer> m_Ref;
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::Pansharpening)
