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


#include "otbFuzzyDescriptorsModelManager.h"
#include "itkMacro.h"
#include "itksys/SystemTools.hxx"
#include "otb_tinyxml.h"

namespace otb
{


FuzzyDescriptorsModelManager::FuzzyDescriptorsModelManager()
{
}

FuzzyDescriptorsModelManager::PairType FuzzyDescriptorsModelManager::GetDescriptor(const char* model, const DescriptorsModelType& descModel)
{
  PairType                             out;
  bool                                 found = false;
  DescriptorsModelType::const_iterator it    = descModel.begin();

  while (it != descModel.end() && found == false)
  {
    if ((*it).first == model)
    {
      found = true;
      out   = (*it);
    }
    ++it;
  }

  if (found == false)
  {
    itkGenericExceptionMacro(<< "No models named " << model << " found...");
  }

  return out;
}


void FuzzyDescriptorsModelManager::AddDescriptor(PairType modelPair, DescriptorsModelType& descModel)
{
  descModel.push_back(modelPair);
}


void FuzzyDescriptorsModelManager::AddDescriptor(std::string key, std::vector<double> model, DescriptorsModelType& descModel)
{
  PairType lPair(key, model);
  descModel.push_back(lPair);
}

void FuzzyDescriptorsModelManager::Print(const DescriptorsModelType& descModel)
{
  for (unsigned int i = 0; i < descModel.size(); ++i)
  {
    std::cout << "Descriptor " << descModel[i].first << " parameters:" << std::endl;
    const ParameterType& param = descModel[i].second;

    for (unsigned int j = 0; j < param.size(); ++j)
    {
      std::cout << param[j] << "  ";
    }
    std::cout << std::endl;
  }
}

FuzzyDescriptorsModelManager::DescriptorsModelType FuzzyDescriptorsModelManager::Read(const std::string& filename)
{
  // Check if the filename is not empty
  if (filename.empty() == true)
  {
    itkGenericExceptionMacro(<< "The XML output FileName is empty");
  }

  // Check that the right extension is given : expected .xml */
  if (itksys::SystemTools::GetFilenameLastExtension(filename) != ".xml")
  {
    itkGenericExceptionMacro(<< itksys::SystemTools::GetFilenameLastExtension(filename) << " is a wrong Extension FileName : Expected .xml");
  }

  // Open the xml file
  TiXmlDocument doc(filename.c_str());
  if (!doc.LoadFile())
  {
    itkGenericExceptionMacro(<< "Can't open file " << filename);
  }

  DescriptorsModelType descModel;

  TiXmlHandle hDoc(&doc);
  TiXmlHandle root = hDoc.FirstChildElement("FuzzydDescriptorsModel");

  // Iterate through the tree to get all the stats
  for (TiXmlElement* currentStat = root.FirstChildElement().ToElement(); currentStat != nullptr; currentStat = currentStat->NextSiblingElement())
  {
    PairType currentDescriptor;
    // Store the descriptor type name
    currentDescriptor.first = currentStat->Attribute("name");

    // The size is not stored in the XML file
    // Store the value in a std::vector, get the size and then
    // build a measurement vector
    ParameterType param;

    for (TiXmlElement* sample = currentStat->FirstChildElement("Parameter"); sample != nullptr; sample = sample->NextSiblingElement())
    {
      // Get the current value of the descriptor
      double value;
      sample->QueryDoubleAttribute("value", &value);
      // Store the value
      param.push_back(value);
    }

    // Add descriptor parameters
    currentDescriptor.second = param;

    // Add the pai to the model
    descModel.push_back(currentDescriptor);
  }

  return descModel;
}


void FuzzyDescriptorsModelManager::Save(const std::string& filename, DescriptorsModelType& model)
{
  // Check if the filename is not empty
  if (filename.empty() == true)
  {
    itkGenericExceptionMacro(<< "The XML output FileName is empty");
  }

  // Check that the right extension is given : expected .xml */
  if (itksys::SystemTools::GetFilenameLastExtension(filename) != ".xml")
  {
    itkGenericExceptionMacro(<< itksys::SystemTools::GetFilenameLastExtension(filename) << " is a wrong Extension FileName : Expected .xml");
  }

  // Write the XML file
  TiXmlDocument doc;

  TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
  doc.LinkEndChild(decl);

  TiXmlElement* root = new TiXmlElement("FuzzydDescriptorsModel");
  doc.LinkEndChild(root);

  // Iterate through the input
  for (unsigned int i = 0; i < model.size(); ++i)
  {
    // The current statistic
    TiXmlElement* desc = new TiXmlElement("Descriptor");
    desc->SetAttribute("name", model[i].first.c_str());
    root->LinkEndChild(desc);

    ParameterType param = model[i].second;
    // Store the value for this statistic
    for (unsigned int cindex = 0; cindex < param.size(); ++cindex)
    {
      // For each value in Measurementvector
      TiXmlElement* curStatisticVector = new TiXmlElement("Parameter");
      curStatisticVector->SetDoubleAttribute("value", param[cindex]);
      desc->LinkEndChild(curStatisticVector);
    }
  }

  // Finally, write the file
  doc.SaveFile(filename.c_str());
}

FuzzyDescriptorsModelManager::DescriptorListType FuzzyDescriptorsModelManager::GetDescriptorList(const DescriptorsModelType& descModel)
{
  DescriptorListType out;

  DescriptorsModelType::const_iterator it = descModel.begin();

  while (it != descModel.end())
  {
    out.push_back((*it).first);
    ++it;
  }

  return out;
}

} // End namespace otb
