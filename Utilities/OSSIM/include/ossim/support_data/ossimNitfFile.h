//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfFile.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimNitfFile_HEADER
#define ossimNitfFile_HEADER

#include <iosfwd>
#include <vector>

#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimIrect.h>

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
   const ossimRefPtr<ossimNitfFileHeader> getHeader()const;
   ossimIrect getImageRect()const;
   
   ossimNitfImageHeader*  getNewImageHeader(long imageNumber)const;
   ossimNitfSymbolHeader* getNewSymbolHeader(long symbolNumber)const;
   ossimNitfLabelHeader*  getNewLabelHeader(long labelNumber)const;
   ossimNitfTextHeader*   getNewTextHeader(long textNumber)const;
   ossimNitfDataExtensionSegment* getNewDataExtensionSegment(long dataExtNumber)const;
   
   ossimString getVersion()const;

protected:
   ossimNitfImageHeader* allocateImageHeader()const;
   
   ossimFilename                    theFilename;
   ossimRefPtr<ossimNitfFileHeader> theNitfFileHeader;
};

#endif
