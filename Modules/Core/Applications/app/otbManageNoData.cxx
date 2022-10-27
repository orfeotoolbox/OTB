/*
 * Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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

#include "otbImageToNoDataMaskFilter.h"
#include "otbChangeNoDataValueFilter.h"
#include "itkMaskImageFilter.h"
#include "otbVectorImageToImageListFilter.h"
#include "otbImageListToVectorImageFilter.h"

namespace otb
{
namespace Wrapper
{

class ManageNoData : public Application
{
public:
  /** Standard class typedefs. */
  typedef ManageNoData                  Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ManageNoData, otb::Application);

  /** Filters typedef */
  typedef otb::ImageToNoDataMaskFilter<FloatVectorImageType, UInt8ImageType>       FilterType;
  typedef otb::ChangeNoDataValueFilter<FloatVectorImageType, FloatVectorImageType> ChangeNoDataFilterType;

  typedef otb::ImageList<FloatImageType> ImageListType;
  typedef otb::VectorImageToImageListFilter<FloatVectorImageType, ImageListType> VectorToListFilterType;
  typedef otb::ImageListToVectorImageFilter<ImageListType, FloatVectorImageType> ListToVectorFilterType;
  typedef itk::MaskImageFilter<FloatImageType, UInt8ImageType, FloatImageType> MaskFilterType;

private:
  void DoInit() override
  {
    SetName("ManageNoData");
    SetDescription("Manage No-Data");
    // Documentation
    SetDocLongDescription(
        "This application has two modes. The first allows building a mask of no-data pixels from the no-data flags read from the image file. The second allows "
        "updating the change the no-data value of an image (pixels value and metadata). This last mode also allows replacing NaN in images with a proper "
        "no-data value. To do so, one should activate the NaN is no-data option.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("BandMath");

    AddDocTag(Tags::Manip);
    AddDocTag("Conversion");
    AddDocTag("Image Dynamic");

    AddParameter(ParameterType_InputImage, "in", "Input image");
    SetParameterDescription("in", "Input image");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out", "Output image");

    AddParameter(ParameterType_Bool, "usenan", "Consider NaN as no-data");
    SetParameterDescription("usenan", "If active, the application will consider NaN as no-data values as well");

    AddParameter(ParameterType_Choice, "mode", "No-data handling mode");
    SetParameterDescription("mode", "Allows choosing between different no-data handling options");

    AddChoice("mode.buildmask", "Build a no-data Mask");

    AddParameter(ParameterType_Float, "mode.buildmask.inv", "Inside Value");
    SetParameterDescription("mode.buildmask.inv", "Value given in the output mask to pixels that are not no data pixels");
    SetDefaultParameterInt("mode.buildmask.inv", 1);

    AddParameter(ParameterType_Float, "mode.buildmask.outv", "Outside Value");
    SetParameterDescription("mode.buildmask.outv", "Value given in the output mask to pixels that are no data pixels");
    SetDefaultParameterInt("mode.buildmask.outv", 0);

    AddChoice("mode.changevalue", "Change the no-data value");

    AddParameter(ParameterType_Float, "mode.changevalue.newv", "The new no-data value");
    SetParameterDescription("mode.changevalue.newv", "The new no-data value");
    SetDefaultParameterInt("mode.changevalue.newv", 0);

    AddChoice("mode.apply", "Apply a mask as no-data");

    SetParameterDescription("mode.apply", "Apply an external mask to an image using the no-data value of the input image");
    AddParameter(ParameterType_InputImage, "mode.apply.mask", "Mask image");
    SetParameterDescription("mode.apply.mask", "Mask to be applied on input image (valid pixels have non null values)");
    AddParameter(ParameterType_Float, "mode.apply.ndval", "Nodata value used");
    SetParameterDescription("mode.apply.ndval", "No Data value used according to the mask image");
    SetDefaultParameterFloat("mode.apply.ndval", 0.0);

    SetParameterString("mode", "buildmask");

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "QB_Toulouse_Ortho_XS.tif");
    SetDocExampleParameterValue("out", "QB_Toulouse_Ortho_XS_nodatamask.tif uint8");
    SetDocExampleParameterValue("mode.buildmask.inv", "255");
    SetDocExampleParameterValue("mode.buildmask.outv", "0");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here for the parameters : all are independent
  }


  void DoExecute() override
  {
    FloatVectorImageType::Pointer inputPtr = this->GetParameterImage("in");

    if (GetParameterString("mode") == "buildmask")
    {
      auto filter = FilterType::New();
      filter->SetInsideValue(this->GetParameterFloat("mode.buildmask.inv"));
      filter->SetOutsideValue(this->GetParameterFloat("mode.buildmask.outv"));
      filter->SetNaNIsNoData(GetParameterInt("usenan"));
      filter->SetInput(inputPtr);

      SetParameterOutputImage("out", filter->GetOutput());
      RegisterPipeline();
    }
    else if (GetParameterString("mode") == "changevalue")
    {
      auto changeNoDataFilter = ChangeNoDataFilterType::New();
      changeNoDataFilter->SetInput(inputPtr);
      changeNoDataFilter->SetNaNIsNoData(GetParameterInt("usenan"));

      std::vector<double> newNoData(inputPtr->GetNumberOfComponentsPerPixel(), GetParameterFloat("mode.changevalue.newv"));
      changeNoDataFilter->SetNewNoDataValues(newNoData);

      SetParameterOutputImage("out", changeNoDataFilter->GetOutput());
      RegisterPipeline();
    }
    else if (GetParameterString("mode") == "apply")
    {
      UInt8ImageType::Pointer  maskPtr = this->GetParameterUInt8Image("mode.apply.mask");
      unsigned int             nbBands = inputPtr->GetNumberOfComponentsPerPixel();
      const auto & imd                 = inputPtr->GetImageMetadata();
      std::vector<bool>        flags;
      std::vector<double>      values;
      bool                     ret = otb::ReadNoDataFlags(imd, flags, values);
      if (!ret)
      {
        flags = std::vector<bool>(nbBands, true);
        values = std::vector<double>(nbBands, GetParameterFloat("mode.apply.ndval"));
      }

      auto V2L = VectorToListFilterType::New();
      V2L->SetInput(inputPtr);
      ImageListType::Pointer inputList = V2L->GetOutput();
      inputList->UpdateOutputInformation();
      ImageListType::Pointer outputList = ImageListType::New();
      std::vector<MaskFilterType::Pointer> maskFilters;
      for (unsigned int i = 0; i < nbBands; ++i)
      {
        if (flags[i])
        {
          MaskFilterType::Pointer masker = MaskFilterType::New();
          masker->SetInput(inputList->GetNthElement(i));
          masker->SetMaskImage(maskPtr);
          masker->SetMaskingValue(0);
          masker->SetOutsideValue(values[i]);
          outputList->PushBack(masker->GetOutput());
          maskFilters.push_back(masker);
        }
        else
        {
          outputList->PushBack(inputList->GetNthElement(i));
        }
      }
      auto L2V = ListToVectorFilterType::New();
      L2V->SetInput(outputList);
      if (!ret)
      {
        // write the new NoData values in the output metadata (mode.apply.ndval)
        L2V->UpdateOutputInformation();
        otb::WriteNoDataFlags(flags, values, L2V->GetOutput()->GetImageMetadata());
      }
      SetParameterOutputImage("out", L2V->GetOutput());
      RegisterPipeline();
    }
  }
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ManageNoData)
