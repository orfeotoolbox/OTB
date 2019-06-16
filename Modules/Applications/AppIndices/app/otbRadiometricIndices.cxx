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
#include "otbWrapperNumericalParameter.h"

#include "otbVegetationIndicesFunctor.h"
#include "otbWaterIndicesFunctor.h"
#include "otbBuiltUpIndicesFunctor.h"
#include "otbSoilIndicesFunctor.h"
#include "otbIndicesStackFunctor.h"
#include "otbFunctorImageFilter.h"

namespace otb
{
namespace Wrapper
{

class RadiometricIndices: public Application
{
public:
  /** Standard class typedefs. */
  typedef RadiometricIndices       Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(RadiometricIndices, otb::Wrapper::Application);

  using InputType  = FloatVectorImageType::InternalPixelType;
  using OutputType = FloatImageType::PixelType;

  using RadiometricIndexType    = otb::Functor::RadiometricIndex<InputType, OutputType>;
  using IndicesStackFunctorType = otb::Functor::IndicesStackFunctor<RadiometricIndexType>;

  class indiceSpec
  {
  public:
    indiceSpec(std::string k, std::string i, RadiometricIndexType* ind) : key(k), item(i), indice(ind)
    {
    }
    std::string key;
    std::string item;
    std::unique_ptr<RadiometricIndexType> indice;
  };


private:
  void DoInit() override
  {
    SetName("RadiometricIndices");
    SetDescription("Compute radiometric indices.");

    // Documentation
    SetDocLongDescription("This application computes radiometric indices using the relevant channels of the input image. The output is a multi band image into which each channel is one of the selected indices.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("otbVegetationIndicesFunctor, otbWaterIndicesFunctor and otbSoilIndicesFunctor classes");

    AddDocTag(Tags::FeatureExtraction);
	AddDocTag("Radiometric Indices");

    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in", "Input image");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out", "Radiometric indices output image");

    AddParameter(ParameterType_Group, "channels", "Channels selection");
    SetParameterDescription("channels", "Channels selection");

    AddParameter(ParameterType_Int,  "channels.blue",  "Blue Channel");
    SetParameterDescription("channels.blue", "Blue channel index");
    SetDefaultParameterInt("channels.blue", 1);
    SetMinimumParameterIntValue("channels.blue", 1);

    AddParameter(ParameterType_Int,  "channels.green",  "Green Channel");
    SetParameterDescription("channels.green", "Green channel index");
    SetDefaultParameterInt("channels.green", 1);
    SetMinimumParameterIntValue("channels.green", 1);

    AddParameter(ParameterType_Int,  "channels.red",  "Red Channel");
    SetParameterDescription("channels.red", "Red channel index");
    SetDefaultParameterInt("channels.red", 1);
    SetMinimumParameterIntValue("channels.red", 1);

    AddParameter(ParameterType_Int,  "channels.nir",  "NIR Channel");
    SetParameterDescription("channels.nir", "NIR channel index");
    SetDefaultParameterInt("channels.nir", 1);
    SetMinimumParameterIntValue("channels.nir", 1);

    AddParameter(ParameterType_Int,  "channels.mir",  "Mir Channel");
    SetParameterDescription("channels.mir", "Mir channel index");
    SetDefaultParameterInt("channels.mir", 1);
    SetMinimumParameterIntValue("channels.mir", 1);

    AddParameter(ParameterType_ListView,  "list", "Available Radiometric Indices");
    SetParameterDescription("list",
                            "List of available radiometric indices with their relevant channels in brackets:\n\n"
                            "* Vegetation:NDVI - Normalized difference vegetation index (Red, NIR)\n"
                            "* Vegetation:TNDVI - Transformed normalized difference vegetation index (Red, NIR)\n"
                            "* Vegetation:RVI - Ratio vegetation index (Red, NIR)\n"
                            "* Vegetation:SAVI - Soil adjusted vegetation index (Red, NIR)\n"
                            "* Vegetation:TSAVI - Transformed soil adjusted vegetation index (Red, NIR)\n"
                            "* Vegetation:MSAVI - Modified soil adjusted vegetation index (Red, NIR)\n"
                            "* Vegetation:MSAVI2 - Modified soil adjusted vegetation index 2 (Red, NIR)\n"
                            "* Vegetation:GEMI - Global environment monitoring index (Red, NIR)\n"
                            "* Vegetation:IPVI - Infrared percentage vegetation index (Red, NIR)\n"
                            "* Vegetation:LAIFromNDVILog - Leaf Area Index from log NDVI (Red, NIR)\n"
                            "* Vegetation::LAIFromReflLinear - Leaf Area Index from reflectances with linear combination (Red, NIR)\n"
                            "* Vegetation::LAIFromNDVIFormo - Leaf Area Index from Formosat 2  TOC (Red, NIR)\n"
                            "* Water:NDWI - Normalized difference water index (Gao 1996) (NIR, MIR)\n"
                            "* Water:NDWI2 - Normalized difference water index (Mc Feeters 1996) (Green, NIR)\n"
                            "* Water:MNDWI - Modified normalized difference water index (Xu 2006) (Green, MIR)\n"
                            "* Water:NDTI - Normalized difference turbidity index (Lacaux et al.) (Red, Green)\n"
                            "* Soil:RI - Redness index (Red, Green)\n"
                            "* Soil:CI - Color index (Red, Green)\n"
                            "* Soil:BI - Brightness index (Red, Green)\n"
                            "* Soil:BI2 - Brightness index 2 (NIR, Red, Green)\n"
                            "* BuiltUp:ISU - Built Surfaces Index (NIR,Red) ");

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "qb_RoadExtract.tif");
    SetDocExampleParameterValue("list", "Vegetation:NDVI Vegetation:RVI Vegetation:IPVI");
    SetDocExampleParameterValue("out", "RadiometricIndicesImage.tif");

