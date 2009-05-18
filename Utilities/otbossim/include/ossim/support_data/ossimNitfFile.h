//*******************************************************************
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
// $Id: ossimNitfFile.h 14243 2009-04-07 20:35:45Z dburken $
#ifndef ossimNitfFile_HEADER
#define ossimNitfFile_HEADER

#include <iosfwd>
#include <vector>

#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/support_data/ossimNitfFileHeader.h>

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

   /**
    * @brief print method that outputs a key/value type format adding prefix
    * to keys.
    * @param out String to output to.
    * @param prefix This will be prepended to key.
    * e.g. Where prefix = "nitf." and key is "file_name" key becomes:
    * "nitf.file_name:"
    * @return output stream.
    */
   std::ostream& print(std::ostream& out,
                       const std::string& prefix=std::string()) const;
   
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

   /** @return The filename parsed by this object. */
   ossimFilename getFilename() const;

protected:
   ossimNitfImageHeader* allocateImageHeader()const;
   
   ossimFilename                    theFilename;
   ossimRefPtr<ossimNitfFileHeader> theNitfFileHeader;
};

#endif
