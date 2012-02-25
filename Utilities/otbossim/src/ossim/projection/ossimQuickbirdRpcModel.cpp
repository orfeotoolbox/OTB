//*****************************************************************************
// FILE: ossimQuickbirdRpcModel.cpp
//
// License:  See top level LICENSE.txt file.
//
// DESCRIPTION: Contains declaration of class ossimQuickbirdRpcModel. This 
//    derived class implements the capability of reading Quickbird RPC support
//    data.
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimQuickbirdRpcModel.cpp 20606 2012-02-24 12:29:52Z gpotts $

#include <ossim/projection/ossimQuickbirdRpcModel.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/support_data/ossimQuickbirdRpcHeader.h>
#include <ossim/support_data/ossimQuickbirdTile.h>
#include <ossim/support_data/ossimNitfFile.h>
#include <ossim/support_data/ossimNitfFileHeader.h>
#include <ossim/support_data/ossimNitfImageHeader.h>
#include <ossim/support_data/ossimNitfRpcBase.h>
#include <ossim/support_data/ossimNitfUse00aTag.h>
#include <ossim/support_data/ossimNitfPiaimcTag.h>
#include <ossim/imaging/ossimTiffTileSource.h>
#include <ossim/imaging/ossimQbTileFilesHandler.h>

static const char* RPC00A_TAG = "RPC00A";
static const char* RPC00B_TAG = "RPC00B";
static const char* PIAIMC_TAG = "PIAIMC";
static const char* USE00A_TAG = "USE00A";

RTTI_DEF1(ossimQuickbirdRpcModel, "ossimQuickbirdRpcModel", ossimRpcModel);


//*************************************************************************************************
// Constructor
//*************************************************************************************************
ossimQuickbirdRpcModel::ossimQuickbirdRpcModel()
   :ossimRpcModel(),
    theSupportData(new ossimQuickbirdMetaData())
{
}

//*************************************************************************************************
// Constructor
//*************************************************************************************************
ossimQuickbirdRpcModel::ossimQuickbirdRpcModel(const ossimQuickbirdRpcModel& rhs)
   : ossimRpcModel(rhs),
     theSupportData(new ossimQuickbirdMetaData())
{
}

//*************************************************************************************************
//! Constructor for multiple tile-files sharing common RPC model initializes given pointer
//! to multi-tile-files handler.
//*************************************************************************************************
ossimQuickbirdRpcModel::ossimQuickbirdRpcModel(const ossimQbTileFilesHandler* handler)
:  ossimRpcModel(),
   theSupportData(new ossimQuickbirdMetaData())
{
   setErrorStatus();
   if (!handler)
      return;

   // Make the gsd nan so it gets computed.
   theGSD.makeNan();

   theImageClipRect = handler->getImageRectangle();

   ossimFilename imageFile = handler->getFilename();
   if (!parseRpcData(imageFile))
      return;

   finishConstruction();
   clearErrorStatus();
   return;
}

//*************************************************************************************************
// Destructor
//*************************************************************************************************
ossimQuickbirdRpcModel::~ossimQuickbirdRpcModel()
{
   theSupportData = 0;
}

//*************************************************************************************************
// Infamous DUP 
//*************************************************************************************************
ossimObject* ossimQuickbirdRpcModel::dup() const
{
   return new ossimQuickbirdRpcModel(*this);
}

//*************************************************************************************************
//! Public method for parsing generic image file.
//*************************************************************************************************
bool ossimQuickbirdRpcModel::parseFile(const ossimFilename& file)
{
   if (!parseNitfFile(file))
   {
      return parseTiffFile(file);
   }
   return true;
}

