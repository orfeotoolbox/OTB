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
//  $Id: ossimQuickbirdRpcModel.cpp 12105 2007-12-02 17:01:48Z dburken $

#include <ossim/projection/ossimQuickbirdRpcModel.h>
#include <ossim/base/ossimException.h>
#include <ossim/support_data/ossimQuickbirdRpcHeader.h>
#include <ossim/support_data/ossimQuickbirdTile.h>
#include <ossim/support_data/ossimNitfFile.h>
#include <ossim/support_data/ossimNitfFileHeader.h>
#include <ossim/support_data/ossimNitfImageHeader.h>
#include <ossim/support_data/ossimNitfRpcBase.h>
#include <ossim/support_data/ossimNitfUse00aTag.h>
#include <ossim/support_data/ossimNitfPiaimcTag.h>
#include <ossim/imaging/ossimTiffTileSource.h>

static const char* RPC00A_TAG = "RPC00A";
static const char* RPC00B_TAG = "RPC00B";
static const char* PIAIMC_TAG = "PIAIMC";
static const char* USE00A_TAG = "USE00A";

RTTI_DEF1(ossimQuickbirdRpcModel, "ossimQuickbirdRpcModel", ossimRpcModel);


ossimQuickbirdRpcModel::ossimQuickbirdRpcModel()
      :ossimRpcModel()
{
}

ossimQuickbirdRpcModel::ossimQuickbirdRpcModel(const ossimQuickbirdRpcModel& rhs)
      : ossimRpcModel(rhs)
{
}

ossimObject* ossimQuickbirdRpcModel::dup() const
{
   return new ossimQuickbirdRpcModel(*this);
}

bool ossimQuickbirdRpcModel::parseFile(const ossimFilename& file)
{
   if (!parseNitfFile(file))
   {
      return parseTiffFile(file);
   }
   return true;
}

