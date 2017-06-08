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

#include "otbFrostImageFilter.h"
#include "otbLeeImageFilter.h"
#include "otbGammaMAPImageFilter.h"
#include "otbKuanImageFilter.h"
#include "otbMultiToMonoChannelExtractROI.h"

namespace otb
{
namespace Wrapper
{

class Despeckle : public Application
{
public:
  /** Standard class typedefs. */
  typedef Despeckle                           Self;
  typedef Application                         Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;

  typedef itk::ImageToImageFilter<FloatImageType, FloatImageType> SpeckleFilterType;

  typedef LeeImageFilter<FloatImageType, FloatImageType>        LeeFilterType;
  typedef FrostImageFilter<FloatImageType, FloatImageType>      FrostFilterType;
  typedef GammaMAPImageFilter<FloatImageType, FloatImageType>   GammaMAPFilterType;
  typedef KuanImageFilter<FloatImageType, FloatImageType>       KuanFilterType;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(Despeckle, otb::Application);

private:
  void DoInit() override
  {
    SetName("Despeckle");
    SetDescription("Perform speckle noise reduction on SAR image.");

    // Documentation
    SetDocName("Despeckle");
    SetDocLongDescription("This application reduce speckle noise. Four methods are available: Lee, Frost, GammaMAP and Kuan.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Filter);
	AddDocTag(Tags::SAR);

    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    SetParameterDescription("in", "Input image.");
    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription("out", "Output image.");

    AddRAMParameter();

    AddParameter(ParameterType_Choice,"filter","speckle filtering method");
    AddChoice("filter.lee","Lee");
    SetParameterDescription("filter.lee","Lee filter");
    AddChoice("filter.frost","Frost");
    SetParameterDescription("filter.frost","Frost filter");
    AddChoice("filter.gammamap","GammaMap");
    SetParameterDescription("filter.gammamap","GammaMap filter");
    AddChoice("filter.kuan","Kuan");
    SetParameterDescription("filter.kuan","Kuan filter");

    AddParameter(ParameterType_Int,"filter.lee.rad","Radius");
    SetParameterDescription("filter.lee.rad","Radius for lee filter");

    AddParameter(ParameterType_Float,"filter.lee.nblooks","nb looks");
    SetParameterDescription("filter.lee.nblooks","Nb looks for lee filter");

    AddParameter(ParameterType_Int,"filter.frost.rad","Radius");
    SetParameterDescription("filter.frost.rad","Radius for frost filter");

    AddParameter(ParameterType_Float,"filter.frost.deramp","deramp");
    SetParameterDescription("filter.frost.deramp","Decrease factor declaration");
    
    AddParameter(ParameterType_Int,"filter.gammamap.rad","Radius");
    SetParameterDescription("filter.gammamap.rad","Radius for GammaMAP filter");

    AddParameter(ParameterType_Float,"filter.gammamap.nblooks","nb looks");
    SetParameterDescription("filter.gammamap.nblooks","Nb looks for GammaMAP filter");
    
    AddParameter(ParameterType_Int,"filter.kuan.rad","Radius");
    SetParameterDescription("filter.kuan.rad","Radius for Kuan filter");

    AddParameter(ParameterType_Float,"filter.kuan.nblooks","nb looks");
    SetParameterDescription("filter.kuan.nblooks","Nb looks for Kuan filter");

    // Default values
    SetDefaultParameterInt("filter.lee.rad", 1);
    SetDefaultParameterFloat("filter.lee.nblooks", 1.);
    SetDefaultParameterInt("filter.frost.rad",  1);
    SetDefaultParameterFloat("filter.frost.deramp",  0.1);
    SetDefaultParameterInt("filter.gammamap.rad", 1);
    SetDefaultParameterFloat("filter.gammamap.nblooks", 1.);


    // Doc example parameter settings
    SetDocExampleParameterValue("in", "sar.tif");
    SetDocExampleParameterValue("filter","lee");
    SetDocExampleParameterValue("filter.lee.rad", "5");
    SetDocExampleParameterValue("out", "despeckle.tif");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute() override
  {
    FloatVectorImageType* inVImage = GetParameterImage("in");

    // Transform to otb::Image
    typedef otb::Image<FloatVectorImageType::InternalPixelType> InternalFloatImageType;
    typedef otb::MultiToMonoChannelExtractROI<float,float> ExtractFilterType;

    ExtractFilterType::Pointer channelSelect = ExtractFilterType::New();
    m_Ref.push_back(channelSelect.GetPointer());
    channelSelect->SetChannel(1);
    channelSelect->SetInput(inVImage);
    channelSelect->UpdateOutputInformation();
    InternalFloatImageType::Pointer inImage = channelSelect->GetOutput();
    inImage->UpdateOutputInformation();

    switch (GetParameterInt("filter"))
      {
      case 0:
      {
      LeeFilterType::Pointer  filter = LeeFilterType::New();
      m_Ref.push_back(filter.GetPointer());

      filter->SetInput(inImage);

      LeeFilterType::SizeType lradius;
      lradius.Fill(GetParameterInt("filter.lee.rad"));

      filter->SetRadius(lradius);
      filter->SetNbLooks(GetParameterFloat("filter.lee.nblooks"));

      otbAppLogINFO( << "Lee filter" );
      m_SpeckleFilter = filter;
      break;
      }
      case 1:
      {
      FrostFilterType::Pointer  filter = FrostFilterType::New();
      m_Ref.push_back(filter.GetPointer());

      filter->SetInput(inImage);

      FrostFilterType::SizeType lradius;
      lradius.Fill(GetParameterInt("filter.frost.rad"));

      filter->SetRadius(lradius);
      filter->SetDeramp(GetParameterFloat("filter.frost.deramp"));

      otbAppLogINFO( << "Frost filter" );
      m_SpeckleFilter = filter;
      break;
      }
      case 2:
      {
      GammaMAPFilterType::Pointer  filter = GammaMAPFilterType::New();
      m_Ref.push_back(filter.GetPointer());

      filter->SetInput(inImage);

      GammaMAPFilterType::SizeType lradius;
      lradius.Fill(GetParameterInt("filter.gammamap.rad"));

      filter->SetRadius(lradius);
      filter->SetNbLooks(GetParameterFloat("filter.gammamap.nblooks"));

      otbAppLogINFO( << "GammaMAP filter" );
      m_SpeckleFilter = filter;
      break;
      }
      case 3:
      {
      KuanFilterType::Pointer  filter = KuanFilterType::New();
      m_Ref.push_back(filter.GetPointer());

      filter->SetInput(inImage);

      KuanFilterType::SizeType lradius;
      lradius.Fill(GetParameterInt("filter.kuan.rad"));

      filter->SetRadius(lradius);
      filter->SetNbLooks(GetParameterFloat("filter.kuan.nblooks"));

      otbAppLogINFO( << "Kuan filter" );
      m_SpeckleFilter = filter;
      break;
      }
      default:
      {
      otbAppLogFATAL(<<"non defined speckle reduction filter "<<GetParameterInt("filter")<<std::endl);
      break;
      }
      return;
      }

    SetParameterOutputImage("out", m_SpeckleFilter->GetOutput());
  }
  std::vector<itk::ProcessObject::Pointer> m_Ref;
  SpeckleFilterType::Pointer m_SpeckleFilter;
};

} //end namespace Wrapper
} //end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::Despeckle)
