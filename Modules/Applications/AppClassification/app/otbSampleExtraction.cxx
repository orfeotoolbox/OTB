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

#include "otbImageSampleExtractorFilter.h"

namespace otb
{
namespace Wrapper
{
/** Utility function to negate std::isalnum */
bool IsNotAlphaNum(char c)
  {
  return !std::isalnum(c);
  }

class SampleExtraction : public Application
{
public:
  /** Standard class typedefs. */
  typedef SampleExtraction              Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(SampleExtraction, otb::Application);

  /** Filters typedef */
  typedef otb::ImageSampleExtractorFilter<FloatVectorImageType> FilterType;

private:
  SampleExtraction() {}

  void DoInit() override
  {
    SetName("SampleExtraction");
    SetDescription("Extracts samples values from an image.");

    // Documentation
    SetDocLongDescription("The application extracts samples values from an"
      "image using positions contained in a vector data file. ");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Learning);

    AddParameter(ParameterType_InputImage,  "in",   "InputImage");
    SetParameterDescription("in", "Support image");

    AddParameter(ParameterType_InputFilename, "vec", "Input sampling positions");
    SetParameterDescription("vec","Vector data file containing sampling"
                                  "positions. (OGR format)");

    AddParameter(ParameterType_OutputFilename, "out", "Output samples");
    SetParameterDescription("out","Output vector data file storing sample"
      "values (OGR format). If not given, the input vector data file is updated");
    MandatoryOff("out");

    AddParameter(ParameterType_Choice, "outfield", "Output field names");
    SetParameterDescription("outfield", "Choice between naming method for output fields");

    AddChoice("outfield.prefix","Use a prefix and an incremental counter");
    SetParameterDescription("outfield.prefix","Use a prefix and an incremental counter");

    AddParameter(ParameterType_String, "outfield.prefix.name", "Output field prefix");
    SetParameterDescription("outfield.prefix.name","Prefix used to form the field names that"
      "will contain the extracted values.");
    SetParameterString("outfield.prefix.name", "value_");

    AddChoice("outfield.list","Use the given name list");
    SetParameterDescription("outfield.list","Use the given name list");

    AddParameter(ParameterType_StringList, "outfield.list.names", "Output field names");
    SetParameterDescription("outfield.list.names","Full list of output field names.");

    AddParameter(ParameterType_ListView, "field", "Field Name");
    SetParameterDescription("field","Name of the field carrying the class name in the input vectors.");
    SetListViewSingleSelectionMode("field",true);
    
    AddParameter(ParameterType_Int, "layer", "Layer Index");
    SetParameterDescription("layer", "Layer index to read in the input vector file.");
    MandatoryOff("layer");
    SetDefaultParameterInt("layer",0);

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "support_image.tif");
    SetDocExampleParameterValue("vec", "sample_positions.sqlite");
    SetDocExampleParameterValue("outfield","prefix");
    SetDocExampleParameterValue("outfield.prefix.name","band_");
    SetDocExampleParameterValue("field", "label");
    SetDocExampleParameterValue("out","sample_values.sqlite");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    if ( HasValue("vec") )
      {
      std::string vectorFile = GetParameterString("vec");
      ogr::DataSource::Pointer ogrDS =
        ogr::DataSource::New(vectorFile, ogr::DataSource::Modes::Read);
      ogr::Layer layer = ogrDS->GetLayer(this->GetParameterInt("layer"));
      ogr::Feature feature = layer.ogr().GetNextFeature();

      if(!feature.addr())
        {
        otbAppLogFATAL(<<"No features found in "<<vectorFile);
        }

      ClearChoices("field");
      
      for(int iField=0; iField<feature.ogr().GetFieldCount(); iField++)
        {
        std::string key, item = feature.ogr().GetFieldDefnRef(iField)->GetNameRef();
        key = item;
        std::string::iterator end = std::remove_if(key.begin(),key.end(),IsNotAlphaNum);
        std::transform(key.begin(), end, key.begin(), tolower);
        
        OGRFieldType fieldType = feature.ogr().GetFieldDefnRef(iField)->GetType();
        
        if(fieldType == OFTString || fieldType == OFTInteger || fieldType == OFTInteger64)
          {
          std::string tmpKey="field."+key.substr(0, end - key.begin());
          AddChoice(tmpKey,item);
          }
        }
      }
  }

  void DoExecute() override
    {
    ogr::DataSource::Pointer vectors;
    ogr::DataSource::Pointer output;
    if (IsParameterEnabled("out") && HasValue("out"))
      {
      vectors = ogr::DataSource::New(this->GetParameterString("vec"));
      output = ogr::DataSource::New(this->GetParameterString("out"),
                                    ogr::DataSource::Modes::Overwrite);
      }
    else
      {
      // Update mode
      vectors = ogr::DataSource::New(this->GetParameterString("vec"),
                                    ogr::DataSource::Modes::Update_LayerUpdate);
      output = vectors;
      }

    // Retrieve the field name
    std::vector<int> selectedCFieldIdx = GetSelectedItems("field");

    if(selectedCFieldIdx.empty())
      {
      otbAppLogFATAL(<<"No field has been selected for data labelling!");
      }

  std::vector<std::string> cFieldNames = GetChoiceNames("field");  
  std::string fieldName = cFieldNames[selectedCFieldIdx.front()];
    
    std::vector<std::string> nameList;
    std::string namePrefix("");
    if (this->GetParameterString("outfield").compare("prefix") == 0)
      {
      namePrefix = this->GetParameterString("outfield.prefix.name");
      }
    else if (this->GetParameterString("outfield").compare("list") == 0)
      {
      nameList = this->GetParameterStringList("outfield.list.names");
      }
    else
      {
      otbAppLogFATAL("Unknown output field option : " << this->GetParameterString("outfield"));
      }
    

    FilterType::Pointer filter = FilterType::New();
    filter->SetInput(this->GetParameterImage("in"));
    filter->SetLayerIndex(this->GetParameterInt("layer"));
    filter->SetSamplePositions(vectors);
    filter->SetOutputSamples(output);
    filter->SetClassFieldName(fieldName);
    filter->SetOutputFieldPrefix(namePrefix);
    filter->SetOutputFieldNames(nameList);
    filter->GetStreamer()->SetAutomaticAdaptativeStreaming(GetParameterInt("ram"));

    
    AddProcess(filter->GetStreamer(),"Extracting sample values...");
    filter->Update();
    output->SyncToDisk();
    }

};

} // end of namespace Wrapper
} // end of namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::SampleExtraction)
