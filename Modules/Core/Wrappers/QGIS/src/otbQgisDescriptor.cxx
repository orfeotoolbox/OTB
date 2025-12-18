/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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
#include "otbWrapperFieldParameter.h"
#include "otbWrapperBandParameter.h"
#include "otbWrapperApplicationRegistry.h"

#include <iostream>
#include <cassert>
#include <fstream>
#include <regex>

#define QGIS_DEBUG 0

using namespace otb::Wrapper;

std::string getMandatoryOutputImageParam(const Application::Pointer app,
                                         const std::vector<std::string>& app_keys) {
  for (const std::string& app_key: app_keys)
  {
    Parameter::Pointer param = app->GetParameterByKey(app_key);
    if (param->GetMandatory() && param->GetType() == ParameterType_OutputImage)
    {
      return app_key;
    }
  }
  return ""; // if nothing found
}

std::string QGISLineFactory(const Application::Pointer app,
                            const std::string& paramKey)
{
  static const std::unordered_map<ParameterType, std::string> parameterTypeToString {
    {ParameterType_Bool, "QgsProcessingParameterBoolean"},
    {ParameterType_Int, "QgsProcessingParameterNumber"},
    {ParameterType_Float, "QgsProcessingParameterNumber"},
    {ParameterType_Double, "QgsProcessingParameterNumber"},
    {ParameterType_RAM, "QgsProcessingParameterNumber"},
    {ParameterType_Radius, "QgsProcessingParameterNumber"},
    {ParameterType_Choice, "OTBParameterChoice"},
    {ParameterType_String, "QgsProcessingParameterString"},
    {ParameterType_InputImage, "QgsProcessingParameterRasterLayer"},
    {ParameterType_InputFilename, "QgsProcessingParameterFile"},
    {ParameterType_InputImageList, "QgsProcessingParameterMultipleLayers"},
    {ParameterType_InputVectorData, "QgsProcessingParameterVectorLayer"},
    {ParameterType_InputFilenameList, "QgsProcessingParameterMultipleLayers"},
    {ParameterType_InputVectorDataList, "QgsProcessingParameterMultipleLayers"},
    {ParameterType_OutputImage, "QgsProcessingParameterRasterDestination"},
    {ParameterType_OutputVectorData, "QgsProcessingParameterVectorDestination"},
    {ParameterType_OutputFilename, "QgsProcessingParameterFileDestination"},
    {ParameterType_Directory, "QgsProcessingParameterFile"},
    {ParameterType_Field, "QgsProcessingParameterField"},
    {ParameterType_Band, "QgsProcessingParameterBand"},
    {ParameterType_StringList, "QgsProcessingParameterString"},
    {ParameterType_ListView, "QgsProcessingParameterEnum"}
  };

  enum FilterType {
    NONE = 0b00000000,
    STRING = 0b00000001,
    NUMERIC = 0b00000010,
    MIXED = 0b00000011
  };


  std::ostringstream qgis_line;
  const Parameter::Pointer param         (app->GetParameterByKey(paramKey));
  ParameterType            type          (param->GetType());
  const std::string        description   (app->GetParameterName(paramKey));
  std::string              optional      ("True");
  std::string              default_value ("None");


  if (type == ParameterType_Group || type == ParameterType_RAM || param->GetRole() == Role_Output)
  {
    // group parameter cannot have any value.
    // ram is added by qgis-otb processing provider plugin as an advanced parameter for all apps
    // parameter role cannot be of type Role_Output
    return "";
  }

  // if there is no choice value for a list view parameter
  // we switch to parameter stringlist to avoid aving empty combobox in QGIS
  if (type == ParameterType_ListView && app->GetChoiceNames(paramKey).empty()) {
    type = ParameterType_StringList;
  }


  auto it = parameterTypeToString.find(type);
  if (it == parameterTypeToString.end())
  {
    // NOTE get also module name
    throw std::runtime_error("No mapping found for parameter '" + paramKey +
                             "' type=" + parameterTypesStrings[type]);
  }
  
  std::string qgis_type = it->second;

  if (param->GetMandatory())
  {
    // TODO: avoid workaround for stringlist types (fix appengine)
    // type == ParameterType_StringList check is needed because:
    // If parameter is  mandatory it can have no value
    // It is accepted in OTB that, string list could be generated dynamically
    // qgis has no such option to handle dynamic values yet..
    // So mandatory parameters whose type is StringList is considered optional
    optional = param->HasValue() || type == ParameterType_StringList ? "True" : "False";
  }


  bool isEpsgCode = false;

  // use QgsProcessingParameterCrs if required.
  // Check if paramKey == "epsg" only or epsg is one of the extensions
  // use double \ to get one in final expression
  const std::regex epsg_regex(".+\\.epsg\\..+");
  if (paramKey == "epsg" || std::regex_match(paramKey, epsg_regex)) {
    qgis_type  = "QgsProcessingParameterCrs";
    isEpsgCode = true;
  }

  qgis_line << qgis_type << "|" << paramKey << "|" << description;

  int filterType;
  std::vector<std::string> name_list;
  std::vector<std::string> key_list;
  std::string values;
  std::string proc_param_field;
  ListViewParameter *lv_param;
  ChoiceParameter* choiceParam;
  FieldParameter* fieldParam;
  BandParameter* bandParam;

  switch (type)
  {
  case ParameterType_Int:
    if (isEpsgCode)
    {
      if (param->HasValue() && app->GetParameterInt(paramKey) < 1)
        default_value = "EPSG: " + app->GetParameterAsString(paramKey);
      else
        default_value = "ProjectCrs";
    }
    else
    {
      qgis_line << "|QgsProcessingParameterNumber.Integer";
      default_value = param->HasValue() ? app->GetParameterAsString(paramKey) : "0";
    }
    break;
  case ParameterType_Float:
  case ParameterType_Double:
    qgis_line << "|QgsProcessingParameterNumber.Double";
    default_value = param->HasValue() ? app->GetParameterAsString(paramKey) : "0";
    break;
  case ParameterType_Radius:
    qgis_line << "|QgsProcessingParameterNumber.Integer";
    default_value = param->HasValue() ? app->GetParameterAsString(paramKey) : "0";
    break;
  case ParameterType_InputFilename:
    // TODO: if parameter InputFilename can give supported extensions
    // we can use it gitlab #1559
    qgis_line << "|QgsProcessingParameterFile.File|None";
    break;
  case ParameterType_Directory:
    qgis_line << "|QgsProcessingParameterFile.Folder|False";
    break;
  case ParameterType_InputImageList:
    qgis_line << "|3"; // QgsProcessing.TypeRaster
    break;
  case ParameterType_InputVectorDataList:
  case ParameterType_InputVectorData:
  case ParameterType_OutputVectorData:
    qgis_line << "|-1"; // QgsProcessing.TypeVectorAnyGeometry
    break;
  case ParameterType_InputFilenameList:
    qgis_line << "|4"; // QgsProcessing.TypeFile"
    break;
  case ParameterType_String:
    // Below line is interpreted in qgis processing as
    // 1. default_value = None
    // 2. multiLine = False
    // For more details,
    // please refer to documentation of QgsProcessingParameterString.
    default_value = "None|False";
    break;
  case ParameterType_StringList:
    // Below line is interpreted in qgis processing as
    // 1. default_value = None
    // 2. multiLine = True
    // For more details,
    // please refer to documentation of QgsProcessingParameterString.
    // setting default_value this way is an exception for ParameterType_StringList and ParameterType_String
    default_value = "None|True";
    break;
  case ParameterType_InputImage:
  case ParameterType_OutputImage:
    // default is None and nothing to add to qgis_line
    break;
  case ParameterType_OutputFilename:
    // fileFilter and defaultValue is None
    // OTB does not have any implementation for file extension.
    default_value = "None|None";
    break;
  case ParameterType_ListView:
    lv_param = dynamic_cast<ListViewParameter*>(param.GetPointer());
    name_list  = app->GetChoiceNames(paramKey);
    for( auto k : name_list)
      values += k + ";";
    values.pop_back();
    qgis_line << "|" << values ;
    qgis_line << "|" << (lv_param->GetSingleSelection() ? "False" : "True");
    break;
  case ParameterType_Bool:
    default_value = app->GetParameterAsString(paramKey);
    break;
  case ParameterType_Choice:
    key_list = app->GetChoiceKeys(paramKey);
    for (auto k : key_list)
      values += k + ";";

    if (!values.empty())
      values.pop_back();

    qgis_line << "|" << values;
    choiceParam = dynamic_cast<ChoiceParameter*>(param.GetPointer());
    default_value           = std::to_string(choiceParam->GetValue());
    break;
  case ParameterType_Field:
    fieldParam = dynamic_cast<FieldParameter*>(param.GetPointer());
    filterType = FilterType::NONE;

    for (auto type : fieldParam->GetTypeFilter())
    {
      if (type == OFTString)
      filterType |= FilterType::STRING;
      else if (type == OFTInteger || type == OFTInteger64 || type == OFTReal)
      filterType |= FilterType::NUMERIC;
    }

    if (filterType == FilterType::NONE || filterType == FilterType::MIXED)
      proc_param_field = "QgsProcessingParameterField.Any";
    else if (filterType == FilterType::STRING)
      proc_param_field = "QgsProcessingParameterField.String";
    else if (filterType == FilterType::NUMERIC)
      proc_param_field = "QgsProcessingParameterField.Numeric";

    qgis_line << "|None|"
          << fieldParam->GetVectorData()
          << "|" << proc_param_field
          << "|" << (fieldParam->GetSingleSelection() ? "False" : "True")
          << "|" << optional;
    break;
  case ParameterType_Band:
    bandParam = dynamic_cast<BandParameter*>(param.GetPointer());

    qgis_line << "|None|"
          << bandParam->GetRasterData() << "|"
          << optional
          << "|" << (bandParam->GetSingleSelection() ? "False" : "True");
    break;
  default:
    throw std::runtime_error("ERROR: default_value is empty for '" + paramKey +
                             "' type='" + qgis_type  + "'");
    break;
  }

#if QGIS_DEBUG
      std::cerr << paramKey;
      std::cerr << " mandatory=" << param->GetMandatory();
      std::cerr << " HasValue=" << param->HasValue();
      std::cerr << " qgis_type=" << qgis_type;
      std::cerr << " optional=" << optional << std::endl;
#endif

  // optional and default value are not in the same order than
  // other QGis processing parameters for field and band
  if (type != ParameterType_Field && type != ParameterType_Band)
  {
    qgis_line << "|" << default_value << "|" << optional;
  }

  return qgis_line.str();
}

