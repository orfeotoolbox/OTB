//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
// 
// Description:
// 
// Class definition for ossimQuickbirdMetaData.
// 
// This class parses a Space Imaging Quickbird meta data file.
//
//********************************************************************
// $Id: ossimQuickbirdMetaData.cpp 14431 2009-04-30 21:58:33Z dburken $

#include <cstdio>
#include <iostream>
#include <ossim/support_data/ossimQuickbirdMetaData.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>

RTTI_DEF1(ossimQuickbirdMetaData, "ossimQuickbirdMetaData", ossimObject);

// Define Trace flags for use within this file:
static ossimTrace traceExec  ("ossimQuickbirdMetaData:exec");
static ossimTrace traceDebug ("ossimQuickbirdMetaData:debug");

ossimQuickbirdMetaData::ossimQuickbirdMetaData()
  :
  theGenerationTime("Unknown"),
  theBandId("Unknown"),
  theSunAzimuth(0.0),
  theSunElevation(0.0)

{
}

ossimQuickbirdMetaData::~ossimQuickbirdMetaData()
{
}

bool ossimQuickbirdMetaData::open(const ossimFilename& imageFile)
{
   static const char MODULE[] = "ossimQuickbirdMetaData::open";

   clearFields();

   //retrieve information from the metadata file
   //if the Quickbird tif is 02APR01105228-M1BS-000000128955_01_P001.TIF
   //the metadata file will be 02APR01105228-M1BS-000000128955_01_P001.IMD

//    ossimString separator(".");
   //ossimString filenamebase = imageFile.fileNoExtension();
//    std::vector< ossimString > filenameparts = filenamebase.split(separator);
// metadatafile += ".IMD";
// metadatafile.setPath(imageFile.path());

std::cout<<"ImageFile : "<<imageFile<<std::endl;
   ossimFilename metadatafile = imageFile;
   metadatafile.setExtension(ossimString("IMD"));

std::cout<<"metadatafile : "<<metadatafile<<std::endl;

   if( parseMetaData(metadatafile) == false )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " errors parsing metadata" << std::endl;
      }
      return false;
   }
   
   ossimFilename geofile = imageFile;
   geofile.setExtension(ossimString("GEO"));
std::cout<<"geofile : "<<geofile<<std::endl;
   if( parseGEOData(geofile) == false )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " errors parsing GEO" << std::endl;
      }
      return false;
   }


   return true;
}

void ossimQuickbirdMetaData::clearFields()
{
  theSunAzimuth = 0.0;
  theSunElevation = 0.0;
  theBandId = "Unknown";
  theGenerationTime = "Unknown";
}

std::ostream& ossimQuickbirdMetaData::print(std::ostream& out) const
{

  out << "\n----------------- Info on Quickbird Image -------------------"
      << "\n  "
      << "\n  Generation date:   " << theGenerationTime
      << "\n  Band Id:   " << theBandId
      << "\n  Sun Azimuth:    " << theSunAzimuth
      << "\n  Sun Elevation:   " << theSunElevation
      << "\n"
      << "\n---------------------------------------------------------"
      << "\n  " << std::endl;
  return out;
}

bool ossimQuickbirdMetaData::saveState(ossimKeywordlist& kwl,
                                          const char* prefix)const
{


  kwl.add(prefix,
          ossimKeywordNames::TYPE_KW,
          "ossimQuickbirdMetaData",
          true);

  kwl.add(prefix,
          "generation_time",
          theGenerationTime,
          true);

  kwl.add(prefix,
          "band_id",
          theBandId,
          true);

  kwl.add(prefix,
          ossimKeywordNames::AZIMUTH_ANGLE_KW,
          theSunAzimuth,
          true);

  kwl.add(prefix,
          ossimKeywordNames::ELEVATION_ANGLE_KW,
          theSunElevation,
          true);


  return true;
}

bool ossimQuickbirdMetaData::loadState(const ossimKeywordlist& kwl,
                                          const char* prefix)
{
  clearFields();

  const char* lookup = 0;
  ossimString s;
  
  lookup = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
  if (lookup)
  {
     s = lookup;
     if(s != "ossimQuickbirdMetaData")
     {
        return false;
     }
  }

  lookup = kwl.find(prefix, "generation_time");
  if (lookup)
  {
     theGenerationTime = lookup;
  }

  lookup = kwl.find(prefix, "band_id");
  if (lookup)
  {
     theBandId = lookup;
  }

  lookup = kwl.find(prefix, ossimKeywordNames::AZIMUTH_ANGLE_KW);
  if (lookup)
  {
     s = lookup;
     theSunAzimuth = s.toFloat64();
  }

  lookup = kwl.find(prefix, ossimKeywordNames::ELEVATION_ANGLE_KW);
  if (lookup)
  {
     s = lookup;
     theSunElevation = s.toFloat64();
  }

  return true;
}

