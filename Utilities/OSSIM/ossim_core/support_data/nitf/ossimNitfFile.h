//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfFile.h,v 1.4 2005/11/08 17:40:53 dburken Exp $
#ifndef ossimNitfFile_HEADER
#define ossimNitfFile_HEADER

#include <iostream>
#include <vector>

#include <base/common/ossimReferenced.h>
#include <base/data_types/ossimString.h>
#include <base/data_types/ossimFilename.h>
#include <base/data_types/ossimIrect.h>

class ossimNitfFileHeader;
class ossimNitfImageHeader;
class ossimNitfSymbolHeader;
class ossimNitfLabelHeader;
class ossimNitfTextHeader;
class ossimNitfDataExtensionSegment;
class ossimFilename;
class ossimNitfRegisteredTag;
class ossimNitfTagInformation;

class OSSIMDLLEXPORT ossimNitfFile : public ossimReferenced
{
public:
   friend OSSIMDLLEXPORT std::ostream& operator <<(std::ostream& out,
                                                   const ossimNitfFile &data);
   virtual std::ostream& print(std::ostream& out) const;
   
   ossimNitfFile();
   virtual ~ossimNitfFile();

   /*!
    *  Opens the nitf file and attempts to parse.
    *  Returns true on success, false on error.
    */
   bool parseFile(const ossimFilename &file);

   /*!
    * Will return the header.
    */
   const ossimNitfFileHeader* getHeader()const;
   ossimIrect getImageRect()const;
   
   ossimNitfImageHeader*  getNewImageHeader(long imageNumber)const;
   ossimNitfSymbolHeader* getNewSymbolHeader(long symbolNumber)const;
   ossimNitfLabelHeader*  getNewLabelHeader(long labelNumber)const;
   ossimNitfTextHeader*   getNewTextHeader(long textNumber)const;
   ossimNitfDataExtensionSegment* getNewDataExtensionSegment(long dataExtNumber)const;
   
   ossimString getVersion()const;

protected:
   ossimNitfImageHeader* allocateImageHeader()const;
   
   ossimFilename         theFilename;
   ossimNitfFileHeader*  theNitfFileHeader;
};

#endif