    SetOfficialDocLink();

    m_Map.clear();

    m_Map.push_back({"list.ndvi", "Vegetation:NDVI", new otb::Functor::NDVI<InputType, OutputType>()});
    m_Map.push_back({"list.tndvi", "Vegetation:TNDVI", new otb::Functor::TNDVI<InputType, OutputType>()});
    m_Map.push_back({"list.rdvi", "Vegetation:RVI", new otb::Functor::RVI<InputType, OutputType>()});
    m_Map.push_back({"list.savi", "Vegetation:SAVI", new otb::Functor::SAVI<InputType, OutputType>()});
    m_Map.push_back({"list.tsavi", "Vegetation:TSAVI", new otb::Functor::TSAVI<InputType, OutputType>()});
    m_Map.push_back({"list.msavi", "Vegetation:MSAVI", new otb::Functor::MSAVI<InputType, OutputType>()});
    m_Map.push_back({"list.msavi2", "Vegetation:MSAVI2", new otb::Functor::MSAVI2<InputType, OutputType>()});
    m_Map.push_back({"list.gemi", "Vegetation:GEMI", new otb::Functor::GEMI<InputType, OutputType>()});
    m_Map.push_back({"list.ipvi", "Vegetation:IPVI", new otb::Functor::IPVI<InputType, OutputType>()});
    m_Map.push_back({"list.laindvilog", "Vegetation:LAIFromNDVILog", new otb::Functor::LAIFromNDVILogarithmic<InputType, OutputType>()});
    m_Map.push_back({"list.lairefl", "Vegetation:LAIFromReflLinear", new otb::Functor::LAIFromReflectancesLinear<InputType, OutputType>()});
    m_Map.push_back({"list.laindviformo", "Vegetation:LAIFromNDVIFormo", new otb::Functor::LAIFromNDVIFormosat2Functor<InputType, OutputType>()});
    m_Map.push_back({"list.ndwi", "Water:NDWI", new otb::Functor::NDWI<InputType, OutputType>()});
    m_Map.push_back({"list.ndwi2", "Water:NDWI2", new otb::Functor::NDWI2<InputType, OutputType>()});
    m_Map.push_back({"list.mndwi", "Water:MNDWI", new otb::Functor::MNDWI<InputType, OutputType>()});
    m_Map.push_back({"list.ndti", "Water:NDTI", new otb::Functor::NDTI<InputType, OutputType>()});
    m_Map.push_back({"list.ri", "Soil:RI", new otb::Functor::RI<InputType, OutputType>()});
    m_Map.push_back({"list.ci", "Soil:CI", new otb::Functor::CI<InputType, OutputType>()});
    m_Map.push_back({"list.bi", "Soil:BI", new otb::Functor::BI<InputType, OutputType>()});
    m_Map.push_back({"list.bi2", "Soil:BI2", new otb::Functor::BI2<InputType, OutputType>()});
    m_Map.push_back({"list.isu", "BuiltUp:ISU", new otb::Functor::ISU<InputType, OutputType>()});

