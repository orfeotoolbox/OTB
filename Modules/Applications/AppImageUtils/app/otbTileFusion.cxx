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

#include "otbTileImageFilter.h"

namespace otb
{
namespace Wrapper
{

class TileFusion : public Application
{
public:
  /** Standard class typedefs. */
  typedef TileFusion                    Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(TileFusion, otb::Application);

  typedef otb::TileImageFilter<FloatVectorImageType> TileFilterType;

private:
  void DoInit() override
  {
    SetName("TileFusion");
    SetDescription("Fusion of an image made of several tile files.");

    // Documentation
    SetDocLongDescription(
        "Automatically mosaic a set of non overlapping tile files into a single image. Images must have a matching number of bands and they must be listed in "
        "lexicographic order.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Manip);

    AddParameter(ParameterType_InputImageList, "il", "Input Tile Images");
    SetParameterDescription("il", "Input images to concatenate (in lexicographic order, for instance: (0,0) (1,0) (0,1) (1,1)).");

    AddParameter(ParameterType_Int, "cols", "Number of tile columns");
    SetParameterDescription("cols", "Number of columns in the tile array");

    AddParameter(ParameterType_Int, "rows", "Number of tile rows");
    SetParameterDescription("rows", "Number of rows in the tile array");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out", "Output entire image");

    // Doc example parameter settings
    SetDocExampleParameterValue("il", "Scene_R1C1.tif Scene_R1C2.tif Scene_R2C1.tif Scene_R2C2.tif");
    SetDocExampleParameterValue("cols", "2");
    SetDocExampleParameterValue("rows", "2");
    SetDocExampleParameterValue("out", "EntireImage.tif");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to be done
  }

  void DoExecute() override
  {
    // Get the input image list
    FloatVectorImageListType::Pointer tileList = this->GetParameterImageList("il");

    if (tileList->Size() == 0)
    {
      itkExceptionMacro("No input Image set...");
    }

    TileFilterType::Pointer fusionFilter = TileFilterType::New();

    TileFilterType::SizeType layout;
    layout[0] = this->GetParameterInt("cols");
    layout[1] = this->GetParameterInt("rows");
    fusionFilter->SetLayout(layout);

    for (unsigned int i = 0; i < (layout[0] * layout[1]); i++)
    {
      fusionFilter->SetInput(i, tileList->GetNthElement(i));
    }

    SetParameterOutputImage("out", fusionFilter->GetOutput());
    RegisterPipeline();
  }
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::TileFusion)
