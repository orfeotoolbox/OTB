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
// $Id: ossimNitfEmbeddedRpfDes.h,v 1.3 2004/09/24 14:38:46 gpotts Exp $
#ifndef ossimNitfEmbeddedRpfDes_HEADER
#define ossimNitfEmbeddedRpfDes_HEADER
#include "ossimNitfRegisteredTag.h"
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
