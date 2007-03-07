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
// This is the DatumFactory class.  You give it a code and it will
// construct a Datum class.  It returns NULL if no code was found.
//*******************************************************************
//  $Id: ossimDatumFactory.h,v 1.4 2004/05/14 13:45:56 gpotts Exp $

#ifndef ossimDatumFactory_HEADER
#define ossimDatumFactory_HEADER
#include <list>
#include <map>
using namespace std;
#include "base/data_types/ossimString.h"
#include "ossimEllipsoidFactory.h"
class ossimFilename;
class ossimDatum;
class ossimWgs84Datum;
class ossimWgs72Datum;

class OSSIMDLLEXPORT ossimDatumFactory
{
public:
   ~ossimDatumFactory();
   const ossimDatum* create(const ossimString &code)const;
   const ossimDatum* create(const ossimDatum* copy) const;

   static ossimDatumFactory* instance();
   const ossimDatum* wgs84()const{return theWgs84Datum;}
   const ossimDatum* wgs72()const{return theWgs72Datum;}
   list<ossimString> getList()const;
   void writeCStructure(const ossimFilename& file);
protected:
   static ossimDatumFactory*       theInstance;
   const ossimDatum*               theWgs84Datum;
   const ossimDatum*               theWgs72Datum;
   static map<ossimString, ossimDatum*> theDatumTable;
   

   ossimDatumFactory()
      :theWgs84Datum(0),
       theWgs72Datum(0)
      {}
   ossimDatumFactory(const ossimDatumFactory&){}
   void deleteAll();
   void initializeDefaults();
};
#endif