//*************************************************************************************************
//! Parses a NITF image file for RPC info. Returns TRUE if successful.
//*************************************************************************************************
bool ossimQuickbirdRpcModel::parseNitfFile(const ossimFilename& file)
{
   setErrorStatus();
   ossimFilename nitfFile = file;
   
   ossimRefPtr<ossimNitfFile> nitfFilePtr = new ossimNitfFile;
   
   if(!nitfFilePtr->parseFile(nitfFile))
   {
      nitfFile = nitfFile.setExtension("NTF");
      if(!nitfFilePtr->parseFile(nitfFile))
      {
         nitfFile = nitfFile.setExtension("ntf");
         if(!nitfFilePtr->parseFile(nitfFile))
            return false;
      }
   }
   
   ossimRefPtr<ossimNitfImageHeader> ih = nitfFilePtr->getNewImageHeader(0);
   if (!ih)
      return false;

   theImageClipRect = ih->getImageRect();
 
   // Give preference to external RPC data file:
   bool useInternalRpcTags = false;
   if(!parseRpcData(file))
      useInternalRpcTags = true;
   
   if (!parseTileData(file))
      return false;
  
   // Check for IMD (metadata) file:
   parseMetaData(file);

   // Get the gsd.
   theGSD.line = ossim::nan();
   theGSD.samp = ossim::nan();
   
   ossimRefPtr<ossimNitfRegisteredTag> tag;
   tag = ih->getTagData(PIAIMC_TAG);
   if (tag.valid())
   {
      ossimNitfPiaimcTag* p = PTR_CAST(ossimNitfPiaimcTag, tag.get());
      if (p)
      {
         theGSD.line = p->getMeanGsdInMeters();
         theGSD.samp = theGSD.line;
      }
   }
   if (ossim::isnan(theGSD.line))
   {
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

   // If external RPC data file was correctly parsed, then we can bypass this code block. Otherwise
   // need to parse internal NITF tags for RPC data:
   if (useInternalRpcTags)
   {
      // Get the the RPC tag:
      ossimNitfRpcBase* rpcTag = NULL;
      
      // Look for the RPC00B tag first.
      tag = ih->getTagData(RPC00B_TAG);
      if (tag.valid())
         rpcTag = PTR_CAST(ossimNitfRpcBase, tag.get());
      
      if (!tag.valid())
      {
         // Look for RPC00A tag.
         tag = ih->getTagData(RPC00A_TAG);
         if (tag.valid())
            rpcTag = PTR_CAST(ossimNitfRpcBase, tag.get());
      }
      
      if (!rpcTag)
         return false;
      
      // Set the polynomial type.
      if (rpcTag->getRegisterTagName() == "RPC00B")
         thePolyType = B;
      else
         thePolyType = A;

      // Parse coefficients:
      for (ossim_uint32 i=0; i<20; ++i)
      {
         theLineNumCoef[i] = rpcTag->getLineNumeratorCoeff(i).toFloat64();
         theLineDenCoef[i] = rpcTag->getLineDenominatorCoeff(i).toFloat64();
         theSampNumCoef[i] = rpcTag->getSampleNumeratorCoeff(i).toFloat64();
         theSampDenCoef[i] = rpcTag->getSampleDenominatorCoeff(i).toFloat64();
      }

      // Initialize other items in tags:
      theLineScale  = rpcTag->getLineScale().toFloat64();
      theSampScale  = rpcTag->getSampleScale().toFloat64();
      theLatScale   = rpcTag->getGeodeticLatScale().toFloat64();
      theLonScale   = rpcTag->getGeodeticLonScale().toFloat64();
      theHgtScale   = rpcTag->getGeodeticHeightScale().toFloat64();
      theLineOffset = rpcTag->getLineOffset().toFloat64();
      theSampOffset = rpcTag->getSampleOffset().toFloat64();
      theLatOffset  = rpcTag->getGeodeticLatOffset().toFloat64();
      theLonOffset  = rpcTag->getGeodeticLonOffset().toFloat64();
      theHgtOffset  = rpcTag->getGeodeticHeightOffset().toFloat64();
      theImageID    = ih->getImageId();
   }

   finishConstruction();
   clearErrorStatus();
   return true;
}

//*************************************************************************************************
//! Parses a tagged TIFF image file for RPC info. Returns TRUE if successful.
//*************************************************************************************************
bool ossimQuickbirdRpcModel::parseTiffFile(const ossimFilename& file)
{
   setErrorStatus();

   // Make the gsd nan so it gets computed.
   theGSD.makeNan();

   ossimFilename tiffFile = file;
   ossimRefPtr<ossimTiffTileSource> tiff = new ossimTiffTileSource();
   if (!tiff->open(file))
   {
      return false;
   }

   theImageClipRect = tiff->getImageRectangle();

   parseMetaData(file);

   if (!parseRpcData(file))
      return false;

   if (!parseTileData(file))
      return false;

   finishConstruction();
   clearErrorStatus();
   return true;
}

//*************************************************************************************************
//! Given some base name for the image data, parses the associated RPC data file. Returns TRUE
//! if successful
//*************************************************************************************************
bool ossimQuickbirdRpcModel::parseRpcData(const ossimFilename& base_name)
{
   ossimFilename rpcFile (base_name);

   // There are two possibilities for RPC data files: either each image file has its own RPC data
   // file, or a single RPC file is provided for a multi-tile scene.
   rpcFile.setExtension("RPB");
   if (!findSupportFile(rpcFile))
   {
      rpcFile.setExtension("RPA");
      if (!findSupportFile(rpcFile))
         return false;
   }

   // An RPC file was located, open it:
   ossimQuickbirdRpcHeader hdr;
   if(!hdr.open(rpcFile))
      return false;

   if(hdr.isAPolynomial())
      thePolyType = A;
   else
      thePolyType = B;

   std::copy(hdr.theLineNumCoeff.begin(), hdr.theLineNumCoeff.end(), theLineNumCoef);
   std::copy(hdr.theLineDenCoeff.begin(), hdr.theLineDenCoeff.end(), theLineDenCoef);
   std::copy(hdr.theSampNumCoeff.begin(), hdr.theSampNumCoeff.end(), theSampNumCoef);
   std::copy(hdr.theSampDenCoeff.begin(), hdr.theSampDenCoeff.end(), theSampDenCoef);

   theLineScale  = hdr.theLineScale;
   theSampScale  = hdr.theSampScale;
   theLatScale   = hdr.theLatScale;
   theLonScale   = hdr.theLonScale;
   theHgtScale   = hdr.theHeightScale;
   theLineOffset = hdr.theLineOffset;
   theSampOffset = hdr.theSampOffset;
   theLatOffset  = hdr.theLatOffset;
   theLonOffset  = hdr.theLonOffset;
   theHgtOffset  = hdr.theHeightOffset;
   theImageID    = rpcFile.fileNoExtension();

   return true;
}


//*************************************************************************************************
//! Initializes the support data member with metadata file info. Returns TRUE if successful
//*************************************************************************************************
bool ossimQuickbirdRpcModel::parseMetaData(const ossimFilename& base_name)
{

   ossimFilename metadataFile (base_name);
   metadataFile.setExtension("IMD");
   if (!findSupportFile(metadataFile))
      return false;

   if ( !theSupportData.valid() )
      theSupportData = new ossimQuickbirdMetaData();

   if(!theSupportData->open(metadataFile))
   {
      theSupportData = 0; // ossimRefPtr
      ossimNotify(ossimNotifyLevel_WARN) << "ossimQuickbirdRpcModel::parseNitfFile WARNING:"
         << "\nCould not open IMD file.  Sensor ID unknown." << std::endl;
      return false;
   }

   theSensorID = theSupportData->getSatID();
   return true;
}

//*************************************************************************************************
//! Reads the TIL file for pertinent info. Returns TRUE if successful
//*************************************************************************************************
bool ossimQuickbirdRpcModel::parseTileData(const ossimFilename& image_file)
{
   ossimFilename tileFile (image_file);
   tileFile.setExtension("TIL");
   if (!findSupportFile(tileFile))
      return false;

   ossimQuickbirdTile tileHdr;
   if(!tileHdr.open(tileFile))
      return false;

   ossimQuickbirdTileInfo info;
   if(!tileHdr.getInfo(info, image_file.file()))
      return false;

   if((info.theUlXOffset != OSSIM_INT_NAN) && (info.theUlYOffset != OSSIM_INT_NAN) &&
      (info.theLrXOffset != OSSIM_INT_NAN) && (info.theLrYOffset != OSSIM_INT_NAN) &&
      (info.theLlXOffset != OSSIM_INT_NAN) && (info.theLlYOffset != OSSIM_INT_NAN) &&
      (info.theUrXOffset != OSSIM_INT_NAN) && (info.theUrYOffset != OSSIM_INT_NAN))
   {
      theImageClipRect = ossimIrect(ossimIpt(info.theUlXOffset, info.theUlYOffset),
                                    ossimIpt(info.theUrXOffset, info.theUrYOffset),
                                    ossimIpt(info.theLrXOffset, info.theLrYOffset),
                                    ossimIpt(info.theLlXOffset, info.theLlYOffset));
   }
   else if ((info.theUlXOffset != OSSIM_INT_NAN) && (info.theUlYOffset != OSSIM_INT_NAN) &&
      (theImageClipRect.width() != OSSIM_INT_NAN) && (theImageClipRect.height() != OSSIM_INT_NAN))
   {
      theImageClipRect = ossimIrect(info.theUlXOffset, info.theUlYOffset,
                                    info.theUlXOffset+theImageClipRect.width()-1, 
                                    info.theUlYOffset+theImageClipRect.height()-1);
   }

   return true;
}

//*************************************************************************************************
//! Collects common code among all parsers
//*************************************************************************************************
void ossimQuickbirdRpcModel::finishConstruction()
{
   theImageSize.line = theImageClipRect.height();
   theImageSize.samp = theImageClipRect.width();
   theRefImgPt.line = theImageClipRect.midPoint().y;
   theRefImgPt.samp = theImageClipRect.midPoint().x;
   theRefGndPt.lat = theLatOffset;
   theRefGndPt.lon = theLonOffset;
   theRefGndPt.hgt = theHgtOffset;

   //---
   // NOTE:  We must call "updateModel()" to set parameter used by base
   // ossimRpcModel prior to calling lineSampleHeightToWorld or all
   // the world points will be same.
   //---
   updateModel();

   ossimGpt v0, v1, v2, v3;
   lineSampleHeightToWorld(theImageClipRect.ul(), theHgtOffset, v0);
   lineSampleHeightToWorld(theImageClipRect.ur(), theHgtOffset, v1);
   lineSampleHeightToWorld(theImageClipRect.lr(), theHgtOffset, v2);
   lineSampleHeightToWorld(theImageClipRect.ll(), theHgtOffset, v3);

   theBoundGndPolygon = ossimPolygon (ossimDpt(v0), ossimDpt(v1), ossimDpt(v2), ossimDpt(v3));

   // Set the ground reference point using the model.
   lineSampleHeightToWorld(theRefImgPt, theHgtOffset, theRefGndPt);

   if( theGSD.hasNans() )
   {
      try
      {
         // This will set theGSD and theMeanGSD. Method throws ossimException.
         computeGsd();
      }
      catch (const ossimException& e)
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimQuickbirdRpcModel::finishConstruction -- caught exception:\n"
            << e.what() << std::endl;
      }
   }
}

