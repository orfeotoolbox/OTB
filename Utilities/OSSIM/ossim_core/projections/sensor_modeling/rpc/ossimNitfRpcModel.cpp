//*****************************************************************************
// FILE: ossimNitfRpcModel.cc
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// AUTHOR: Oscar Kramer
//
// DESCRIPTION: Contains implementation of class ossimNitfRpcModel. This 
//    derived class implements the capability of reading Nitf RPC support
//    data.
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimNitfRpcModel.cpp,v 1.19 2004/12/21 16:49:22 gpotts Exp $

#include <projections/sensor_modeling/rpc/ossimNitfRpcModel.h>

RTTI_DEF1(ossimNitfRpcModel, "ossimNitfRpcModel", ossimRpcModel);

#include <base/data_types/ossimKeywordlist.h>
#include <base/context/ossimNotifyContext.h>
#include <support_data/nitf/ossimNitfFile.h>
#include <support_data/nitf/ossimNitfFileHeader.h>
#include <support_data/nitf/ossimNitfImageHeader.h>
//***
// Define Trace flags for use within this file:
//***
#include <base/common/ossimTrace.h>
static ossimTrace traceExec  ("ossimNitfRpcModel:exec");
static ossimTrace traceDebug ("ossimNitfRpcModel:debug");

static const char* RPC00A_TAG = "RPC00A";
static const char* RPC00B_TAG = "RPC00B";
static const char* PIAIMC_TAG = "PIAIMC";
static const char* STDIDC_TAG = "STDIDC";
static const char* USE26A_TAG = "USE26A";
static const char* USE00A_TAG = "USE00A";

ossimNitfRpcModel::ossimNitfRpcModel()
  :ossimRpcModel(),
   theBiasError(0.0),
   theRandError(0.0)
{
}

ossimNitfRpcModel::ossimNitfRpcModel(const ossimNitfRpcModel& rhs)
  :ossimRpcModel(rhs),
   theBiasError(0.0),
   theRandError(0.0)
{
}

