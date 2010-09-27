//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
// Description:
//
// Thie file contains the ossimEllipsoidFactory.
//*******************************************************************
//  $Id: ossimEllipsoidFactory.h 17815 2010-08-03 13:23:14Z dburken $

#ifndef ossimEllipsoidFactory_HEADER
#define ossimEllipsoidFactory_HEADER
#include <map>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>

class ossimEllipsoid;
class ossimWgs84Ellipsoid;
class ossimWgs72Ellipsoid;

class OSSIMDLLEXPORT ossimEllipsoidFactory
{
public:
   virtual ~ossimEllipsoidFactory();
   const ossimEllipsoid* create(const ossimString &code)const;
   const ossimEllipsoid* wgs84()const{return theWgs84Ellipsoid;}
   const ossimEllipsoid* wgs72()const{return theWgs72Ellipsoid;}

   //! Given an alpha code (for example "WE" for WGS84), returns the corresponding EPSG code or 0
   //! if not found.
   ossim_uint32 findEpsgCode(const ossimString &alpha_code) const;

   static ossimEllipsoidFactory* instance();
   
private:
   typedef std::map<ossimString, ossimEllipsoid*> TableType;
   typedef std::map<ossim_uint32, ossimString> EpsgTableType;

   static ossimEllipsoidFactory*     theInstance;
   ossimEllipsoid*       theWgs84Ellipsoid;
   ossimEllipsoid*       theWgs72Ellipsoid;
   TableType             theEllipsoidTable;
   EpsgTableType         theEpsgTable;
   ossimEllipsoidFactory();
   void initializeTable();
   void deleteAll();
};

#endif
