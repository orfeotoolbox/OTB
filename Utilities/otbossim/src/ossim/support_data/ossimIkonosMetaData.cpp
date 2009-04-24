//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken <dburken@imagelinks.com>
//
// Description:
//
// Class definition for ossimIkonosMetaData.
//
// This class parses a Space Imaging Ikonos meta data file.
//
//********************************************************************
// $Id: ossimIkonosMetaData.cpp 9963 2006-11-28 21:11:01Z gpotts $

#include <ossim/support_data/ossimIkonosMetaData.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>

// Define Trace flags for use within this file:
static ossimTrace traceExec  ("ossimIkonosMetaData:exec");
static ossimTrace traceDebug ("ossimIkonosMetaData:debug");

RTTI_DEF2(ossimIkonosMetaData, "ossimIkonosMetaData", ossimObject, ossimErrorStatusInterface);

ossimIkonosMetaData::ossimIkonosMetaData()
{
}

ossimIkonosMetaData::ossimIkonosMetaData(const char* metaDataFile)
{
}

ossimIkonosMetaData::~ossimIkonosMetaData()
{
}

void ossimIkonosMetaData::clearFields()
{
  clearErrorStatus();
  theSunAzimuth = 0.0;
  theSunElevation = 0.0;
  theBandName = "Unknown";
}

void ossimIkonosMetaData::printInfo(ostream& os) const
{

  os << "\n----------------- Info on Ikonos Image -------------------"
      << "\n  "
      << "\n  Sun Azimuth:    " << theSunAzimuth
      << "\n  Sun Elevation:   " << theSunElevation
      << "\n"
      << "\n---------------------------------------------------------"
      << "\n  " << std::endl;
}

bool ossimIkonosMetaData::saveState(ossimKeywordlist& kwl,
                                          const char* prefix)const
{

  kwl.add(prefix,
          ossimKeywordNames::TYPE_KW,
          "ossimIkonosMetaData",
          true);

  kwl.add(prefix,
          ossimKeywordNames::AZIMUTH_ANGLE_KW,
          theSunAzimuth,
          true);

  kwl.add(prefix,
          ossimKeywordNames::ELEVATION_ANGLE_KW,
          theSunElevation,
          true);

  kwl.add(prefix,
          "band_name",
          theBandName,
          true);

  return true;
}

bool ossimIkonosMetaData::loadState(const ossimKeywordlist& kwl,
                                          const char* prefix)
{
  clearFields();

  ossimString type = kwl.find(prefix, ossimKeywordNames::TYPE_KW);

  if(type != "ossimIkonosMetaData")
  {
    return false;
  }

  theSunAzimuth   = ossimString(kwl.find(prefix, ossimKeywordNames::AZIMUTH_ANGLE_KW)).toDouble();
  theSunElevation = ossimString(kwl.find(prefix, ossimKeywordNames::ELEVATION_ANGLE_KW)).toDouble();

  return true;
}

//*****************************************************************************
// PROTECTED METHOD: ossimIkonosMetaData::parseMetaData()
//
//  Parses the Ikonos metadata file.
//
//*****************************************************************************
bool ossimIkonosMetaData::parseMetaData(const ossimFilename& data_file)
{
  if (traceExec())
  {
    ossimNotify(ossimNotifyLevel_DEBUG)
        << "DEBUG ossimIkonosRpcModel::parseMetaData(data_file): entering..."
        << std::endl;
  }

  FILE* fptr = fopen (data_file, "r");
  if (!fptr)
  {
    ++theErrorStatus;
    if (traceDebug())
    {
      ossimNotify(ossimNotifyLevel_DEBUG)
          << "ossimIkonosRpcModel::parseMetaData(data_file) DEBUG:"
          << "\nCould not open Meta data file:  " << data_file
          << "\nreturning with error..." << std::endl;
    }
    return false;
  }

  char* strptr;
   // char linebuf[80];
  char dummy[80], name[80];
  double value;
   //***
   // Read the file into a buffer:
   //***
  char filebuf[5000];
  fread(filebuf, 1, 5000, fptr);

    //***
   // Sun Azimuth:
   //***
  strptr = strstr(strptr, "\nSun Angle Azimuth:");
  if (!strptr)
  {
    if(traceDebug())
    {
      ossimNotify(ossimNotifyLevel_FATAL)
          << "FATAL ossimIkonosRpcModel::parseMetaData(data_file): "
          << "\n\tAborting construction. Error encountered parsing "
          << "presumed meta-data file." << endl;

      return false;
    }
  }

  sscanf(strptr, "%19c %lf %s", dummy, &value, dummy);
  theSunAzimuth = value;

   //***
   // Sun Elevation:
   //***
  strptr = strstr(strptr, "\nSun Angle Elevation:");
  if (!strptr)
  {
    if(traceDebug())
    {
      ossimNotify(ossimNotifyLevel_FATAL)
          << "FATAL ossimIkonosRpcModel::parseMetaData(data_file): "
          << "\n\tAborting construction. Error encountered parsing "
          << "presumed meta-data file." << endl;

      return false;
    }
  }

  sscanf(strptr, "%21c %lf %s", dummy, &value, name);
  theSunElevation = value;


  if (traceExec())
  {
    ossimNotify(ossimNotifyLevel_DEBUG)
        << "DEBUG ossimIkonosRpcModel::parseMetaData(data_file): returning..."
        << std::endl;
  }

  return true;
}

//*****************************************************************************
// PROTECTED METHOD: ossimIkonosMetaData::parseHdrData()
//
//  Parses the Ikonos hdr file.
//
//*****************************************************************************
bool ossimIkonosMetaData::parseHdrData(const ossimFilename& data_file)
{
  if (traceExec())
  {
    ossimNotify(ossimNotifyLevel_DEBUG)
        << "DEBUG ossimIkonosRpcModel::parseHdrData(data_file): entering..."
        << std::endl;
  }

  FILE* fptr = fopen (data_file, "r");
  if (!fptr)
  {
    ++theErrorStatus;

    if (traceDebug())
    {
      ossimNotify(ossimNotifyLevel_WARN)
          << "ossimIkonosRpcModel::parseHdrData(data_file) WARN:"
          << "\nCould not open hdr data file <" << data_file << ">. "
          << "returning with error..." << std::endl;
    }
    return false;
  }

  char* strptr;
   // char linebuf[80];
  char dummy[80];
  char name[80];

   //***
   // Read the file into a buffer:
   //***
  char filebuf[5000];
  fread(filebuf, 1, 5000, fptr);


   //***
   // Band name:
   //***
  strptr = strstr(filebuf, "\nBand:");
  if (!strptr)
  {
    if(traceDebug())
    {
      ossimNotify(ossimNotifyLevel_WARN)
          << "ossimIkonosRpcModel::parseHdrData(data_file):"
          << "\n\tAborting construction. Error encountered parsing "
          << "presumed hdr file." << endl;
    }

    return false;
  }

  sscanf(strptr, "%6c %s", dummy, name);
  theBandName = name;
  std::cout << "***************** theBandName: " << theBandName << std::endl;

  if (traceExec())
  {
    ossimNotify(ossimNotifyLevel_DEBUG)
        << "DEBUG ossimIkonosRpcModel::parseHdrData(data_file): returning..."
        << std::endl;
  }
  return true;
}

