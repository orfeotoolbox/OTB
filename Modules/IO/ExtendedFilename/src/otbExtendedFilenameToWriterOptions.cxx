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
#include "otbExtendedFilenameToWriterOptions.h"
#include "otb_boost_string_header.h"
#include <itksys/RegularExpression.hxx>
#include "otb_boost_tokenizer_header.h"

namespace otb
{

ExtendedFilenameToWriterOptions
::ExtendedFilenameToWriterOptions() : ExtendedFilenameHelper()
{
  m_Options.simpleFileName.first  = false;
  m_Options.simpleFileName.second = "";

  m_Options.writeGEOMFile.first  = false;
  m_Options.writeGEOMFile.second = true;

  m_Options.writeRPCTags.first = false;
  m_Options.writeRPCTags.second = false;
  
  m_Options.gdalCreationOptions.first = false;
  m_Options.streamingType.first       = false;
  m_Options.streamingSizeMode.first   = false;
  m_Options.streamingSizeValue.first  = false;

  m_Options.optionList.push_back("writegeom");
  m_Options.optionList.push_back("writerpctags");
  m_Options.optionList.push_back("streaming:type");
  m_Options.optionList.push_back("streaming:sizemode");
  m_Options.optionList.push_back("streaming:sizevalue");
  m_Options.optionList.push_back("box");
}

void
ExtendedFilenameToWriterOptions
::SetExtendedFileName(const char *extFname)
{
  this->Superclass::SetExtendedFileName(extFname);
  // TODO: Rename map to a less confusing (with std::map) name
  MapType map = this->GetOptionMap();

  m_Options.simpleFileName.first  = true;
  m_Options.simpleFileName.second = this->GetSimpleFileName();

  MapIteratorType it;
  for ( it=map.begin(); it != map.end(); it++ )
    {
    std::vector<std::string> tmp;
    boost::split(tmp, it->first, boost::is_any_of(":"), boost::token_compress_on);

    if (tmp.size()>2)
      if ((tmp[0]=="gdal") && (tmp[1]=="co"))
        {
        m_Options.gdalCreationOptions.first = true;
        m_Options.gdalCreationOptions.second.push_back(tmp[2] + "=" +it->second);
        }
    }

  if (!map["writegeom"].empty())
     {
     m_Options.writeGEOMFile.first = true;
     if (   map["writegeom"] == "Off"
         || map["writegeom"] == "off"
         || map["writegeom"] == "OFF"
         || map["writegeom"] == "false"
         || map["writegeom"] == "False"
         || map["writegeom"] == "0"   )
       {
       m_Options.writeGEOMFile.second = false;
       }
     }

  if (!map["writerpctags"].empty())
     {
     m_Options.writeRPCTags.first = true;
     if (   map["writerpctags"] == "On"
         || map["writerpctags"] == "on"
         || map["writerpctags"] == "ON"
         || map["writerpctags"] == "true"
         || map["writerpctags"] == "True"
         || map["writerpctags"] == "1"   )
       {
       m_Options.writeRPCTags.second = true;
       }
     }
  
  if(!map["streaming:type"].empty())
    {
    if(map["streaming:type"] == "auto"
       || map["streaming:type"] == "tiled"
       || map["streaming:type"] == "stripped"
       || map["streaming:type"] == "none")
      {
      m_Options.streamingType.first=true;
      m_Options.streamingType.second = map["streaming:type"];
      }
    else
      {
      itkWarningMacro("Unkwown value "<<map["streaming:type"]<<" for streaming:type option. Available values are auto,tiled,stripped.");
      }
    }

  if(!map["streaming:sizemode"].empty())
    {
    if(map["streaming:sizemode"] == "auto"
       || map["streaming:sizemode"] == "nbsplits"
       || map["streaming:sizemode"] == "height")
      {
      m_Options.streamingSizeMode.first=true;
      m_Options.streamingSizeMode.second = map["streaming:sizemode"];
      }
    else
      {
      itkWarningMacro("Unkwown value "<<map["streaming:sizemode"]<<" for streaming:sizemode option. Available values are auto,nbsplits,height.");
      }
    }

  if(!map["streaming:sizevalue"].empty())
    {
    m_Options.streamingSizeValue.first=true;
    m_Options.streamingSizeValue.second = atof(map["streaming:sizevalue"].c_str());
    }

  //Manage region size to write in output image
  if(!map["box"].empty())
    {
    itksys::RegularExpression reg;
    reg.compile("([^0-9\\.]:[^0-9\\.]:[^0-9\\.]:[^0-9\\.])");

    if(!reg.find(map["box"]))
      {
      m_Options.box.first=true;
      m_Options.box.second = map["box"];
      }
    else
      {
      itkWarningMacro("Unkwown value "<<map["box"]<<" for image region to write. Must be 'xmin ymin sizex sizey', with whitespace as separator");
      }
    }

  //Option Checking
  for ( it=map.begin(); it != map.end(); it++ )
    {
    std::vector<std::string> tmp;
    boost::split(tmp, it->first, boost::is_any_of(":"), boost::token_compress_on);
    if (tmp.size()>2)
      {
      if (!(tmp[0]=="gdal") || !(tmp[1]=="co"))
        {
        itkWarningMacro("Unknown option detected: " << it->first << ".");
        }
      }
    else
      {
      bool isKnown=false;
      for (unsigned int i=0; i<m_Options.optionList.size(); i++)
        {
        if (m_Options.optionList[i]==it->first)
          isKnown=true;
        }
      if(!isKnown)
        itkWarningMacro("Unknown option detected: " << it->first << ".");
      }
    }
}

bool
ExtendedFilenameToWriterOptions
::SimpleFileNameIsSet () const
{
  return m_Options.simpleFileName.first;
}

bool
ExtendedFilenameToWriterOptions
::WriteGEOMFileIsSet () const
{
  return m_Options.writeGEOMFile.first;
}

bool
ExtendedFilenameToWriterOptions
::WriteRPCTagsIsSet () const
{
  return m_Options.writeRPCTags.first;
}

bool
ExtendedFilenameToWriterOptions
::GetWriteGEOMFile () const
{
  return m_Options.writeGEOMFile.second;
}

bool
ExtendedFilenameToWriterOptions
::GetWriteRPCTags () const
{
  return m_Options.writeRPCTags.second;
}

bool
ExtendedFilenameToWriterOptions
::gdalCreationOptionsIsSet () const
{
  return m_Options.gdalCreationOptions.first;
}
ExtendedFilenameToWriterOptions
::GDALCOType
ExtendedFilenameToWriterOptions
::GetgdalCreationOptions () const
{
  return m_Options.gdalCreationOptions.second;
}

bool
ExtendedFilenameToWriterOptions
::StreamingTypeIsSet() const
{
  return m_Options.streamingType.first;
}

bool
ExtendedFilenameToWriterOptions
::StreamingSizeModeIsSet() const
{
  return m_Options.streamingSizeMode.first;
}

bool
ExtendedFilenameToWriterOptions
::StreamingSizeValueIsSet() const
{
  return m_Options.streamingSizeValue.first;
}

std::string
ExtendedFilenameToWriterOptions
::GetStreamingType() const
{
  return m_Options.streamingType.second;
}

std::string
ExtendedFilenameToWriterOptions
::GetStreamingSizeMode() const
{
  return m_Options.streamingSizeMode.second;
}

double
ExtendedFilenameToWriterOptions
::GetStreamingSizeValue() const
{
  return m_Options.streamingSizeValue.second;
}

bool
ExtendedFilenameToWriterOptions
::BoxIsSet() const
{
  return m_Options.box.first;
}

std::string
ExtendedFilenameToWriterOptions
::GetBox() const
{
  return m_Options.box.second;
}


} // end namespace otb
