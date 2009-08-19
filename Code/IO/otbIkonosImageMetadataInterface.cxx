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

#include "otbIkonosImageMetadataInterface.h"
#include "itkMetaDataObject.h"

namespace otb
{


IkonosImageMetadataInterface
::IkonosImageMetadataInterface()
{
}

bool
IkonosImageMetadataInterface::CanRead( const MetaDataDictionaryType & dict ) const
{
  std::string sensorID = GetSensorID(dict);
  if (sensorID.find("IKONOS-2") != std::string::npos)
    return true;
  else
    return false;
}

IkonosImageMetadataInterface::VariableLengthVectorType
IkonosImageMetadataInterface::GetSolarIrradiance( const MetaDataDictionaryType & dict ) const
{
  if( !this->CanRead( dict ) )
  {
  	itkExceptionMacro(<<"Invalid Metadata, no Ikonos Image");
  }
  
  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

 VariableLengthVectorType outputValuesVariableLengthVector;
 ossimKeywordlist kwl;
 imageKeywordlist.convertToOSSIMKeywordlist(kwl);

 outputValuesVariableLengthVector.SetSize(1);
 // values from geoeye
 std::string key= "support_data.band_name";
 ossimString keywordString = kwl.find(key.c_str());
 if(keywordString=="Pan")
	{
	  outputValuesVariableLengthVector[0] = 1375.8;
	}
      else if(keywordString=="Blue")
	{
	  outputValuesVariableLengthVector[0] = 1930.9;
	}
      else if(keywordString=="Green")
	{
	  outputValuesVariableLengthVector[0] = 1854.8;
	}
      else if(keywordString=="Red")
	{
	  outputValuesVariableLengthVector[0] = 1556.5;
	}
      else if(keywordString=="NIR")
	{
	  outputValuesVariableLengthVector[0] = 1156.9;
	}
     
  return outputValuesVariableLengthVector;
}


int
IkonosImageMetadataInterface::GetDay( const MetaDataDictionaryType & dict ) const
{
  if( !this->CanRead( dict ) )
  {
  	itkExceptionMacro(<<"Invalid Metadata, no Ikonos Image");
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
 
  key = "support_data.production_date";
  separatorList = "/";
 
  ossimString keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);
 
  if(keywordStrings.size() <= 2)
    itkExceptionMacro(<<"Invalid Day");

 ossimString day = keywordStrings[2];
 // MM/DD/YY
 day = keywordStrings[1];

 return day.toInt();
}


int
IkonosImageMetadataInterface::GetMonth( const MetaDataDictionaryType & dict ) const
{
  if( !this->CanRead( dict ) )
  {
  	itkExceptionMacro(<<"Invalid Metadata, no Ikonos Image");
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

  key = "support_data.production_date";
  separatorList = "/";
  ossimString keywordString = kwl.find(key.c_str());
  //ossimString separatorList = "-T";
  //std::string key= "support_data.image_date";
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);

  //assert(keywordStrings.size() > 2);
  if(keywordStrings.size() <= 2)
    itkExceptionMacro(<<"Invalid Month");

  ossimString month = keywordStrings[1];
  month = keywordStrings[0];

  return month.toInt();
}

int
IkonosImageMetadataInterface::GetYear( const MetaDataDictionaryType & dict ) const
{
  if( !this->CanRead( dict ) )
  {
  	itkExceptionMacro(<<"Invalid Metadata, no Ikonos Image");
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

  key = "support_data.production_date";
  separatorList = "/";

  ossimString keywordString = kwl.find(key.c_str());
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);
  //assert(keywordStrings.size() > 2);
  if(  keywordStrings.size() <= 2 )
    itkExceptionMacro("Invalid Year");

  ossimString year = keywordStrings[0];
  // For Ikonos 2002 is 02

  year = keywordStrings[2];
  year = "20"+year;

  return year.toInt();
}


IkonosImageMetadataInterface::VariableLengthVectorType
IkonosImageMetadataInterface
::GetPhysicalBias( const MetaDataDictionaryType & dict ) const
{
  if( !this->CanRead( dict ) )
  {
  	itkExceptionMacro(<<"Invalid Metadata, no Ikonos Image");
  }
  
  VariableLengthVectorType outputValuesVariableLengthVector;
  outputValuesVariableLengthVector.SetSize(GetNumberOfBands(dict));
  outputValuesVariableLengthVector.Fill(0.0);

  return outputValuesVariableLengthVector;
}


IkonosImageMetadataInterface::VariableLengthVectorType
IkonosImageMetadataInterface
::GetPhysicalGain( const MetaDataDictionaryType & dict ) const
{
  if( !this->CanRead( dict ) )
  {
  	itkExceptionMacro(<<"Invalid Metadata, no Ikonos Image");
  }
  
  ImageKeywordlistType ImageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, ImageKeywordlist);
  }
  ossimKeywordlist kwl;
  ImageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key= "support_data.production_date";
  ossimString keywordString = kwl.find(key.c_str());
  std::string output(keywordString.chars());

  //The Ikonos production date has the format MM/DD/YY
  ossimString separatorList = "/";
  std::vector<ossimString> keywordStrings = keywordString.split(separatorList);
  if (keywordStrings.size() < 3)
  {
    itkGenericExceptionMacro(<<"Could not retrieve the production date for Ikonos");
  }

  int productionYear = keywordStrings[2].toInt();
  int productionMonth = keywordStrings[0].toInt();
  int productionDay = keywordStrings[1].toInt();
  bool isPost20010122 = false;
  if ((productionYear > 2) || (productionYear < 99)) isPost20010122 = true;
  else
  {
    if (productionYear == 2)
    {
      if (productionMonth > 1) isPost20010122 = true;
      else
        if (productionDay >= 22) isPost20010122 = true;
    }
  }

    //Value computed from
  // http://www.geoeye.com/CorpSite/assets/docs/technical-papers/2009/IKONOS_Esun_Calculations.pdf
  // to get the equivalent of the SPOT alpha
  VariableLengthVectorType gain;
  gain.SetSize(5);
  if (isPost20010122)
  {
    gain[0] = 6.48830;//Pan
    gain[1] = 5.19064;//Blue
    gain[2] = 6.44122;//Green
    gain[3] = 6.24442;//Red
    gain[4] = 8.04222;//NIR
  }
  else
  {
    gain[0] = 6.48830;//Pan
    gain[1] = 4.51329;//Blue
    gain[2] = 5.75014;//Green
    gain[3] = 5.52720;//Red
    gain[4] = 7.11684;//NIR
  }


  std::vector<std::string> bandName = GetBandName(dict);

  VariableLengthVectorType outputValuesVariableLengthVector;
  unsigned int numBands = GetNumberOfBands(dict);
  outputValuesVariableLengthVector.SetSize(numBands);
  for(unsigned int i=0; i<numBands; ++i)
  {
    if (bandName[i].find("Pan") != std::string::npos)
      outputValuesVariableLengthVector[i]=gain[0];
    if (bandName[i].find("Blue") != std::string::npos)
      outputValuesVariableLengthVector[i]=gain[1];
    if (bandName[i].find("Green") != std::string::npos)
      outputValuesVariableLengthVector[i]=gain[2];
    if (bandName[i].find("Red") != std::string::npos)
      outputValuesVariableLengthVector[i]=gain[3];
    if (bandName[i].find("NIR") != std::string::npos)
      outputValuesVariableLengthVector[i]=gain[4];
  }

  return outputValuesVariableLengthVector;
}

} // end namespace otb
