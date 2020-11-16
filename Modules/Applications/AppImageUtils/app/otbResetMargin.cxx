/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#include "otbResetMarginFilter.h"
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

namespace otb
{
namespace Wrapper
{

/**
 * Application that fills margins to 0.
 *
 * This application is similar to ExtractROI with the difference the margin is
 * kept, and filled with 0.
 *
 * This application is used to implement the _cut_ processing in S1Tiling
 * chain.
 *
 * \author Luc Hermitte (CS Group)
 * \copyright CNES
 */
class ResetMargin : public Application
{
public:
  using Self    = ResetMargin;
  using Pointer = itk::SmartPointer<Self>;

  itkNewMacro(Self);
  itkTypeMacro(ResetMargin, otb::Wrapper::Application);

private:
  void DoInit() override
  {
    SetName("ResetMargin");

    SetDescription("This is the ResetMargin application");
    SetDocLongDescription(
        "This application is similar to ExtractROI in the sense it extracts a Region of Interrest.\n"
        "However, the region outside of the ROI isn't trimmed, but set to 0.\n"
        "\n"
        "The filter set lines of index < threshold.y, and of index >= threshold.y to 0\n"
        "The filter set columns of index < threshold.x, and of index >= threshold.x to 0");

    SetDocLimitations("This application only works on scalar (and complex) images.");
    SetDocAuthors("Luc Hermitte (CS Group)");
    SetDocSeeAlso("ManageNoData, ExtractROI");
    AddDocTag(Tags::Manip);

    AddParameter(ParameterType_InputImage,  "in",   "Input image");
    SetParameterDescription("in", "Scalar Input image");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out", "Scalar Output image");

    AddParameter(ParameterType_Group, "threshold", "threshold group");
    AddParameter(ParameterType_Group, "threshold.y", "threshold group");
    MandatoryOff("threshold");
    MandatoryOff("threshold.y");
    AddParameter(ParameterType_Int, "threshold.x", "Column index threshold");
    SetParameterDescription("threshold.x", "Column index threshold");
    SetDefaultParameterInt("threshold.x", 0);

    AddParameter(ParameterType_Int, "threshold.y.start", "Top line index threshold");
    SetParameterDescription("threshold.y.start", "Top line index threshold");
    SetDefaultParameterInt("threshold.y.start", 0);

    AddParameter(ParameterType_Int, "threshold.y.end", "Bottom line index threshold");
    SetParameterDescription("threshold.y.end", "Bottom line index threshold");
    SetDefaultParameterInt("threshold.y.end", 0);

    SetMinimumParameterIntValue("threshold.x",       0);
    SetMinimumParameterIntValue("threshold.y.start", 0);
    SetMinimumParameterIntValue("threshold.y.end",   0);

    AddParameter(ParameterType_Group, "roi", "ROI group");
    SetParameterDescription("roi", "Contains index and size of ROI");

    AddParameter(ParameterType_Int, "roi.startx", "X start index");
    SetParameterDescription("roi.startx", "X position of ROI start (in pixels)");
    SetDefaultParameterInt("roi.startx", 0);

    AddParameter(ParameterType_Int, "roi.starty", "Y start index");
    SetParameterDescription("roi.starty", "Y position of ROI start (in pixels)");
    SetDefaultParameterInt("roi.starty", 0);

    AddParameter(ParameterType_Int, "roi.sizex", "X size");
    SetParameterDescription("roi.sizex", "X size of ROI (in pixels)");
    SetDefaultParameterInt("roi.sizex", 0);

    AddParameter(ParameterType_Int, "roi.sizey", "Y size");
    SetParameterDescription("roi.sizey", "Y size of ROI (in pixels)");
    SetDefaultParameterInt("roi.sizey", 0);

    MandatoryOff("roi.startx");
    MandatoryOff("roi.starty");
    MandatoryOff("roi.sizex");
    MandatoryOff("roi.sizey");

    AddParameter(ParameterType_Choice, "mode", "Region mode");
    AddChoice("mode.roi", "Pixel region with start and size");

    AddChoice("mode.threshold", "Threshold for X, Y top and botton (DEPRECATED)");

    AddParameter(ParameterType_Float, "val", "Padding value");
    SetParameterDescription("val", "Value to insert in margins");
    SetDefaultParameterFloat("val", 0.0);
    MandatoryOff("val");

    AddRAMParameter();

    SetDocExampleParameterValue("in", "ResetMarginInput100x100.tiff");
    SetDocExampleParameterValue("threshold.x",       "10");
    SetDocExampleParameterValue("threshold.y.start", "12");
    SetDocExampleParameterValue("threshold.y.end",    "25");
    SetDocExampleParameterValue("out", "ResetMargin.tiff");
    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {}

  void DoExecute() override
  {
    FloatImageType* input = GetParameterFloatImage("in");
    FloatImageType::RegionType region;
    switch (GetParameterInt("mode"))
      {
      case 0: // roi
        {
        region.SetIndex({GetParameterInt("roi.startx"),GetParameterInt("roi.starty")});
        region.SetSize({(unsigned long)GetParameterInt("roi.sizex"),(unsigned long)GetParameterInt("roi.sizey")});
        break;
        }
      case 1: // threshold
        {
        region = input->GetLargestPossibleRegion();
        FloatImageType::IndexType idx = region.GetIndex();
        FloatImageType::SizeType sz = region.GetSize();
        int thrX = GetParameterInt("threshold.x");
        int thrYtop = GetParameterInt("threshold.y.start");
        int thrYbot = GetParameterInt("threshold.y.end");
        region.SetIndex(0, idx[0] + thrX);
        region.SetSize(0,  std::max(0UL, sz[0] - 2 * thrX));
        region.SetIndex(1, idx[1] + thrYtop);
        region.SetSize(1, std::max(0UL, sz[1] - thrYtop - thrYbot));
        break;
        }
      default:
        {
        otbAppLogFATAL("Unknown region mode: "<<GetParameterString("mode"));
        break;
        }
      }

    auto filter = ResetMarginFilter<FloatImageType>::New();
    filter->SetROI(region);
    filter->SetInput(input);
    filter->SetPaddingValue(GetParameterFloat("val"));

    SetParameterOutputImage("out", filter->GetOutput());
    RegisterPipeline();
  }

};

} // otb::Wrapper namespace
} // otb namespace

OTB_APPLICATION_EXPORT(otb::Wrapper::ResetMargin)
