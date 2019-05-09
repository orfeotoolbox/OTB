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
    SetDocLongDescription("SAR images are affected by speckle noise that inherently exists in and which"
     " degrades the image quality. It is caused by the coherent nature of"
     " back-scattered waves from multiple distributed targets. It is locally strong"
     " and it increases the mean Grey level of a local area. \n\n"
     "Reducing the speckle noise enhances radiometric resolution but tend to decrease the spatial resolution."
     "Several different methods are used to eliminate speckle noise, based upon"
     " different mathematical models of the phenomenon. The application includes four"
     " methods: Lee [1], Frost [2], GammaMAP [3] and Kuan [4]. \n\n"
     "We sum up below the basic principle of this four methods:\n\n"
     "* Lee: Estimate the signal by mean square error minimization (MMSE) on a sliding window.\n"
     "* Frost: Also derived from the MMSE criteria with a weighted sum of the values within the window. The weighting factors decrease with distance from the pixel of interest.\n"
     "* GammaMAP: Derived under the assumption of the image follows a Gamma distribution.\n"
     "* Kuan: Also derived from the MMSE criteria under the assumption of non stationary mean and variance. It is quite similar to Lee filter in form."
      );

    SetDocLimitations("The application does not handle complex image as input.");

    SetDocAuthors("OTB-Team");

    SetDocSeeAlso("[1] J. Lee. Digital image enhancement and noise filtering by"
                  "use of local statistics. IEEE Transactions on Pattern Analysis and Machine"
                  "Intelligence, 2:165–168, 1980.\n"
                  "[2] V. S. Frost, et al., A Model for Radar Images and Its"
                  "Application to Adaptive Digital Filtering of Multiplicative"
                  "Noise, IEEE Trans. Pattern Anal., Machine Intell., vol. 4,"
                  "no. 2, pp. 157-166, Mar. 1982.\n"
                  "[3] A. Lopes, E. Nezry, R. Touzi and H. Laur, Maximum A"
                  "Posteriori Speckle Filtering And First Order Texture Models"
                  "In Sar Images, 10th" "Annual International Symposium on"
                  "Geoscience and Remote Sensing, 1990,pp. 2409-2412.  doi:"
                  "10.1109/IGARSS.1990.689026\n"
                  "[4] Kuan, D.  T., Sawchuk, A.  A., Strand, T.  C, and Chavel,"
                  "P., 1987.  Adaptive restoration of image with speckle.  IEEE"
                  "Trans on Acoustic Speech and Signal Processing, 35,"
                  "pp. 373-383.");

    AddDocTag(Tags::Filter);
	  AddDocTag(Tags::SAR);

    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    SetParameterDescription("in", "Input image.");
    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription("out", "Output image.");

    AddParameter(ParameterType_Choice,"filter","Speckle filtering method");
    
    AddChoice("filter.lee","Lee");
    SetParameterDescription("filter.lee","Lee filter");

    AddChoice("filter.frost","Frost");
    SetParameterDescription("filter.frost","Frost filter");

    AddChoice("filter.gammamap","GammaMap");
    SetParameterDescription("filter.gammamap","GammaMap filter");

    AddChoice("filter.kuan","Kuan");
    SetParameterDescription("filter.kuan","Kuan filter");

    AddParameter(ParameterType_Int,"filter.lee.rad","Radius");
    SetParameterDescription("filter.lee.rad","Radius in pixel");

    AddParameter(ParameterType_Float,"filter.lee.nblooks","Number of looks");
    SetParameterDescription("filter.lee.nblooks","Number of looks in the input image.");

    AddParameter(ParameterType_Int,"filter.frost.rad","Radius");
    SetParameterDescription("filter.frost.rad","Radius in pixel.");

    AddParameter(ParameterType_Float,"filter.frost.deramp","Deramp factor");
    SetParameterDescription("filter.frost.deramp","factor use to control the\n"
       "exponential function used to weight effect of the distance between the\n"
       "central pixel and its neighborhood. Increasing the deramp parameter will\n"
       "lead to take more into account pixels farther from the center and\n"
       "therefore increase the smoothing effects.");
    
    AddParameter(ParameterType_Int,"filter.gammamap.rad","Radius");
    SetParameterDescription("filter.gammamap.rad","Radius in pixel.");

    AddParameter(ParameterType_Float,"filter.gammamap.nblooks","Number of looks");
    SetParameterDescription("filter.gammamap.nblooks","Number of looks in the input image.");
    
    AddParameter(ParameterType_Int,"filter.kuan.rad","Radius");
    SetParameterDescription("filter.kuan.rad","Radius in pixel.");

    AddParameter(ParameterType_Float,"filter.kuan.nblooks","Number of looks");
    SetParameterDescription("filter.kuan.nblooks","Number of looks in the input image.");

    // Default values
    SetDefaultParameterInt("filter.lee.rad", 1);
    SetDefaultParameterFloat("filter.lee.nblooks", 1.);
    SetDefaultParameterInt("filter.frost.rad",  1);
    SetDefaultParameterFloat("filter.frost.deramp",  0.1);
    SetDefaultParameterInt("filter.gammamap.rad", 1);
    SetDefaultParameterFloat("filter.gammamap.nblooks", 1.);

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "sar.tif");
    SetDocExampleParameterValue("filter","lee");
    SetDocExampleParameterValue("filter.lee.rad", "5");
    SetDocExampleParameterValue("out", "despeckle.tif");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here: all parameters are independent
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