//*****************************************************************************
//  CONSTRUCTOR: ossimNitfRpcModel
//  
//  Constructs given filename for NITF file
//  
//*****************************************************************************
ossimNitfRpcModel::ossimNitfRpcModel(const ossimFilename& nitfFile)
   :
      ossimRpcModel()
{
   if (traceExec())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimNitfRpcModel::ossimNitfRpcModel(nitfFile): entering..."
         << std::endl;
   }

   ossimNitfFile* file = new ossimNitfFile;

   if(!file->parseFile(nitfFile))
   {
      delete file;
      return;
   }
   const ossimNitfFileHeader* header = file->getHeader();

   if(!header)
   {
      delete file;
      return;
   }
   ossimNitfImageHeader* ih = file->getNewImageHeader(0);

   if(!ih)
   {
      delete file;
      return;
   }

   std::ifstream in(nitfFile.c_str());

   char charbuf[128];
   //***
   // Open the NITF file:
   //***
   if (!in)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimNitfRpcModel::ossimNitfRpcModel DEBUG:"
            << "\nCould not open NITF file at: <"
            << nitfFile << ">."
            << "\nAborting with error..."
            << std::endl;
      }

      theErrorStatus++;
      delete ih;
      delete file;
      return;
   }

   //***
   // Fetch Image ID:
   //***
   theImageID = ih->getImageId();


   ossimIrect imageRect = ih->getImageRect();
   
   //***
   // Fetch Image Size:
   //***
   theImageSize.line = imageRect.height();
   theImageSize.samp = imageRect.width();
      
   ossimNitfTagInformation tag;
   if(ih->getTag(tag, PIAIMC_TAG))
   {
//      in.seekg(tag.getTagDataOffset()+348);
      in.seekg(tag.getTagDataOffset()+337);
      
      //***
      // Parse the tag for GSD (in inches):
      //***
      in.read(charbuf, 7);
      charbuf[7] = '\0';
      theGSD.line = atof(charbuf)/12.0 * MTRS_PER_FT;
      theGSD.samp = theGSD.line;
   }
   else if(ih->getTag(tag, USE26A_TAG))
   {
	   //***
	   // Parse the tag for GSD (in inches):
	   //***
	   //         in.seekg(tag.getTagDataOffset()+15);
	   in.seekg(tag.getTagDataOffset()+4);
	   in.read(charbuf, 5);
	   charbuf[6] = '\0';
	   theGSD.line = atof(charbuf)/12.0 * MTRS_PER_FT;
	   theGSD.samp = theGSD.line;
   }
   else if(ih->getTag(tag, USE00A_TAG))
   {
	   in.seekg(tag.getTagDataOffset()+4);
	   in.read(charbuf, 5);
	   charbuf[6] = '\0';
	   theGSD.line = atof(charbuf)/12.0 * MTRS_PER_FT;
	   theGSD.samp = theGSD.line;
   }
   else
   {
      theGSD.line = OSSIM_DBL_NAN;
      theGSD.samp = OSSIM_DBL_NAN;
   }
      
   //***
   // Search for the STDID Tag to fetch mission (satellite) name:
   //***
   if(ih->getTag(tag, STDIDC_TAG))
   {
	   in.seekg(tag.getTagDataOffset()+14);
	   in.read(charbuf, 14);
	   charbuf[14] = '\0';
	   theSensorID = charbuf;
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimNitfRpcModel::ossimNitfRpcModel DEBUG:"
            << "\nCould not find the STDIDC tag <" << STDIDC_TAG
            << ">\n in the NITF file at: <"
            << nitfFile << ">." << " Setting platform ID to \"UNKNOWN\"..."
            << std::endl;
      }

      theSensorID = "UNKNOWN";
   }
   
   //---
   // Search the header for the start of the RPC tag:
   //---
   if(ih->getTag(tag, RPC00A_TAG))
   {
      thePolyType = A;
   }
   else
   {
      if(ih->getTag(tag, RPC00B_TAG))
      {
         thePolyType = B;
      }
      else
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "ossimNitfRpcModel::ossimNitfRpcModel DEBUG:"
               << "\nCould not find neither RPC tags <" << RPC00A_TAG
               << "> nor <" << RPC00B_TAG << "> in the NITF file at: <"
               << nitfFile << ">." << " Aborting with error..."
               << std::endl;
         }

         theErrorStatus++;
         delete ih;
         delete file;
         return;
      }
   }

   char* tag_ptr = new char[tag.getTagLength()];
   in.seekg(tag.getTagDataOffset());
   in.read(tag_ptr, tag.getTagLength());

   //***
   // Now parse the tag block for pertinent data:
   //***
   int line_off, samp_off, hgt_off, line_scl, samp_scl, hgt_scl;
   
   char* tempBufPtr = tag_ptr;
   
   theBiasError = ossimString(tempBufPtr+1,
                              tempBufPtr+8).toDouble();
   tempBufPtr+=8;
   theRandError = ossimString(tempBufPtr,
                              tempBufPtr+7).toDouble();
   tempBufPtr+=7;
   line_off = ossimString(tempBufPtr,
                          tempBufPtr+6).toInt();
   tempBufPtr+=6;
   samp_off = ossimString(tempBufPtr,
                          tempBufPtr+5).toInt();
   tempBufPtr+=5;
   
   theLatOffset = ossimString(tempBufPtr,
                              tempBufPtr+8).toDouble();
   tempBufPtr+=8;

   theLonOffset = ossimString(tempBufPtr,
                              tempBufPtr+9).toDouble();
   tempBufPtr+=9;

   hgt_off = ossimString(tempBufPtr,
                         tempBufPtr+5).toInt();
   tempBufPtr+=5;
   line_scl = ossimString(tempBufPtr,
                         tempBufPtr+6).toInt();
   tempBufPtr+=6;
   samp_scl = ossimString(tempBufPtr,
                         tempBufPtr+5).toInt();
   tempBufPtr+=5;
   theLatScale = ossimString(tempBufPtr,
                              tempBufPtr+8).toDouble();
   tempBufPtr+=8;

   theLonScale = ossimString(tempBufPtr,
                              tempBufPtr+9).toDouble();
   tempBufPtr+=9;
   hgt_scl = ossimString(tempBufPtr,
                         tempBufPtr+5).toInt();
   tempBufPtr+=5;
   
//    sscanf(tag_ptr,
//           "%7lf %7lf %6d %5d %8lf %9lf %5d %6d %5d %8lf %9lf %5d",
//           &theBiasError,
//           &theRandError,
//           &line_off,
//           &samp_off,
//           &theLatOffset,
//           &theLonOffset,
//           &hgt_off,
//           &line_scl,
//           &samp_scl,
//           &theLatScale,
//           &theLonScale,
//           &hgt_scl);

//    cout << "******************bias error he got is " << theBiasError << endl;
   theLineOffset = (double) line_off;
   theSampOffset = (double) samp_off;
   theHgtOffset  = (double) hgt_off;
   theLineScale  = (double) line_scl;
   theSampScale  = (double) samp_scl;
   theHgtScale   = (double) hgt_scl;

//    theRefImgPt.line = theLineOffset;
//    theRefImgPt.samp = theSampOffset;
//    theRefGndPt.lat  = theLatOffset;
//    theRefGndPt.lon  = theLonOffset;
//    theRefGndPt.hgt = theHgtOffset;

   //***
   // Parse coefficients:
   //***
   tempBufPtr = tag_ptr + 81;