//*****************************************************************************
// PROTECTED METHOD: ossimQuickbirdMetaData::parseMetaData()
//
//  Parses the Quickbird IMD file.
//
//*****************************************************************************
bool ossimQuickbirdMetaData::parseMetaData(const ossimFilename& data_file)
{
  if (traceExec())
  {
    ossimNotify(ossimNotifyLevel_DEBUG)
        << "DEBUG ossimQuickbirdRpcModel::parseMetaData(data_file): entering..."
        << std::endl;
  }

  FILE* fptr = fopen (data_file, "r");
  if (!fptr)
  {
    if (traceDebug())
    {
      ossimNotify(ossimNotifyLevel_DEBUG)
          << "ossimQuickbirdRpcModel::parseMetaData(data_file) DEBUG:"
          << "\nCould not open Meta data file:  " << data_file
          << "\nreturning with error..." << std::endl;
    }
    return false;
  }

  char* strptr;
   // char linebuf[80];
  char dummy[80], name[80];
  double value;

  //---
  // Read the file into a buffer:
  //---
  ossim_int32 fileSize = static_cast<ossim_int32>(data_file.fileSize());
  char* filebuf = new char[fileSize];
  fread(filebuf, 1, fileSize, fptr);
  fclose(fptr);

  //---
  // Generation time:
  //---
  strptr = strstr(filebuf, "\ngenerationTime =");
  if (!strptr)
  {
    if(traceDebug())
    {
      ossimNotify(ossimNotifyLevel_FATAL)
          << "FATAL ossimQuickbirdRpcModel::parseMetaData(data_file): "
          << "\n\tAborting construction. Error encountered parsing "
          << "presumed meta-data file." << std::endl;

      delete [] filebuf;
      return false;
    }
  }

  sscanf(strptr, "%17c %s", dummy, name);
  theGenerationTime = name;
std::cout<<name<<std::endl;


  //---
  // BandId:
  //---
  strptr = strstr(filebuf, "\nbandId =");
  if (!strptr)
  {
    if(traceDebug())
    {
      ossimNotify(ossimNotifyLevel_FATAL)
          << "FATAL ossimQuickbirdRpcModel::parseMetaData(data_file): "
          << "\n\tAborting construction. Error encountered parsing "
          << "presumed meta-data file." << std::endl;

      delete [] filebuf;
      return false;
    }
  }

  sscanf(strptr, "%9c %s", dummy, name);
  theBandId = name;
std::cout<<name<<std::endl;

   //***
   // Sun Azimuth:
   //***
  strptr = strstr(strptr, "\n\tsunAz =");
  if (!strptr)
  {
    if(traceDebug())
    {
      ossimNotify(ossimNotifyLevel_FATAL)
          << "FATAL ossimQuickbirdRpcModel::parseMetaData(data_file): "
          << "\n\tAborting construction. Error encountered parsing "
          << "presumed meta-data file." << std::endl;

      delete [] filebuf;
      return false;
    }
  }

  sscanf(strptr, "%9c %s", dummy, name);
  //theSunAzimuth = name;
std::cout<<name<<std::endl;

   //***
   // Sun Elevation:
   //***
  strptr = strstr(strptr, "\n\tsunEl =");
  if (!strptr)
  {
    if(traceDebug())
    {
      ossimNotify(ossimNotifyLevel_FATAL)
          << "FATAL ossimQuickbirdRpcModel::parseMetaData(data_file): "
          << "\n\tAborting construction. Error encountered parsing "
          << "presumed meta-data file." << std::endl;

      delete [] filebuf;
      return false;
    }
  }

  sscanf(strptr, "%9c %s", dummy, name);
 // theSunElevation = value;
std::cout<<name<<std::endl;

  delete [] filebuf;
  filebuf = 0;

  if (traceExec())
  {
    ossimNotify(ossimNotifyLevel_DEBUG)
        << "DEBUG ossimQuickbirdRpcModel::parseMetaData(data_file): returning..."
        << std::endl;
  }

  return true;
}

//*****************************************************************************
// PROTECTED METHOD: ossimQuickbirdMetaData::parseGEOData()
//
//  Parses the Quickbird GEO file.
//
//*****************************************************************************
bool ossimQuickbirdMetaData::parseGEOData(const ossimFilename& data_file)
{
  return true;
}

//*****************************************************************************
// PROTECTED METHOD: ossimQuickbirdMetaData::parseRpcData()
//
//  Parses the Quickbird EPH file.
//
//*****************************************************************************
bool ossimQuickbirdMetaData::parseEPHData(const ossimFilename& data_file)
{
  return true;
}

//*****************************************************************************
// PROTECTED METHOD: ossimQuickbirdMetaData::parseRpcData()
//
//  Parses the Quickbird ATT file.
//
//*****************************************************************************
bool ossimQuickbirdMetaData::parseATTData(const ossimFilename& data_file)
{
  return true;
}