bool ossimQuickbirdRpcModel::parseNitfFile(const ossimFilename& file)
{
   ossimFilename nitfFile = file;
   
   ossimRefPtr<ossimNitfFile> nitfFilePtr = new ossimNitfFile;
   
   if(!nitfFilePtr->parseFile(nitfFile))
   {
      nitfFile = nitfFile.setExtension("NTF");
      if(!nitfFilePtr->parseFile(nitfFile))
      {
         nitfFile = nitfFile.setExtension("ntf");
         if(!nitfFilePtr->parseFile(nitfFile))
         {
            return false;
         }
      }
   }
   
   ossimRefPtr<ossimNitfImageHeader> ih = nitfFilePtr->getNewImageHeader(0);
   if (!ih)
   {
      return false;
   }
 
   ossimQuickbirdRpcHeader hdr;
   ossimQuickbirdTile tileHdr;
   ossimFilename tileFile = file;
   ossimFilename rpcFile = file;

   tileFile = tileFile.replaceAllThatMatch("_R[0-9]+C[0-9]+");
   rpcFile  = rpcFile.replaceAllThatMatch("_R[0-9]+C[0-9]+");
   bool useInternalRpcTags = false;
   
   if(!hdr.open(rpcFile))
   {
      rpcFile = rpcFile.setExtension("RPB");
      if(!hdr.open(rpcFile))
      {
         rpcFile = rpcFile.setExtension("rpb");
         if(!hdr.open(rpcFile))
         {
            useInternalRpcTags = true;
         }
      }
   }
   
   tileFile = tileFile.setExtension("TIL");
   if(!tileHdr.open(tileFile))
   {
      tileFile = tileFile.setExtension("til");
      if(!tileHdr.open(tileFile))
      {
         return false;
      }
   }
  
   ossimQuickbirdTileInfo info;
   
   if(!tileHdr.getInfo(info, nitfFile.file()))
   {
      return false;
   }

   ossimIrect imageRect = ih->getImageRect();
   ossim_uint32 w = imageRect.width();
   ossim_uint32 h = imageRect.height();
   
   if(w < 1) w = 1;
   if(h < 1) h = 1;
   
   if((info.theUlXOffset != OSSIM_INT_NAN) &&
      (info.theUlYOffset != OSSIM_INT_NAN) &&
      (info.theLrXOffset != OSSIM_INT_NAN) &&
      (info.theLrYOffset != OSSIM_INT_NAN) &&
      (info.theLlXOffset != OSSIM_INT_NAN) &&
      (info.theLlYOffset != OSSIM_INT_NAN) &&
      (info.theUrXOffset != OSSIM_INT_NAN) &&
      (info.theUrYOffset != OSSIM_INT_NAN))
   {
      imageRect = ossimIrect(ossimIpt(info.theUlXOffset,
                                      info.theUlYOffset),
                             ossimIpt(info.theUrXOffset,
                                      info.theUrYOffset),
                             ossimIpt(info.theLrXOffset,
                                      info.theLrYOffset),
                             ossimIpt(info.theLlXOffset,
                                      info.theLlYOffset));
   }
   else if((info.theUlXOffset != OSSIM_INT_NAN)&&
           (info.theUlYOffset != OSSIM_INT_NAN))
   {
      imageRect = ossimIrect(info.theUlXOffset,
                             info.theUlYOffset,
                             info.theUlXOffset + (w-1),
                             info.theUlYOffset + (h-1));
   }

   // char charbuf[128];
   // std::ifstream in(nitfFile.c_str());

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

   if(useInternalRpcTags)
   {
      // Get the the RPC tag:
      ossimNitfRpcBase* rpcTag = NULL;
      
      // Look for the RPC00B tag first.
      tag = ih->getTagData(RPC00B_TAG);
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
         ++theErrorStatus;
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

      // Parse coefficients:
      for (ossim_uint32 i=0; i<20; ++i)
      {
         theLineNumCoef[i] = rpcTag->getLineNumeratorCoeff(i).toFloat64();
         theLineDenCoef[i] = rpcTag->getLineDenominatorCoeff(i).toFloat64();
         theSampNumCoef[i] = rpcTag->getSampleNumeratorCoeff(i).toFloat64();
         theSampDenCoef[i] = rpcTag->getSampleDenominatorCoeff(i).toFloat64();
      }
   }
   else
   {
      if(hdr.isAPolynomial())
      {
         thePolyType = A;
      }
      else
      {
         thePolyType = B;
      }
      
      std::copy(hdr.theLineNumCoeff.begin(),
                hdr.theLineNumCoeff.end(),
                theLineNumCoef);
      std::copy(hdr.theLineDenCoeff.begin(),
                hdr.theLineDenCoeff.end(),
                theLineDenCoef);
      std::copy(hdr.theSampNumCoeff.begin(),
                hdr.theSampNumCoeff.end(),
                theSampNumCoef);
      std::copy(hdr.theSampDenCoeff.begin(),
                hdr.theSampDenCoeff.end(),
                theSampDenCoef);
   }

   
   theLineScale = hdr.theLineScale;
   theSampScale = hdr.theSampScale;
   theLatScale  = hdr.theLatScale;
   theLonScale  = hdr.theLonScale;
   theHgtScale  = hdr.theHeightScale;
   theLineOffset = hdr.theLineOffset;
   theSampOffset = hdr.theSampOffset;
   theLatOffset  = hdr.theLatOffset;
   theLonOffset  = hdr.theLonOffset;
   theHgtOffset = hdr.theHeightOffset;
   theImageSize.line = imageRect.height();
   theImageSize.samp = imageRect.width();
   theRefImgPt.line = imageRect.midPoint().y;
   theRefImgPt.samp = imageRect.midPoint().x;
   theRefGndPt.lat = theLatOffset;
   theRefGndPt.lon = theLonOffset;
   theRefGndPt.hgt = theHgtOffset;
//    theImageClipRect = ossimIrect(0,
//                                  0,
//                                  theImageSize.samp-1,
//                                  theImageSize.line-1);
   theImageClipRect = imageRect;
   theImageID = hdr.theSatId;

   //---
   // NOTE:  We must call "updateModel()" to set parameter used by base
   // ossimRpcModel prior to calling lineSampleHeightToWorld or all
   // the world points will be same.
   //---
   updateModel();

   ossimGpt v0, v1, v2, v3;
   lineSampleHeightToWorld(imageRect.ul(), theHgtOffset, v0);
   lineSampleHeightToWorld(imageRect.ur(), theHgtOffset, v1);
   lineSampleHeightToWorld(imageRect.lr(), theHgtOffset, v2);
   lineSampleHeightToWorld(imageRect.ll(), theHgtOffset, v3);
   
   theBoundGndPolygon
      = ossimPolygon (ossimDpt(v0), ossimDpt(v1), ossimDpt(v2), ossimDpt(v3));

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
            << "ossimNitfRpcModel::ossimNitfRpcModel Caught Exception:\n"
            << e.what() << std::endl;
      }
   }

   return true;
}

