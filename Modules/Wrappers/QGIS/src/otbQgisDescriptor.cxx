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

#include "otbWrapperChoiceParameter.h"
#include "otbWrapperListViewParameter.h"
#include "otbWrapperBoolParameter.h"
#include "otbWrapperApplicationRegistry.h"

#include <iostream>
#include <cassert>
#include <fstream>

int main(int argc, char* argv[])
{
  if (argc < 3)
    {
    std::cerr << "Usage : " << argv[0] << " name OTB_APPLICATION_PATH [out_dir]" << std::endl;
    return EXIT_FAILURE;
    }
    
  using namespace otb::Wrapper;

  const std::string module(argv[1]);

  /* TestApplication is removed in CMakeLists.txt */
#if 0
  if (module == "TestApplication")
    return EXIT_SUCCESS;
#endif

  ApplicationRegistry::AddApplicationPath(argv[2]);
  Application::Pointer appli =  ApplicationRegistry::CreateApplicationFaster(module);
  
  assert(!appli.IsNull());

  const std::string group = appli->GetDocTags().size() > 0 ? appli->GetDocTags()[0] : "";
  assert(!group.empty());

  std::map<ParameterType, std::string> parameterTypeToString;
  parameterTypeToString[ParameterType_Bool] = "QgsProcessingParameterBoolean";
  parameterTypeToString[ParameterType_Int] = "QgsProcessingParameterNumber";
  parameterTypeToString[ParameterType_Float] = "QgsProcessingParameterNumber";
  parameterTypeToString[ParameterType_RAM] = "QgsProcessingParameterNumber";
  parameterTypeToString[ParameterType_Radius] = "QgsProcessingParameterNumber";
  parameterTypeToString[ParameterType_Choice] = "OTBParameterChoice";
  parameterTypeToString[ParameterType_String] = "QgsProcessingParameterString";
  parameterTypeToString[ParameterType_InputImage] = "QgsProcessingParameterRasterLayer";
  parameterTypeToString[ParameterType_InputFilename] = "QgsProcessingParameterFile";
  parameterTypeToString[ParameterType_InputImageList] = "QgsProcessingParameterMultipleLayers";
  parameterTypeToString[ParameterType_InputVectorData] = "QgsProcessingParameterVectorLayer";
  parameterTypeToString[ParameterType_InputFilenameList] = "QgsProcessingParameterMultipleLayers";
  parameterTypeToString[ParameterType_InputVectorDataList] = "QgsProcessingParameterMultipleLayers";
  parameterTypeToString[ParameterType_OutputImage] = "QgsProcessingParameterRasterDestination";
  parameterTypeToString[ParameterType_OutputVectorData] = "QgsProcessingParameterVectorDestination";
  parameterTypeToString[ParameterType_OutputFilename] = "QgsProcessingParameterFileDestination";
  parameterTypeToString[ParameterType_Directory] = "QgsProcessingParameterFile";
  // TODO
  parameterTypeToString[ParameterType_StringList] = "QgsProcessingParameterString";
  // ListView parameters are treated as plain string (QLineEdit) in qgis processing ui.
  // This seems rather unpleasant when comparing Qgis processing with Monteverdi/Mapla in OTB
  // We tried to push something simple with checkboxes but its too risky for this version
  // and clock is ticking...

  parameterTypeToString[ParameterType_ListView] = "QgsProcessingParameterString";

  // For next update of plugin code ListView should use a custom widget wrapper and behave
  // exactly like OTB Mapla. And this #if 0 block is our TODO remainder.
  #if 0
  parameterTypeToString[ParameterType_ListView] = "OTBParameterListView";
  #endif

  const std::vector<std::string> appKeyList = appli->GetParametersKeys(true);
  const unsigned int nbOfParam = appKeyList.size();

  std::string output_file = module + ".txt";
  std::string  algs_txt = "algs.txt";
  if (argc > 3)
    {
    output_file = std::string(argv[3]) + module + ".txt";
    algs_txt = std::string(argv[3])  +  "algs.txt";
    }
  std::ofstream dFile;
  dFile.open (output_file, std::ios::out);

  std::string output_parameter_name;
  bool hasRasterOutput = false;
  {
  for (unsigned int i = 0; i < nbOfParam; i++)
    {
    Parameter::Pointer param = appli->GetParameterByKey(appKeyList[i]);
    if (param->GetMandatory())
      {
	if (appli->GetParameterType(appKeyList[i]) == ParameterType_OutputImage)
	  {
	    output_parameter_name = appKeyList[i];
	    hasRasterOutput = true;
	  }
      }
    }
  }

  if(output_parameter_name.empty())
    dFile << module << std::endl;
  else
    dFile << module << "|" <<  output_parameter_name << std::endl;
  
  dFile << appli->GetDescription() << std::endl;
  dFile << group << std::endl;
  
  for (unsigned int i = 0; i < nbOfParam; i++)
    {
    const std::string name = appKeyList[i];
    const Parameter::Pointer param = appli->GetParameterByKey(name);
    const ParameterType type = appli->GetParameterType(name);
    const std::string description = appli->GetParameterName(name);
    if (  type == ParameterType_Group  ||
          type == ParameterType_RAM ||
          param->GetRole() == Role_Output )
      {
      // group parameter cannot have any value.
      // ram is added by qgis-otb processing provider plugin as an advanced parameter for all apps
      // parameter role cannot be of type Role_Output
      continue;
      }
    auto it = parameterTypeToString.find(type);
    assert(  it != parameterTypeToString.end() );
    if( it == parameterTypeToString.end() )
      {
      std::cerr << "No mapping found for parameter '" <<name <<"' type=" << type << std::endl;
      return EXIT_FAILURE;
      }
    std::string qgis_type = it->second;
#if 0
      if (type == ParameterType_ListView)
	{
	  ListViewParameter *lv_param = dynamic_cast<ListViewParameter*>(param.GetPointer());
	  std::cerr << "lv_param->GetSingleSelection()" << lv_param->GetSingleSelection() << std::endl;
	  if (lv_param->GetSingleSelection())
	    {

	      qgis_type = "QgsProcessingParameterEnum";
	      std::vector<std::string>  key_list  = appli->GetChoiceKeys(name);
	      std::string values = "";
	      for( auto k : key_list)
		values += k + ";";
	      values.pop_back();
	      dFile << "|" << values ;
	    }
	}
#endif

    bool isDestination = false;
    bool isEpsgCode = false;

    // use QgsProcessingParameterCrs if required.
    // TODO: do a regex on name to match ==epsg || *\.epsg.\*
    if ( name == "epsg"
	      || name == "map.epsg.code"
	      || name == "mapproj.epsg.code"
	      || name == "mode.epsg.code")
        {
        qgis_type = "QgsProcessingParameterCrs";
        isEpsgCode = true;
        }

    dFile << qgis_type << "|" << name << "|" << description;

      std::string default_value = "None";
    if (type == ParameterType_Int)
      {
      if (isEpsgCode)
        {
          if (param->HasValue() && appli->GetParameterInt(name) < 1)
    	default_value =  "EPSG: " + appli->GetParameterAsString(name);
          else
    	default_value =  "ProjectCrs";
        }
      else
        {
        dFile << "|QgsProcessingParameterNumber.Integer";
        default_value = param->HasValue() ? appli->GetParameterAsString(name): "0";
        }
      }
    else if (type == ParameterType_Float)
      {	
      dFile << "|QgsProcessingParameterNumber.Double";
      default_value = param->HasValue() ? appli->GetParameterAsString(name): "0";
      }
    else if (type == ParameterType_Radius)
      {	
      dFile << "|QgsProcessingParameterNumber.Integer";
      default_value = param->HasValue() ? appli->GetParameterAsString(name): "0";
      }
    else if(type == ParameterType_InputFilename)
      {
      // TODO: if parameter InputFilename can give supported extensions
      // we can use it gitlab #1559
      dFile << "|QgsProcessingParameterFile.File|None";
      }
    else if(type == ParameterType_Directory)
      {
      dFile << "|QgsProcessingParameterFile.Folder|False";
      }      
    else if (type == ParameterType_InputImageList)
      {
      dFile << "|3"; //QgsProcessing.TypeRaster
      }
    else if (type == ParameterType_InputVectorDataList)
      {
      dFile << "|-1"; //QgsProcessing.TypeVectorAnyGeometry
      }
    else if (type == ParameterType_InputVectorData)
      {
      dFile << "|-1"; //QgsProcessing.TypeVectorAnyGeometry
      }
    else if(type == ParameterType_InputFilenameList)
      {
      dFile << "|4"; //QgsProcessing.TypeFile"
      }
    else if(type == ParameterType_String)
      {
      // Below line is interpreted in qgis processing as
      // 1. default_value = None
      // 2. multiLine = False
      // For more details,
      // please refer to documetation of QgsProcessingParameterString.
      default_value = "None|False";
      }
    else if(type == ParameterType_StringList)
      {
      // Below line is interpreted in qgis processing as
      // 1. default_value = None
      // 2. multiLine = True
      // For more details,
      // please refer to documetation of QgsProcessingParameterString.
      // setting default_value this way is an exception for ParameterType_StringList and ParameterType_String
      default_value = "None|True";
      }
    else if (type == ParameterType_InputImage)
      {
      // default is None and nothing to add to dFile
      }
    else if(type == ParameterType_ListView)
      {
      // default is None and nothing to add to dFile
      }
    else if(type == ParameterType_Bool)
      {
      default_value = appli->GetParameterAsString(name);
      }
    else if(type == ParameterType_Choice)
      {
      std::vector<std::string>  key_list  = appli->GetChoiceKeys(name);
      std::string values = "";
      for( auto k : key_list)
        values += k + ";";

  	  values.pop_back();
  	  dFile << "|" << values ;
  	  ChoiceParameter *cparam = dynamic_cast<ChoiceParameter*>(param.GetPointer());
  	  default_value = std::to_string(cparam->GetValue());	  
      }
    else if(type == ParameterType_OutputVectorData ||
	          type == ParameterType_OutputImage ||
	          type == ParameterType_OutputFilename)
      {
      // No need for default_value, optional and extra fields in dFile.
      // If parameter is a destination type. qgis_type|name|description is enough.
      // So we simply set isDestination to true and skip to end to append a new line.
      isDestination =  true;
      }
    else
      {
      std::cout << "ERROR: default_value is empty for '" << name << "' type='" << qgis_type << "'" << std::endl;
      return EXIT_FAILURE;	  
      }

    if (!isDestination)
      {
      std::string optional;
      if (param->GetMandatory())
        {
        // TODO: avoid workaround for stringlist types (fix appengine)
        // type == ParameterType_StringList check is needed because:
        // If parameter is mandatory it can have no value
        // It is accepted in OTB that, string list could be generated dynamically
        // qgis has no such option to handle dynamic values yet..
        // So mandatory parameters whose type is StringList is considered optional
        optional = param->HasValue() || type == ParameterType_StringList  ? "True" : "False";
        }
      else
        {
        optional = "True";
        }
              #if 0
    	  std::cerr << name;
    	  std::cerr << " mandatory=" << param->GetMandatory();
    	  std::cerr << " HasValue=" << param->HasValue();
    	  std::cerr << " qgis_type=" << qgis_type;
    	  std::cerr << " optional=" << optional << std::endl;
              #endif
    	dFile << "|" << default_value << "|" << optional;
      }
    dFile << std::endl;
    }

  if(hasRasterOutput)
    {
    dFile << "*QgsProcessingParameterEnum|outputpixeltype|Output pixel type|uint8;int;float;double|False|2|True" << std::endl;
    }

  std::cerr << "Writing " << output_file << std::endl;
  dFile.close();
  
  std::ofstream indexFile;
  indexFile.open (algs_txt, std::ios::out | std::ios::app );
  indexFile << group << "|" << module << std::endl;
  indexFile.close();
  std::cerr << "Updated " << algs_txt << std::endl;

  appli = nullptr;
  ApplicationRegistry::CleanRegistry();

  return EXIT_SUCCESS;
  }


