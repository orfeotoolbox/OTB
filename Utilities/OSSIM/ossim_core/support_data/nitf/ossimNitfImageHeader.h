//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// LICENSE: LGPL see top level LICENSE.txt for more details
//
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfImageHeader.h,v 1.22 2005/10/05 12:50:48 gpotts Exp $
#ifndef ossimNitfImageHeader_HEADER
#define ossimNitfImageHeader_HEADER
#include <iostream>
#include <vector>
using namespace std;
#include <base/data_types/ossimString.h>
#include <base/data_types/ossimDrect.h>
#include <base/common/ossimNBandLutDataObject.h>
#include <base/common/ossimObject.h>
#include <base/common/ossimPropertyInterface.h>
#include "ossimNitfTagInformation.h"
#include "ossimNitfCompressionHeader.h"
#include "ossimNitfImageBand.h"

class OSSIMDLLEXPORT ossimNitfImageHeader : public ossimObject,
                                            public ossimPropertyInterface
{
public:
   ossimNitfImageHeader();
   virtual ~ossimNitfImageHeader();

   virtual bool  getTagInformation(ossimNitfTagInformation& tag,
                                   ossim_uint32 idx)const;

   virtual int getNumberOfTags()const;

   virtual bool getTag(ossimNitfTagInformation& tagInfo,
                       const ossimString& tagName)const;
   virtual void addTag(ossimNitfTagInformation tag);
   virtual void removeTag(const ossimString& tagName);
   
   virtual void parseStream(std::istream &in)= 0;
   virtual void writeStream(std::ostream &out)= 0;

   virtual bool isCompressed()const=0;
   virtual bool isEncrypted()const=0;

   virtual ossimString getCompressionCode()const=0;
   virtual long getDisplayLevel()const=0;
   virtual long getAttachmentLevel()const=0;
   
   virtual long getNumberOfRows()const=0;
   virtual long getNumberOfBands()const=0;
   virtual long getNumberOfCols()const=0;
   virtual long getNumberOfBlocksPerRow()const=0;
   virtual long getNumberOfBlocksPerCol()const=0;
   virtual long getNumberOfPixelsPerBlockHoriz()const=0;
   virtual long getNumberOfPixelsPerBlockVert()const=0;
   virtual ossimString getImageId()const=0;
   virtual long        getActualBitsPerPixelPerBand()const=0;
   virtual long        getBitsPerPixelPerBand()const=0;
   virtual ossimDrect  getImageRect()const=0;
   /**
    * Blocks might not be the same size as the significant rows and cols used
    * by the getImageRect.
    */
   virtual ossimDrect   getBlockImageRect()const=0;
   virtual ossimString  getIMode()const=0;
   virtual ossim_uint64 getDataLocation()const=0;
   virtual ossimString  getAcquisitionDateMonthDayYear(ossim_uint8 separationChar='-')const=0;
   virtual ossimString  getSecurityClassification()const=0;
   virtual ossimString  getCategory()const=0;
   virtual ossimString  getImageSource()const=0;
   virtual ossimString  getRepresentation()const=0;
   virtual ossimString  getCoordinateSystem()const=0;
   virtual ossimString  getGeographicLocation()const=0;
   virtual ossimString  getPixelValueType()const=0;
   virtual bool hasBlockMaskRecords()const=0;
   virtual bool hasPadPixelMaskRecords()const=0;
   virtual bool hasTransparentCode()const=0;
   virtual ossim_uint32 getTransparentCode()const=0;
   virtual ossim_uint32 getBlockMaskRecordOffset(ossim_uint32 blockNumber,
                                                 ossim_uint32 bandNumber)const=0;
   
   virtual ossim_uint32 getPadPixelMaskRecordOffset(ossim_uint32 blockNumber,
                                                    ossim_uint32 bandNumber)const=0;
   virtual const ossimRefPtr<ossimNitfCompressionHeader> getCompressionHeader()const=0;
   virtual const ossimRefPtr<ossimNitfImageBand> getBandInformation(ossim_uint32 idx)const=0;
   bool hasLut()const;
   ossimRefPtr<ossimNBandLutDataObject> createLut(ossim_uint32 bandIdx)const;
   virtual bool isSameAs(const ossimNitfImageHeader* hdr) const;
   
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   virtual ossim_uint32 getTotalTagLength()const;
   void printTags(std::ostream& out)const;

protected:

   /**
    * Method to check tie point to see if it will be truncated in field
    * IGEOLO which has only arc second resolution.
    * 
    * Spits out warning if tie does not evenly divide by an arc second.
    *
    * @param tie Tie point to check.
    */
   void checkForGeographicTiePointTruncation(const ossimDpt& tie) const;
   
   std::vector<ossimNitfTagInformation> theTagList;

TYPE_DATA
};

#endif