void writeModuleQGISDescriptor(const Application::Pointer app,
                               const std::string& module_name,
                               const std::string& group,
                               const std::string& descriptor_file_path) {
  // RAII capsule to automatically close file when we are out of scope
  // as this function is able to throw, we must ensure that the file is
  // correctly closed
  auto module_descriptor = std::unique_ptr<std::ofstream, std::function<void(std::ofstream*)>>(
                              new std::ofstream(descriptor_file_path, std::ios::out),
                              [](std::ofstream* o) { o->close(); }
                           );

  const std::vector<std::string> appKeyList = app->GetParametersKeys(true);

  // --------------QGIS Module file header-----------------
  // composed like following (line break included):
  // <module name>|<mandatory output param name (if exists)>
  // <description>
  // <group>
  const std::string mandatory_out_param_name = getMandatoryOutputImageParam(app, appKeyList);

  // Add app name and mandatory output if it exists
  if (mandatory_out_param_name.empty())
    *module_descriptor << module_name << std::endl;
  else
    *module_descriptor << module_name << "|" << mandatory_out_param_name << std::endl;

  // add description and group
  *module_descriptor << app->GetDescription() << std::endl;
  *module_descriptor << group << std::endl;

  //-------------QGIS Module parameters part----------------

  /*
  From here onwards each line appended to module_descriptor is passed to QgsProcessingParameter* class constructor.
  module_descriptor is nothing but a csv (with a .txt) with "|" as delimiter character.
  First field is the name of Qgis parameter class and rest of it are extracted as list and passed to class constructor.
  Parsing and calling of parameter classes are done by python.
  source available : qgis/python/plugins/processing/core/parameters.py
  source code of qgis parameter is available at: qgis/src/core/processing/qgsprocessingparameters.cpp
  */
  std::string param_line;
  for (const std::string& paramKey: appKeyList) {
    // the following function can throw, but let is thow upper
    param_line = QGISLineFactory(app, paramKey);
    if (!param_line.empty())
      *module_descriptor << param_line << std::endl;
  }

  // if there is a mandatory output, add a parameter to specify the type of
  // output pixels
  if (!mandatory_out_param_name.empty())
  {
    *module_descriptor << "*QgsProcessingParameterEnum|outputpixeltype|Output pixel type|uint8;int;float;double|False|2|True" << std::endl;
  }
}


