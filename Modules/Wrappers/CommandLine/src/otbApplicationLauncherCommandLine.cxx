/*=========================================================================

 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/

#include "otbWrapperCommandLineLauncher.h"
#include "otb_tinyxml.h"
#include <vector>

const std::string GetChildNodeTextOf(TiXmlElement *parentElement, std::string key);
std::string PrepareExpressionFromXML(std::string filename);
std::vector<std::string> PrepareVectorExpressionFromXML(std::string filename);
std::string CleanWord(const std::string & word);


std::string PrepareExpressionFromXML(std::string filename)
{
  std::string expression;

  if(filename.empty())
    {
    std::cerr <<"Input XML Filename is empty" << std::endl;
    return expression;
    }
  std::string ext = filename.substr(filename.size()-4,filename.size());
  if(ext != ".xml" )
    std::cerr << ext  << " is a wrong extension: Expected .xml " << __FILE__ << std::endl;

  // Open the xml file
  TiXmlDocument doc;

  //Use itksys::SystemTools::FOpen() and close it below because
  //TiXmlDocument::TiXmlFileOpen( ) is not exposed from tinyXML library. Even
  //though its available in the TiXmlDocument::SaveFile().
  FILE* fp =  itksys::SystemTools::Fopen(filename.c_str(), "rb");

  if (!doc.LoadFile(fp , TIXML_ENCODING_UTF8))
    {
    std::cerr << "Can't open file " << filename << std::endl;
    fclose(fp);
    exit(1);

    }

  TiXmlHandle handle(&doc);

  TiXmlElement *n_OTB;
  n_OTB = handle.FirstChild("OTB").Element();

  if(!n_OTB)
  {
    std::string info = "Input XML file " + filename + " is invalid.";
    std::cerr << info  << std::endl;
    fclose(fp);
    exit(1);
  }

  TiXmlElement *n_AppNode   = n_OTB->FirstChildElement("application");

  std::string moduleName;
  moduleName = GetChildNodeTextOf(n_AppNode, "name");
  /*
  AddMetaData("appname", app_Name);

  app_Descr = this_->GetChildNodeTextOf(n_AppNode, "descr");
  AddMetaData("appdescr", app_Descr);

  TiXmlElement* n_Doc    = n_AppNode->FirstChildElement("doc");

  std::string doc_Name, doc_Descr, doc_Author, doc_Limitation, doc_SeeAlso;

  doc_Name = this_->GetChildNodeTextOf(n_Doc, "name");
  AddMetaData("docname", doc_Name);

  doc_Descr = this_->GetChildNodeTextOf(n_Doc, "longdescr");
  AddMetaData("doclongdescr", doc_Descr);

  doc_Author = this_->GetChildNodeTextOf(n_Doc, "authors");
  AddMetaData("docauthors", doc_Author);

  doc_Limitation = this_->GetChildNodeTextOf(n_Doc, "limitations");
  AddMetaData("doclimitations", doc_Limitation);

  doc_SeeAlso = this_->GetChildNodeTextOf(n_Doc, "seealso");
  AddMetaData("docseealso", doc_SeeAlso);
  */

  expression.append(moduleName);

  for( TiXmlElement* n_Parameter = n_AppNode->FirstChildElement("parameter"); n_Parameter != NULL;
       n_Parameter = n_Parameter->NextSiblingElement() )
    {
    std::string key="-";
    key.append(GetChildNodeTextOf(n_Parameter, "key"));

    TiXmlElement* n_Values = NULL;
    n_Values = n_Parameter->FirstChildElement("values");
    if(n_Values)
      {
      std::string values;
      for(TiXmlElement* n_Value = n_Values->FirstChildElement("value"); n_Value != NULL;
          n_Value = n_Value->NextSiblingElement())
        {
        values.append(n_Value->GetText());
        values.append(" ");
        }
      values = values.substr(0,values.size()-1);
      expression.append(" ");
      expression.append(key);
      expression.append(" ");
      expression.append(values);
      }
    else
      {
      std::string value;
      value = GetChildNodeTextOf(n_Parameter, "value");

      expression.append(" ");
      expression.append(key);
      expression.append(" ");
      expression.append(value);

      std::string type = GetChildNodeTextOf(n_Parameter, "type");
      if (type == "OutputImage")
        {
        std::string t = GetChildNodeTextOf(n_Parameter, "pixtype");
        expression.append(" ");
        expression.append(t);
        }
      }
    }

  fclose(fp);

  return expression;
}