    ClearChoices("list");

    for ( unsigned int i=0; i<m_Map.size(); i++ )
      {
      AddChoice(m_Map[i].key, m_Map[i].item);
      }
  }

  // Compute required bands for selected indices
  std::set<CommonBandNames> GetRequiredBands()
  {
    std::set<CommonBandNames> required;

    for (unsigned int idx = 0; idx < GetSelectedItems("list").size(); ++idx)
    {
      auto requiredForCurrentIndice = m_Map[GetSelectedItems("list")[idx]].indice->GetRequiredBands();
      required.insert(requiredForCurrentIndice.begin(), requiredForCurrentIndice.end());
    }
    return required;
  }

  void DoUpdateParameters() override
  {
    //Nothing to do here
  }

  void DoExecute() override
  {
    // Retrieve number of bands of input image
    unsigned int nbChan = GetParameterImage("in")->GetNumberOfComponentsPerPixel();

    // Derive required bands from selected indices
    auto requiredBands = GetRequiredBands();

    // Map to store association between bands and indices
    std::map<CommonBandNames, size_t> bandIndicesMap;

    // Lambda that will:
    // - Check if band is required,
    // - Check band index range,
    // - Populate the bandIndicesMap
    auto bandChecker = [this, requiredBands, nbChan](std::map<CommonBandNames, size_t>& indicesMap, const CommonBandNames& band, const std::string& key) {
      if (requiredBands.find(band) != requiredBands.end())
      {
        unsigned int idx = this->GetParameterInt(key);

        if (idx > nbChan)
        {
          otbAppLogFATAL(<< "Index for band " << key << " exceeds the number of channels in image (" << nbChan << ")");
        }
        else
        {
          indicesMap[band] = idx;
        }
      }
    };

    // Call lambda for each possible band
    bandChecker(bandIndicesMap, CommonBandNames::BLUE, "channels.blue");
    bandChecker(bandIndicesMap, CommonBandNames::GREEN, "channels.green");
    bandChecker(bandIndicesMap, CommonBandNames::RED, "channels.red");
    bandChecker(bandIndicesMap, CommonBandNames::NIR, "channels.nir");
    bandChecker(bandIndicesMap, CommonBandNames::MIR, "channels.mir");

    std::vector<RadiometricIndexType*> indices;

    // Find selected indices
    for (unsigned int idx = 0; idx < GetSelectedItems("list").size(); ++idx)
    {
      // Retrieve the indice instance
      indices.push_back(m_Map[GetSelectedItems("list")[idx]].indice.get());

      // And set bands using the band map
      indices.back()->SetBandsIndices(bandIndicesMap);
      }

      // Build a composite indices functor to compute all indices at
      // once
      auto compositeFunctor = IndicesStackFunctorType(indices);

      // Build and plug functor filter
      auto filter = NewFunctorFilter(compositeFunctor);
      filter->SetInputs(GetParameterImage("in"));
      SetParameterOutputImage("out", filter->GetOutput());

      // Call register pipeline to allow streaming and garbage collection
      RegisterPipeline();
  }

  std::vector<indiceSpec> m_Map;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::RadiometricIndices)
