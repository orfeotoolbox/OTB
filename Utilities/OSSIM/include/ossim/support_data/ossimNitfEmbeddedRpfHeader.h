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
// $Id: ossimNitfEmbeddedRpfHeader.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimNitfEmbeddedRpfHeader_HEADER
#define ossimNitfEmbeddedRpfHeader_HEADER
#include <ossim/support_data/ossimNitfRegisteredTag.h>
class ossimRpfHeader;

class OSSIMDLLEXPORT ossimNitfEmbeddedRpfHeader : public ossimNitfRegisteredTag
{
public:
   ossimNitfEmbeddedRpfHeader();
   virtual ~ossimNitfEmbeddedRpfHeader();
   /*!
    * This will return the name of the registered tag for this
    * user defined header.
    */
   virtual ossimString getRegisterTagName()const;

   /*!
    * This class will ignore the headerTag since it is the header
    * tag.
    */
   virtual void parseStream(std::istream &in);
   virtual void writeStream(std::ostream& out);
   virtual std::ostream& print(std::ostream& out)const;
   virtual ossim_uint32 getSizeInBytes()const;
   
   virtual const ossimRpfHeader* getRpfHeader()const;
   

TYPE_DATA
private:
   ossimRpfHeader *theRpfHeader;
};

#endif
