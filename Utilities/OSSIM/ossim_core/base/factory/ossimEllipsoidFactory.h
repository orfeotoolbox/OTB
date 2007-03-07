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
//
// Description:
//
// Thie file contains the ossimEllipsoidFactory.
//*******************************************************************
//  $Id: ossimEllipsoidFactory.h,v 1.2 2002/11/25 19:16:28 gpotts Exp $

#ifndef ossimEllipsoidFactory_HEADER
#define ossimEllipsoidFactory_HEADER
#include <map>
using namespace std;
#include "base/data_types/ossimString.h"
class OSSIMDLLEXPORT ossimString;
class OSSIMDLLEXPORT ossimEllipsoid;
class OSSIMDLLEXPORT ossimWgs84Ellipsoid;
class OSSIMDLLEXPORT ossimWgs72Ellipsoid;

class OSSIMDLLEXPORT ossimEllipsoidFactory
{
public:
   typedef map<ossimString, ossimEllipsoid*> TableType;
   virtual ~ossimEllipsoidFactory();
   const ossimEllipsoid* create(const ossimString &code)const;
   const ossimEllipsoid* wgs84()const{return theWgs84Ellipsoid;}
   const ossimEllipsoid* wgs72()const{return theWgs72Ellipsoid;}

   static ossimEllipsoidFactory* instance();
   
private:
   static ossimEllipsoidFactory*     theInstance;
   ossimEllipsoid*       theWgs84Ellipsoid;
   ossimEllipsoid*       theWgs72Ellipsoid;
   TableType             theEllipsoidTable;
   ossimEllipsoidFactory();
   void initializeTable();
   void deleteAll();
};

#endif