int main(int argc, char* argv[])
{
  // ------- Args parsing --------
  if (argc < 3)
  {
    std::cerr << "Usage : " << argv[0] << " module_list_file OTB_APPLICATION_PATH [out_dir]" << std::endl;
    return EXIT_FAILURE;
  }

  const std::string module_list_path(argv[1]);
  ApplicationRegistry::AddApplicationPath(argv[2]);

  std::string work_dir("");
  std::string output_file = module_list_path + ".txt";
  if (argc > 3)
  {
    work_dir = std::string(argv[3]) + "/";
  }
  // ------------------------

  // get the list of all modules to generate their descriptors
  std::ifstream module_list_file(module_list_path, std::ios_base::in);
  const std::string algs_txt    = work_dir + "algs.txt";

  // get the algs_txt that contains modules and their groups
  std::ofstream algs_list;
  algs_list.open(algs_txt, std::ios::out | std::ios::app);

  std::string module_name;

  // for all modules list, write the detailled QGIS descriptor and add it
  // in the algs.txt list with its group
  while (std::getline(module_list_file, module_name)) {
    bool descriptor_written = false;
    Application::Pointer appli = ApplicationRegistry::
                                  CreateApplicationFaster(module_name);
    assert(!appli.IsNull());
    const std::string group = appli->GetDocTags().size() > 0 ?
                              appli->GetDocTags()[0] : "";
    assert(!group.empty());

    std::string qgis_desc_path = work_dir + module_name + ".txt";

    // write detailled module desc
    try {
      writeModuleQGISDescriptor(appli, module_name, group, qgis_desc_path);
      descriptor_written = true;
    }
    catch (const std::runtime_error& e) {
      std::cerr << "Error while writing " << qgis_desc_path << ": " << e.what()
                << std::endl;
    }

    // add it in list
    if (descriptor_written) {
      algs_list << group << "|" << module_name << std::endl;
    }

    appli = nullptr;
  }

  algs_list.close();
  module_list_file.close();
  ApplicationRegistry::CleanRegistry();

  return EXIT_SUCCESS;
}
