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

#include "otbWrapperChoiceParameter.h"
#include "otbWrapperListViewParameter.h"
#include "otbWrapperBoolParameter.h"
#include "otbWrapperApplicationRegistry.h"
#include "otbWrapperTypes.h"
#include "otbWrapperApplication.h"

#include <vector>
#include <string>
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
  Application::Pointer appli =  ApplicationRegistry::CreateApplicationFaster(module.c_str());
  
  assert(!appli.IsNull());
  
  appli->Init();

  std::map<ParameterType, std::string> parameterTypeToString;

  parameterTypeToString[ParameterType_Empty] = "QgsProcessingParameterBoolean";
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
  //TODO
  parameterTypeToString[ParameterType_StringList] = "QgsProcessingParameterString";
  

  const std::vector<std::string> appKeyList = appli->GetParametersKeys(true);
  const unsigned int nbOfParam = appKeyList.size();

  std::string output_file = module + ".txt";
  std::string  algs_txt = "algs.txt";
  if (argc > 3) {
   #if defined(WIN32)
   std::string output_dir = std::string(argv[3]) + "\\";
   #else
   std::string output_dir = std::string(argv[3]) + "/";
   #endif
    output_file = output_dir + module + ".txt";
    algs_txt = output_dir +  "algs.txt";
    }

  #if 0
  std::cout << "output_file: " << output_file << std::endl;
  std::cout << "algs_txt: " << algs_txt << std::endl;
  #endif
  std::ofstream dFile;
  dFile.open (output_file, std::ios::out);
  std::cerr << "Writing " << output_file << std::endl;

  std::string output_parameter_name;
  bool hasRasterOutput = false;
  for (unsigned int i = 0; i < nbOfParam; i++)
    {
      Parameter::Pointer param = appli->GetParameterByKey(appKeyList[i]);
      if (param->GetMandatory())
	{
	  ParameterType type = appli->GetParameterType(appKeyList[i]);
	  if (type == ParameterType_OutputImage )
	    {
	      output_parameter_name = appKeyList[i];
	      hasRasterOutput = true;
	    }
	}
    }

  if(output_parameter_name.empty())
    dFile << module << std::endl;
  else
    dFile << module << "|" <<  output_parameter_name << std::endl;
  
  dFile << appli->GetDescription() << std::endl;
  const std::string group = appli->GetDocTags().size() > 0 ? appli->GetDocTags()[0] : "UNCLASSIFIED";
  dFile << group << std::endl;
  
  for (unsigned int i = 0; i < nbOfParam; i++)
    {
      const std::string name = appKeyList[i];
      
      Parameter::Pointer param = appli->GetParameterByKey(name);
      ParameterType type = appli->GetParameterType(name);
      const std::string description = param->GetName();
      const std::string qgis_type = parameterTypeToString[type];


      //ListView parameters types are added as N number of QgsParameterBoolean where N being
      //number of items in a listview.
      //Example case: RadiometricIndices application
      //For next update of plugin code ListView can use custom widget wrapper and behave
      //exactly like OTB.
      if(type == ParameterType_ListView)
	{
	  ListViewParameter *lparam = dynamic_cast<ListViewParameter*>(param.GetPointer());
	  for( unsigned int c = 0; c < lparam->GetNbChoices(); ++c )	    
	    dFile << "QgsProcessingParameterBoolean|"
		  << lparam->GetChoiceKey(c)  << "|"
		  << lparam->GetChoiceName(c)
		  << "|False|True" << std::endl;
	}

      if (  type == ParameterType_Group  ||
	    type == ParameterType_OutputProcessXML  ||
	    type == ParameterType_InputProcessXML  ||
	    type == ParameterType_ListView ||
	    type == ParameterType_RAM
	    )
	{
	  // group parameter cannot have any value.
	  //outxml and inxml parameters are not relevant for QGIS and is considered a bit noisy
	  //ram is added by qgis-otb processing provider plugin as an advanced parameter for all apps
	  //listview is a special case. We indeed add them to descriptor file but with different logic.
	  continue;
	}


      assert(!qgis_type.empty());
      if(qgis_type.empty())
	{
	  std::cerr << "No mapping found for parameter '" <<name <<"' type=" << type << std::endl;
	  return EXIT_FAILURE;
	}      


      bool isDestination = false;

      dFile << qgis_type << "|" << name << "|" << description;

      std::string default_value = "None";
      if (type == ParameterType_Int)
	{ 
	  dFile << "|QgsProcessingParameterNumber.Integer";
	  default_value = param->HasValue() ? appli->GetParameterAsString(name): "0";
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
	  dFile << "|QgsProcessingParameterFile.File|txt";
	}
      else if(type == ParameterType_Directory)
	{
	  dFile << "|QgsProcessingParameterFile.Folder|False";
	}      
      else if (type == ParameterType_InputImageList)
	{
	   dFile << "|3";
	}
      else if (type == ParameterType_InputVectorDataList)
	{
	  dFile << "|-1";
	}
      else if (type == ParameterType_InputVectorData)
	{
	  dFile << "|-1";
	}
      else if(type == ParameterType_InputFilenameList)
	{
	  dFile << "|4";
	}
      else if (type == ParameterType_InputImage)
	{
	  //default is None and nothing to add to dFile
	}      
      else if(type ==ParameterType_String)
	{
	  //default is None and nothing to add to dFile
	}
      else if(type ==ParameterType_StringList)
	{
	  //default is None and nothing to add to dFile
	}      
      else if(type == ParameterType_Bool)
	{
	  //BoolParameter *bparam = dynamic_cast<BoolParameter*>(param.GetPointer());
	  //default_value = bparam->GetValueAsString();
	  default_value =  appli->GetParameterAsString(name);
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
	  //no need for default_value, optional and extra fields in dFile
	  //if parameter is a destination type. qgis_type|name|description is enough.
	  //so we simply set isDestination to true and skip to end to append a new line.
	  isDestination =  true;
	}
      else
	{
     	  std::cout << "ERROR: default_value is empty for '" << name << "' type='" << qgis_type << "'" << std::endl;
	  return EXIT_FAILURE;	  
	}

      if (!isDestination)
	{
	  //const char*  optional = param->GetMandatory() ? "False" : "True";
	  std::string optional;
	  if (param->GetMandatory())
	    if (param->HasValue())
	      {
		optional = "True";
	      }
	    else{
	      optional = "False";
	    }
	  else {
	    optional = "True";
	  }

	  dFile << "|" << default_value << "|" << optional;
	}
      dFile << std::endl;
   }
  
  dFile.close();


  
  std::ofstream indexFile;
  indexFile.open (algs_txt, std::ios::out | std::ios::app );
  indexFile << group << "|" << module << std::endl;
  indexFile.close();
  return EXIT_SUCCESS;
}


