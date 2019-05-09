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

#include "otbMultiChannelExtractROI.h"
#include "otbStreamingShrinkImageFilter.h"

namespace otb
{
namespace Wrapper
{

class Quicklook : public Application
{
public:
  /** Standard class typedefs. */
  typedef Quicklook                     Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(Quicklook, otb::Application);

  /** Typedefs */
  typedef float InputPixelType;
  typedef float OutputPixelType;
  typedef otb::MultiChannelExtractROI< InputPixelType, OutputPixelType >   ExtractROIFilterType;
  typedef ExtractROIFilterType::InputImageType InputImageType;
  typedef ExtractROIFilterType::OutputImageType OutputImageType;
  typedef otb::StreamingShrinkImageFilter
        <ExtractROIFilterType::OutputImageType, ExtractROIFilterType::OutputImageType> ShrinkImageFilterType;

private:
  void DoInit() override
  {
    SetName("Quicklook");
    SetDescription("Generates a subsampled version of an image extract");
    SetDocLongDescription("Generates a subsampled version of an extract of an image defined by ROIStart and ROISize.\n"
                          "This extract is subsampled using the ratio OR the output image Size.");
    SetDocLimitations("This application does not provide yet the optimal way to decode coarser level of resolution from JPEG2000 images (like in Monteverdi).\n"
                      "Trying to subsampled huge JPEG200 image with the application will lead to poor performances for now.");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Manip);

    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    SetParameterDescription( "in", "The image to read" );

    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription( "out" , "The subsampled image" );

    AddParameter(ParameterType_ListView, "cl", "Channel List");
    SetParameterDescription( "cl" , "Selected channels" );
    MandatoryOff("cl");

    AddParameter(ParameterType_Int, "rox",  "ROI Origin X");
    SetParameterDescription( "rox" , "first point of ROI in x-direction" );
    MandatoryOff("rox");

    AddParameter(ParameterType_Int, "roy",  "ROI Origin Y");
    SetParameterDescription( "roy" , "first point of ROI in y-direction" );
    MandatoryOff("roy");

    AddParameter(ParameterType_Int, "rsx",  "ROI Size X");
    SetParameterDescription( "rsx" , "size of ROI in x-direction" );
    MandatoryOff("rsx");

    AddParameter(ParameterType_Int, "rsy",  "ROI Size Y");
    SetParameterDescription( "rsy" , "size of ROI in y-direction" );
    MandatoryOff("rsy");

    AddParameter(ParameterType_Int, "sr",  "Sampling ratio");
    SetParameterDescription( "sr" , "Sampling Ratio, default is 2" );
    SetDefaultParameterInt("sr", 2);
    SetMinimumParameterIntValue("sr", 1);
    MandatoryOff("sr");

    AddParameter(ParameterType_Int, "sx",  "Size X");
    SetParameterDescription( "sx" , "quicklook size in x-direction (used if no sampling ratio is given)" );
    MandatoryOff("sx");
    DisableParameter("sx");

    AddParameter(ParameterType_Int, "sy",  "Size Y");
    SetParameterDescription( "sy" , "quicklook size in y-direction (used if no sampling ratio is given)" );
    MandatoryOff("sy");
    DisableParameter("sy");

