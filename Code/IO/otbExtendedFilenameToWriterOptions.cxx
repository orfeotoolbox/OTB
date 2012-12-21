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
#include <boost/algorithm/string.hpp>

namespace otb
{

ExtendedFilenameToWriterOptions
::ExtendedFilenameToWriterOptions() : itk::Object()
{
  m_FilenameHelper = FNameHelperType::New();

  m_Options.simpleFileName.first  = false;
  m_Options.simpleFileName.second = "";

  m_Options.writeGEOMFile.first  = false;
  m_Options.writeGEOMFile.second = true;

  m_Options.gdalCreationOptions.first  = false;

  m_Options.optionList.push_back("writegeom");
}

void
ExtendedFilenameToWriterOptions
::SetExtendedFileName(const char *extFname)
{
  this->m_FilenameHelper->SetExtendedFileName(extFname);
  MapType map = this->m_FilenameHelper->GetOptionMap();

  m_Options.simpleFileName.first  = true;
  m_Options.simpleFileName.second = this->m_FilenameHelper->GetSimpleFileName();

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
const char*
ExtendedFilenameToWriterOptions
::GetSimpleFileName () const
{
  return m_Options.simpleFileName.second.c_str();
}

bool
ExtendedFilenameToWriterOptions
::WriteGEOMFileIsSet () const
{
  return m_Options.writeGEOMFile.first;
}
bool
ExtendedFilenameToWriterOptions
::GetWriteGEOMFile () const
{
  return m_Options.writeGEOMFile.second;
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

} // end namespace otb
