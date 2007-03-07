//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfTagInformation.h,v 1.6 2004/09/29 11:17:46 gpotts Exp $
#ifndef ossimNitfTagInformation_HEADER
#define ossimNitfTagInformation_HEADER
#include <iostream>
using namespace std;

#include "base/common/ossimObject.h"
#include "base/data_types/ossimString.h"
#include <support_data/nitf/ossimNitfRegisteredTag.h>

class OSSIMDLLEXPORT ossimNitfTagInformation : public ossimObject
{
public:
   ossimNitfTagInformation(ossimRefPtr<ossimNitfRegisteredTag> tagData = NULL);
   ~ossimNitfTagInformation();
   
   virtual void parseStream(std::istream& in);
   virtual void writeStream(std::ostream& out);

   void setTagName(const ossimString& tagName);
   void setTagLength(ossim_uint32 tagLength);

   /**
    * Length of the 5 byte tag length the 6 byte tag name and
    * the data length.
    * So we have Data length + 11 bytes.
    */
   ossim_uint32 getTotalTagLength()const;

   /**
    * Should return the value of theTagLength which is the length of
    * the data in bytes.
    */
   ossim_uint32 getTagLength()const;
   ossim_uint64 getTagOffset()const;
   ossim_uint64 getTagDataOffset()const;
   
   ossimString   getTagName()const;
   virtual std::ostream& print(std::ostream& out)const;
   void clearFields();
   
   ossimRefPtr<ossimNitfRegisteredTag> getTagData();
   const ossimRefPtr<ossimNitfRegisteredTag> getTagData()const;
   void setTagData(ossimRefPtr<ossimNitfRegisteredTag> tagData);

   
private:

   /**
    * This is a 6 byte field
    */
   char          theTagName[7];

   /**
    * This is a 5 byte field
    */
   char           theTagLength[6];

   /**
    * This will hold the offset to the start of the above information
    * This is just a work variable.
    */
   ossim_uint64 theTagOffset;

   /**
    * This will hold the start to the data.  This is just the
    * position in the file just past the tag length field.
    * This is just a work variable
    */
   ossim_uint64 theTagDataOffset;

   /**
    * Used to hold the tag data.
    */
   ossimRefPtr<ossimNitfRegisteredTag> theTagData;
};

#endif