std::vector<std::string> PrepareVectorExpressionFromXML(std::string filename)
{
  std::vector<std::string> expression;

  if(filename.empty())
    {
    std::cerr <<"Input XML Filename is empty" << std::endl;
    return expression;
    }
  std::string ext = filename.substr(filename.size()-4,filename.size());
  if(ext != ".xml" )
    std::cerr << ext  << " is a wrong extension: Expected .xml " << __FILE__ << std::endl;

  // Open the xml file
  TiXmlDocument doc;

  //Use itksys::SystemTools::FOpen() and close it below because
  //TiXmlDocument::TiXmlFileOpen( ) is not exposed from tinyXML library. Even
  //though its available in the TiXmlDocument::SaveFile().
  FILE* fp =  itksys::SystemTools::Fopen(filename.c_str(), "rb");

  if (!doc.LoadFile(fp , TIXML_ENCODING_UTF8))
    {
    std::cerr << "Can't open file " << filename << std::endl;
    fclose(fp);
    exit(1);

    }

  TiXmlHandle handle(&doc);

  TiXmlElement *n_OTB;
  n_OTB = handle.FirstChild("OTB").Element();

  if(!n_OTB)
  {
    std::string info = "Input XML file " + filename + " is invalid.";
    std::cerr << info  << std::endl;
    fclose(fp);
    exit(1);
  }

  TiXmlElement *n_AppNode   = n_OTB->FirstChildElement("application");

  std::string moduleName;
  moduleName = GetChildNodeTextOf(n_AppNode, "name");

  expression.push_back(CleanWord(moduleName));

  for( TiXmlElement* n_Parameter = n_AppNode->FirstChildElement("parameter"); n_Parameter != NULL;
       n_Parameter = n_Parameter->NextSiblingElement() )
    {
    std::string key="-";
    key.append(GetChildNodeTextOf(n_Parameter, "key"));
    expression.push_back(CleanWord(key));

    TiXmlElement* n_Values = NULL;
    n_Values = n_Parameter->FirstChildElement("values");
    if(n_Values)
      {
      std::string values;
      for(TiXmlElement* n_Value = n_Values->FirstChildElement("value"); n_Value != NULL;
          n_Value = n_Value->NextSiblingElement())
        {
        expression.push_back(CleanWord(n_Value->GetText()));
        }
      }
    else
      {
      std::string value;
      value = GetChildNodeTextOf(n_Parameter, "value");
      expression.push_back(CleanWord(value));

      std::string type = GetChildNodeTextOf(n_Parameter, "type");
      if (type == "OutputImage")
        {
        std::string t = GetChildNodeTextOf(n_Parameter, "pixtype");
        expression.push_back(CleanWord(t));
        }
      }
    }

  fclose(fp);

  return expression;
}

std::string CleanWord(const std::string & word)
{
  std::string res("");
  // Suppress whitespace characters at the beginning and ending of the string
  std::string::size_type cleanStart = word.find_first_not_of(" \t");
  std::string::size_type cleanEnd = word.find_last_not_of(" \t\f\v\n\r");
  // cleanStart == npos implies cleanEnd == npos
  if (cleanEnd != std::string::npos)
    {
    res = word.substr(cleanStart, cleanEnd - cleanStart + 1);
    }
  return res;
}

int main(int argc, char* argv[])
{
  if (argc < 2)
    {
    std::cerr << "Usage : " << argv[0] << " module_name [MODULEPATH] [arguments]" << std::endl;
    return EXIT_FAILURE;
    }

  std::vector<std::string> vexp;

  std::string exp;
  if (strcmp(argv[1], "-inxml") == 0)
    {
    //exp = PrepareExpressionFromXML(argv[2]);
    vexp = PrepareVectorExpressionFromXML(argv[2]);
    }
  else
    {
    // Construct the string expression
    for (int i = 1; i < argc; i++)
      {
      /*if (i != argc - 1)
        {
        exp.append(argv[i]);
        exp.append(" ");
        }
      else
        {
        exp.append(argv[i]);
        }*/
      std::string strarg(argv[i]);
      std::string cleanArg = CleanWord(strarg);
      if (cleanArg.empty())
        {
        // Empty argument !
        continue;
        }
      vexp.push_back(cleanArg);
      }
    }
  //  std::cerr << exp << ":\n";

  typedef otb::Wrapper::CommandLineLauncher LauncherType;
  LauncherType::Pointer launcher = LauncherType::New();

  //if (launcher->Load(exp) == true)
    if (launcher->Load(vexp) == true)
    {
    if (launcher->ExecuteAndWriteOutput() == false)
      {
      return EXIT_FAILURE;
      }
    }
  else
    {
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

const std::string GetChildNodeTextOf(TiXmlElement *parentElement, std::string key)
{
  std::string value="";

  if(parentElement)
    {
    TiXmlElement* childElement = 0;
    childElement = parentElement->FirstChildElement(key.c_str());

    //same as childElement->GetText() does but that call is failing if there is
    //no such node.
    //but the below code works and is a replacement for GetText()
    if(childElement)
      {
      const TiXmlNode* child = childElement->FirstChild();
      if ( child )
        {
        const TiXmlText* childText = child->ToText();
        if ( childText )
          {
          value = childText->Value();
          }
        }
      }
    }
  return value;
}
