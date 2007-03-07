//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author:  David Burken (dburken@imagelinks.com)
//
// Description:
//
// Contains class definition for  ADR_Data.
//*******************************************************************
//  $Id: ossimAdrData.cpp,v 1.3 2004/04/02 22:03:54 gpotts Exp $


#include <stdlib.h>
#include <iostream>
#include <iomanip>
using namespace std;


#include <base/misc/ossimAdrData.h>
#include <base/data_types/ossimKeywordlist.h>
#include <base/data_types/ossimKeyword.h>
#include <base/context/ossimNotifyContext.h>
//! Specifies the longitude of the ul corner of image in degrees
static const ossimKeyword UL_LONGITUDE("ul_longitude",
                                  "\
Specifies the longitude of the ul corner of image in degrees.");

//! Specifies the latitude of the ul corner of image in degrees
static const ossimKeyword UL_LATITUDE("ul_latitude",
                                 "\
Specifies the latitude of the ul corner of image in degrees.");

//! Specifies the longitude of the ur corner of image in degrees
static const ossimKeyword UR_LONGITUDE("ur_longitude",
                                  "\
Specifies the longitude of the ur corner of image in degrees.");

//! Specifies the latitude of the ur corner of image in degrees
static const ossimKeyword UR_LATITUDE("ur_latitude",
                                 "\
Specifies the latitude of the ur corner of image in degrees.");

//! Specifies the longitude of the lr corner of image in degrees
static const ossimKeyword LR_LONGITUDE("lr_longitude",
                                  "\
Specifies the longitude of the lr corner of image in degrees.");

//! Specifies the latitude of the lr corner of image in degrees
static const ossimKeyword LR_LATITUDE("lr_latitude",
                                 "\
Specifies the latitude of the lr corner of image in degrees.");

//! Specifies the longitude of the ll corner of image in degrees
static const ossimKeyword LL_LONGITUDE("ll_longitude",
                                  "\
Specifies the longitude of the ll corner of image in degrees.");

//! Specifies the latitude of the ll corner of image in degrees
static const ossimKeyword LL_LATITUDE("ll_latitude",
                                 "\
Specifies the latitude of the ll corner of image in degrees.");

//! Number of lines in the raster image
static const ossimKeyword NUMBER_LINES("number_lines",
                                  "\
   Number of lines in the raster image.");

//! Number of samples in the raster image
static const ossimKeyword NUMBER_SAMPLES("number_samples",
                                    "\
   Number of samples in the raster image.");

//! Upper left latitude of the image
static const ossimKeyword UPPER_LAT("Upper Lat",
                               "Upper left latitude of the image.");

//! Upper left longitude of the image
static const ossimKeyword LEFT_LON("Left Lon",
                              "Upper left longitude of the image.");

//! Lower right latitude of the image
static const ossimKeyword LOWER_LAT("Lower Lat",
                               "Lower right latitude of the image.");

//! Lower Right longitude of the image
static const ossimKeyword RIGHT_LON("Right Lon",
                               "Lower Right longitude of the image.");

//! Number of samples of the image
static const ossimKeyword SAMPLES("Width",
                             "Number of samples of the image.");

//! Number of lines of the image
static const ossimKeyword LINES("Height",
                           "Number of lines of the image.");
			   
//! One sigma of position error in meters
static const ossimKeyword
POSITION_ERROR("position_error",
               "One sigma of position error in meters.");