    SetDefaultParameterInt("rox", 0);
    SetDefaultParameterInt("roy", 0);
    SetDefaultParameterInt("rsx", 0);
    SetDefaultParameterInt("rsy", 0);

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "qb_RoadExtract.tif");
    SetDocExampleParameterValue("out", "quicklookImage.tif");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Update the sizes only if the user does not defined a size
    if ( HasValue("in") )
      {
      InputImageType::Pointer inImage = GetParameterImage("in");

      InputImageType::RegionType  largestRegion = inImage->GetLargestPossibleRegion();

      // Update the values of the channels to be selected
      unsigned int nbComponents = inImage->GetNumberOfComponentsPerPixel();
      if (nbComponents != GetChoiceKeys("cl").size())
        {
        ClearChoices("cl");
        for (unsigned int idx = 0; idx < nbComponents; ++idx)
          {
          std::ostringstream key, item;
          key<<"cl.channel"<<idx+1;
          item<<"Channel"<<idx+1;
          AddChoice(key.str(), item.str());
          }
        }

      if (!HasUserValue("rsx")  && !HasUserValue("rsy") )
        {
        SetParameterInt("rsx",largestRegion.GetSize()[0]);
        SetParameterInt("rsy",largestRegion.GetSize()[1]);
        }

      // Put the limit of the index and the size relative the image
      SetMinimumParameterIntValue("rsx", 0);
      SetMaximumParameterIntValue("rsx", largestRegion.GetSize(0));

      SetMinimumParameterIntValue("rsy", 0);
      SetMaximumParameterIntValue("rsy", largestRegion.GetSize(1));

      SetMinimumParameterIntValue("rox", 0);
      SetMaximumParameterIntValue("rox", largestRegion.GetSize(0)-1);

      SetMinimumParameterIntValue("roy", 0);
      SetMaximumParameterIntValue("roy", largestRegion.GetSize(1)-1);

      // Crop the roi region to be included in the largest possible
      // region
      if(!this->CropRegionOfInterest())
        {
        // Put the index of the ROI to origin and try to crop again
        SetParameterInt("rox",0);
        SetParameterInt("roy",0);
        this->CropRegionOfInterest();
        }
      }

  }

bool CropRegionOfInterest()
  {
    FloatVectorImageType::RegionType region;
    region.SetSize(0,  GetParameterInt("rsx"));
    region.SetSize(1,  GetParameterInt("rsy"));
    region.SetIndex(0, GetParameterInt("rox"));
    region.SetIndex(1, GetParameterInt("roy"));
    FloatVectorImageType::RegionType region0 = region;

    if ( HasValue("in") )
      {
        if (region.Crop(GetParameterImage("in")->GetLargestPossibleRegion()))
          {
            if (region0.GetSize(0) != region.GetSize(0))
              SetParameterInt( "rsx", region.GetSize(0));
            if (region0.GetSize(1) != region.GetSize(1))
              SetParameterInt( "rsy", region.GetSize(1));
            if (region0.GetIndex(0) != region.GetIndex(0))
              SetParameterInt( "rox", region.GetIndex(0));
            if (region0.GetIndex(1) != region.GetIndex(1))
              SetParameterInt( "roy", region.GetIndex(1));
            return true;
          }
      }
    return false;
  }

  void DoExecute() override
  {
    InputImageType::Pointer inImage = GetParameterImage("in");

    ExtractROIFilterType::Pointer extractROIFilter =
      ExtractROIFilterType::New();
    ShrinkImageFilterType::Pointer resamplingFilter =
      ShrinkImageFilterType::New();

    // The image on which the quicklook will be generated
    // Will eventually be the extractROIFilter output

    if (HasUserValue("rox") || HasUserValue("roy")
        || HasUserValue("rsx") || HasUserValue("rsy")
        || (GetSelectedItems("cl").size() > 0))
      {
      extractROIFilter->SetInput(inImage);
      extractROIFilter->SetStartX(GetParameterInt("rox"));
      extractROIFilter->SetStartY(GetParameterInt("roy"));
      extractROIFilter->SetSizeX(GetParameterInt("rsx"));
      extractROIFilter->SetSizeY(GetParameterInt("rsy"));

      if ((GetSelectedItems("cl").size() > 0))
        {
        for (unsigned int idx = 0; idx < GetSelectedItems("cl").size(); ++idx)
          {
          extractROIFilter->SetChannel(GetSelectedItems("cl")[idx] + 1 );
          }
        }
      else
        {
        unsigned int nbComponents = inImage->GetNumberOfComponentsPerPixel();
        for (unsigned int idx = 0; idx < nbComponents; ++idx)
          {
          extractROIFilter->SetChannel(idx + 1);
          }
        }
      resamplingFilter->SetInput( extractROIFilter->GetOutput() );
      }
    else
      {
      resamplingFilter->SetInput(inImage);
      }

    unsigned int Ratio = static_cast<unsigned int>(GetParameterInt("sr"));
    unsigned int SamplingRatioX = 1;
    unsigned int SamplingRatioY = 1;

    if ( !HasUserValue("sr") )
      {
      if ( IsParameterEnabled("sx") && IsParameterEnabled("sy") )
        {
        SamplingRatioX =  GetParameterInt("rsx") / GetParameterInt("sx");
        SamplingRatioY =  GetParameterInt("rsy") / GetParameterInt("sy");
        if (SamplingRatioX > Ratio) Ratio = SamplingRatioX;
        if (SamplingRatioY > Ratio) Ratio = SamplingRatioY;
        }
      else
        {
        if ( IsParameterEnabled("sx") )
          {
          Ratio =  GetParameterInt("rsx") / GetParameterInt("sx");
          }
        if ( IsParameterEnabled("sy") )
          {
          Ratio =  GetParameterInt("rsy") / GetParameterInt("sy");
          }
        }
      }

    if ( Ratio < 1)
      {
      otbAppLogFATAL( << "Error in SizeX and/or SizeY : ratio must be greater than 1.");
      return;
      }
    otbAppLogINFO( << "Ratio used: "<<Ratio << ".");

    resamplingFilter->SetShrinkFactor( Ratio );
    resamplingFilter->Update();

    SetParameterOutputImage("out", resamplingFilter->GetOutput());
    RegisterPipeline();
  }

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::Quicklook)