//   tag_ptr += 92;
//   tag_ptr += 92;
   for (int i=0; i<20; i++)
   {
      sscanf(tempBufPtr, "%12lf", &theLineNumCoef[i]);
      tempBufPtr += 12;
   }
   for (int i=0; i<20; i++)
   {
      sscanf(tempBufPtr, "%12lf", &theLineDenCoef[i]);
      tempBufPtr += 12;
   }
   for (int i=0; i<20; i++)
   {
      sscanf(tempBufPtr, "%12lf", &theSampNumCoef[i]);
      tempBufPtr += 12;
   }
   for (int i=0; i<20; i++)
   {
      sscanf(tempBufPtr, "%12lf", &theSampDenCoef[i]);
      tempBufPtr += 12;
   }

   delete [] tag_ptr;
   //***
   // Assign other data members to default values:
   //***
   theNominalPosError = sqrt(theBiasError*theBiasError +
                             theRandError*theRandError); // meters

   delete ih;
   delete file;
   
   //***
   // Assign other data members:
   //***
   thePolyType = B; // This may not be true for early RPC imagery
   theRefImgPt.line = theImageSize.line/2.0;
   theRefImgPt.samp = theImageSize.samp/2.0;
   theRefGndPt.lat  = theLatOffset;
   theRefGndPt.lon  = theLonOffset;
   theRefGndPt.hgt  = theHgtOffset;
   
   
   

   //***
   // Assign the bounding image space rectangle:
   //***
   theImageClipRect = ossimDrect(0.0, 0.0,
                                 theImageSize.samp-1, theImageSize.line-1);

   //***
   // Assign the bounding ground polygon:
   //***
   ossimGpt v0, v1, v2, v3;
   ossimDpt ip0 (0.0, 0.0);
   lineSampleHeightToWorld(ip0, theHgtOffset, v0);
   ossimDpt ip1 (theImageSize.samp-1.0, 0.0);
   lineSampleHeightToWorld(ip1, theHgtOffset, v1);
   ossimDpt ip2 (theImageSize.samp-1.0, theImageSize.line-1.0);
   lineSampleHeightToWorld(ip2, theHgtOffset, v2);
   ossimDpt ip3 (0.0, theImageSize.line-1.0);
   lineSampleHeightToWorld(ip3, theHgtOffset, v3);

   theBoundGndPolygon
      = ossimPolygon (ossimDpt(v0), ossimDpt(v1), ossimDpt(v2), ossimDpt(v3));

   
   lineSampleHeightToWorld(theRefImgPt, theHgtOffset, theRefGndPt);

   if( (theGSD.line == OSSIM_DBL_NAN) || (theGSD.samp == OSSIM_DBL_NAN) )
   {
	   ossimGpt rightGpt;
	   ossimGpt topGpt;
	   
	   lineSampleHeightToWorld(theRefImgPt + ossimDpt(1, 0),
							   theHgtOffset,
							   rightGpt);
	   lineSampleHeightToWorld(theRefImgPt + ossimDpt(0, -1),
							   theHgtOffset,
							   topGpt);
	   ossimEcefPoint rightPt = rightGpt;
	   ossimEcefPoint topPt   = topGpt;
	   ossimEcefPoint origin  = theRefGndPt;
	   
	   ossim_float64 gsdx = (rightPt-origin).magnitude();
	   ossim_float64 gsdy = (topPt-origin).magnitude();
	   theGSD.x = (gsdx + gsdy)/2.0;
	   theGSD.y = theGSD.x;
   }
   theMeanGSD = (theGSD.x + theGSD.y)/2.0;
   if (ossimString::toString(theRefGndPt.lat).trim().upcase() == "NAN" 
       || ossimString::toString(theRefGndPt.lon).trim().upcase() == "NAN")
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimNitfRpcModel::ossimNitfRpcModel DEBUG:"
            << "\nGround Reference Point not valid." 
            << " Aborting with error..."
            << std::endl;
      }

      theErrorStatus++;
      delete ih;
      return;
   }

   ossimString drivePart;
   ossimString pathPart;
   ossimString filePart;
   ossimString extPart;
   nitfFile.split(drivePart,
                  pathPart,
                  filePart,
                  extPart);
   
   if (traceExec())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimNitfRpcModel::ossimNitfRpcModel: returning..."
         << std::endl;
   }
   return;
}

bool ossimNitfRpcModel::saveState(ossimKeywordlist& kwl,
				           const char* prefix) const
{
  kwl.add(prefix, 
	  "bias_error",
	  theBiasError,
	  true);
  kwl.add(prefix,
	  "rand_error",
	  theRandError,
	  true);
  
  return ossimRpcModel::saveState(kwl, prefix);
}

bool ossimNitfRpcModel::loadState(const ossimKeywordlist& kwl,
				  const char* prefix)
{
  theBiasError = ossimString(kwl.find(prefix, "bias_error")).toDouble();
  theRandError = ossimString(kwl.find(prefix, "rand_error")).toDouble();

  return ossimRpcModel::loadState(kwl, prefix);
}