bool ossimQuickbirdRpcModel::saveState(ossimKeywordlist& kwl,
                                       const char* prefix) const
{
   // sanity check only.  This shoulc always be true
   if(theSupportData.valid())
   {
      ossimString supportPrefix = ossimString(prefix) + "support_data.";
      theSupportData->saveState(kwl, supportPrefix);
   }

   return ossimRpcModel::saveState(kwl, prefix);
}

bool ossimQuickbirdRpcModel::loadState(const ossimKeywordlist& kwl,
                                       const char* prefix)
{
   // sanity check only.  This shoulc always be true
   if(theSupportData.valid())
   {
      ossimString supportPrefix = ossimString(prefix) + "support_data.";
      theSupportData->loadState(kwl, supportPrefix);
   }
   return ossimRpcModel::loadState(kwl, prefix);
}

//*************************************************************************************************
// Given an initial filename with case-agnostic extension, this method searches first for an
// image-specific instance of that file (i.e., with R*C* in the filename) before considering
// the mosaic-global support file (R*C* removed). If a file is found, the argument is modified to
// match the actual filename and TRUE is returned. Otherwise, argument filename is left unchanged
// and FALSE is returned.
//*************************************************************************************************
bool ossimQuickbirdRpcModel::findSupportFile(ossimFilename& filename) const
{
   ossimFilename f (filename);
   ossimString extension = f.ext();
   while (true)
   {
      // Search for support file with same basename as image:
      extension.upcase();
      f.setExtension(extension);
      if (f.exists())  
         break;
      extension.downcase();
      f.setExtension(extension);
      if (f.exists())  
         break;

      // None found so far, search for mosaic-global support file:
      f = f.replaceAllThatMatch("_R[0-9]+C[0-9]+");
      if (f.exists())  
         break;
      extension.upcase();
      f.setExtension(extension);
      if (f.exists())  
         break;

      // Nothing found:
      return false;
   }

   // Modify argument to match good filename:
   filename = f;
   return true;
}
