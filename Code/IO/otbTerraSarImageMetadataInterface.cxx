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
#include "base/ossimDate.h"

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
  kwl.print(std::cout);
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


ossimplugins::Noise *
TerraSarImageMetadataInterface::GetNoise( const MetaDataDictionaryType & dict ) const
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

  ossimplugins::Noise * noise = new ossimplugins::Noise();
  noise->loadState( kwl, "");
  

  return noise;
}


unsigned int
TerraSarImageMetadataInterface::GetNumberOfNoiseRecords( const MetaDataDictionaryType & dict ) const
{
  ossimplugins::Noise * noise = this->GetNoise( dict );
  
  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);
  std::string key = "noise.numberOfNoiseRecords";
  ossimString nbRec = kwl.find(key.c_str());


  delete noise;

  return static_cast<unsigned int>(nbRec.toInt());
}


TerraSarImageMetadataInterface::UIntVectorType
TerraSarImageMetadataInterface::GetNoisePolynomialDegrees( const MetaDataDictionaryType & dict ) const
{
  ossimplugins::Noise * noise = this->GetNoise( dict );

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  UIntVectorType polDeg;
  itk::OStringStream oss;

  unsigned int nbRec = this->GetNumberOfNoiseRecords(dict);
  for(unsigned int i=0; i<nbRec; i++)
    {
      oss.str("");
      oss << "noise[" << i << "]imageNoise.noiseEstimate.polynomialDegree";
      ossimString tempVal = kwl.find(oss.str().c_str());
      polDeg.push_back( static_cast<unsigned int>(tempVal.toInt()) );
    }  
  
  delete noise;

  return polDeg;
}

TerraSarImageMetadataInterface::DoubleVectorVectorType
TerraSarImageMetadataInterface::GetNoisePolynomialCoefficientsList( const MetaDataDictionaryType & dict ) const
{
  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  ossimplugins::Noise * noise = this->GetNoise( dict );
  unsigned int nbRec = this->GetNumberOfNoiseRecords(dict);
  UIntVectorType polDegs = this->GetNoisePolynomialDegrees(dict);
  
  DoubleVectorVectorType polCoefList;
  DoubleVectorType       polCoef; 
  itk::OStringStream oss;
  
  for(unsigned int i=0; i<nbRec; i++)
    {
      polCoef.clear();
      // set <= condition because degree N means N+1 coeff
      for(unsigned int j=0; j<=polDegs.size(); j++)
 	{ 
	  oss.str("");
	  oss << "noise[" << i << "]imageNoise.noiseEstimate.coefficient[" << j <<"]";
	  ossimString tempVal = kwl.find(oss.str().c_str());
	  polCoef.push_back( static_cast<double>(tempVal.toDouble()) );
	}
      polCoefList.push_back(polCoef);
    }  
  
  delete noise;

  return polCoefList;
}


TerraSarImageMetadataInterface::DoubleVectorType
TerraSarImageMetadataInterface::GetNoiseTimeUTCList( const MetaDataDictionaryType & dict ) const
{
  ossimplugins::Noise * noise = this->GetNoise( dict );

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  DoubleVectorType timeList;
  itk::OStringStream oss;
  ossimString separatorList = "-T:Z";
  std::vector<ossimString> splittedDate;

  int year, month, day, hour, minu, sec;
  double secDec, julianDay;
  unsigned int nbRec = this->GetNumberOfNoiseRecords(dict);
  for(unsigned int i=0; i<nbRec; i++)
    {
      oss.str("");
      oss << "noise[" << i << "]imageNoise.timeUTC";
      ossimString tempVal = kwl.find(oss.str().c_str());
      splittedDate = tempVal.split(separatorList);
    
      year = splittedDate[0].toInt();
      month = splittedDate[1].toInt();
      day = splittedDate[2].toInt();
      hour = splittedDate[3].toInt();
      minu = splittedDate[4].toInt();
      double secFull = splittedDate[5].toDouble();
      sec = static_cast<int>(vcl_floor(secFull));
      // store the decimal second value
      secDec = secFull - vcl_floor(secFull);
      ossimDate myDate(month, day, year);
      myDate.setHour(hour);
      myDate.setMin(minu);
      myDate.setSec(sec);
      
      std::cout<<secDec*1e-5/0.864<<std::endl;
      julianDay = myDate.getJulian();
      // add the decimal second to the julian day (0.00001 <-> 0.864s)
      julianDay += secDec*1e-5/0.864;


      timeList.push_back( julianDay );
    }  

  delete noise;

  return timeList;
}


TerraSarImageMetadataInterface::DoubleVectorType
TerraSarImageMetadataInterface::GetNoiseValidityRangeMaxList( const MetaDataDictionaryType & dict ) const
{
  ossimplugins::Noise * noise = this->GetNoise( dict );

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  DoubleVectorType maxList;
  itk::OStringStream oss;

  unsigned int nbRec = this->GetNumberOfNoiseRecords(dict);
  for(unsigned int i=0; i<nbRec; i++)
    {
      oss.str("");
      oss << "noise[" << i << "]imageNoise.noiseEstimate.validityRangeMax";
      ossimString tempVal = kwl.find(oss.str().c_str());
      maxList.push_back( tempVal.toDouble() );
    }  

  delete noise;

  return maxList;
}



TerraSarImageMetadataInterface::DoubleVectorType
TerraSarImageMetadataInterface::GetNoiseValidityRangeMinList( const MetaDataDictionaryType & dict ) const
{
  ossimplugins::Noise * noise = this->GetNoise( dict );

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  DoubleVectorType minList;
  itk::OStringStream oss;

  unsigned int nbRec = this->GetNumberOfNoiseRecords(dict);
  for(unsigned int i=0; i<nbRec; i++)
    {
      oss.str("");
      oss << "noise[" << i << "]imageNoise.noiseEstimate.validityRangeMin";
      ossimString tempVal = kwl.find(oss.str().c_str());

      minList.push_back( tempVal.toDouble() );
    }  

  delete noise;

  return minList;
}

TerraSarImageMetadataInterface::DoubleVectorType
TerraSarImageMetadataInterface::GetNoiseReferencePointList( const MetaDataDictionaryType & dict ) const
{
  ossimplugins::Noise * noise = this->GetNoise( dict );

  ImageKeywordlistType imageKeywordlist;

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
  {
    itk::ExposeMetaData<ImageKeywordlistType>(dict, MetaDataKey::OSSIMKeywordlistKey, imageKeywordlist);
  }

  ossimKeywordlist kwl;
  imageKeywordlist.convertToOSSIMKeywordlist(kwl);

  DoubleVectorType refPointList;
  itk::OStringStream oss;

  unsigned int nbRec = this->GetNumberOfNoiseRecords(dict);
  for(unsigned int i=0; i<nbRec; i++)
    {
      oss.str("");
      oss << "noise[" << i << "]imageNoise.noiseEstimate.referencePoint";
      ossimString tempVal = kwl.find(oss.str().c_str());

      refPointList.push_back( tempVal.toDouble() );
    }  

  delete noise;

  return refPointList;
}


double
TerraSarImageMetadataInterface::GetRadarFrequency( const MetaDataDictionaryType & dict ) const
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

  ossimString tempVal = kwl.find("radarFrequency");
  double freq = tempVal.toDouble();

  return freq;
}

} // end namespace otb
