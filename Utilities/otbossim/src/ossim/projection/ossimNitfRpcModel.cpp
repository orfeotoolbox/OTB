//*****************************************************************************
// FILE: ossimNitfRpcModel.cc
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  See top level LICENSE.txt file.
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
//  $Id: ossimNitfRpcModel.cpp 9576 2006-09-15 19:10:45Z dburken $

#include <ossim/projection/ossimNitfRpcModel.h>

RTTI_DEF1(ossimNitfRpcModel, "ossimNitfRpcModel", ossimRpcModel);

#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/support_data/ossimNitfFile.h>
#include <ossim/support_data/ossimNitfFileHeader.h>
#include <ossim/support_data/ossimNitfImageHeader.h>
#include <ossim/support_data/ossimNitfUse00aTag.h>
#include <ossim/support_data/ossimNitfPiaimcTag.h>
#include <ossim/support_data/ossimNitfStdidcTag.h>
#include <ossim/support_data/ossimNitfRpcBase.h>

//***
// Define Trace flags for use within this file:
//***
#include <ossim/base/ossimTrace.h>
static ossimTrace traceExec  ("ossimNitfRpcModel:exec");
static ossimTrace traceDebug ("ossimNitfRpcModel:debug");

static const char* RPC00A_TAG = "RPC00A";
static const char* RPC00B_TAG = "RPC00B";
static const char* PIAIMC_TAG = "PIAIMC";
static const char* STDIDC_TAG = "STDIDC";
static const char* USE00A_TAG = "USE00A";

ossimNitfRpcModel::ossimNitfRpcModel()
  :ossimRpcModel()
{
}

ossimNitfRpcModel::ossimNitfRpcModel(const ossimNitfRpcModel& rhs)
  :ossimRpcModel(rhs)
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

   ossimRefPtr<ossimNitfFile> file = new ossimNitfFile;

   if(!file->parseFile(nitfFile))
   {
      return;
   }

   ossimRefPtr<ossimNitfImageHeader> ih = file->getNewImageHeader(0);
   if(!ih)
   {
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

   // Search for the STDID Tag to fetch mission (satellite) name:
   getSensorID(ih.get());

   if (getRpcData(ih.get()) == false)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimNitfRpcModel::ossimNitfRpcModel DEBUG:"
            << "\nError parsing rpc tags.  Aborting with error."
            << std::endl;
      }

      ++theErrorStatus;
      return;
   }

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

   //---
   // See bugzilla #1187
   // Since the rpc model can compute the ground sample distance (gsd)
   // accurately, and
   // we have experienced bad "MEANGSD" fields in the PIAIMC tag the code
   // has been changed to always compute "theGSD".  Since the gsd is for
   // user information purposes only, in other words, not used in the
   // ossimRpcModel::neSampleHeightToWorld this is a very safe code change.
   //---
   {
      // Limit the scope of all this {} ...
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

      ++theErrorStatus;
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
}

void ossimNitfRpcModel::getGsd(const ossimNitfImageHeader* ih)
{
   theGSD.line = OSSIM_DBL_NAN;
   theGSD.samp = OSSIM_DBL_NAN;

   if (!ih)
   {
      return;
   }
   
   ossimRefPtr<ossimNitfRegisteredTag> tag;
   tag = ih->getTagData(PIAIMC_TAG);
   if (tag.valid())
   {
      ossimNitfPiaimcTag* p = PTR_CAST(ossimNitfPiaimcTag, tag.get());
      if (p)
      {
         theGSD.line = p->getMeanGsdInMeters();
         theGSD.samp = theGSD.line;
         return;
      }
   }

   tag = ih->getTagData(USE00A_TAG);
   if (tag.valid())
   {
      ossimNitfUse00aTag* p = PTR_CAST(ossimNitfUse00aTag, tag.get());
      if (p)
      {
         theGSD.line = p->getMeanGsdInMeters();
         theGSD.samp = theGSD.line;
      }
   }
}

void ossimNitfRpcModel::getSensorID(const ossimNitfImageHeader* ih)
{
   theSensorID = "UNKNOWN";

   if (!ih)
   {
      return;
   }
   
   ossimRefPtr<ossimNitfRegisteredTag> tag;
   tag = ih->getTagData(STDIDC_TAG);
   if (tag.valid())
   {
      ossimNitfStdidcTag* p = PTR_CAST(ossimNitfStdidcTag, tag.get());
      if (p)
      {
         theSensorID = p->getMission();
         return;
      }
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimNitfRpcModel::getSensorID DEBUG:"
            << "\ntheSensorID set to: " << theSensorID << endl;
   }
}

bool ossimNitfRpcModel::getRpcData(const ossimNitfImageHeader* ih)
{
   if (!ih)
   {
      return false;
   }

   // Get the the RPC tag:
   ossimNitfRpcBase* rpcTag = NULL;

   // Look for the RPC00B tag first.
   ossimRefPtr<ossimNitfRegisteredTag> tag = ih->getTagData(RPC00B_TAG);
   if (tag.valid())
   {
      rpcTag = PTR_CAST(ossimNitfRpcBase, tag.get());
   }

   if (!rpcTag)
   {
      // Look for RPC00A tag.
      tag = ih->getTagData(RPC00A_TAG);
      if (tag.valid())
      {
         rpcTag = PTR_CAST(ossimNitfRpcBase, tag.get());
      }
   }

   if (!rpcTag)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimNitfRpcModel::getRpcData DEBUG:"
            << "\nCould not find neither RPC tags <" << RPC00A_TAG
            << "> nor <" << RPC00B_TAG
            << "\nAborting with error..."
            << std::endl;
      }
      return false;
   }

   // Set the polynomial type.
   if (rpcTag->getRegisterTagName() == "RPC00B")
   {
      thePolyType = B;
   }
   else
   {
      thePolyType = A;
   }
   
   // Now parse the tag block for pertinent data:
   theBiasError  = rpcTag->getErrorBias().toFloat64();
   theRandError  = rpcTag->getErrorRand().toFloat64();
   theLineOffset = rpcTag->getLineOffset().toFloat64();
   theSampOffset = rpcTag->getSampleOffset().toFloat64();
   theLatOffset  = rpcTag->getGeodeticLatOffset().toFloat64();
   theLonOffset  = rpcTag->getGeodeticLonOffset().toFloat64();
   theHgtOffset  = rpcTag->getGeodeticHeightOffset().toFloat64();
   theLineScale  = rpcTag->getLineScale().toFloat64();
   theSampScale  = rpcTag->getSampleScale().toFloat64();
   theLatScale   = rpcTag->getGeodeticLatScale().toFloat64();
   theLonScale   = rpcTag->getGeodeticLonScale().toFloat64();
   theHgtScale   = rpcTag->getGeodeticHeightScale().toFloat64();

   // Parse coefficients:
   ossim_uint32 i;
   
   for (i=0; i<20; ++i)
   {
      theLineNumCoef[i] = rpcTag->getLineNumeratorCoeff(i).toFloat64();
      theLineDenCoef[i] = rpcTag->getLineDenominatorCoeff(i).toFloat64();
      theSampNumCoef[i] = rpcTag->getSampleNumeratorCoeff(i).toFloat64();
      theSampDenCoef[i] = rpcTag->getSampleDenominatorCoeff(i).toFloat64();
   }
   
   // Assign other data members to default values:
   theNominalPosError = sqrt(theBiasError*theBiasError +
                             theRandError*theRandError); // meters

   return true;
}
