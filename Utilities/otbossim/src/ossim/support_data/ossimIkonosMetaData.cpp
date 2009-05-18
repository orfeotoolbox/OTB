//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
// 
// Description:
// 
// Class definition for ossimIkonosMetaData.
// 
// This class parses a Space Imaging Ikonos meta data file.
//
//********************************************************************
// $Id: ossimIkonosMetaData.cpp 14431 2009-04-30 21:58:33Z dburken $

#include <cstdio>
#include <iostream>
#include <ossim/support_data/ossimIkonosMetaData.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTrace.h>

RTTI_DEF1(ossimIkonosMetaData, "ossimIkonosMetaData", ossimObject);

// Define Trace flags for use within this file:
static ossimTrace traceExec  ("ossimIkonosMetaData:exec");
static ossimTrace traceDebug ("ossimIkonosMetaData:debug");

ossimIkonosMetaData::ossimIkonosMetaData()
  :
  theSunAzimuth(0.0),
  theSunElevation(0.0),
  theNumBands(0),
  theBandName("Unknown"),
  theProductionDate("Unknown")
{
}

ossimIkonosMetaData::~ossimIkonosMetaData()
{
}

bool ossimIkonosMetaData::open(const ossimFilename& imageFile)
{
   static const char MODULE[] = "ossimIkonosMetaData::open";
   
   clearFields();
   
   //retrieve information from the metadata file
   //if the ikonos tif is po_2619900_pan_0000000.tif
   //the metadata file will be po_2619900_metadata.txt

   ossimString separator("_");
   ossimString filenamebase = imageFile.fileNoExtension();
   std::vector< ossimString > filenameparts = filenamebase.split(separator);
   
   if(filenameparts.size() < 2)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " Ikonos filename non standard" << std::endl;
      }
      return false;
   }
   ossimFilename metadatafile = filenameparts[0];
   metadatafile += "_";
   metadatafile += filenameparts[1];
   metadatafile += "_metadata.txt";
   
   metadatafile.setPath(imageFile.path());
   
   if( parseMetaData(metadatafile) == false )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " errors parsing metadata" << std::endl;
      }
      return false;
   }
   
   ossimFilename hdrfile = imageFile;
   hdrfile.setExtension(ossimString("hdr"));
   if( parseHdrData(hdrfile) == false )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " errors parsing hdr" << std::endl;
      }
      return false;
   }
   
   ossimFilename rpcfile = imageFile.noExtension();
   rpcfile += "_rpc.txt";
   if (parseRpcData (rpcfile) == false)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " errors parsing rpc" << std::endl;
      }
      return false;
   }

   return true;
}

void ossimIkonosMetaData::clearFields()
{
  theSunAzimuth = 0.0;
  theSunElevation = 0.0;
  theNumBands = 0;
  theBandName = "Unknown";
  theProductionDate = "Unknown";
}

std::ostream& ossimIkonosMetaData::print(std::ostream& out) const
{

  out << "\n----------------- Info on Ikonos Image -------------------"
      << "\n  "
      << "\n  Sun Azimuth:    " << theSunAzimuth
      << "\n  Sun Elevation:   " << theSunElevation
      << "\n  Number of bands:   " << theNumBands
      << "\n  Band name:   " << theBandName
      << "\n  Production date:   " << theProductionDate
      << "\n"
      << "\n---------------------------------------------------------"
      << "\n  " << std::endl;
  return out;
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
          ossimKeywordNames::NUMBER_BANDS_KW,
          theNumBands,
          true);

  kwl.add(prefix,
          "band_name",
          theBandName,
          true);

  kwl.add(prefix,
          "production_date",
          theProductionDate,
          true);

  return true;
}

bool ossimIkonosMetaData::loadState(const ossimKeywordlist& kwl,
                                          const char* prefix)
{
  clearFields();

  const char* lookup = 0;
  ossimString s;
  
  lookup = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
  if (lookup)
  {
     s = lookup;
     if(s != "ossimIkonosMetaData")
     {
        return false;
     }
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
  
  lookup = kwl.find(prefix, ossimKeywordNames::NUMBER_BANDS_KW);
  if (lookup)
  {
     s = lookup;
     theNumBands = s.toUInt32();
  }

  lookup = kwl.find(prefix, "band_name");
  if (lookup)
  {
     theBandName = lookup;
  }

  lookup = kwl.find(prefix, "production_date");
  if (lookup)
  {
     theProductionDate = lookup;
  }

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

  //---
  // Read the file into a buffer:
  //---
  ossim_int32 fileSize = static_cast<ossim_int32>(data_file.fileSize());
  char* filebuf = new char[fileSize];
  fread(filebuf, 1, fileSize, fptr);
  fclose(fptr);

  //---
  // Production date:
  //---
  strptr = strstr(filebuf, "\nCreation Date:");
  if (!strptr)
  {
    if(traceDebug())
    {
      ossimNotify(ossimNotifyLevel_FATAL)
          << "FATAL ossimIkonosRpcModel::parseMetaData(data_file): "
          << "\n\tAborting construction. Error encountered parsing "
          << "presumed meta-data file." << std::endl;

      delete [] filebuf;
      return false;
    }
  }

  sscanf(strptr, "%15c %s", dummy, name);
  theProductionDate = name;

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
          << "presumed meta-data file." << std::endl;

      delete [] filebuf;
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
          << "presumed meta-data file." << std::endl;

      delete [] filebuf;
      return false;
    }
  }

  sscanf(strptr, "%21c %lf %s", dummy, &value, name);
  theSunElevation = value;

  delete [] filebuf;
  filebuf = 0;

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
  int value=0;

   //***
   // Read the file into a buffer:
   //***
  char filebuf[5000];
  fread(filebuf, 1, 5000, fptr);
  fclose(fptr);

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
          << "presumed hdr file." << std::endl;
    }

    return false;
  }

  sscanf(strptr, "%6c %s", dummy, name);
  theBandName = name;

   //***
   // Number of Bands:
   //***
  strptr = strstr(filebuf, "\nNumber of Bands:");
  if (!strptr)
  {
    if(traceDebug())
    {
      ossimNotify(ossimNotifyLevel_WARN)
          << "ossimIkonosRpcModel::parseHdrData(data_file):"
          << "\n\tAborting construction. Error encountered parsing "
          << "presumed hdr file." << std::endl;
    }

    return false;
  }

  sscanf(strptr, "%17c %d", dummy, &value);
  theNumBands = value;

  if (traceExec())
  {
    ossimNotify(ossimNotifyLevel_DEBUG)
        << "DEBUG ossimIkonosRpcModel::parseHdrData(data_file): returning..."
        << std::endl;
  }
  return true;
}

//*****************************************************************************
// PROTECTED METHOD: ossimIkonosMetaData::parseRpcData()
//
//  Parses the Ikonos rpc file.
//
//*****************************************************************************
bool ossimIkonosMetaData::parseRpcData(const ossimFilename& data_file)
{
  return true;
}
