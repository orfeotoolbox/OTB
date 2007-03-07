//*****************************************************************************
// FILE: ossimQuickbirdRpcModel.cpp
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// DESCRIPTION: Contains declaration of class ossimQuickbirdRpcModel. This 
//    derived class implements the capability of reading Quickbird RPC support
//    data.
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimQuickbirdRpcModel.cpp,v 1.6 2005/11/08 20:04:13 dburken Exp $

#include <projections/sensor_modeling/rpc/ossimQuickbirdRpcModel.h>
#include <support_data/qb/ossimQuickbirdRpcHeader.h>
#include <support_data/qb/ossimQuickbirdTile.h>
#include <support_data/nitf/ossimNitfFile.h>
#include <support_data/nitf/ossimNitfFileHeader.h>
#include <support_data/nitf/ossimNitfImageHeader.h>
#include <imaging/formats/tiff/ossimTiffTileSource.h>

static const char* RPC00A_TAG = "RPC00A";
static const char* RPC00B_TAG = "RPC00B";
static const char* PIAIMC_TAG = "PIAIMC";
// static const char* STDIDC_TAG = "STDIDC";
static const char* USE26A_TAG = "USE26A";
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

   const ossimNitfFileHeader* header = nitfFilePtr->getHeader();

   if(!header)
   {
      return false;
   }
   
   ossimQuickbirdTileInfo info;
   
   if(!tileHdr.getInfo(info, nitfFile.file()))
   {
      return false;
   }

   ossimRefPtr<ossimNitfImageHeader> ih =
      nitfFilePtr->getNewImageHeader(0);
   
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

   char charbuf[128];
   std::ifstream in(nitfFile.c_str());

   ossimNitfTagInformation tag;
   if(ih->getTag(tag, PIAIMC_TAG))
   {
      in.seekg(tag.getTagDataOffset()+337);
      
      in.read(charbuf, 7);
      charbuf[7] = '\0';
      theGSD.line = atof(charbuf)/12.0 * MTRS_PER_FT;
      theGSD.samp = theGSD.line;
   }
   else if(ih->getTag(tag, USE26A_TAG))
   {
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

   if(useInternalRpcTags)
   {
      //***
      // Search the header for the start of the RPC tag:
      //***
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
            ossimNotify(ossimNotifyLevel_FATAL)
               << "FATAL ossimNitfRpcModel::ossimNitfRpcModel: ERROR"
               << "\nCould not find neither RPC tags <" << RPC00A_TAG
               << "> nor <" << RPC00B_TAG
               << "> in the NITF file at: <"
               << nitfFile << ">." << " Aborting with error..." << std::endl;
            theErrorStatus++;
            return false;
         }
      }
      char* tag_ptr = new char[tag.getTagLength()];
      in.seekg(tag.getTagDataOffset());
      in.read(tag_ptr, tag.getTagLength());

      char* tempBufPtr = tag_ptr + 81;
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

   ossimGpt v0, v1, v2, v3;
   lineSampleHeightToWorld(imageRect.ul(), theHgtOffset, v0);
   lineSampleHeightToWorld(imageRect.ur(), theHgtOffset, v1);
   lineSampleHeightToWorld(imageRect.lr(), theHgtOffset, v2);
   lineSampleHeightToWorld(imageRect.ll(), theHgtOffset, v3);
   
   theBoundGndPolygon
      = ossimPolygon (ossimDpt(v0), ossimDpt(v1), ossimDpt(v2), ossimDpt(v3));

   lineSampleHeightToWorld(theRefImgPt, theHgtOffset, theRefGndPt);
   ossimGpt rightGpt;
   ossimGpt topGpt;
   
   lineSampleHeightToWorld(theRefImgPt + ossimDpt(1, 0),
                           theHgtOffset,
                           rightGpt);
   lineSampleHeightToWorld(theRefImgPt + ossimDpt(0, -1),
                           theHgtOffset,
                           topGpt);
   
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

   ossimGpt v0, v1, v2, v3;
   lineSampleHeightToWorld(imageRect.ul(), theHgtOffset, v0);
   lineSampleHeightToWorld(imageRect.ur(), theHgtOffset, v1);
   lineSampleHeightToWorld(imageRect.lr(), theHgtOffset, v2);
   lineSampleHeightToWorld(imageRect.ll(), theHgtOffset, v3);
   
   theBoundGndPolygon
      = ossimPolygon (ossimDpt(v0), ossimDpt(v1), ossimDpt(v2), ossimDpt(v3));

   lineSampleHeightToWorld(theRefImgPt, theHgtOffset, theRefGndPt);
   ossimGpt rightGpt;
   ossimGpt topGpt;
   
   lineSampleHeightToWorld(theRefImgPt + ossimDpt(1, 0),
                           theHgtOffset,
                           rightGpt);
   lineSampleHeightToWorld(theRefImgPt + ossimDpt(0, -1),
                           theHgtOffset,
                           topGpt);
   
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
