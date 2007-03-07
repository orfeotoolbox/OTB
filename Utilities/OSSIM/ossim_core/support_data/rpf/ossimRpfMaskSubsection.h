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
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfMaskSubsection.h,v 1.1 2003/04/02 20:45:00 gpotts Exp $
#ifndef ossimRpfMaskSubsection_HEADER
#define ossimRpfMaskSubsection_HEADER
#include <iostream>
using namespace std;

#include "base/common/ossimConstants.h" // ossiByteOrder and RPF constants
#include "base/context/ossimErrorContext.h" // for ossimErrorCode

class ossimRpfMaskSubheader;
class ossimRpfImageDescriptionSubheader;

class ossimRpfMaskSubsection
{
public:
   friend ostream& operator <<(ostream& out,
                               const ossimRpfMaskSubsection& data);
   ossimRpfMaskSubsection();
   virtual ~ossimRpfMaskSubsection();

   ossimErrorCode parseStream(istream& in, ossimByteOrder byteOrder);
   virtual void print(ostream& out)const;
   
private:
   void deleteAll();
   ossimRpfMaskSubheader *theMaskSubheader;   
};

#endif
