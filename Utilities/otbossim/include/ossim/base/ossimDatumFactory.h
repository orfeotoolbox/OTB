//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
// Description:
//
// This is the DatumFactory class.  You give it a code and it will
// construct a Datum class.  It returns NULL if no code was found.
//*******************************************************************
//  $Id: ossimDatumFactory.h 9094 2006-06-13 19:12:40Z dburken $

#ifndef ossimDatumFactory_HEADER
#define ossimDatumFactory_HEADER
#include <list>
#include <map>

#include <ossim/base/ossimEllipsoidFactory.h>
class ossimFilename;
class ossimDatum;
class ossimWgs84Datum;
class ossimWgs72Datum;
class ossimString;

class OSSIMDLLEXPORT ossimDatumFactory
{
public:
   ~ossimDatumFactory();
   const ossimDatum* create(const ossimString &code)const;
   const ossimDatum* create(const ossimDatum* copy) const;

   static ossimDatumFactory* instance();
   const ossimDatum* wgs84()const{return theWgs84Datum;}
   const ossimDatum* wgs72()const{return theWgs72Datum;}
   std::list<ossimString> getList()const;
   void writeCStructure(const ossimFilename& file);
protected:
   static ossimDatumFactory*       theInstance;
   const ossimDatum*               theWgs84Datum;
   const ossimDatum*               theWgs72Datum;
   static std::map<ossimString, ossimDatum*> theDatumTable;
   

   ossimDatumFactory()
      :theWgs84Datum(0),
       theWgs72Datum(0)
      {}
   ossimDatumFactory(const ossimDatumFactory&){}
   void deleteAll();
   void initializeDefaults();
};
#endif
