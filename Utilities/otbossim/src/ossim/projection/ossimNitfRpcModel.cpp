//*****************************************************************************
// FILE: ossimNitfRpcModel.cc
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
//  $Id: ossimNitfRpcModel.cpp 16643 2010-02-22 21:32:43Z dburken $

#include <ossim/projection/ossimNitfRpcModel.h>

RTTI_DEF1(ossimNitfRpcModel, "ossimNitfRpcModel", ossimRpcModel);

#include <ossim/base/ossimException.h>
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
   :
   ossimRpcModel(),
   theDecimation(1.0)
{
}

ossimNitfRpcModel::ossimNitfRpcModel(const ossimNitfRpcModel& rhs)
  :
   ossimRpcModel(rhs),
   theDecimation(1.0)
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
   ossimRpcModel(),
   theDecimation(1.0)
{
   if (traceExec())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimNitfRpcModel::ossimNitfRpcModel(nitfFile): entering..."
         << std::endl;
   }
   if(!parseFile(nitfFile))
   {
      if (traceExec())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimNitfRpcModel::ossimNitfRpcModel(nitfFile): Unable to parse file " << nitfFile
            << std::endl;
      }
      ++theErrorStatus;
   }

   if (traceExec())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimNitfRpcModel::ossimNitfRpcModel(nitfFile): returning..."
         << std::endl;
   }
}

ossimObject* ossimNitfRpcModel::dup() const
{
   return new ossimNitfRpcModel(*this);
}

bool ossimNitfRpcModel::parseFile(const ossimFilename& nitfFile,
                                  ossim_uint32 entryIndex)
{
   ossimRefPtr<ossimNitfFile> file = new ossimNitfFile;

   if(!file->parseFile(nitfFile))
   {
      setErrorStatus();
      return false;
   }
   
   ossimRefPtr<ossimNitfImageHeader> ih = file->getNewImageHeader(entryIndex);
   if(!ih)
   {
      setErrorStatus();
      return false;
   }

   return parseImageHeader(ih.get());
   
}

bool ossimNitfRpcModel::parseImageHeader(const ossimNitfImageHeader* ih)
{
   // Do this first so we don't waste time if not rpc image.
   if (getRpcData(ih) == false)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNitfRpcModel::parseFile DEBUG:"
         << "\nError parsing rpc tags.  Aborting with error."
         << std::endl;
      }
      setErrorStatus();
      return false;
   }
   
   
   //---
   // Get the decimation if any from the header "IMAG" field.
   // 
   // Look for string like:
   // "/2" = 1/2
   // "/4  = 1/4
   // ...
   // "/16 = 1/16
   // If it is full resolution it should be "1.0"
   //---
   ossimString os = ih->getImageMagnification();
   if ( os.contains("/") )
   {
      os = os.after("/");
      ossim_float64 d = os.toFloat64();
      if (d)
      {
         theDecimation = 1.0 / d;
      }
   }
   
   //***
   // Fetch Image ID:
   //***
   theImageID = ih->getImageId();
   
   ossimIrect imageRect = ih->getImageRect();
   
   //---
   // Fetch Image Size:
   //---
   theImageSize.line =
   static_cast<ossim_int32>(imageRect.height() / theDecimation);
   theImageSize.samp =
   static_cast<ossim_int32>(imageRect.width() / theDecimation);
   
   // Search for the STDID Tag to fetch mission (satellite) name:
   getSensorID(ih);
   
   //***
   // Assign other data members:
   //***
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
   
   //---
   // Assign the bounding ground polygon:
   //
   // NOTE:  We will use the base ossimRpcModel for transformation since all
   // of our calls are in full image space (not decimated).
   //---
   ossimGpt v0, v1, v2, v3;
   ossimDpt ip0 (0.0, 0.0);
   ossimRpcModel::lineSampleHeightToWorld(ip0, theHgtOffset, v0);
   ossimDpt ip1 (theImageSize.samp-1.0, 0.0);
   ossimRpcModel::lineSampleHeightToWorld(ip1, theHgtOffset, v1);
   ossimDpt ip2 (theImageSize.samp-1.0, theImageSize.line-1.0);
   ossimRpcModel::lineSampleHeightToWorld(ip2, theHgtOffset, v2);
   ossimDpt ip3 (0.0, theImageSize.line-1.0);
   ossimRpcModel::lineSampleHeightToWorld(ip3, theHgtOffset, v3);
   
   theBoundGndPolygon
   = ossimPolygon (ossimDpt(v0), ossimDpt(v1), ossimDpt(v2), ossimDpt(v3));
   
   updateModel();
   
   // Set the ground reference point.
   ossimRpcModel::lineSampleHeightToWorld(theRefImgPt,
                                          theHgtOffset,
                                          theRefGndPt);
   if ( theRefGndPt.isLatNan() || theRefGndPt.isLonNan() )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNitfRpcModel::ossimNitfRpcModel DEBUG:"
         << "\nGround Reference Point not valid." 
         << " Aborting with error..."
         << std::endl;
      }
      setErrorStatus();
      return false;
   }
   
   //---
   // This will set theGSD and theMeanGSD.  This model doesn't need these but
   // others do.
   //---
   try
   {
      computeGsd();
   }
   catch (const ossimException& e)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimNitfRpcModel::ossimNitfRpcModel DEBUG:\n"
         << e.what() << std::endl;
      }
   }
   
   if (traceExec())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG ossimNitfRpcModel::parseFile: returning..."
      << std::endl;
   }
   
   return true;
}

void ossimNitfRpcModel::worldToLineSample(const ossimGpt& world_point,
                                          ossimDpt&       image_point) const
{
   // Get the full res (not decimated) point.
   ossimRpcModel::worldToLineSample(world_point, image_point);

   // Apply decimation.
   image_point.x = image_point.x * theDecimation;
   image_point.y = image_point.y * theDecimation;
}

void ossimNitfRpcModel::lineSampleHeightToWorld(
   const ossimDpt& image_point,
   const double&   heightEllipsoid,
   ossimGpt&       worldPoint) const
{
   // Convert image point to full res (not decimated) point.
   ossimDpt pt;
   pt.x = image_point.x / theDecimation;
   pt.y = image_point.y / theDecimation;

   // Call base...
   ossimRpcModel::lineSampleHeightToWorld(pt, heightEllipsoid, worldPoint);
}

bool ossimNitfRpcModel::saveState(ossimKeywordlist& kwl,
                                  const char* prefix) const
{
   // Save the decimation.
   kwl.add(prefix, "decimation", theDecimation);

   // Call base.
   return ossimRpcModel::saveState(kwl, prefix);
}

bool ossimNitfRpcModel::loadState(const ossimKeywordlist& kwl,
                                  const char* prefix)
{
   // Lookup decimation.
   const char* value = kwl.find(prefix, "decimation");
   if (value)
   {
      theDecimation = ossimString(value).toFloat64();
      if (theDecimation <= 0.0)
      {
         // Do not allow negative or "0.0"(divide by zero).
         theDecimation = 1.0;
      }
   }

   // Call base.
   return ossimRpcModel::loadState(kwl, prefix);
}

void ossimNitfRpcModel::getGsd(const ossimNitfImageHeader* ih)
{
   theGSD.line = ossim::nan();
   theGSD.samp = ossim::nan();

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
   ossimNitfRpcBase* rpcTag = 0;

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

   if ( rpcTag->getSuccess() == false )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimNitfRpcModel::getRpcData DEBUG:"
            << "\nSuccess flag set to false."
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