//***************************************************************************
//! Public Constructor:
//***************************************************************************
ossimAdrData::ossimAdrData()
   :
      theLines(0),
      theSamples(0),
      theUlLon(0.0),
      theUlLat(0.0),
      theUrLon(0.0),
      theUrLat(0.0),
      theLrLon(0.0),
      theLrLat(0.0),
      theLlLon(0.0),
      theLlLat(0.0),
      thePositionError(0.0),
      theErrorStatus(ossimErrorCodes::OSSIM_OK)
{}
//***************************************************************************
// !Public Constructor:
//***************************************************************************
ossimAdrData::ossimAdrData(const ossimKeywordlist& kwl)
   :
      theLines(0),
      theSamples(0),
      theUlLon(0.0),
      theUlLat(0.0),
      theUrLon(0.0),
      theUrLat(0.0),
      theLrLon(0.0),
      theLrLat(0.0),
      theLlLon(0.0),
      theLlLat(0.0),
      thePositionError(0.0),
      theErrorStatus(ossimErrorCodes::OSSIM_OK)
{
   static const char MODULE[] = "ossimAdrData::ossimAdrData";

   if (kwl.getErrorStatus())
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      ossimNotify(ossimNotifyLevel_FATAL) << MODULE << " ERROR:\n"
                                          << "Detected an error in the keywordlist.\n";
      return;
   }

   const char* kwlLookup = 0;
   
   kwlLookup = kwl.find(UL_LONGITUDE);
   if(kwlLookup)
   {
      theUlLon = atof(kwlLookup);
   }
   else
   {
      kwlLookup = kwl.find(LEFT_LON);
      if (kwlLookup)
      {
         theUlLon = atof(kwlLookup);
      }
      else
      {
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         ossimNotify(ossimNotifyLevel_FATAL) << MODULE << " ERROR:\n"
                                             << "Error looking up upper left longitude keyword.\n"
                                             << "Could not find:  " << UL_LONGITUDE << " or:  " << LEFT_LON
                                             << " keyword.\n";
      }
   }

   kwlLookup = kwl.find(UL_LATITUDE);
   if(kwlLookup)
   {
      theUlLat = atof(kwlLookup);
   }
   else
   {
      kwlLookup = kwl.find(UPPER_LAT); 
      if (kwlLookup)
      {
         theUlLat = atof(kwlLookup);
      }
      else
      {
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         ossimNotify(ossimNotifyLevel_FATAL) << MODULE << " ERROR:\n"
                                             << "Error looking up upper left latitude keyword.\n"
                                             << "Could not find:  " << UL_LATITUDE << " or:  " << UPPER_LAT
                                             << " keyword.\n";
      }
   }

   kwlLookup = kwl.find(UR_LONGITUDE);
   if(kwlLookup)
   {
      theUrLon = atof(kwlLookup);
   }
   else
   {
      kwlLookup = kwl.find(RIGHT_LON);
      if (kwlLookup)
      {
         theUrLon = atof(kwlLookup);
      }
      else
      {
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         ossimNotify(ossimNotifyLevel_FATAL) << MODULE << " ERROR:\n"
                                             << "Error looking up upper right longitude keyword.\n"
                                             << "Could not find:  " << UR_LONGITUDE << " or:  " << RIGHT_LON
                                             << " keyword.\n";
      }
   }

   kwlLookup = kwl.find(UR_LATITUDE);
   if(kwlLookup)
   {
      theUrLat = atof(kwlLookup);
   }
   else
   {
      kwlLookup = kwl.find(UPPER_LAT); 
      if (kwlLookup)
      {
         theUrLat = atof(kwlLookup);
      }
      else
      {
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         ossimNotify(ossimNotifyLevel_FATAL) << MODULE << " ERROR:\n"
                                             << "Error looking up upper right latitude keyword.\n"
                                             << "Could not find:  " << UR_LATITUDE << " or:  " << UPPER_LAT
                                             << " keyword.\n";
      }
   }

   kwlLookup = kwl.find(LR_LONGITUDE);
   if(kwlLookup)
   {
      theLrLon = atof(kwlLookup);
   }
   else
   {
      kwlLookup = kwl.find(RIGHT_LON);
      if (kwlLookup)
      {
         theLrLon = atof(kwlLookup);
      }
      else
      {
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         ossimNotify(ossimNotifyLevel_FATAL)<< MODULE << " ERROR:\n"
                                            << "Error looking up lower right longitude keyword.\n"
                                            << "Could not find:  " << LR_LONGITUDE << " or:  " << RIGHT_LON
                                            << " keyword.\n";
      }
   }

   kwlLookup = kwl.find(LR_LATITUDE);
   if(kwlLookup)
   {
      theLrLat = atof(kwlLookup);
   }
   else
   {
      kwlLookup = kwl.find(LOWER_LAT); 
      if (kwlLookup)
      {
         theLrLat = atof(kwlLookup);
      }
      else
      {
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         ossimNotify(ossimNotifyLevel_FATAL) << MODULE << " ERROR:\n"
                                             << "Error looking up lower right latitude keyword.\n"
                                             << "Could not find:  " << LR_LATITUDE << " or:  " << LOWER_LAT
                                             << " keyword.\n";
      }
   }

   kwlLookup = kwl.find(LL_LONGITUDE);
   if(kwlLookup)
   {
      theLlLon = atof(kwlLookup);
   }
   else
   {
      kwlLookup = kwl.find(LEFT_LON);
      if (kwlLookup)
      {
         theLlLon = atof(kwlLookup);
      }
      else
      {
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         ossimNotify(ossimNotifyLevel_FATAL) << MODULE << " ERROR:\n"
                                             << "Error looking up lower left longitude keyword.\n"
                                             << "Could not find:  " << LL_LONGITUDE << " or:  " << LEFT_LON
                                             << " keyword.\n";
      }
   }

   kwlLookup = kwl.find(LL_LATITUDE);
   if(kwlLookup)
   {
      theLlLat = atof(kwlLookup);
   }
   else
   {
      kwlLookup = kwl.find(LOWER_LAT); 
      if (kwlLookup)
      {
         theLlLat = atof(kwlLookup);
      }
      else
      {
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         ossimNotify(ossimNotifyLevel_FATAL) << MODULE << " ERROR:\n"
                                             << "Error looking up lower left latitude keyword.\n"
                                             << "Could not find:  " << LL_LATITUDE << " or:  " << LOWER_LAT
                                             << " keyword.\n";
      }
   }

   kwlLookup = kwl.find(NUMBER_LINES);
   if(kwlLookup)
   {
      theLines = atoi(kwlLookup);
   }
   else
   {
      kwlLookup = kwl.find(LINES);  
      if (kwlLookup)
      {
         theLines = atoi(kwlLookup);
      }
      else
      {
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         ossimNotify(ossimNotifyLevel_FATAL) << MODULE << " ERROR:\n"
                                             << "Error looking up lines keyword.\n"
                                             << "Could not find:  " << NUMBER_LINES << " or: " << LINES
                                             << " kwyword.\n";
      }
   }

   kwlLookup = kwl.find(NUMBER_SAMPLES);
   if(kwlLookup)
   {
      theSamples = atoi(kwlLookup);
   }
   else
   {
      kwlLookup = kwl.find(SAMPLES);  
      if (kwlLookup)
      {
         theSamples = atoi(kwlLookup);
      }
      else
      {
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         ossimNotify(ossimNotifyLevel_FATAL) << MODULE << " ERROR:\n"
                                             << "Error looking up lines keyword.\n"
                                             << "Could not find:  " << NUMBER_SAMPLES << " or: " << SAMPLES
                                             << " kwyword.\n";
      }
   }

   kwlLookup = kwl.find(POSITION_ERROR);
   if(kwlLookup)
   {
      thePositionError = atof(kwlLookup);
   }
}

//***************************************************************************
// Public Method:
//***************************************************************************
//! Prints a list of keywords and respective values

void ossimAdrData::print(ostream& os) const
{
   os << setiosflags(ios::fixed) << setprecision(10)
      << "number_lines:        " << theLines            << "\n"
      << "number_samples:      " << theSamples          << "\n"
      << "ul_longitude:        " << theUlLon            << "\n"
      << "ul_latitude:         " << theUlLat            << "\n"
      << "ur_longitude:        " << theUrLon            << "\n"
      << "ur_latitude:         " << theUrLat            << "\n"
      << "lr_longitude:        " << theLrLon            << "\n"
      << "lr_latitude:         " << theLrLat            << "\n"
      << "ll_longitude:        " << theLlLon            << "\n"
      << "ll_latitude:         " << theLlLat            << "\n"
      << "position_error:      " << thePositionError    << "\n"
      << "\n";
}

//***************************************************************************
// Function:
//***************************************************************************
ostream& operator<<(ostream& os, const ossimAdrData& ad)
{
   ad.print(os);

   return os;
}
