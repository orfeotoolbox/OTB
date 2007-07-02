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
// $Id: ossimNitfEmbeddedRpfDes.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimNitfEmbeddedRpfDes_HEADER
#define ossimNitfEmbeddedRpfDes_HEADER
#include <ossim/support_data/ossimNitfRegisteredTag.h>
class ossimNitfFileHeader;

class OSSIMDLLEXPORT ossimNitfEmbeddedRpfDes : public ossimNitfRegisteredTag
{
public:
   ossimNitfEmbeddedRpfDes();
   virtual ~ossimNitfEmbeddedRpfDes();
   /*!
    * This will return the name of the registered tag for this
    * user defined header.
    */
   virtual ossimString getRegisterTagName()const{return "RPFDES";}

   virtual void parseStream(std::istream &in);
   virtual std::ostream& print(std::ostream& out)const;

TYPE_DATA
private:

};

#endif
