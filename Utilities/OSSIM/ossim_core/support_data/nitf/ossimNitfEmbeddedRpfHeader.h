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
// $Id: ossimNitfEmbeddedRpfHeader.h,v 1.6 2004/09/28 15:35:30 gpotts Exp $
#ifndef ossimNitfEmbeddedRpfHeader_HEADER
#define ossimNitfEmbeddedRpfHeader_HEADER
#include "ossimNitfRegisteredTag.h"
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