bool ossimQuickbirdRpcModel::parseTiffFile(const ossimFilename& file)
{
   // Make the gsd nan so it gets computed.
   theGSD.makeNan();
   
   ossimFilename tiffFile = file;

   ossimRefPtr<ossimTiffTileSource> tiff = new ossimTiffTileSource();

   if (!tiff->open(file))
   {
      return false;
   }

   ossimIrect imageRect = tiff->getImageRectangle();
   
   ossimQuickbirdRpcHeader hdr;
   ossimQuickbirdTile tileHdr;
   ossimFilename tileFile = file;
   ossimFilename rpcFile = file;

   tileFile = tileFile.replaceAllThatMatch("_R[0-9]+C[0-9]+");
   rpcFile  = rpcFile.replaceAllThatMatch("_R[0-9]+C[0-9]+");
   
   if(!hdr.open(rpcFile))
   {
      rpcFile = rpcFile.setExtension("RPB");
      if(!hdr.open(rpcFile))
      {
         rpcFile = rpcFile.setExtension("rpb");
         if(!hdr.open(rpcFile))
         {
            return false;
         }
      }
   }
   
   tileFile = tileFile.setExtension("TIL");
   if(!tileHdr.open(tileFile))
   {
      tileFile = tileFile.setExtension("til");
      if(!tileHdr.open(tileFile))
      {
         return false;
      }
   }

   ossimQuickbirdTileInfo info;
   
   if(!tileHdr.getInfo(info, file.file()))
   {
      return false;
   }

   ossim_uint32 w = imageRect.width();
   ossim_uint32 h = imageRect.height();
   
   if(w < 1) w = 1;
   if(h < 1) h = 1;
   
   if((info.theUlXOffset != OSSIM_INT_NAN) &&
      (info.theUlYOffset != OSSIM_INT_NAN) &&
      (info.theLrXOffset != OSSIM_INT_NAN) &&
      (info.theLrYOffset != OSSIM_INT_NAN) &&
      (info.theLlXOffset != OSSIM_INT_NAN) &&
      (info.theLlYOffset != OSSIM_INT_NAN) &&
      (info.theUrXOffset != OSSIM_INT_NAN) &&
      (info.theUrYOffset != OSSIM_INT_NAN))
   {
      imageRect = ossimIrect(ossimIpt(info.theUlXOffset,
                                      info.theUlYOffset),
                             ossimIpt(info.theUrXOffset,
                                      info.theUrYOffset),
                             ossimIpt(info.theLrXOffset,
                                      info.theLrYOffset),
                             ossimIpt(info.theLlXOffset,
                                      info.theLlYOffset));
   }
   else if((info.theUlXOffset != OSSIM_INT_NAN)&&
           (info.theUlYOffset != OSSIM_INT_NAN))
   {
      imageRect = ossimIrect(info.theUlXOffset,
                             info.theUlYOffset,
                             info.theUlXOffset + (w-1),
                             info.theUlYOffset + (h-1));
   }

   if(hdr.isAPolynomial())
   {
      thePolyType = A;
   }
   else
   {
      thePolyType = B;
   }
   
   std::copy(hdr.theLineNumCoeff.begin(),
             hdr.theLineNumCoeff.end(),
             theLineNumCoef);
   std::copy(hdr.theLineDenCoeff.begin(),
             hdr.theLineDenCoeff.end(),
             theLineDenCoef);
   std::copy(hdr.theSampNumCoeff.begin(),
             hdr.theSampNumCoeff.end(),
             theSampNumCoef);
   std::copy(hdr.theSampDenCoeff.begin(),
             hdr.theSampDenCoeff.end(),
             theSampDenCoef);

   
   theLineScale = hdr.theLineScale;
   theSampScale = hdr.theSampScale;
   theLatScale  = hdr.theLatScale;
   theLonScale  = hdr.theLonScale;
   theHgtScale  = hdr.theHeightScale;
   theLineOffset = hdr.theLineOffset;
   theSampOffset = hdr.theSampOffset;
   theLatOffset  = hdr.theLatOffset;
   theLonOffset  = hdr.theLonOffset;
   theHgtOffset = hdr.theHeightOffset;
   theImageSize.line = imageRect.height();
   theImageSize.samp = imageRect.width();
   theRefImgPt.line = imageRect.midPoint().y;
   theRefImgPt.samp = imageRect.midPoint().x;
   theRefGndPt.lat = theLatOffset;
   theRefGndPt.lon = theLonOffset;
   theRefGndPt.hgt = theHgtOffset;
   theImageClipRect = imageRect;
   theImageID = hdr.theSatId;

   //---
   // NOTE:  We must call "updateModel()" to set parameter used by base
   // ossimRpcModel prior to calling lineSampleHeightToWorld or all
   // the world points will be same.
   //---
   updateModel();
   
   ossimGpt v0, v1, v2, v3;
   lineSampleHeightToWorld(imageRect.ul(), theHgtOffset, v0);
   lineSampleHeightToWorld(imageRect.ur(), theHgtOffset, v1);
   lineSampleHeightToWorld(imageRect.lr(), theHgtOffset, v2);
   lineSampleHeightToWorld(imageRect.ll(), theHgtOffset, v3);
   
   theBoundGndPolygon
      = ossimPolygon (ossimDpt(v0), ossimDpt(v1), ossimDpt(v2), ossimDpt(v3));

   // Set the ground reference point using the model.
   lineSampleHeightToWorld(theRefImgPt, theHgtOffset, theRefGndPt);

   if( theGSD.hasNans() )
   {
      // Call the base class method to compute.
      try
      {
         // This will set theGSD and theMeanGSD. Method throws ossimException.
         computeGsd();
      }
      catch (const ossimException& e)
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimQuickbirdRpcModel::ossimQuickbirdRpcModel Caught"
            << " Exception:\n"
            << e.what() << std::endl;
      }
   }
   
   return true;
}

bool ossimQuickbirdRpcModel::saveState(ossimKeywordlist& kwl,
                                       const char* prefix) const
{
   return ossimRpcModel::saveState(kwl, prefix);
}

bool ossimQuickbirdRpcModel::loadState(const ossimKeywordlist& kwl,
                                       const char* prefix)
{
   return ossimRpcModel::loadState(kwl, prefix);
}
