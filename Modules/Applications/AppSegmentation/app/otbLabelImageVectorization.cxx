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

#include "otbPersistentLabelImageVectorizationFilter.h"
#include "otbConnectedComponentStreamStitchingFilter.h"


namespace otb
{
namespace Wrapper
{

class LabelImageVectorization: public Application
{
public:
  /** Standard class typedefs. */
  typedef LabelImageVectorization        Self;
  typedef Application                    Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  
  /** Vector data typedefs. */
  typedef otb::ogr::DataSource           OGRDataSourceType;
  typedef otb::ogr::Layer                OGRLayerType;
  typedef otb::ogr::Feature              OGRFeatureType;

  /** Raster data typedef */ 
  typedef unsigned int                   PixelType;
  typedef otb::Image<PixelType, 2>       LabelImageType;

  /** Filters typedefs */
  typedef ConnectedComponentStreamStitchingFilter< LabelImageType >             
                              ConnectedComponentStreamStitchingFilterType;
  typedef LabelImageVectorizationFilter<LabelImageType>                         
                              LabelImageVectorizationFilterType;
  
  
  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(LabelImageVectorization, otb::Application);

private:
  void DoInit() override
  {
    SetName("LabelImageVectorization");
    SetDescription("This application performs the vectorization of an "
      "input label image");

    // Documentation
    SetDocName("Mask Filtering");
    SetDocLongDescription("Given an input label raster image, this "
      "application will output a vector data file containing a polygon "
      "for each connected component of the input raster, where two "
      "components are connected if they share the same label. If the "
      "fusion parameter is activated, segments in the output image that "
      "might have been splitted due to streaming are merged (two segment "
      "are merged if they are touching and if they share the same label)." 
      "By default, all segments with a label different than 0 are vectorized, "
      "but the feat parameter can be set to choose which label should be"
      "vectorized.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    
    SetDocSeeAlso("LargeScaleConnectedComponent");
    AddDocTag("LargeScaleConnectedComponent");

    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    SetParameterDescription("in", "Input label image to be vectorized.");
    
    AddParameter(ParameterType_Bool,"connectivity","Full connectivity");
    SetParameterDescription("connectivity","if this parameter is set to true,"
    "an eight pixels neighbourhood will be used instead of the default"
    "four pixels neighbourdhood.");

    AddParameter(ParameterType_OutputFilename, "out", 
      "Output vector data file ");
    SetParameterDescription("out","Output vector");
     
    AddParameter(ParameterType_Float,"tolerance",
      "Tolerance used for polygon simplification");
    SetParameterDescription("tolerance",
      "Simplify polygons according to a given tolerance (in pixel). "
      "This option allows reducing the size of the output file or database.");
    MandatoryOff("tolerance");
    
    AddParameter(ParameterType_Bool,"fusion","flag to perform polygon fusion");
    SetParameterDescription("fusion","If this parameter is set to true , "
    "segments in the output image that might have been splitted due to "
    "streaming are merged (two segment are merged if they are touching and if "
    "they share the same label).");
     
    AddParameter(ParameterType_StringList,  "feat", "Input image labels");
    SetParameterDescription("feat","label in the input image to be vectorized."
      "By default all labels but 0 are used. This parameter can be set for "
      "example to vectorize a classification map, and output a polygon by "
      "connex component of a same class");
    MandatoryOff("feat");

    AddParameter(ParameterType_Int,"tile","Tile Size");
    SetParameterDescription("tile","Size of the tiles used for streaming,"
      "by default, size tile is automatically computed using the available"
      "RAM.");
    MandatoryOff("tile");
    
    AddRAMParameter();

    // Default values
    SetParameterInt("connectivity", 0);
    SetDefaultParameterFloat("tolerance", 0.);
    SetParameterInt("fusion", 1);
    
    // Doc example parameter settings
    SetDocExampleParameterValue("in", "ROI_QB_MUL_1_SVN_CLASS_MULTI.png");
    SetDocExampleParameterValue("out","test.sqlite");
    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute() override
  {
    // Create the OGR DataSource with the appropriate fields
    std::string projRef = 
            this->GetParameterUInt32Image("in")->GetProjectionRef();
    OGRSpatialReference oSRS(projRef.c_str());
    std::string layer_name = "layer";
    std::string field_name = "field";
    std::string dataSourceName = GetParameterString("out");

    // Create the datasource
    auto ogrDS = otb::ogr::DataSource::New(dataSourceName, 
                                otb::ogr::DataSource::Modes::Overwrite);
    auto layer = ogrDS->CreateLayer(layer_name, &oSRS, wkbPolygon);
    OGRFieldDefn field(field_name.c_str(), OFTInteger);
    layer.CreateField(field, true);
    
    // Mask FIlter : Vectorization of the input raster
    auto labelImageFilter = LabelImageVectorizationFilterType::New();
    
    // Labeled image to be vectorized
    labelImageFilter->SetInput(this->GetParameterUInt32Image("in"));
    labelImageFilter->SetOGRLayer(layer) ;
    if (IsParameterEnabled("tile") && HasValue("tile"))
      {
      labelImageFilter->GetStreamer()->SetTileDimensionTiledStreaming(
        this->GetParameterInt("tile"));
      }
    else
      {
      labelImageFilter->GetStreamer()->SetAutomaticTiledStreaming();
      }
    
    /* Enlarge is not a parameter of the application, if the fusion option is 
      'on' we enlarge, if it is 'off' we don't */
    if (this->GetParameterInt("fusion") == 1)
      {
      labelImageFilter->SetEnlarge(1);
      }
    
    // Input labels : Labels in the input image that will be vectorized
    std::vector<int> inputLabels;
    if(IsParameterEnabled("feat") )
      {
      std::vector<std::string> inputLabelsStr = GetParameterStringList("feat");
      for ( const auto & label : inputLabelsStr )
        {
        inputLabels.push_back(std::stoi(label));
        }
      }
    
    labelImageFilter->SetLabels(inputLabels);
    labelImageFilter->SetUse8Connected(this->GetParameterInt("connectivity"));
    labelImageFilter->SetTolerance(this->GetParameterFloat("tolerance"));
    labelImageFilter->SetFieldName( field_name);
    labelImageFilter->Initialize();
    AddProcess(labelImageFilter->GetStreamer(),"Vectorizing connected segments...");
    labelImageFilter->Update();
    ogrDS->SyncToDisk();
    
    otbAppLogINFO("Vectorization done")
    
    // Fusion Filter : Regroup polygons splitted across tiles.
    if (this->GetParameterInt("fusion") == 1)
      {
      auto fusionFilter = ConnectedComponentStreamStitchingFilterType::New();
      fusionFilter->SetInput(this->GetParameterUInt32Image("in"));
      fusionFilter->SetOGRLayer(layer);
      fusionFilter->SetStreamSize(labelImageFilter->GetStreamSize());
      AddProcess(fusionFilter,"Merging connected segments splitted by streaming ...");
      fusionFilter->GenerateData();
      }

  }
}; 

} //end namespace Wrapper
} //end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::LabelImageVectorization)
