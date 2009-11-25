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

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include <typeinfo>
#include <cassert>

#include "otbMacro.h"

#include "otbTerraSarImageMetadataInterface.h"

#include "itkMetaDataObject.h"
#include "otbVectorDataKeywordlist.h"

namespace otb
{


TerraSarImageMetadataInterface
::TerraSarImageMetadataInterface()
{
}


std::string
TerraSarImageMetadataInterface::GetSensorID( const MetaDataDictionaryType & dict ) const
{
  ImageKeywordlistType ImageKeywordlist;
  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, ImageKeywordlist);
  }
  ossimKeywordlist kwl;
  ImageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key= "sensor";
  ossimString keywordString = kwl.find(key.c_str());
  std::string output(keywordString.chars());
 
  return output;
}

bool
TerraSarImageMetadataInterface::CanRead( const MetaDataDictionaryType & dict ) const
{
  std::string sensorID = GetSensorID(dict);
  if (sensorID.find("TSX") != std::string::npos)
    return true;
  else
    return false;
}


int
TerraSarImageMetadataInterface::GetDay( const MetaDataDictionaryType & dict ) const
{
  if( !this->CanRead( dict ) )
  {
         itkExceptionMacro(<<"Invalid Metadata, no TerraSar Image");
  }
  
  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;
  key = "azimuth_start_time";
  separatorList = "-T";

  ossimString keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if(keywordStrings.size() <= 2)
    itkExceptionMacro(<<"Invalid Day");

 ossimString day = keywordStrings[2];

   return day.toInt();
}



int
TerraSarImageMetadataInterface::GetMonth( const MetaDataDictionaryType & dict ) const
{
  if( !this->CanRead( dict ) )
  {
         itkExceptionMacro(<<"Invalid Metadata, no TerraSar Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;
  key = "azimuth_start_time";
  separatorList = "-T";


  ossimString keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if(keywordStrings.size() <= 2)
    itkExceptionMacro(<<"Invalid Month");

  ossimString month = keywordStrings[1];

  return month.toInt();
}


int
TerraSarImageMetadataInterface::GetYear( const MetaDataDictionaryType & dict ) const
{
  if( !this->CanRead( dict ) )
  {
         itkExceptionMacro(<<"Invalid Metadata, no TerraSar Image");
  }
  
  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;
  key = "azimuth_start_time";
  separatorList = "-T";

  ossimString keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if(  keywordStrings.size() <= 2 )
    itkExceptionMacro("Invalid Year");

  ossimString year = keywordStrings[0];

  return year.toInt();

}

int
TerraSarImageMetadataInterface::GetHour( const MetaDataDictionaryType & dict ) const
{
  if( !this->CanRead( dict ) )
  {
         itkExceptionMacro(<<"Invalid Metadata, no TerraSar Image");
  }
  
  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;
  key = "azimuth_start_time";
  separatorList = "-T:";

  ossimString keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if(  keywordStrings.size() <= 4 )
    itkExceptionMacro("Invalid Hour");

  ossimString hour = keywordStrings[3];

  return hour.toInt();
}

int
TerraSarImageMetadataInterface::GetMinute( const MetaDataDictionaryType & dict ) const
{
  if( !this->CanRead( dict ) )
  {
         itkExceptionMacro(<<"Invalid Metadata, no TerraSar Image");
  }
 
  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;
  key = "azimuth_start_time";
  separatorList = "-T:";

  ossimString keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);
  if(  keywordStrings.size() <= 5 )
    itkExceptionMacro("Invalid Minute");

  ossimString minute = keywordStrings[4];

  return minute.toInt();
}

int
TerraSarImageMetadataInterface::GetProductionDay( const MetaDataDictionaryType & dict ) const
{
  if( !this->CanRead( dict ) )
  {
         itkExceptionMacro(<<"Invalid Metadata, no TerraSar Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;
  key = "generation_time";
  separatorList = "-T:";

  ossimString keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if(keywordStrings.size() <= 2)
    itkExceptionMacro(<<"Invalid Production Day");

  ossimString day = keywordStrings[2];

  return day.toInt();
}

int
TerraSarImageMetadataInterface::GetProductionMonth( const MetaDataDictionaryType & dict ) const
{
  if( !this->CanRead( dict ) )
  {
         itkExceptionMacro(<<"Invalid Metadata, no TerraSar Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;
  key = "generation_time";
  separatorList = "-T";

  ossimString keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if(keywordStrings.size() <= 2)
    itkExceptionMacro(<<"Invalid Production Month");

  ossimString month = keywordStrings[1];

  return month.toInt();
}


int
TerraSarImageMetadataInterface::GetProductionYear( const MetaDataDictionaryType & dict ) const
{
  if( !this->CanRead( dict ) )
  {
         itkExceptionMacro(<<"Invalid Metadata, no TerraSar Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key;
  ossimString separatorList;
  key = "generation_time";
  separatorList = "-T";

  ossimString keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  if(  keywordStrings.size() <= 2 )
    itkExceptionMacro("Invalid Production Year");

  ossimString year = keywordStrings[0];

  return year.toInt();
}

double
TerraSarImageMetadataInterface::GetCalibrationFactor( const MetaDataDictionaryType & dict ) const
{
  if( !this->CanRead( dict ) )
  {
         itkExceptionMacro(<<"Invalid Metadata, no TerraSar Image");
  }

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  std::string key = "calibration.calibrationConstant.calFactor";
  ossimString calFac = kwl.find(key.c_str());

  return calFac.toDouble();
}


} // end namespace otb
